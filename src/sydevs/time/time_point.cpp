#include <sydevs/time/time_point.h>
#include <sydevs/core/quantity.h>
#include <algorithm>
#include <cstdlib>
#include <ostream>
#include <iomanip>

namespace sydevs {


time_point::time_point()
    : sign_(1)
    , precision_()
    , digits_()
{
}


time_point::time_point(duration dt)
    : sign_(1)
    , precision_()
    , digits_()
{
    (*this) += dt;
}


int64 time_point::scale_digit(scale precision) const
{
    return precision < precision_              ? 0 :
           precision >= precision_ + nscales() ? 0 : 
                                                 digits_[precision - precision_];
}


int64 time_point::scale_phase(scale precision) const
{
    int64 phase = scale_digit(precision);
    if (sign_ == -1) {
        phase = 1000 - phase;
        if (precision_ < precision) {
            --phase;
        }
        else {
            if (phase == 1000) {
                phase = 0;
            }
        }
    }
    return phase;
}


int64 time_point::epoch_phase(scale precision) const
{
    int64 phase = 0;
    for (auto s = precision + 4; s >= precision; --s) {
        phase = 1000*phase + scale_digit(s);
    }
    if (sign_ == -1) {
        phase = quantity_limit - phase;
        if (precision_ < precision) {
            --phase;
        }
        else {
            if (phase == quantity_limit) {
                phase = 0;
            }
        }
    }
    return phase;
}


time_point& time_point::advance(duration rhs)
{
    if (!rhs.valid()) throw std::invalid_argument("Advancement duration must be valid");
    if (rhs < 0_s) throw std::domain_error("Advancement duration must be non-negative");
    if (!rhs.finite()) throw std::domain_error("Advancement duration must be finite");    
    if (rhs > 0_s) {
        if (precision_ < rhs.precision()) {
            if (sign_ == -1) {
                *this += duration(-1, rhs.precision());
            }
            auto precision = rhs.precision();
            if (nscales() > precision - precision_) {
                while (scale_digit(precision) == 0) {
                    ++precision;
                }
            }
            digits_.erase(std::begin(digits_), std::begin(digits_) + std::min(precision - precision_, int64(digits_.size())));
            precision_ = precision;
        }
        *this += rhs;
    }
    return *this;
}


time_point& time_point::operator+=(duration rhs)
{
    if (!rhs.valid()) throw std::invalid_argument("Adjustment duration must be valid");
    if (!rhs.finite()) throw std::domain_error("Adjustment duration must be finite");
    if (rhs != 0_s) {
        auto dt = sign_*rhs.coarsened();
        {  // Restructure the digits vector if necessary.
            if (digits_.empty()) {  // Add the first digit if necessary
                precision_ = dt.precision();
                digits_.emplace_back(0);
            }
            else {  // Otherwise, extend the digits vector if necessary.
                if (precision_ > dt.precision()) {
                    digits_.insert(std::begin(digits_), precision_ - dt.precision(), 0);
                    precision_ = dt.precision();
                }
                if (precision_ + nscales() <= dt.precision()) {
                    digits_.insert(std::end(digits_), dt.precision() - precision_ - nscales() + 1, 0);
                }
            }
        }
        {  // Perform the addition on each digit, carrying the overflow values.
            int64 tick_count = dt.multiplier();
            for (size_t i = size_t(dt.precision() - precision_); tick_count != 0; ++i) {
                int64 rest = tick_count/1000;
                int64 offset = tick_count - 1000*rest;
                int64 sum = digits_[i] + offset;
                if (sum >= 1000) {
                    digits_[i] = sum - 1000;
                    tick_count = rest + 1;
                }
                else if (sum >= 0) {
                    digits_[i] = sum;
                    tick_count = rest;
                }
                else {
                    digits_[i] = sum + 1000;
                    tick_count = rest - 1;
                }
                if (tick_count != 0 && i + 1 == digits_.size()) {
                    digits_.emplace_back(0);
                    if (tick_count < 0 && tick_count > -1000) {
                        digits_[i + 1] = tick_count;
                        tick_count = 0;
                    }
                }
            }
        }
        {  // Finalize the operation.
            {  // Remove zero-valued digits from the fine end.
                size_t i = 0;
                while (i < digits_.size() && digits_[i] == 0) {
                    ++i;
                }
                if (i > 0) {
                    precision_ += i;
                    digits_.erase(std::begin(digits_), std::begin(digits_) + i);
                }
            }
            if (digits_.empty()) {  // If no digits remain, reset the time point.
                precision_ = unit;
                sign_ = 1;
            }
            else {  // Otherwise...
                if (digits_.back() < 0) {  // If the coarsest digit has a negative value, flip the sign of the time point.
                    digits_.back() = -digits_.back();
                    sign_ = -sign_;
                    for (size_t i = size_t(nscales()) - 1; i >= 1; --i) {
                        digits_[i] -= 1;
                        digits_[i - 1] = 1000 - digits_[i - 1];
                    }
                }
                {  // Remove zero-valued digits from the coarse end.
                    while (digits_.back() == 0) {
                        digits_.pop_back();
                    }
                }
            }
        }
    }
    return *this;
}


time_point& time_point::operator-=(duration rhs)
{
    (*this) += -rhs;
    return *this;
}


const time_point time_point::operator+(duration rhs) const
{
    auto tp = *this;
    tp += rhs;
    return tp;
}


const time_point time_point::operator-(duration rhs) const
{
    auto tp = *this;
    tp -= rhs;
    return tp;
}


const duration time_point::operator-(const time_point& rhs) const
{
    auto min_precision = std::min(precision_, rhs.precision_);
    auto max_precision = std::max(precision_ + nscales(), rhs.precision_ + rhs.nscales()) - 1;
    auto dt = duration((sign() - rhs.sign())/2, max_precision + 1);
    for (auto s = max_precision; dt.finite() && s >= min_precision; --s) {
        auto scale_dt = duration(scale_phase(s) - rhs.scale_phase(s), s);
        auto next_dt = dt + scale_dt;
        if (next_dt - dt != scale_dt) {
            dt = (dt > 0_s ? duration::inf() : -duration::inf());
        }
        else {
            dt = next_dt;
        }
    }
    return dt;
}


duration time_point::gap(const time_point& rhs) const
{
    auto min_precision = std::min(precision_, rhs.precision_);
    auto max_precision = std::max(precision_ + nscales(), rhs.precision_ + rhs.nscales()) - 1;
    auto big_dt = duration((sign() - rhs.sign())/2, max_precision + 1);
    auto precision = min_precision;
    {
        for (auto s = max_precision; precision == min_precision && s >= min_precision; --s) {
            auto scale_dt = duration(scale_phase(s) - rhs.scale_phase(s), s);
            auto next_dt = big_dt + scale_dt;
            if (next_dt - big_dt != scale_dt) {
                precision = s + 1;
            }
            else {
                big_dt = next_dt;
            }
        }
    }
    auto small_dt = 0_s;
    {
        for (auto s = min_precision; s < precision; ++s) {
            small_dt += duration(scale_phase(s) - rhs.scale_phase(s), s);
            auto rounded_dt = (small_dt + (0_s).fixed_at(s - 4)).unfixed();
            if (rounded_dt.finite()) {
                small_dt = rounded_dt;
            }
        }
    }
    auto dt = big_dt;
    if (small_dt == duration(500, precision - 1)) {
        if (big_dt > 0_s) {
            dt = big_dt + duration(1, precision);
        }
    }
    else if (small_dt == duration(-500, precision - 1)) {
        if (big_dt < 0_s) {
            dt = big_dt - duration(1, precision);
        }
    }
    else {
        dt = big_dt + (small_dt + (0_s).fixed_at(precision)).unfixed();
    }
    return dt;
}


bool time_point::operator==(const time_point& rhs) const
{
    auto upd_precision = upper_discrepant_precision(rhs);
    return upd_precision < std::min(precision_, rhs.precision_);
}


bool time_point::operator!=(const time_point& rhs) const
{
    auto upd_precision = upper_discrepant_precision(rhs);
    return upd_precision >= std::min(precision_, rhs.precision_);
}


bool time_point::operator<(const time_point& rhs) const
{
    auto upd_precision = upper_discrepant_precision(rhs);
    return sign_*scale_digit(upd_precision) < rhs.sign_*rhs.scale_digit(upd_precision);
}


bool time_point::operator>(const time_point& rhs) const
{
    auto upd_precision = upper_discrepant_precision(rhs);
    return sign_*scale_digit(upd_precision) > rhs.sign_*rhs.scale_digit(upd_precision);
}


bool time_point::operator<=(const time_point& rhs) const
{
    auto upd_precision = upper_discrepant_precision(rhs);
    return sign_*scale_digit(upd_precision) <= rhs.sign_*rhs.scale_digit(upd_precision);
}


bool time_point::operator>=(const time_point& rhs) const
{
    auto upd_precision = upper_discrepant_precision(rhs);
    return sign_*scale_digit(upd_precision) >= rhs.sign_*rhs.scale_digit(upd_precision);
}


bool time_point::operator==(duration rhs) const
{
    return *this == time_point(rhs);
}


bool time_point::operator!=(duration rhs) const
{
    return *this != time_point(rhs);
}


bool time_point::operator<(duration rhs) const
{
    return *this < time_point(rhs);
}


bool time_point::operator>(duration rhs) const
{
    return *this > time_point(rhs);
}


bool time_point::operator<=(duration rhs) const
{
    return *this <= time_point(rhs);
}


bool time_point::operator>=(duration rhs) const
{
    return *this >= time_point(rhs);
}


scale time_point::upper_discrepant_precision(const time_point& rhs) const
{
    auto equal_so_far = true;
    auto min_precision = std::min(precision_, rhs.precision_);
    auto upd_precision = std::max(precision_ + nscales(), rhs.precision_ + rhs.nscales()) - 1;
    while (equal_so_far && upd_precision >= min_precision) {
        if (sign_*scale_digit(upd_precision) != rhs.sign_*rhs.scale_digit(upd_precision)) {
            equal_so_far = false;
        }
        else {
            --upd_precision;
        }
    }
    return upd_precision;
}


std::ostream& operator<<(std::ostream& os, const time_point& rhs)
{
    os << "time_point()";
    for (auto s = rhs.precision() + rhs.nscales() - 1; s >= rhs.precision(); --s) {
        os << (rhs.sign() == 1 ? " + " : " - ") << duration(rhs.scale_digit(s), s);
    }
    return os;
}

    
bool operator==(duration lhs, const time_point& rhs)
{
    return time_point(lhs) == rhs;
}


bool operator!=(duration lhs, const time_point& rhs)
{
    return time_point(lhs) != rhs;
}


bool operator<(duration lhs, const time_point& rhs)
{
    return time_point(lhs) < rhs;
}


bool operator>(duration lhs, const time_point& rhs)
{
    return time_point(lhs) > rhs;
}


bool operator<=(duration lhs, const time_point& rhs)
{
    return time_point(lhs) <= rhs;
}


bool operator>=(duration lhs, const time_point& rhs)
{
    return time_point(lhs) >= rhs;
}


}  // namespace
