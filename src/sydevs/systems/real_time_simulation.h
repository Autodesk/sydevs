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

    typename Node::injection_type& injection();
    const typename Node::observation_type& observation();

    float64 time_advancement_rate() const;
    int64 time_advancement_depth() const;

    void update_time_advancement_rate(float64 ta_rate);
    void update_time_advancement_depth(int64 ta_depth);

    int64 frame_index() const;

    int64 process_frame_if_time_reached();

private:
    void validate_interative();

    std::unique_ptr<typename Node::interaction_data> interaction_data_ptr_;
    real_time_buffer ta_buffer_;
};


template<typename Node>
inline real_time_simulation<Node>::real_time_simulation(const time_point& start_t, const time_point& end_t, bool can_end_early, int64 seed, std::ostream& stream)
    : simulation<Node>(start_t, end_t, can_end_early, seed, stream)
    , interaction_data_ptr_()
    , ta_buffer_(std::numeric_limits<float64>::infinity(), 1)
{
    validate_interative();
    interaction_data_ptr_.reset(top.acquire_interaction_data())
}


template<typename Node>
inline real_time_simulation<Node>::real_time_simulation(duration total_dt, int64 seed, std::ostream& stream)
    : simulation<Node>::(total_dt, seed, stream)
    , interaction_data_ptr_()
    , ta_buffer_(std::numeric_limits<float64>::infinity(), 1)
{
    validate_interative();
    interaction_data_ptr_.reset(top.acquire_interaction_data())
}


template<typename Node>
inline typename Node::injection_type& real_time_simulation<Node>::injection()
{
    return interaction_data_ptr_->injection();
}


template<typename Node>
inline const typename Node::observation_type& real_time_simulation<Node>::observation()
{
    return interaction_data_ptr_->observation();
}


template<typename Node>
inline float64 real_time_simulation<Node>::time_advancement_rate() const
{
    return ta_buffer_.time_advancement_rate();
}


template<typename Node>
inline int64 real_time_simulation<Node>::time_advancement_depth() const
{
    return ta_buffer_.time_advancement_depth();
}


template<typename Node>
inline void real_time_simulation<Node>::update_time_advancement_rate(float64 ta_rate)
{
    ta_buffer_.update_time_advancement_rate(ta_rate);
}


template<typename Node>
inline void real_time_simulation<Node>::update_time_advancement_depth(int64 ta_depth)
{
    ta_buffer_.update_time_advancement_depth(ta_depth);
}


template<typename Node>
inline int64 real_time_simulation<Node>::frame_index() const
{
    return top.frame_index();
}


template<typename Node>
inline int64 real_time_simulation<Node>::process_frame_if_time_reached()
{
    int64 event_count = 0;
    if (!finished()) {
        auto t = time().t();
        auto clock_t = clock::now();
        if (clock_t >= ta_buffer_.planned_clock_time()) {
            int64 current_frame_index = frame_index();
            auto done = false;
            while (!done) {
                event_count += process_next_events();
                done = finished() || (frame_index() > current_frame_index);
                if (!done) {
                    t = time().t();
                    clock_t = clock::now();
                }
            }
            ta_buffer_.retain(t, clock_t);
        }
    }
    return event_count;
}


template<typename Node>
inline void real_time_simulation<Node>::validate_interative()
{
    static_assert(std::is_base_of<interactive_system, Node>::value, "Node must inherit from interactive_system");
}


}  // namespace
}  // namespace

#endif
