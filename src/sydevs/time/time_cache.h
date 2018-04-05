#pragma once
#ifndef SYDEVS_TIME_CACHE_H_
#define SYDEVS_TIME_CACHE_H_

#include <sydevs/time/time_queue.h>

namespace sydevs {


/**
 * @brief A data structure which provides durations elapsed since past events.
 *
 * A `time_cache` object represents past events in order to obtain elapsed
 * event times as required by DEVS. Like a time queue, the time cache has a
 * current time that can be advanced. Events should be added using the
 * `retain_event` member function at the time when the event occurs and at the
 * scale at which the event was scheduled. The `duration_since` member function
 * is used to obtain that elapsed time for the retained event, which increases
 * from zero as time is advanced after the associated `retain_event` call.
 *
 * The `time_cache` data structure, which tracks past events, is implemented
 * using a time queue, which tracks future events. This works by way of a trick
 * where each event time is offset forward in time by 999,999,999,999,999 times
 * the specified precision. Subtracting the planned duration from this number
 * gives the elapsed duration.
 */
class time_cache
{
public:
    time_cache();                               ///< Constructs an empty time cache with a current time of zero
    explicit time_cache(duration dt0);          ///< Constructs an empty time cache with a current time of `dt0`
    explicit time_cache(const time_point& t0);  ///< Constructs an empty time cache with a current time of `t0`

    time_cache(const time_cache&)            = default;  ///< Copy constructor
    time_cache& operator=(const time_cache&) = default;  ///< Copy assignment
    time_cache(time_cache&&)                 = default;  ///< Move constructor
    time_cache& operator=(time_cache&&)      = default;  ///< Move assignment
    ~time_cache()                            = default;  ///< Destructor

    bool empty() const;  ///< Returns `true` if the time cache is empty.
    int64 size() const;  ///< Returns the number of events in the time cache.

    const time_point& current_time() const;  ///< Returns the current time.

    const time_point& advance_time(duration dt);          ///< Advances the current time by `dt`.
    const time_point& advance_time(const time_point& t);  ///< Advances the current time to `t`.

    duration duration_since(int64 event_id) const;  ///< Returns the duration since the specified event, or an infinite duration.

    void retain_event(int64 event_id, scale precision);  ///< Retains a record of the event with id `event_id` at the specified `precision`.
    bool release_event(int64 event_id);                  ///< Erases the record of the event with id `event_id`, returning `true` if successful.

    const time_queue::event_id_set& event_ids() const;  ///< Return the set of ids of all retained events.

private:
    time_queue tq_;
    time_queue::event_id_set event_ids_;
};


inline bool time_cache::empty() const
{
    return event_ids_.empty();
}


inline int64 time_cache::size() const
{
    return event_ids_.size();
}


inline const time_point& time_cache::current_time() const
{
    return tq_.current_time();
}


inline const time_queue::event_id_set& time_cache::event_ids() const
{
    return event_ids_;
}


}  // namespace

#endif
