#include <sydevs/systems/real_time_buffer.h>

namespace sydevs {
namespace systems {

    
real_time_buffer::real_time_buffer(float64 ta_rate, int64 ta_depth)
    : ta_rate_(ta_rate)
    , ta_depth_(ta_depth)
    , max_depth_(0)
    , time_points_()
    , clock_times_()
    , retention_flags_()
    , planned_dt_(0_s)
    , planned_clock_t_()
{
}


void real_time_buffer::update_time_advancement_rate(float64 ta_rate)
{
    ta_rate_ = ta_rate;
    if (max_depth_ > 0) {
        max_depth_ = 1;
    }
    recompute_planned_clock_time();
}


void real_time_buffer::update_time_advancement_depth(int64 ta_depth)
{
    ta_depth_ = ta_depth;
    recompute_planned_clock_time();
}


void real_time_buffer::retain(const time_point& t, const clock_time& clock_t, duration planned_dt)
{
    auto insert = true;
    auto insert_t = t;
    auto insert_clock_t = clock_t;
    for (int64 i = 0; insert && i <= max_depth_ && i < 64; ++i) {
        std::swap(time_points_[i], insert_t);
        std::swap(clock_times_[i], insert_clock_t);
        if (i == max_depth_) {
            ++max_depth_;
            insert = false;
            retention_flags_[i] = 1;
        }
        else {
            insert = (retention_flags_[i] == 1);
            retention_flags_[i] = 1 - retention_flags_[i];
        }
    }
    planned_dt_ = planned_dt;
    recompute_planned_clock_time();
}


void real_time_buffer::recompute_planned_clock_time()
{
    planned_clock_t_ = clock_time();
    int64 depth = std::min(ta_depth_, max_depth_);
    if (depth > 0) {
        float64 accel_clock_dt_sum = 0.0;
        float64 decel_clock_dt_sum = 0.0;
        int64 accel_clock_dt_count = 0;
        int64 decel_clock_dt_count = 0;
        const auto& retained_t0 = time_points_[0];
        const auto& retained_clock_t0 = clock_times_[0];
        float64 clock_dt0 = 0.0;
        for (int64 i = 0; i < depth; ++i) {
            const auto& retained_ti = time_points_[i];
            const auto& retained_clock_ti = clock_times_[i];
            auto dti = retained_t0.gap(retained_ti) + planned_dt_.unfixed();
            auto clock_ti = retained_clock_ti + std::chrono::microseconds(int64((dti/ta_rate_)/1_us));
            float64 clock_dti = 0.0;
            if (clock_ti > retained_clock_t0) {
                clock_dti = float64(std::chrono::duration_cast<std::chrono::microseconds>(clock_ti - retained_clock_t0).count());
            }
            if (i == 0) {
                clock_dt0 = clock_dti;
            }
            else {
                clock_dti = std::min(clock_dti, 2.0*clock_dt0);
            }
            if (clock_dti <= clock_dt0) {
                accel_clock_dt_sum += clock_dti;
                ++accel_clock_dt_count;
            }
            if (clock_dti >= clock_dt0) {
                decel_clock_dt_sum += clock_dti;
                ++decel_clock_dt_count;
            }
            if (i == depth - 1) {
                if (clock_dti < clock_dt0) {
                    decel_clock_dt_sum = 0.0;
                    decel_clock_dt_count = 0;
                }
                else if (clock_dti > clock_dt0) {
                    accel_clock_dt_sum = 0.0;
                    accel_clock_dt_count = 0;
                }
                else {
                    accel_clock_dt_sum = clock_dti;
                    accel_clock_dt_count = 1;
                    decel_clock_dt_sum = 0.0;
                    decel_clock_dt_count = 0;
                }
            }
        }
        float64 clock_dt = (accel_clock_dt_sum + decel_clock_dt_sum)/(accel_clock_dt_count + decel_clock_dt_count);
        planned_clock_t_ = retained_clock_t0 + std::chrono::microseconds(int64(clock_dt));
    }
}


}  // namespace
}  // namespace
