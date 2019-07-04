#pragma once
#ifndef SYDEVS_SYSTEMS_SYSTEM_NODE_H_
#define SYDEVS_SYSTEMS_SYSTEM_NODE_H_

#include <sydevs/systems/node_interface.h>
#include <sydevs/systems/port.h>
#include <sydevs/core/timer.h>

namespace sydevs {
namespace systems {


/**
 * @brief A base class for all nodes from which systems models are constructed.
 *
 * @details
 * The `system_node` abstract base class is inherited by classes representing
 * various types of system nodes:
 *
 * - `atomic_node`, which supports event handlers;
 * - `composite_node`, which supports fixed-structure compositions;
 * - `collection_node`, which supports variable-length collections;
 * - `function_node`, which supports functions.
 *
 * Concrete derived classes must implement the following pure virtual member
 * functions:
 *
 * - `node_dmode`, which distinguishes between `flow` (data flow, untimed) 
 *   nodes and `message` (message passing, timed) nodes;
 * - `time_precision`, which indicates the time quantum that must evenly divide
 *   all planned and elapsed durations associated with the node (unless the
 *   time precision is `no_scale`);
 * - `adopt_component_print_flags`, which copies print flags from the 
 *    components of another node to the components of `this` node.
 * - `handle_initialization_event`, which is called at the beginning of a
 *   simulation;
 * - `handle_unplanned_event`, which is called whenever a message is received;
 * - `handle_planned_event`, which is called when the planned duration elapses;
 * - `handle_finalization_event`, which is called at the end of a simulation.
 */
class system_node
{
public:
    class error;

    system_node(const system_node&)            = delete;   ///< No copy constructor
    system_node& operator=(const system_node&) = delete;   ///< No copy assignment
    system_node(system_node&&)                 = delete;   ///< No move constructor
    system_node& operator=(system_node&&)      = delete;   ///< No move assignment
    virtual ~system_node()                     = default;  ///< Destructor   

    const std::string& node_name() const;  ///< Returns the name of the node.
    const std::string& full_name() const;  ///< Returns the full name of the node, including parent nodes.
    int64 node_index() const;              ///< Returns the index of the node within the parent node.

    const node_interface& external_interface() const;  ///< Returns the object responsible for exchanging information between the node and its surrounding context.

    const timer& event_timer() const;  ///< Returns the object that accumulated wallclock event durations.

    template<typename T>
    void print(const T& X);               ///< Prints `X` while indicating the simulation time and node.
    void print(const char* raw_text);     ///< Prints `raw_text` while indicating the simulation time and node.
    void print(const std::string& text);  ///< Prints `text` while indicating the simulation time and node.

    void print_on_event(bool flag = true) const;  ///< If `flag` is `true`, all event types are printed for this node.

    void adopt_print_flags(const system_node& node) const;  ///< Adopts the print flags of `node`.

    virtual data_mode node_dmode() const = 0;  ///< Returns `flow` if the node has data flow elements only, and `message` otherwise.
    virtual scale time_precision() const = 0;  ///< Returns the time precision associated with the node, or `no_scale`.

    duration process_initialization_event();                ///< Processes an initialization event; returns a planned duration.
    duration process_unplanned_event(duration elapsed_dt);  ///< Processes an unplanned event after an elapsed duration `elapsed_dt`; returns a planned duration.
    duration process_planned_event(duration elapsed_dt);    ///< Processes a planned event after an elapsed duration `elapsed_dt`; returns a planned duration.
    void process_finalization_event(duration elapsed_dt);   ///< Processes a finalization event after an elapsed duration `elapsed_dt`.

protected:
    std::mt19937& rng;  ///< Reference to the random number generator.

    /**
     * @brief Constructs a `system_node`.
     * 
     * @details
     * Constructs the system node and its associated `node_interface` object
     * that associates the node with its surrounding context. The `rng`
     * reference is connected with the main random number generator.
     *
     * @param node_name The name of the node within the encompassing context. 
     * @param external_context The context in which the node is constructed.
     */
    system_node(const std::string& node_name, const node_context& external_context);

    node_interface& external_IO() const;  ///< Returns a non-const reference to the node's external interface.
    timer& ET() const;                    ///< Returns a non-const reference to the node's event timer.

