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

    void push(const time_point& t, const clock_time& clock_t, duration planned_dt);

private:
    void update_planned_clock_time();

    float64 ta_rate_;
    int64 offset_;
    int64 count_;
    std::vector<time_point> time_points_;
    std::vector<clock_time> clock_times_;
    std::vector<int64> frame_indices_;
    std::vector<int64> replacement_flags_;
    duration planned_dt;
    clock_time planned_clock_t_;
};


real_time_buffer::real_time_buffer(float64 ta_rate, int64 ta_precision)
    : ta_rate_(ta_rate)
    , offset_(0)
    , count_(0)
    , time_points_(ta_precision, time_point())
    , clock_times_(ta_precision, clock_time())
    , frame_indices_(ta_precision, 0)
    , replacement_flags_(ta_precision, 0)
    , planned_dt(0_s)
    , planned_clock_t_(clock::now())
{
}


float64 real_time_buffer::time_advancement_rate() const
{
    return ta_rate_;
}


int64 real_time_buffer::time_advancement_precision() const
{
    return int64(time_points_.size());
}


void real_time_buffer::update_time_advancement_rate(float64 ta_rate)
{
    ta_rate_ = ta_rate;
    offset_ = (offset_ + count_ - 1)%time_advancement_precision();
    count_ = 1;
    update_planned_clock_time();
}


void real_time_buffer::update_time_advancement_precision(int64 ta_precision)
{
    auto time_points = std::vector<time_point>(ta_precision, time_point());
    auto clock_times = std::vector<clock_time>(ta_precision, clock_time());
    auto frame_indices = std::vector<int64>(ta_precision, 0);
    auto replacement_flags = std::vector<int64>(ta_precision, 0);
    int64 count = std::min(count_, ta_precision);
    for (int64 i = 0; i < count_; ++i) {
        time_points[i] = time_points[(offset_ + i)%time_advancement_precision()];
        clock_times[i] = clock_times[(offset_ + i)%time_advancement_precision()];
        frame_indices[i] = frame_indices[(offset_ + i)%time_advancement_precision()];
        replacement_flags[i] = replacement_flags[(offset_ + i)%time_advancement_precision()];
    }
    time_points = std::move(time_points_);
    clock_times = std::move(clock_times_);
    frame_indices = std::move(frame_indices_);
    replacement_flags = std::move(replacement_flags_);
    if (count < count_) {
        count_ = count;
        update_planned_clock_time();
    }
}


clock_time real_time_buffer::planned_clock_time() const
{
    return planned_clock_t_;
}


void real_time_buffer::push(const time_point& t, const clock_time& clock_t, duration planned_dt)
{
}


void real_time_buffer::update_planned_clock_time()
{
}


}  // namespace
}  // namespace

#endif
