#pragma once
#ifndef SYDEVS_SYSTEMS_INTERACTIVE_SYSTEM_H_
#define SYDEVS_SYSTEMS_INTERACTIVE_SYSTEM_H_

#include <sydevs/systems/collection_node.h>

namespace sydevs {
namespace systems {


/**
 * @brief A base class template for all interactive closed system nodes intended
 *        to be used at the highest level of a real time simulation model.
 *
 * @details
 * The `interactive_system` abstract base class is inherited by classes that
 * represent a system that interacts with either a user or a process external to
 * the model hierarchy. Interaction is facilitated by an `interaction_data`
 * object. This object supports the injection of information into the simulation
 * model via an instance of type `InjData`. It also supports observations of the
 * simulation model via an instance of type `ObsData`.
 *
 * The class inherits from the `collection_node`. This provides macro-level
 * behavior supporting interaction and frames, and micro-level behavior 
 * represented by one (or possibly more) agents of type `Node`. 
 *
 * Concrete derived classes must implement the following pure virtual member
 * functions:
 *
 * - `time_precision`, which indicates the time quantum that must evenly divide
 *   all planned and elapsed durations associated with the node (the time
 *   precision should not be `no_scale`, since the intent is that a frame is
 *   produced whenever the planned duration elapses);
 * - `macro_initialization_update`, which is called at the beginning of a
 *   simulation;
 * - `micro_planned_update`, which is called whenever an agent sends a message;
 * - `macro_planned_update`, which is called when the planned duration elapses;
 * - `macro_finalization_update`, which is called at the end of a simulation.
 *
 * Each transition to the next frame at the macro-level is associated with the
 * macro-level planned duration, which is returned from the 
 * `macro_initialization_update` and `macro_planned_update` member functions.
 * The `micro_planned_update` member function does not return a planned
 * duration because it is assumed that micro-level events should not directly
 * affect the frame rate established at the macro level of the interactive 
 * system node.
 */
template<typename AgentID, typename Node, typename InjData, typename ObsData>
class interactive_system : public collection_node<int64, Node>
{
public:
    using injection_type = InjData;
    using observation_type = ObsData;

    class interaction_data;

    virtual ~interactive_system() = default;  ///< Destructor

    std::unique_ptr<interaction_data> acquire_interaction_data();  ///< Transfers ownership of the interaction data object to the caller.

    int64 frame_index() const;          ///< Returns the index of the most recently processed frame.
    duration planned_duration() const;  ///< Returns the planned duration until the next frame is to be processed.

protected:
    /**
     * @brief Constructs an `interactive_system` node.
     * 
     * @details
     * Constructs the interactive system node and associates it with the
     * surrounding context. An exception is thrown if the node has any ports.
     *
     * @param node_name The name of the node within the encompassing context. 
     * @param external_context The context in which the node is constructed.
     */
    interactive_system(const std::string& node_name, const node_context& external_context);

private:
    void validate();

    duration macro_initialization_event();
    duration macro_unplanned_event(duration elapsed_dt);
    duration micro_planned_event(const AgentID& agent_id, duration elapsed_dt);
    duration macro_planned_event(duration elapsed_dt);
    void macro_finalization_event(duration elapsed_dt);

    virtual duration macro_initialization_update(InjData& injection) = 0;
    virtual void micro_planned_update(const AgentID& agent_id, duration elapsed_dt) = 0;
    virtual duration macro_planned_update(duration elapsed_dt, const InjData& injection, ObsData& observation) = 0;
    virtual void macro_finalization_update(duration elapsed_dt) = 0;

    InjData injection_;
    ObsData observation_;
    int64 frame_index_;
    duration planned_dt_;
    std::unique_ptr<interaction_data> interaction_data_;
};


template<typename AgentID, typename Node, typename InjData, typename ObsData>
class interactive_system<AgentID, Node, InjData, ObsData>::interaction_data
{
friend class interactive_system;
public:
    InjData& injection();
    const ObsData& observation();

private:
    interaction_data(InjData& inj, ObsData& obs);

