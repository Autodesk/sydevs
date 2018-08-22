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
    real_time_buffer(float64 t_adv_rate, int64 t_syn_rate);

    float64 time_advancement_rate() const;
    float64 time_synchronization_rate() const;

    void update_time_advancement_rate(float64 t_adv_rate);
    void update_time_synchronization_rate(float64 t_syn_rate);

    time_point synchronization_time() const;
    clock_time synchronization_clock_time() const;

    void update_synchronization_time(const time_point& sim_t, const clock_time& clk_t);

    time_point current_time() const;
    clock_time current_clock_time() const;

    void update_current_time(const time_point& sim_t, const clock_time& clk_t, duration planned_sim_dt);

    clock_time planned_clock_time() const;

private:
    void recompute_planned_clock_duration();

    float64 t_adv_rate_;        // time advancement rate
    float64 t_syn_rate_;        // time synchronization rate
    time_point syn_sim_t_;      // synchronization simulated time
    clock_time syn_clk_t_;      // synchronization clock time
    time_point current_sim_t_;  // current simulated time
    clock_time current_clk_t_;  // current clock time
    duration planned_sim_dt_;   // planned simulated time duration
    float64 planned_clk_dt_;    // planned simulated time duration
    int64 syn_count_;           // synchronization step count
};


inline float64 real_time_buffer::time_advancement_rate() const
{
    return t_adv_rate_;
}


inline float64 real_time_buffer::time_synchronization_rate() const
{
    return t_syn_rate_;
}


inline time_point real_time_buffer::synchronization_time() const
{
    return syn_sim_t_;
}


inline clock_time real_time_buffer::synchronization_clock_time() const
{
    return syn_clk_t_;
}


inline time_point real_time_buffer::current_time() const
{
    return current_sim_t_;
}


inline clock_time real_time_buffer::current_clock_time() const
{
    return current_clk_t_;
}


inline clock_time real_time_buffer::planned_clock_time() const
{
    return current_clk_t_ + std::chrono::milliseconds(int64(planned_clk_dt_));
}


}  // namespace
}  // namespace

#endif
