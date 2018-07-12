#pragma once
#ifndef SYDEVS_SYSTEMS_REAL_TIME_SIMULATION_H_
#define SYDEVS_SYSTEMS_REAL_TIME_SIMULATION_H_

#include <sydevs/systems/simulation.h>
#include <sydevs/systems/real_time_buffer.h>
#include <sydevs/systems/interactive_system.h>

namespace sydevs {
namespace systems {


template<typename Node>
class real_time_simulation : public simulation<Node>
{
public:
    real_time_simulation(const time_point& start_t, const time_point& end_t, bool can_end_early, int64 seed, std::ostream& stream);

    real_time_simulation(duration total_dt, int64 seed, std::ostream& stream);

    virtual ~real_time_simulation() = default;  ///< Destructor   

    std::unique_ptr<interactive_system::data_injector> acquire_injector();
    std::unique_ptr<interactive_system::data_observer> acquire_observer();

    float64 time_advancement_rate() const;
    int64 time_advancement_precision() const;

    void update_time_advancement_rate(float64 ta_rate);
    void update_time_advancement_precision(int64 ta_precision);

    int64 frame_index() const;

    int64 process_next_frame();

private:
    void validate_interative();

    real_time_buffer ta_buffer_;
};


template<typename Node>
real_time_simulation<Node>::real_time_simulation(const time_point& start_t, const time_point& end_t, bool can_end_early, int64 seed, std::ostream& stream)
    : simulation<Node>(start_t, end_t, can_end_early, seed, stream)
    , ta_buffer_(std::numeric_limits<float64>::infinity(), 1)
{
    validate_interative();
}


template<typename Node>
real_time_simulation<Node>::real_time_simulation(duration total_dt, int64 seed, std::ostream& stream)
    : simulation<Node>::(total_dt, seed, stream)
    , ta_buffer_(std::numeric_limits<float64>::infinity(), 1)
{
    validate_interative();
}


template<typename Node>
std::unique_ptr<interactive_system::data_injector> real_time_simulation<Node>::acquire_injector()
{
    return top.acquire_injector();
}


template<typename Node>
std::unique_ptr<interactive_system::data_observer> real_time_simulation<Node>::acquire_observer()
{
    return top.acquire_observer();
}


template<typename Node>
float64 real_time_simulation<Node>::time_advancement_rate() const
{
    return ta_buffer_.time_advancement_rate();
}


template<typename Node>
int64 real_time_simulation<Node>::time_advancement_precision() const
{
    return ta_buffer_.time_precision_rate();
}


template<typename Node>
void real_time_simulation<Node>::update_time_advancement_rate(float64 ta_rate)
{
    ta_buffer_.update_time_advancement_rate(ta_rate);
}


template<typename Node>
void real_time_simulation<Node>::update_time_advancement_precision(int64 ta_precision)
{
    ta_buffer_.update_time_advancement_precision(ta_precision);
}


template<typename Node>
int64 real_time_simulation<Node>::frame_index() const
{
    return top.frame_index();
}


template<typename Node>
int64 real_time_simulation<Node>::process_next_frame()
{
    int64 event_count = 0;
    int64 index0 = frame_index();
    while (!finished_ && event_time().t() < t && frame_index() == index0) {
        event_count += process_next_events();
    }
    return event_count;
}


template<typename Node>
void real_time_simulation<Node>::validate_interative()
{
    static_assert(std::is_base_of<interactive_system, Node>::value, "Node must inherit from interactive_system");
}


}  // namespace
}  // namespace

#endif