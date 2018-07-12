#pragma once
#ifndef SYDEVS_SYSTEMS_REAL_TIME_BUFFER_H_
#define SYDEVS_SYSTEMS_REAL_TIME_BUFFER_H_

#include <sydevs/time/time_point.h>
#include <chrono>

namespace sydevs {
namespace systems {

using clock = std::chrono::steady_clock;
using clock_time = std::chrono::time_point<clock>;


class real_time_buffer
{
public:
    real_time_buffer(float64 ta_rate, int64 ta_precision);

    float64 time_advancement_rate() const;
    int64 time_advancement_precision() const;

    void update_time_advancement_rate(float64 ta_rate);
    void update_time_advancement_precision(int64 ta_precision);

    clock_time planned_clock_time() const;

    void retain(const time_point& t, const clock_time& clock_t, duration planned_dt);

private:
    void recompute_planned_clock_time();

    float64 ta_rate_;
    int64 count_;
    std::vector<time_point> time_points_;
    std::vector<clock_time> clock_times_;
    std::vector<int64> retention_flags_;
    duration planned_dt_;
    clock_time planned_clock_t_;
};


inline float64 real_time_buffer::time_advancement_rate() const
{
    return ta_rate_;
}


inline int64 real_time_buffer::time_advancement_precision() const
{
    return int64(time_points_.size());
}


inline clock_time real_time_buffer::planned_clock_time() const
{
    return planned_clock_t_;
}


}  // namespace
}  // namespace

#endif
