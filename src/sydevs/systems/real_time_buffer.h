#pragma once
#ifndef SYDEVS_SYSTEMS_REAL_TIME_BUFFER_H_
#define SYDEVS_SYSTEMS_REAL_TIME_BUFFER_H_

#include <sydevs/time/time_point.h>
#include <array>
#include <chrono>

namespace sydevs {
namespace systems {

using clock = std::chrono::steady_clock;
using clock_time = std::chrono::time_point<clock>;


class real_time_buffer
{
public:
    real_time_buffer(float64 ta_rate, int64 ta_depth);

    float64 time_advancement_rate() const;
    int64 time_advancement_depth() const;

    void update_time_advancement_rate(float64 ta_rate);
    void update_time_advancement_depth(int64 ta_depth);

    int64 max_depth() const;
    const time_point& cached_time(int64 depth = 0) const;
    const clock_time& cached_clock_time(int64 depth = 0) const;

    clock_time planned_clock_time() const;

    void retain(const time_point& t, const clock_time& clock_t, duration planned_dt);

private:
    void recompute_planned_clock_time();

    float64 ta_rate_;
    int64 ta_depth_;
    int64 max_depth_;
    std::array<time_point, 64> time_points_;
    std::array<clock_time, 64> clock_times_;
    std::array<int64, 64> retention_flags_;
    duration planned_dt_;
    clock_time planned_clock_t_;
};


inline float64 real_time_buffer::time_advancement_rate() const
{
    return ta_rate_;
}


inline int64 real_time_buffer::time_advancement_depth() const
{
    return ta_depth_;
}


inline int64 real_time_buffer::max_depth() const
{
    return max_depth_;
}


inline const time_point& real_time_buffer::cached_time(int64 depth) const
{
    if (depth >= max_depth_) throw std::domain_error("Cached time retrieval depth must be less than the maximum depth");
    return time_points_.at(depth);
}


inline const clock_time& real_time_buffer::cached_clock_time(int64 depth) const
{
    if (depth >= max_depth_) throw std::domain_error("Cached clock time retrieval depth must be less than the maximum depth");
    return clock_times_.at(depth);
}


inline clock_time real_time_buffer::planned_clock_time() const
{
    return planned_clock_t_;
}


}  // namespace
}  // namespace

#endif
