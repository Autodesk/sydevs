#pragma once
#ifndef SYDEVS_TIME_QUEUE_H_
#define SYDEVS_TIME_QUEUE_H_

#include <sydevs/time/time_point.h>
#include <map>
#include <set>
#include <deque>

namespace sydevs {


/**
 * @brief A data structure which supports the scheduling of future events.
 *
 * A `time_queue` object represents a list of scheduled events and their
 * associated times, saving space by storing `duration` values instead of
 * `time_point` objects. All durations associated with the class interface are
 * expressed relative to the current time, which is the only time point stored.
 *
 * Each one of the stored phase durations is expressed relative to the start of
 * one of two epochs associated with its precision: the epoch containing the
 * current time or the subsequent epoch. Because the current time advances only
 * forwards, and because the planned duration of event cannot exceed one epoch
 * duration, it is always possible to determine which of the two epochs
 * contains the scheduled event.
 *
 * It is important to correctly interpret the durations used to plan events and
 * report the remaining duration until these events. A `duration dt` is assumed
 * to `advance` the current time (as per `time_point::advance`), not add to it.
 * Hence, for `time_queue tq`, the actual time of a planned event is
 * `tq.current_time().advance(dt)`, not `tq.current_time() + dt`.
 */
class time_queue
{
public:
    using event_id_set = std::set<int64>;  ///< The container used to store ids of events occurring at the same time.

    time_queue();                               ///< Constructs an empty time queue with a current time of zero
    explicit time_queue(duration dt0);          ///< Constructs an empty time queue with a current time of `dt0`
    explicit time_queue(const time_point& t0);  ///< Constructs an empty time queue with a current time of `t0`

    time_queue(const time_queue&)             = default;  ///< Copy constructor
    time_queue& operator=(const time_queue&)  = default;  ///< Copy assignment
    time_queue(time_queue&&)                  = default;  ///< Move constructor
    time_queue& operator=(time_queue&&)       = default;  ///< Move assignment
    ~time_queue()                             = default;  ///< Destructor

    bool empty() const;        ///< Returns `true` if the time queue is empty.
    int64 size() const;        ///< Returns the number of events in the time queue.
    int64 time_count() const;  ///< Returns the number of distinct event times in the time queue.

    const time_point& current_time() const;  ///< Returns the current time.

    const time_point& advance_time();                    ///< Advances the current time to that of the imminent events.
    const time_point& advance_time(duration dt);         ///< Advances the current time by `dt`.
    const time_point& advance_time(const time_point t);  ///< Advances the current time to `t`.

    bool can_advance_by(duration dt);  ///< Returns `true` if time can be advanced by the specified duration.

    duration imminent_duration() const;             ///< Returns the duration until the imminent event(s).
    duration duration_until(int64 event_id) const;  ///< Returns the duration until the event with id `event_id`, or infinity if no such event exists.
    duration duration_at(int64 time_index) const;   ///< Returns the duration at `time_index`.

    const event_id_set& imminent_event_ids() const;            ///< Returns the imminent events.
    const event_id_set& event_ids_at(int64 time_index) const;  ///< Returns the events at `time_index`.

    void pop_imminent_event(int64 event_id);  ///< Pops the specified imminent event with id `event_id`.
    void pop_imminent_events();               ///< Pops the imminent events.

    void plan_event(int64 event_id, duration dt);  ///< Schedules an event with id `event_id` after a planned duration of `dt`.
    bool cancel_event(int64 event_id);             ///< Cancels the event with id `event_id`, returning `true` if successful.

private:
    using event_account = std::pair<duration, scale>;
    using queue_iterator = std::deque<duration>::iterator;

    duration duration_from_phase(duration pdt) const;
    duration refined_duration(duration dt, scale precision) const;
    duration refined_duration_from_phase(duration pdt, scale precision) const;
    duration rescaled_duration_from_phase(duration pdt, scale precision) const;
    duration phase_from_duration(duration dt);

    queue_iterator lower_bound(duration pdt);

    scale default_precision_;
    time_point ct_;
    std::deque<duration> queue_;
    std::map<duration, event_id_set> event_id_sets_;
    std::map<int64, event_account> event_accounts_;
};


inline bool time_queue::empty() const
{
    return event_accounts_.empty();
}


inline int64 time_queue::size() const
{
    return int64(event_accounts_.size());
}


inline int64 time_queue::time_count() const
{
    return int64(event_id_sets_.size());
}


inline const time_point& time_queue::current_time() const
{
    return ct_;
}


}  // namespace

#endif
