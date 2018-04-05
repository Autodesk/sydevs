#pragma once
#ifndef SYDEVS_SYSTEMS_DISCRETE_EVENT_TIME_H_
#define SYDEVS_SYSTEMS_DISCRETE_EVENT_TIME_H_

#include <sydevs/time/time_point.h>

namespace sydevs {
namespace systems {


/**
 * @brief A data structure which represents progress through a simulation, 
 *        encapsulating both simulated time and a counter of events within a
 *        single point in simulated time.
 *
 * @details
 * A `discrete_event_time` object tracks both a simulated time point and the
 * number of events or steps that have occurred within that time point. 
 * Whenever the simulated time point advances, the counter of past events is
 * reset to zero. 
 *
 * Below is an example of how a `discrete_event_time` object can be initialized
 * and advanced.
 *
 * ~~~
 * auto start_t = time_point() + 100_s;             // Define start time.
 * auto end_t = time_point() + 400_s;               // Define end time.
 * auto event_time = discrete_event_time(start_t);  // Initialize.
 * event_time.advance(57_s, end_t);                 // Advance to 57_s, 0
 * event_time.advance();                            // Advance to 57_s, 1
 * event_time.advance(240_s, end_t);                // Advance to 297_s, 0
 * event_time.advance();                            // Advance to 297_s, 1
 * event_time.advance();                            // Advance to 297_s, 2
 * ~~~
 */
class discrete_event_time
{
public:
    /**
     * @brief Constructs a `discrete_event_time` object starting at time zero
     *        and a counter value of zero.
     */
    discrete_event_time();

    /**
     * @brief Constructs a `discrete_event_time` object starting at time `t`
     *        and a counter value of zero.
     */
    discrete_event_time(const time_point& t);

    /**
     * @brief Constructs a `discrete_event_time` object starting at time `t`
     *        and a counter value of `c`.
     */
    discrete_event_time(const time_point& t, int64 c);

    discrete_event_time(const discrete_event_time&)            = default;  ///< Copy constructor
    discrete_event_time& operator=(const discrete_event_time&) = default;  ///< Copy assignment
    discrete_event_time(discrete_event_time&&)                 = default;  ///< Move constructor
    discrete_event_time& operator=(discrete_event_time&&)      = default;  ///< Move assignment
    virtual ~discrete_event_time()                             = default;  ///< Destructor

    const time_point& t() const;  ///< Return the current point in simulated time.
    int64 t_index() const;        ///< Return the index associated with the current point in simulated time.
    int64 c() const;              ///< Return the counter value of associated with the current point in simulated time.

    void advance(duration dt, const time_point& end_t);  ///< Advance the simulated time point by `dt`, but not beyond `end_t`.
    void advance();                                      ///< Advance the counter without changing simulated time point.

private:
    time_point t_;
    int64 t_index_;
    int64 c_;
};


inline discrete_event_time::discrete_event_time()
    : t_()
    , t_index_(0)
    , c_(0)
{
}


inline discrete_event_time::discrete_event_time(const time_point& t)
    : t_(t)
    , t_index_(0)
    , c_(0)
{
}


inline discrete_event_time::discrete_event_time(const time_point& t, int64 c)
    : t_(t)
    , t_index_(0)
    , c_(c)
{
}


inline const time_point& discrete_event_time::t() const
{
    return t_;
}


inline int64 discrete_event_time::t_index() const
{
    return t_index_;
}


inline int64 discrete_event_time::c() const
{
    return c_;
}


inline void discrete_event_time::advance(duration dt, const time_point& end_t)
{
    if (dt == 0_s) {
        ++c_;
    }
    else {
        if (dt.finite()) {
            t_.advance(dt);
            if (t_ > end_t) {
                t_ = end_t;
            }
        }
        else {
            t_ = end_t;
        }
        ++t_index_;
        c_ = 0;
    }
}


inline void discrete_event_time::advance()
{
    c_++;
}


}  // namespace
}  // namespace

#endif