    /**
     * @brief Adjusts the planned duration obtained from other nodes' event
     *        handlers.
     * 
     * @details
     * Checks for errors in the planned duration `planned_dt` including e.g.
     * invalidity, negativity, and loss of precision. Returns a duration that
     * is equal except that the precision level is set to the node's time
     * precision.
     *
     * @param planned_dt The unscaled planned duration value.
     *
     * @return A planned duration with a value equal to `planned_dt`, but where
     *         the precision level may be changed to match `time_precision()`.
     */
    duration scale_planned_dt(duration planned_dt) const;

private:
    virtual void adopt_component_print_flags(const system_node& node) const = 0;

    virtual duration handle_initialization_event() = 0;
    virtual duration handle_unplanned_event(duration elapsed_dt) = 0;
    virtual duration handle_planned_event(duration elapsed_dt) = 0;
    virtual void handle_finalization_event(duration elapsed_dt) = 0;

    node_interface external_interface_;
    timer event_timer_;
};



class system_node::error : public std::runtime_error
{
public:
    explicit error(const std::string& what_arg);
    explicit error(const char* what_arg);
};


inline const std::string& system_node::node_name() const
{
    return external_IO().node_name();
}


inline const std::string& system_node::full_name() const
{
    return external_IO().full_name();
}


inline int64 system_node::node_index() const
{
    return external_IO().node_index();
}


inline const node_interface& system_node::external_interface() const
{
    return const_cast<const node_interface&>(external_interface_);
}


inline const timer& system_node::event_timer() const
{
    return event_timer_;
}


template<typename T>
inline void system_node::print(const T& X)
{
    print(tostring(X));
}


inline void system_node::print(const char* raw_text)
{
    print(std::string(raw_text));
}


inline void system_node::print(const std::string& text)
{
    external_interface_.print_command_text("print", text);
}


inline void system_node::print_on_event(bool flag) const
{
    external_IO().print_on_event(flag);
}


inline duration system_node::process_initialization_event()
{
    auto planned_dt = duration();
    try {
        planned_dt = handle_initialization_event();
    }
    catch (error&) {
        if (ET().timing()) {
            ET().stop();
        }
        throw;
    }
    catch (const std::exception& e) {
        if (ET().timing()) {
            ET().stop();
        }
        external_IO().print_command_text("error", e.what());
        throw error("Aborting event in node (" + full_name() + ") due to error (\"" + e.what() + "\")");
    }
    return planned_dt;
}


inline duration system_node::process_unplanned_event(duration elapsed_dt)
{
    auto planned_dt = duration();
    try {
        planned_dt = handle_unplanned_event(elapsed_dt);
    }
    catch (error&) {
        if (ET().timing()) {
            ET().stop();
        }
        throw;
    }
    catch (const std::exception& e) {
        if (ET().timing()) {
            ET().stop();
        }
        external_IO().print_command_text("error", e.what());
        throw error("Aborting event in node (" + full_name() + ") due to error (\"" + e.what() + "\")");
    }
    return planned_dt;
}


inline duration system_node::process_planned_event(duration elapsed_dt)
{
    auto planned_dt = duration();
    try {
        planned_dt = handle_planned_event(elapsed_dt);
    }
    catch (error&) {
        if (ET().timing()) {
            ET().stop();
        }
        throw;
    }
    catch (const std::exception& e) {
        if (ET().timing()) {
            ET().stop();
        }
        external_IO().print_command_text("error", e.what());
        throw error("Aborting event in node (" + full_name() + ") due to error (\"" + e.what() + "\")");
    }
    return planned_dt;
}


inline void system_node::process_finalization_event(duration elapsed_dt)
{
    try {
        handle_finalization_event(elapsed_dt);
    }
    catch (error&) {
        if (ET().timing()) {
            ET().stop();
        }
        throw;
    }
    catch (const std::exception& e) {
        if (ET().timing()) {
            ET().stop();
        }
        external_IO().print_command_text("error", e.what());
        throw error("Aborting event in node (" + full_name() + ") due to error (\"" + e.what() + "\")");
    }
}


inline system_node::system_node(const std::string& node_name, const node_context& external_context)
    : rng(const_cast<node_context&>(external_context).rng())
    , external_interface_(node_name, this, external_context)
    , event_timer_()
{
}


inline node_interface& system_node::external_IO() const
{
    return const_cast<node_interface&>(external_interface_);
}


inline timer& system_node::ET() const
{
    return const_cast<timer&>(event_timer_);
}


inline system_node::error::error(const std::string& what_arg)
    : runtime_error(what_arg)
{
}


inline system_node::error::error(const char* what_arg)
    : runtime_error(what_arg)
{
}


}  // namespace
}  // namespace

#endif
