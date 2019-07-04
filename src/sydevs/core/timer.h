#pragma once
#ifndef SYDEVS_TIMER_H_
#define SYDEVS_TIMER_H_

#include <sydevs/core/quantity.h>
#include <chrono>

namespace sydevs {

using clock = std::chrono::steady_clock;
using clock_time = std::chrono::time_point<clock>;


/** 
 * @brief A class for measuring and accumulating intervals of wallclock time.
 *
 * @details
 * A `timer` object measures the interval of wallclock time that elapses between
 * the point at which it is started and the point at which it is stopped. It
 * also accumulates these time intervals. It can be used to measure how much
 * is spent executing a block of code. All wallclock time durations are measured
 * and reported in microseconds.
 */
class timer
{
public:
    /**
     * @brief Constructs a timer that has not yet accumuated any interval measurements.
     */
    timer();

    /**
     * @brief Constructs a timer that initially has a cumulative duration of `cumulative_dt`.
     */
    explicit timer(duration cumulative_dt);

    timer(const timer&)            = delete;   ///< Copy constructor
    timer& operator=(const timer&) = delete;   ///< Copy assignment
    timer(timer&&)                 = default;  ///< Move constructor
    timer& operator=(timer&&)      = default;  ///< Move assignment
    ~timer()                       = default;  ///< Destructor

    bool timing() const;  ///< Returns `true` if the timer has been started but not yet stopped.

    duration cumulative_duration() const;  ///< Returns the cumulative duration of measured time.

    void start();     ///< Starts the timer.
    duration stop();  ///< Stops the timer and returns the measured interval of wallclock time.

private:
    clock_time start_clk_t_;
    duration cumulative_dt_;
};


inline timer::timer()
    : start_clk_t_()
    , cumulative_dt_(0)
{
    cumulative_dt_ = cumulative_dt_.fixed_at(micro);
}


inline timer::timer(duration cumulative_dt)
    : start_clk_t_()
    , cumulative_dt_(cumulative_dt)
{
    cumulative_dt_ = cumulative_dt_.fixed_at(micro);
}

    
inline bool timer::timing() const
{
    return start_clk_t_ != clock_time();
}


inline duration timer::cumulative_duration() const
{
    return cumulative_dt_;
}


inline void timer::start()
{
    if (timing()) {
        throw std::runtime_error("Attempt to start timer that has already been started");
    }
    start_clk_t_ = clock::now();
}


inline duration timer::stop()
{
    auto stop_clk_t = clock::now();
    if (!timing()) {
        throw std::runtime_error("Attempt to stop timer that has not been started");
    }
    auto microsecond_count = std::chrono::duration_cast<std::chrono::microseconds>(stop_clk_t - start_clk_t_).count();
    auto interval_dt = duration(microsecond_count, micro).fixed_at(micro);
    cumulative_dt_ += interval_dt;
    start_clk_t_ = clock_time();
    return interval_dt;
}


}  // namespace

#endif
