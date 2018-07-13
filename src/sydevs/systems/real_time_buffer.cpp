#include <sydevs/systems/real_time_buffer.h>

namespace sydevs {
namespace systems {

    
real_time_buffer::real_time_buffer(float64 ta_rate, int64 ta_precision)
    : ta_rate_(ta_rate)
    , count_(0)
    , time_points_(ta_precision)
    , clock_times_(ta_precision)
    , retention_flags_(ta_precision)
    , planned_dt_(0_s)
    , planned_clock_t_()
{
}


void real_time_buffer::update_time_advancement_rate(float64 ta_rate)
{
    ta_rate_ = ta_rate;
    count_ = 1;
    recompute_planned_clock_time();
}


void real_time_buffer::update_time_advancement_precision(int64 ta_precision)
{
    time_points_.resize(ta_precision);
    clock_times_.resize(ta_precision);
    retention_flags_.resize(ta_precision);
    if (ta_precision < count_) {
        count_ = ta_precision;
        recompute_planned_clock_time();
    }
}


void real_time_buffer::retain(const time_point& t, const clock_time& clock_t, duration planned_dt)
{
    auto insert = true;
    auto insert_t = t;
    auto insert_clock_t = clock_t;
    for (int64 i = 0; insert && i <= count_; ++i) {
        if (i < time_advancement_precision()) {
            std::swap(time_points_[i], insert_t);
            std::swap(clock_times_[i], insert_clock_t);
            if (i == count_) {
                ++count_;
                insert = false;
                retention_flags_[i] = 1;
            }
            else {
                insert = (retention_flags_[i] == 1);
                retention_flags_[i] = 1 - retention_flags_[i];
            }
        }
    }
    planned_dt_ = planned_dt;
    recompute_planned_clock_time();
}


void real_time_buffer::recompute_planned_clock_time()
{
    planned_clock_t_ = clock_time();
    if (count_ > 0) {
        float64 numer = 0.0;
        float64 denom = 0.0;
        const auto& t = time_points_[0];
        const auto& clock_t = clock_times_[0];
        float64 clock_dt0 = 0.0;
        for (int64 i = 0; i < count_; ++i) {
            const auto& ref_ti = time_points_[i];
            const auto& ref_clock_ti = clock_times_[i];
            auto dt = t.gap(ref_ti) + planned_dt_.unfixed();
            auto clock_ti = ref_clock_ti + std::chrono::microseconds(int64((dt/ta_rate_)/1_us));
            float64 clock_dti = 0.0;
            if (clock_ti > clock_t) {
                clock_dti = float64(std::chrono::duration_cast<std::chrono::microseconds>(clock_ti - clock_t).count());
            }
            if (i == 0) {
                clock_dt0 = clock_dti;
            }
            float64 coeff = 0.0;
            if (clock_dt0 > 0.0) {
                coeff = exp2(i*(clock_dti/clock_dt0));
            }            
            numer += coeff*clock_dti;
            denom += coeff;
        }
        if (denom > 0.0) {
            planned_clock_t_ = clock_t + std::chrono::microseconds(int64(numer/denom));
        }
        else {
            planned_clock_t_ = clock_t;
        }
    }
}


}  // namespace
}  // namespace
