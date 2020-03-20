#pragma once
#ifndef SYDEVS_SYSTEMS_SIMULATION_H_
#define SYDEVS_SYSTEMS_SIMULATION_H_

#include <sydevs/systems/system_node.h>
#include <sydevs/time/time_queue.h>
#include <sydevs/time/time_cache.h>

namespace sydevs {
namespace systems {


/**
 * @brief A class template for running simulations.
 *
 * @details
 * The `simulation` class template represents a simulation run based on a
 * `system_node` class specified as a template parameter. Member functions
 * allow the represented simulation to be performed one event at a time or 
 * until completion.
 * 
 * Below is an example of how a `simulation` object can be instantiated and
 * used.
 *
 * ~~~
 * try {
 *     // Create an object to simulate a building for 5 minutes of simulated
 *     // time with a random seed of 0 and printed results directed to std::cout.
 *     simulation<building_closed_system> sim(5_min, 0, std::cout);
 *
 *     // Print the initial temperature.
 *     sim.top.initial_temperature.parameter.print_on_use();
 *
 *     // Print every outdoor temperature send from the weather node.
 *     sim.top.building_dynamics.weather.outdoor_temperature_output.print_on_use();
 *
 *     // Print every outdoor temperature received by the thermodynamics node.
 *     sim.top.building_dynamics.thermodynamics.outdoor_temperature_input.print_on_use();
 *
 *     // Print every event experienced by the occupant node.
 *     sim.top.building_dynamics.occupant.print_on_event();
 *
 *     // Run the simulation until completion.
 *     sim.process_remaining_events();
 * }
 * catch (const system_node::error& e) {
 *     std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
 * }
 * catch (const std::exception& e) {
 *     std::cout << "OTHER ERROR: " << e.what() << std::endl;
 * }
 * ~~~
 *
 * It is also possible to obtain wallclock times at each level in a model
 * hierarchy, the cumulative time that the computer has spent executing events.
 *
 * ~~~
 * // [...]
 *
 * // Run the simulation until completion.
 * sim.process_remaining_events();
 *
 * // Obtain and print cumulative wallclock time of all nodes.
 * auto sim_dt = sim.event_timer().cumulative_duration();
 * std::cout << "sim: " << sim_dt << std::endl;
 *
 * // Obtain and print cumulative wallclock time of all event functions in single node.
 * auto node_dt = sim.top.building_dynamics.thermodynamics.event_timer().cumulative_duration();
 * std::cout << "sim.top.building_dynamics.thermodynamics: " << node_dt << std::endl;
 * ~~~
 */
template<typename Node>
class simulation
{
public:
    /**
     * @brief Constructs a `simulation` with the full set of configuration
     *        options.
     * 
     * @param start_t The starting point in simulated time. 
     * @param end_t The end point in simulated time; only finalization events
     *              can occur at this time.
     * @param can_end_early Flag indicating that the simulation will end before
     *                      the specified end time if all events are exhausted.
     * @param seed The integer used to seed the random number generator.
     * @param stream The output steam onto which printed results are directed.
     *
     * @tparam Node The port-free `system_node` on which the simulation is 
     *              based.
     */
    simulation(const time_point& start_t, const time_point& end_t, bool can_end_early, int64 seed, std::ostream& stream);

    /**
     * @brief Constructs a `simulation` with a reduced set of configuration
     *        options.
     * 
     * @param total_dt The duration of the simulation in simulated time; the
     *                 simulation starts at time zero and does not end early.
     * @param seed The integer used to seed the random number generator.
     * @param stream The output steam onto which printed results are directed.
     *
     * @tparam Node The port-free `system_node` on which the simulation is 
     *              based.
     */
    simulation(duration total_dt, int64 seed, std::ostream& stream);

    simulation(const simulation&)            = delete;   ///< No copy constructor
    simulation& operator=(const simulation&) = delete;   ///< No copy assignment
    simulation(simulation&&)                 = delete;   ///< No move constructor
    simulation& operator=(simulation&&)      = delete;   ///< No move assignment
    virtual ~simulation()                    = default;  ///< Destructor   

