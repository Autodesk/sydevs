#pragma once
#ifndef SYDEVS_SYSTEMS_REAL_TIME_SIMULATION_H_
#define SYDEVS_SYSTEMS_REAL_TIME_SIMULATION_H_

#include <sydevs/systems/simulation.h>
#include <sydevs/systems/real_time_buffer.h>
#include <sydevs/systems/interactive_system.h>

namespace sydevs {
namespace systems {


/**
 * @brief A class template for running simulations in real time.
 *
 * @details
 * The `real_time_simulation` class template represents a simulation run in real
 * time based on an `interactive_system` class specified as a template
 * parameter. Member functions allow the represented simulation to be performed
 * one frame at a time, where each frame includes however many events need to be
 * processed to advance the macro level of the `interactive_system` instance by
 * its planned duration.
 * 
 * A real time simulation is performed by repeatedly calling the
 * `process_frame_if_time_reached` member function. If the resulting event count
 * is zero, nothing happened because not enough wallclock time is deemed to have
 * passed. If the resulting event count is greater than zero, then the
 * simulation has advanced by one frame. It may then be time to observe data
 * from the simulation using the `observation` member function, or inject data
 * into the simulation using the `injection` member function.
 */
template<typename Node>
class real_time_simulation : public simulation<Node>
{
public:
    /**
     * @brief Constructs a `real_time_simulation` with the full set of 
     *        configuration options.
     * 
     * @param start_t The starting point in simulated time. 
     * @param end_t The end point in simulated time; only finalization events
     *              can occur at this time.
     * @param can_end_early Flag indicating that the simulation will end before
     *                      the specified end time if all events are exhausted.
     * @param seed The integer used to seed the random number generator.
     * @param stream The output steam onto which printed results are directed.
     *
     * @tparam Node The port-free `interactive_system` node on which the
     *              simulation is based.
     */
    real_time_simulation(const time_point& start_t, const time_point& end_t, bool can_end_early, int64 seed, std::ostream& stream);

    /**
     * @brief Constructs a `real_time_simulation` with a reduced set of
     *        configuration options.
     * 
     * @param total_dt The duration of the simulation in simulated time; the
     *                 simulation starts at time zero and does not end early.
     * @param seed The integer used to seed the random number generator.
     * @param stream The output steam onto which printed results are directed.
     *
     * @tparam Node The port-free `interactive_system` node on which the
     *              simulation is based.
     */
    real_time_simulation(duration total_dt, int64 seed, std::ostream& stream);

    virtual ~real_time_simulation() = default;  ///< Destructor   

    typename Node::injection_type& injection();            ///< Returns a reference to the injection object that supports communication into the interactive system node.
    const typename Node::observation_type& observation();  ///< Returns a reference to the observation object that supports communication from the interactive system node.

    float64 time_advancement_rate() const;      ///< Returns the time advancement rate.
    float64 time_synchronization_rate() const;  ///< Returns the time synchronization rate.

    void update_time_advancement_rate(float64 t_adv_rate);      ///< Updates the time advancement rate.
    void update_time_synchronization_rate(float64 t_syn_rate);  ///< Updates the time synchonization rate.

    time_point synchronization_time() const;        ///< Returns the simulated time of the synchronization reference point.
    clock_time synchronization_clock_time() const;  ///< Returns the wallclock time of the synchronization reference point.

    void update_synchronization_time(const time_point& sim_t, const clock_time& clk_t);  ///< Updates the synchonization reference point

    int64 frame_index() const;            ///< Returns the index of the most recently processed frame of the simulation.
    time_point frame_time() const;        ///< Returns the simulated time point of the most recently processed frame of the simulation.
    clock_time frame_clock_time() const;  ///< Returns the wallclock time point of the most recently processed frame of the simulation.

    int64 process_frame_if_time_reached();  ///< If a sufficent amount of wallclock time has elapsed, processes all events until the frame index has advanced.

private:
    std::unique_ptr<typename Node::interaction_data> interaction_data_ptr_;
    real_time_buffer ta_buffer_;
};


template<typename Node>
inline real_time_simulation<Node>::real_time_simulation(const time_point& start_t, const time_point& end_t, bool can_end_early, int64 seed, std::ostream& stream)
    : simulation<Node>(start_t, end_t, can_end_early, seed, stream)
    , interaction_data_ptr_()
    , ta_buffer_(std::numeric_limits<float64>::infinity(), 0.0)
{
    interaction_data_ptr_ = this->top.acquire_interaction_data();
}


template<typename Node>
inline real_time_simulation<Node>::real_time_simulation(duration total_dt, int64 seed, std::ostream& stream)
    : simulation<Node>(total_dt, seed, stream)
    , interaction_data_ptr_()
    , ta_buffer_(std::numeric_limits<float64>::infinity(), 0.0)
{
    interaction_data_ptr_ = this->top.acquire_interaction_data();
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
inline float64 real_time_simulation<Node>::time_synchronization_rate() const
{
    return ta_buffer_.time_synchronization_rate();
}


template<typename Node>
inline void real_time_simulation<Node>::update_time_advancement_rate(float64 t_adv_rate)
{
    if (t_adv_rate <= 0.0) throw std::invalid_argument("Time advancement rate must be positive");
    ta_buffer_.update_time_advancement_rate(t_adv_rate);
}


template<typename Node>
inline void real_time_simulation<Node>::update_time_synchronization_rate(float64 t_syn_rate)
{
    if (t_syn_rate < 0.0) throw std::invalid_argument("Time advancement depth must be non-negative");
    ta_buffer_.update_time_synchronization_rate(t_syn_rate);
}


template<typename Node>
inline time_point real_time_simulation<Node>::synchronization_time() const
{
    return ta_buffer_.synchronization_time();
}


template<typename Node>
inline clock_time real_time_simulation<Node>::synchronization_clock_time() const
{
    return ta_buffer_.synchronization_clock_time();
}


template<typename Node>
inline void real_time_simulation<Node>::update_synchronization_time(const time_point& sim_t, const clock_time& clk_t)
{
    ta_buffer_.update_synchronization_time(sim_t, clk_t);
}

    
template<typename Node>
inline int64 real_time_simulation<Node>::frame_index() const
{
    return this->top.frame_index();
}


template<typename Node>
time_point real_time_simulation<Node>::frame_time() const
{
    return ta_buffer_.current_time();
}

    
template<typename Node>
clock_time real_time_simulation<Node>::frame_clock_time() const
{
    return ta_buffer_.current_clock_time();
}


template<typename Node>
inline int64 real_time_simulation<Node>::process_frame_if_time_reached()
{
    int64 event_count = 0;
    if (!this->finished()) {
        auto t = this->time().t();
        auto clock_t = clock::now();
        if (clock_t >= ta_buffer_.planned_clock_time()) {
            int64 current_frame_index = frame_index();
            auto done = false;
            while (!done) {
                event_count += this->process_next_events();
                done = this->finished() || (frame_index() > current_frame_index);
                if (!done) {
                    t = this->time().t();
                    clock_t = clock::now();
                }
            }
            ta_buffer_.update_current_time(t, clock_t, this->top.planned_duration());
        }
    }
    return event_count;
}


}  // namespace
}  // namespace

#endif
