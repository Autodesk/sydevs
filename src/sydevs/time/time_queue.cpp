#include <sydevs/time/time_queue.h>
#include <algorithm>

namespace sydevs {


time_queue::time_queue()
    : default_precision_()
    , ct_()
    , queue_()
    , event_id_sets_()
    , event_accounts_()
{
    default_precision_ = ct_.precision() + ct_.nscales();
}


time_queue::time_queue(duration dt0)
    : default_precision_()
    , ct_(dt0)
    , queue_()
    , event_id_sets_()
    , event_accounts_()
{
    default_precision_ = ct_.precision() + ct_.nscales();
}


time_queue::time_queue(const time_point& t0)
    : default_precision_()
    , ct_(t0)
    , queue_()
    , event_id_sets_()
    , event_accounts_()
{
    default_precision_ = ct_.precision() + ct_.nscales();
}


const time_point& time_queue::advance_time()
{
    if (empty()) throw std::out_of_range("No events exist to constrain time advancement");
    return ct_.advance(imminent_duration());
}


const time_point& time_queue::advance_time(duration dt)
{
    if (!can_advance_by(dt)) throw std::out_of_range("Advancement duration must not set current time later than imminent events");
    return ct_.advance(dt);
}


const time_point& time_queue::advance_time(const time_point t)
{
    if (t < ct_) throw std::out_of_range("Advancement time must be later than current time of time queue");
    while (ct_ < t) {
        advance_time(t.gap(ct_));
    }
    if (ct_ != t) throw std::logic_error("Unexpected error occurred while advancing time queue to specified time");
    return ct_;
}


bool time_queue::can_advance_by(duration dt)
{
    auto okay = true;
    if (dt != 0_s) {
        if (!dt.valid()) throw std::invalid_argument("Advancement duration must be valid");
        if (dt < 0_s) throw std::domain_error("Advancement duration must be non-negative");
        if (!dt.finite()) throw std::domain_error("Advancement duration must be finite");
        auto imminent_dt = imminent_duration();
        if (imminent_dt.finite()) {
            auto precision = std::min(dt.precision(), imminent_dt.precision());
            okay = refined_duration(dt, precision) <= refined_duration(imminent_dt, precision);
        }
    }
    return okay;
}


duration time_queue::imminent_duration() const
{
    auto dt = duration::inf();
    if (!empty()) {
        dt = duration_from_phase(queue_.front());
    }
    return dt;
}


duration time_queue::duration_until(int64 event_id) const
{
    auto dt = duration::inf();
    auto ea_iter = event_accounts_.find(event_id);
    if (ea_iter != std::end(event_accounts_)) {
        const event_account& ea = ea_iter->second;
        dt = rescaled_duration_from_phase(ea.first, ea.second);
    }
    return dt;
}


duration time_queue::duration_at(int64 time_index) const
{
    if (time_index < 0) throw std::domain_error("Duration time index must be non-negative");
    if (time_index >= time_count()) throw std::out_of_range("Duration time index must be less than the number of distinct event times");
    return duration_from_phase(queue_[time_index]);
}


const time_queue::event_id_set& time_queue::imminent_event_ids() const
{
    if (empty()) throw std::out_of_range("No imminent events exist");
    return event_id_sets_.at(queue_.front());
}


const time_queue::event_id_set& time_queue::event_ids_at(int64 time_index) const
{
    if (time_index < 0) throw std::domain_error("Event time index must be non-negative");
    if (time_index >= time_count()) throw std::out_of_range("Event time index must be less than the number of distinct event times");
    return event_id_sets_.at(queue_[time_index]);
}


void time_queue::pop_imminent_event(int64 event_id)
{
    if (empty()) throw std::out_of_range("No imminent events exist to search for specified id");
    auto pdt = queue_.front();
    auto es_iter = event_id_sets_.find(pdt);
    auto e_iter = es_iter->second.find(event_id);
    if (e_iter == std::end(es_iter->second)) throw std::out_of_range("No imminent event has specified id");
    if (es_iter->second.size() > 1) {
        es_iter->second.erase(e_iter);
    }
    else {
        queue_.pop_front();
        event_id_sets_.erase(es_iter);
    }
    event_accounts_.erase(event_id);
}


void time_queue::pop_imminent_events()
{
    if (empty()) throw std::out_of_range("No imminent events exist to be popped");
    auto pdt = queue_.front();
    for (auto event_id : event_id_sets_[pdt]) {
        event_accounts_.erase(event_id);
    }
    queue_.pop_front();
    event_id_sets_.erase(pdt);
}


void time_queue::plan_event(int64 event_id, duration dt)
{
    if (!dt.valid()) throw std::invalid_argument("Planned duration must be valid");
    if (dt < 0_s) throw std::domain_error("Planned duration must be non-negative");
    if (!dt.finite()) throw std::domain_error("Planned duration must be finite");
    cancel_event(event_id);
    if (dt.finite()) {
        auto pdt = phase_from_duration(dt);
        auto q_iter = lower_bound(pdt);
        if (q_iter == std::end(queue_) || *q_iter != pdt) {
            queue_.insert(q_iter, pdt);
            event_id_sets_[pdt] = {event_id};
        }
        else {
            event_id_sets_[pdt].insert(event_id);
        }
        event_accounts_[event_id] = {pdt, dt.precision()};
    }
}


bool time_queue::cancel_event(int64 event_id)
{
    auto removed = false;
    auto ea_iter = event_accounts_.find(event_id);
    if (ea_iter != std::end(event_accounts_)) {
        auto pdt = ea_iter->second.first;
        auto es_iter = event_id_sets_.find(pdt);
        if (es_iter->second.size() > 1) {
            es_iter->second.erase(event_id);
        }
        else {
            auto q_iter = lower_bound(pdt);
            queue_.erase(q_iter);
            event_id_sets_.erase(es_iter);
        }
        event_accounts_.erase(ea_iter);
        removed = true;
    }
    return removed;
}


duration time_queue::duration_from_phase(duration pdt) const
{
    int64 multiplier = pdt.multiplier() - ct_.epoch_phase(pdt.precision());
    if (multiplier < 0) {
        multiplier += quantity_limit;
    }
    return duration(multiplier, pdt.precision());
}


duration time_queue::refined_duration(duration dt, scale precision) const
{
    int64 multiplier = dt.multiplier();
    if (multiplier > 0) {  // Refine the duration without losing precision.
        for (auto s = dt.precision() - 1; multiplier < quantity_limit && s >= precision; --s) {
            multiplier = 1000*multiplier - ct_.scale_phase(s);
        }
    }
    return duration(multiplier, precision);
}


duration time_queue::refined_duration_from_phase(duration pdt, scale precision) const
{
    auto dt = duration_from_phase(pdt);
    return refined_duration(dt, precision);
}


duration time_queue::rescaled_duration_from_phase(duration pdt, scale precision) const
{
    auto dt = duration_from_phase(pdt);
    if (precision <= dt.precision()) {  // Refine the duration.
        dt = refined_duration(dt, precision);
    }
    else {  // Coarsening is needed, which is only possible of the event time is zero and dt.multiplier() is either 0 or 1.
        dt = duration(dt.multiplier(), precision);
    }
    return dt;
}


duration time_queue::phase_from_duration(duration dt)
{
    auto precision = dt.precision();
    if (dt.multiplier() == 0) {
        precision = ct_.precision();
    }
    int64 phase = ct_.epoch_phase(precision) + dt.multiplier();
    {  // Coarsen the phase duration without losing precision.
        auto coarseness_maximized = false;
        auto coarseness_unbounded = false;
        while (!coarseness_maximized && !coarseness_unbounded) {
            auto carry = 0;
            if (phase >= quantity_limit) {
                phase -= quantity_limit;
                carry = 1;
            }
            if (phase%1000 != 0) {
                coarseness_maximized = true;
            }
            else if ((phase == 0) && (precision + 5 >= ct_.precision() + ct_.nscales())) {
                if (ct_.sign() == 1) {
                    coarseness_unbounded = (carry == 0);
                }
                else {
                    coarseness_unbounded = (carry == 1);
                }
            }
            if (!coarseness_maximized && !coarseness_unbounded) {
                phase = phase/1000 + (quantity_limit/1000)*(ct_.scale_phase(precision + 5) + carry);
                ++precision;
            }
        }
        if (coarseness_unbounded) {
            precision = default_precision_;
        }
    }
    return duration(phase, precision);
}


time_queue::queue_iterator time_queue::lower_bound(duration pdt)
{
    auto q_comp = [this](duration entry, duration value) {
        auto precision = std::min(entry.precision(), value.precision());
        auto entry_dt = refined_duration_from_phase(entry, precision);
        auto value_dt = refined_duration_from_phase(value, precision);
        return entry_dt < value_dt;
    };
    return std::lower_bound(std::begin(queue_), std::end(queue_), pdt, q_comp);
}


}  // namespace