    const time_point& start_time() const;  ///< Returns the start time of the simulation.
    const time_point& end_time() const;    ///< Returns the end time of the simulation.
    bool can_end_early() const;            ///< Returns `true` if the simulation can end before the specified end time.

    bool started() const;    ///< Returns `true` if the simulation has started.
    bool finishing() const;  ///< Returns `true` if all events are finished except finalization.
    bool finished() const;   ///< Returns `true` if the simulation has finished.

    const discrete_event_time& time() const;  ///< Returns the current point in discrete event time.
    duration imminent_duration() const;       ///< Returns the duration until the imminent event.

    void process_next_event();                        ///< Runs the next event of the topmost system node.
    int64 process_next_events();                      ///< Runs all events until simulated time advances; returns the number of processed events.
    int64 process_events_until(const time_point& t);  ///< Runs all events until simulated time advances at least to `t`; returns the number of processed events.
    int64 process_remaining_events();                 ///< Runs simulation until completion; returns the number of processed events.

    const timer& event_timer() const;  ///< Returns the object that accumulated wallclock event durations.

private:
    node_interface& top_IO();
    discrete_event_time& event_time();
    void validate();

    void process_initialization_event();
    void process_planned_event();
    void process_finalization_event();
    void advance_time();

    const time_point start_t_;       // (must be declared before member variable external_context_)
    const time_point end_t_;
    bool can_end_early_;
    node_context external_context_;  // (must be declared before member variable top)
    bool started_;
    bool finishing_;
    bool finished_;
    time_queue t_queue_;
    time_cache t_cache_;
    timer event_timer_;

public:
    Node top;  ///< The topmost system node.
};


template<typename Node>
inline simulation<Node>::simulation(const time_point& start_t, const time_point& end_t, bool can_end_early, int64 seed, std::ostream& stream)
    : start_t_(start_t)
    , end_t_(end_t)
    , can_end_early_(can_end_early)
    , external_context_(start_t, seed, stream)
    , started_(false)
    , finishing_(false)
    , finished_(false)
    , t_queue_(start_t)
    , t_cache_(start_t)
    , event_timer_()
    , top("top", external_context_)
{
    validate();
}


template<typename Node>
inline simulation<Node>::simulation(duration total_dt, int64 seed, std::ostream& stream)
    : start_t_()
    , end_t_(total_dt.finite() ? time_point() + total_dt : 
                                 time_point() + duration(1, scale(std::numeric_limits<scale::level_type>::max() - 6)))
    , can_end_early_(!total_dt.finite())
    , external_context_(start_t_, seed, stream)
    , started_(false)
    , finishing_(false)
    , finished_(false)
    , t_queue_()
    , t_cache_()
    , event_timer_()
    , top("top", external_context_)
{
    validate();
}


template<typename Node>
inline const time_point& simulation<Node>::start_time() const
{
    return start_t_;
}


template<typename Node>
inline const time_point& simulation<Node>::end_time() const
{
    return end_t_;
}

    
template<typename Node>
inline bool simulation<Node>::can_end_early() const
{
    return can_end_early_;
}


template<typename Node>
inline bool simulation<Node>::started() const
{
    return started_;
}


template<typename Node>
inline bool simulation<Node>::finishing() const
{
    return finishing_;
}

    
template<typename Node>
inline bool simulation<Node>::finished() const
{
    return finished_;
}


template<typename Node>
inline const discrete_event_time& simulation<Node>::time() const
{
    return const_cast<node_context&>(external_context_).event_time();
}


template<typename Node>
inline duration simulation<Node>::imminent_duration() const
{
    return t_queue_.imminent_duration();
}


template<typename Node>
inline void simulation<Node>::process_next_event()
{
    if (!finished_) {
        if (!finishing_) {
            if (!started_) {
                process_initialization_event();
            }
            else {
                process_planned_event();
            }
            advance_time();        
        }
        else {
            process_finalization_event();
        }
    }
}


template<typename Node>
inline int64 simulation<Node>::process_next_events()
{
    int64 event_count = 0;
    auto t = event_time().t();
    while (!finished_ && event_time().t() == t) {
        process_next_event();
        ++event_count;
    }
    return event_count;
}


template<typename Node>
inline int64 simulation<Node>::process_events_until(const time_point& t)
{
    int64 event_count = 0;
    while (!finished_ && event_time().t() < t) {
        process_next_event();
        ++event_count;
    }
    return event_count;
}


template<typename Node>
inline int64 simulation<Node>::process_remaining_events()
{
    int64 event_count = 0;
    while (!finished_) {
        process_next_event();
        ++event_count;
    }
    return event_count;
}


template<typename Node>
inline const timer& simulation<Node>::event_timer() const
{
    return event_timer_;
}


template<typename Node>
inline node_interface& simulation<Node>::top_IO()
{
    return const_cast<node_interface&>(top.external_interface());
}


template<typename Node>
inline discrete_event_time& simulation<Node>::event_time()
{
    return external_context_.event_time();
}


template<typename Node>
inline void simulation<Node>::validate()
{
    static_assert(std::is_base_of<system_node, Node>::value, "Node must inherit from system_node");

    if (top_IO().flow_input_port_count() != 0 ||
        top_IO().message_input_port_count() != 0 ||
        top_IO().message_output_port_count() != 0 ||
        top_IO().flow_output_port_count() != 0) {
        throw std::invalid_argument("Node to be simulated must have no ports");
    }
}


template<typename Node>
inline void simulation<Node>::process_initialization_event()
{
    started_ = true;
    top_IO().print_event("initialization");
    top_IO().activate(flow, input);
    event_timer_.start();
    auto planned_dt = top.process_initialization_event();
    event_timer_.stop();
    top_IO().deactivate();
    if (planned_dt.finite()) {
        t_queue_.plan_event(0, planned_dt);
    }
    if (top.time_precision() != no_scale) {
        t_cache_.retain_event(0, top.time_precision());
    }
}


template<typename Node>
inline void simulation<Node>::process_planned_event()
{
    top_IO().print_event("planned");
    auto elapsed_dt = duration();
    if (top.time_precision() != no_scale) {
        elapsed_dt = t_cache_.duration_since(0).fixed_at(top.time_precision());
    }
    top_IO().activate(message, output);
    event_timer_.start();
    auto planned_dt = top.process_planned_event(elapsed_dt);
    event_timer_.stop();
    top_IO().deactivate();
    if (planned_dt.finite()) {
        t_queue_.plan_event(0, planned_dt);
    }
    else {
        t_queue_.pop_imminent_event(0);
    }
    if (top.time_precision() != no_scale) {
        t_cache_.retain_event(0, top.time_precision());
    }
}


template<typename Node>
inline void simulation<Node>::process_finalization_event()
{
    top_IO().print_event("finalization");
    auto elapsed_dt = duration();
    if (top.time_precision() != no_scale) {
        elapsed_dt = t_cache_.duration_since(0).fixed_at(top.time_precision());
    }
    top_IO().activate(flow, output);
    event_timer_.start();
    top.process_finalization_event(elapsed_dt);
    event_timer_.stop();
    top_IO().deactivate();
    finished_ = true;
}


template<typename Node>
inline void simulation<Node>::advance_time()
{
    if (!finishing_) {
        auto planned_dt = t_queue_.imminent_duration();
        if (planned_dt.finite() || !can_end_early_) {
            // The event time must advance.
            event_time().advance(planned_dt, end_t_);
            if (planned_dt > 0_s) {
                // The simulated time just advanced.
                external_context_.time_printed() = false;
                t_queue_.advance_time(event_time().t());
                t_cache_.advance_time(event_time().t());
            }
            if (event_time().t() >= end_t_) {
                // The end time has been reached.
                finishing_ = true;
            }
        }
        else {
            // The simulation must finish at the current event time.
            finishing_ = true;
        }
    }
}


}  // namespace
}  // namespace

#endif
