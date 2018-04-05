#include <sydevs/time/time_cache.h>

namespace sydevs {


time_cache::time_cache()
    : tq_()
    , event_ids_()
{
}


time_cache::time_cache(duration dt0)
    : tq_(dt0)
    , event_ids_()
{
}


time_cache::time_cache(const time_point& t0)
    : tq_(t0)
    , event_ids_()
{
}


const time_point& time_cache::advance_time(duration dt)
{
    while (!tq_.can_advance_by(dt)) {
        for (auto event_id : tq_.imminent_event_ids()) {
            event_ids_.erase(event_id);
        }
        tq_.pop_imminent_events();
    }
    return tq_.advance_time(dt);
}


const time_point& time_cache::advance_time(const time_point& t)
{
    if (t < current_time()) throw std::out_of_range("Advancement time must be later than current time of time cache");
    while (current_time() < t) {
        advance_time(t.gap(current_time()));
    }
    if (current_time() != t) throw std::logic_error("Unexpected error occurred while advancing time cache to specified time");
    return current_time();
}


duration time_cache::duration_since(int64 event_id) const
{
    auto dt = duration::inf();
    auto until_dt = tq_.duration_until(event_id);
    if (until_dt.finite()) {
        dt = duration(quantity_limit - 1 - until_dt.multiplier(), until_dt.precision());
    }
    return dt;
}


void time_cache::retain_event(int64 event_id, scale precision)
{
    tq_.plan_event(event_id, duration(quantity_limit - 1, precision));
    event_ids_.insert(event_id);
}


bool time_cache::release_event(int64 event_id)
{
    bool removed = tq_.cancel_event(event_id);
    if (removed) {
        event_ids_.erase(event_id);
    }
    return removed;
}


}  // namespace