    InjData& injection_;
    ObsData& observation_;
};


template<typename AgentID, typename Node, typename InjData, typename ObsData>
std::unique_ptr<typename interactive_system<AgentID, Node, InjData, ObsData>::interaction_data> interactive_system<AgentID, Node, InjData, ObsData>::acquire_interaction_data()
{
    return std::move(interaction_data_);
}


template<typename AgentID, typename Node, typename InjData, typename ObsData>
int64 interactive_system<AgentID, Node, InjData, ObsData>::frame_index() const
{
    return frame_index_;
}


template<typename AgentID, typename Node, typename InjData, typename ObsData>
duration interactive_system<AgentID, Node, InjData, ObsData>::planned_duration() const
{
    return planned_dt_;
}


template<typename AgentID, typename Node, typename InjData, typename ObsData>
void interactive_system<AgentID, Node, InjData, ObsData>::validate()
{
    if (this->external_IO().flow_input_port_count() != 0 ||
        this->external_IO().message_input_port_count() != 0 ||
        this->external_IO().message_output_port_count() != 0 ||
        this->external_IO().flow_output_port_count() != 0) {
        throw std::invalid_argument("Interactive node (" + this->full_name() + ") must have no ports");
    }
}


template<typename AgentID, typename Node, typename InjData, typename ObsData>
interactive_system<AgentID, Node, InjData, ObsData>::interactive_system(const std::string& node_name, const node_context& external_context)
    : collection_node<int64, Node>(node_name, external_context)
    , injection_()
    , observation_()
    , frame_index_(-1)
    , planned_dt_()
    , interaction_data_(new interaction_data(injection_, observation_))
{
    validate();
}


template<typename AgentID, typename Node, typename InjData, typename ObsData>
duration interactive_system<AgentID, Node, InjData, ObsData>::macro_initialization_event()
{
    planned_dt_ = macro_initialization_update(injection_);
    return planned_dt_;
}

    
template<typename AgentID, typename Node, typename InjData, typename ObsData>
duration interactive_system<AgentID, Node, InjData, ObsData>::macro_unplanned_event(duration elapsed_dt)
{
    return duration();
}


template<typename AgentID, typename Node, typename InjData, typename ObsData>
duration interactive_system<AgentID, Node, InjData, ObsData>::micro_planned_event(const AgentID& agent_id, duration elapsed_dt)
{
    micro_planned_update(agent_id, elapsed_dt);
    planned_dt_ -= elapsed_dt;
    return planned_dt_;
}

    
template<typename AgentID, typename Node, typename InjData, typename ObsData>
duration interactive_system<AgentID, Node, InjData, ObsData>::macro_planned_event(duration elapsed_dt)
{
    ++frame_index_;
    planned_dt_ = macro_planned_update(elapsed_dt, injection_, observation_);
    if (planned_dt_ <= 0_s) throw std::logic_error("Planned duration between interact events in interactive system (" + this->full_name() + ") must be positive.");
    return planned_dt_;
}


template<typename AgentID, typename Node, typename InjData, typename ObsData>
void interactive_system<AgentID, Node, InjData, ObsData>::macro_finalization_event(duration elapsed_dt)
{
    macro_finalization_update(elapsed_dt);
}

    
template<typename AgentID, typename Node, typename InjData, typename ObsData>
InjData& interactive_system<AgentID, Node, InjData, ObsData>::interaction_data::injection()
{
    return injection_;
}


template<typename AgentID, typename Node, typename InjData, typename ObsData>
const ObsData& interactive_system<AgentID, Node, InjData, ObsData>::interaction_data::observation()
{
    return observation_;
}


template<typename AgentID, typename Node, typename InjData, typename ObsData>
interactive_system<AgentID, Node, InjData, ObsData>::interaction_data::interaction_data(InjData& inj, ObsData& obs)
    : injection_(inj)
    , observation_(obs)
{
}


}  // namespace
}  // namespace

#endif
