#pragma once
#ifndef SYDEVS_SYSTEMS_REAL_TIME_BUFFER_H_
#define SYDEVS_SYSTEMS_REAL_TIME_BUFFER_H_

#include <sydevs/core/timer.h>
#include <sydevs/time/time_point.h>
#include <array>

namespace sydevs {
namespace systems {


/**
 * @brief A data structure which suggests event wallclock times to aid in the
 *        synchronization of a simulation's execution.
 *
 * A `real_time_buffer` is continually updated with the current point in
 * simulated time, the current point in wallclock time, and the planned
 * duration of simulated time until the next planned event. It then recommends
 * a planned wallclock time point for the future event, with the objective of
 * eventually synchronizing simulated and wallclock time. The synchronization
 * is based on a reference point in both simulated and wallclock time, as well
 * as the time advancement rate.
 *
 * The time advancement rate `t_adv_rate` is the intended ratio of a duration
 * of simulated time to a corresponding duration of wallclock time. If the rate
 * is 1, the objective is that the simulation proceed at exactly real time. If
 * the rate is 2, the objective is to run the simulation twice as fast as real
 * time. If the rate is 0.1, the objective is to run the simulation at one tenth
 * the speed of real-world time.
 *
 * The time synchronization rate `t_syn_rate` measures how aggressively the real
 * time buffer will try to synchronize simulated and wallclock time. If the rate
 * is large (i.e. much greater than 1), the objective is to rapidly bring 
 * simulated and wallclock time into alignment. If rate is small (i.e.
 * considerably less than 1), the objective is to gradually and smoothly
 * synchronize simulated and wallclock time and eventually bring them into
 * alignment.
 */
class real_time_buffer
{
public:
    real_time_buffer(float64 t_adv_rate, int64 t_syn_rate);  ///< Constructs a real time buffer with the specified time advancement rate `t_adv_rate` and time synchronization rate `t_syn_rate`.

    float64 time_advancement_rate() const;      ///< Returns the time advancement rate.
    float64 time_synchronization_rate() const;  ///< Returns the time synchronization rate.

    void update_time_advancement_rate(float64 t_adv_rate);      ///< Updates the time advancement rate and recomputes the planned wallclock time.
    void update_time_synchronization_rate(float64 t_syn_rate);  ///< Updates the time synchonization rate and recomputes the planned wallclock time.

    time_point synchronization_time() const;        ///< Returns the simulated time of the synchronization reference point.
    clock_time synchronization_clock_time() const;  ///< Returns the wallclock time of the synchronization reference point.

    void update_synchronization_time(const time_point& sim_t, const clock_time& clk_t);  ///< Updates the synchonization reference point and recomputes the planned wallclock time.

    time_point current_time() const;        ///< Returns the current simulated time.
    clock_time current_clock_time() const;  ///< Returns the current wallclock time.

    void update_current_time(const time_point& sim_t, const clock_time& clk_t, duration planned_sim_dt);  ///< Updates the current time and the planned duration of simulated time until the planned event.

    clock_time planned_clock_time() const;  ///< Returns the recommended point in wallclock time of the planned event.

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
