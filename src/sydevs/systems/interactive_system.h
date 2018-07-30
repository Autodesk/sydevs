#pragma once
#ifndef SYDEVS_SYSTEMS_INTERACTIVE_SYSTEM_H_
#define SYDEVS_SYSTEMS_INTERACTIVE_SYSTEM_H_

#include <sydevs/systems/collection_node.h>

namespace sydevs {
namespace systems {


template<typename AgentID, typename Node, typename InjData, typename ObsData>
class interactive_system : public collection_node<int64, Node>
{
public:
    using injection_type = InjData;
    using observation_type = ObsData;

    class interaction_data;

    interactive_system(const std::string& node_name, const node_context& external_context);

    virtual ~interactive_system() = default;  ///< Destructor

    std::unique_ptr<interaction_data> acquire_interaction_data();

    int64 frame_index() const;
    duration planned_duration() const;

private:
    InjData injection_;
    ObsData observation_;
    int64 frame_index_;
    duration planned_dt_;
    std::unique_ptr<interaction_data> interaction_data_;

    void validate();

    duration macro_initialization_event();
    duration macro_unplanned_event(duration elapsed_dt);
    duration micro_planned_event(const AgentID& agent_id, duration elapsed_dt);
    duration macro_planned_event(duration elapsed_dt);
    void macro_finalization_event(duration elapsed_dt);

    virtual duration macro_initialization_update() = 0;
    virtual void micro_planned_update(const AgentID& agent_id, duration elapsed_dt) = 0;
    virtual duration macro_planned_update(duration elapsed_dt, const InjData& injection, ObsData& observation) = 0;
    virtual void macro_finalization_update(duration elapsed_dt) = 0;
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
    if (external_IO().flow_input_port_count() != 0 ||
        external_IO().message_input_port_count() != 0 ||
        external_IO().message_output_port_count() != 0 ||
        external_IO().flow_output_port_count() != 0) {
        throw std::invalid_argument("Interactive node (" + full_name() + ") must have no ports");
    }
}


template<typename AgentID, typename Node, typename InjData, typename ObsData>
duration interactive_system<AgentID, Node, InjData, ObsData>::macro_initialization_event()
{
    planned_dt_ = macro_initialization_update();
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
    if (planned_dt_ <= 0_s) throw std::logic_error("Planned duration between interact events must be positive.");
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
};


template<typename AgentID, typename Node, typename InjData, typename ObsData>
const ObsData& interactive_system<AgentID, Node, InjData, ObsData>::interaction_data::observation()
{
    return observation_;
};


template<typename AgentID, typename Node, typename InjData, typename ObsData>
interactive_system<AgentID, Node, InjData, ObsData>::interaction_data::interaction_data(InjData& inj, ObsData& obs)
    : injection_(inj)
    , observation_(obs)
{
};


}  // namespace
}  // namespace

#endif
