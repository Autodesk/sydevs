#include <sydevs/time/time_sequence.h>
#include <algorithm>

namespace sydevs {


time_sequence::time_sequence()
    : partitions_()
    , offsets_()
{
}


void time_sequence::append(const time_point& tp)
{
    auto dt = duration::inf();
    if (!partitions_.empty()) {
        dt = tp - partitions_.back().second;
        if (dt <= offsets_.back()) throw std::out_of_range("Time points must be appended in increasing order");
    }
    if (dt.finite()) {
        offsets_.push_back(dt);
    }
    else {
        partitions_.emplace_back(offsets_.size(), tp);
        offsets_.emplace_back(0);
    }
}


time_sequence::const_iterator time_sequence::begin() const
{
    return const_iterator(*this, 0, 0);
}


time_sequence::const_iterator time_sequence::end() const
{
    return const_iterator(*this, std::max(int64(0), int64(partitions_.size()) - 1), int64(offsets_.size()));
}

    
time_sequence::const_iterator time_sequence::lower_bound(const time_point& tp) const
{
    int64 pi = 0;
    int64 ti = 0;
    if (!empty()) {
        auto p_comp = [](const value_type& entry, const time_point& value) { return entry.second < value; };
        auto p_iter = std::lower_bound(std::begin(partitions_), std::end(partitions_), tp, p_comp);
        if (p_iter != std::end(partitions_) && p_iter->second == tp) {
            pi = p_iter - std::begin(partitions_);
            ti = p_iter->first;
        }
        else {
            if (p_iter != std::begin(partitions_)) {
                pi = (p_iter - 1) - std::begin(partitions_);
                int64 ti0 = (p_iter - 1)->first + 1;
                int64 ti1 = size();
                if (p_iter != std::end(partitions_)) {
                    ti1 = p_iter->first;
                }
                auto tp0 = (p_iter - 1)->second;
                auto t_comp = [&tp0](duration entry, const time_point& value) { return tp0 + entry < value; };
                auto t_iter = std::lower_bound(std::begin(offsets_) + ti0, std::begin(offsets_) + ti1, tp, t_comp);
                ti = t_iter - std::begin(offsets_);
            }
        }
    }
    return const_iterator(*this, pi, ti);
}


time_sequence::const_iterator time_sequence::upper_bound(const time_point& tp) const
{
    int64 pi = 0;
    int64 ti = 0;
    if (!empty()) {
        auto p_comp = [](const time_point& value, const value_type& entry) { return value < entry.second; };
        auto p_iter = std::upper_bound(std::begin(partitions_), std::end(partitions_), tp, p_comp);
        if (p_iter != std::begin(partitions_)) {
            pi = (p_iter - 1) - std::begin(partitions_);
            int64 ti0 = (p_iter - 1)->first + 1;
            int64 ti1 = size();
            if (p_iter != std::end(partitions_)) {
                ti1 = p_iter->first;
            }
            auto tp0 = (p_iter - 1)->second;
            auto t_comp = [&tp0](const time_point& value, duration entry) { return value < tp0 + entry; };
            auto t_iter = std::upper_bound(std::begin(offsets_) + ti0, std::begin(offsets_) + ti1, tp, t_comp);
            ti = t_iter - std::begin(offsets_);
        }
    }
    return const_iterator(*this, pi, ti);
}


const time_point time_sequence::front() const
{
    return begin()->second;
}


const time_point time_sequence::back() const
{
    return (end() - 1)->second;
}


const time_point time_sequence::operator[](int64 rhs) const
{
    return (begin() + rhs)->second;
}


time_sequence::const_iterator::const_iterator(const time_sequence& ts, int64 pi, int64 ti)
    : ts_ptr_(&ts)
    , pi_(pi)
    , entry_(ti, ti < ts.size() ? ts.partitions_[size_t(pi)].second + ts.offsets_[size_t(ti)] : time_point())
{    
}


const time_sequence::value_type& time_sequence::const_iterator::operator*() const
{
    if (entry_.first < 0 || entry_.first >= ts_ptr_->size()) throw std::out_of_range("Time sequence iterator to be dereferenced must be in range");
    return entry_;
}

    
const time_sequence::value_type* time_sequence::const_iterator::operator->() const
{
    if (entry_.first < 0 || entry_.first >= ts_ptr_->size()) throw std::out_of_range("Time sequence iterator supporting member access must be in range");
    return &entry_;
}

    
const time_sequence::value_type time_sequence::const_iterator::operator[](int64 rhs) const
{
    return *(*this + rhs);
}


time_sequence::const_iterator& time_sequence::const_iterator::operator++()
{
    ++entry_.first;
    if (pi_ + 1 < int64(ts_ptr_->partitions_.size()) && ts_ptr_->partitions_[pi_ + 1].first == entry_.first) {
        ++pi_; 
    }
    if (entry_.first >= 0 && entry_.first < ts_ptr_->size()) {
        entry_.second = ts_ptr_->partitions_[pi_].second + ts_ptr_->offsets_[entry_.first];
    }
    return *this;
}


time_sequence::const_iterator time_sequence::const_iterator::operator++(int)
{
    auto old = *this;
    ++(*this);
    return old;
}


time_sequence::const_iterator& time_sequence::const_iterator::operator--()
{
    --entry_.first;
    if (pi_ - 1 >= 0 && entry_.first < ts_ptr_->partitions_[pi_].first) {
        --pi_;
    }
    if (entry_.first >= 0 && entry_.first < ts_ptr_->size()) {
        entry_.second = ts_ptr_->partitions_[pi_].second + ts_ptr_->offsets_[entry_.first];
    }
    return *this;
}


time_sequence::const_iterator time_sequence::const_iterator::operator--(int)
{
    auto old = *this;
    --(*this);
    return old;
}


time_sequence::const_iterator& time_sequence::const_iterator::operator+=(int64 rhs)
{
    entry_.first += rhs;
    auto p_comp = [](const value_type& entry, int64 value) { return entry.first < value; };
    auto p_iter = std::lower_bound(std::begin(ts_ptr_->partitions_), std::end(ts_ptr_->partitions_), entry_.first, p_comp);
    if (p_iter != std::end(ts_ptr_->partitions_) && p_iter->first == entry_.first) {
        pi_ = p_iter - std::begin(ts_ptr_->partitions_);
    }
    else {
        if (p_iter == std::begin(ts_ptr_->partitions_)) {
            pi_ = 0;
        }
        else {
            pi_ = p_iter - std::begin(ts_ptr_->partitions_) - 1;
        }
    }
    if (entry_.first >= 0 && entry_.first < ts_ptr_->size()) {
        entry_.second = ts_ptr_->partitions_[pi_].second + ts_ptr_->offsets_[entry_.first];
    }
    else {
        entry_.second = time_point();
    }
    return *this;
}


time_sequence::const_iterator& time_sequence::const_iterator::operator-=(int64 rhs)
{
    *this += -rhs;
    return *this;
}


time_sequence::const_iterator time_sequence::const_iterator::operator+(int64 rhs) const
{
    auto ts_iter = *this;
    ts_iter += rhs;
    return ts_iter;
}


time_sequence::const_iterator time_sequence::const_iterator::operator-(int64 rhs) const
{
    auto ts_iter = *this;
    ts_iter += -rhs;
    return ts_iter;
}


int64 time_sequence::const_iterator::operator-(const_iterator rhs) const
{
    return entry_.first - rhs.entry_.first;
}


bool time_sequence::const_iterator::operator==(const const_iterator& rhs) const
{
    return entry_.first == rhs.entry_.first;
}


bool time_sequence::const_iterator::operator!=(const const_iterator& rhs) const
{
    return entry_.first != rhs.entry_.first;
}


bool time_sequence::const_iterator::operator<(const const_iterator& rhs) const
{
    return entry_.first < rhs.entry_.first;
}


bool time_sequence::const_iterator::operator>(const const_iterator& rhs) const
{
    return entry_.first > rhs.entry_.first;
}


bool time_sequence::const_iterator::operator<=(const const_iterator& rhs) const
{
    return entry_.first <= rhs.entry_.first;
}


bool time_sequence::const_iterator::operator>=(const const_iterator& rhs) const
{
    return entry_.first >= rhs.entry_.first;
}


}  // namespace
