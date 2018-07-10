#pragma once
#ifndef SYDEVS_SYSTEMS_INTERACTIVE_SYSTEM_H_
#define SYDEVS_SYSTEMS_INTERACTIVE_SYSTEM_H_

#include <sydevs/systems/collection_node.h>

namespace sydevs {
namespace systems {


template <typename AgentID, typename Node, typename InjData, typename ObsData>
class interactive_system : public collection_node<int64, Node>
{
public:
    class data_injector;
    class data_observer;

    interactive_system(const std::string& node_name, const node_context& external_context);

    virtual ~interactive_system() = default;  ///< Destructor

    std::unique_ptr<data_injector> acquire_injector();
    std::unique_ptr<data_observer> acquire_observer();

    int64 frame_index() const;

private:
    std::unique_ptr<InjData> injection_;
    std::unique_ptr<ObsData> observation_;
    int64 frame_index_;
    std::unique_ptr<data_injector> injector_;
    std::unique_ptr<data_observer> observer_;

    void validate();

    duration macro_initialization_event();
    duration macro_unplanned_event(duration elapsed_dt);
    duration micro_planned_event(const AgentID& agent_id, duration elapsed_dt);
    duration macro_planned_event(duration elapsed_dt);
    void macro_finalization_event(duration elapsed_dt);

    virtual duration initialize() = 0;
    virtual void update(const AgentID& agent_id, duration elapsed_dt) = 0;
    virtual duration interact(duration elapsed_dt, const std::unique_ptr<InjData>& injection, const std::unique_ptr<ObsData>& observation) = 0;
    virtual void finalize(duration elapsed_dt) = 0;
};


template <typename AgentID, typename Node, typename InjData, typename ObsData>
class interactive_system<AgentID, Node, InjData, ObsData>::data_injector
{
friend class interative_system;
public:
    std::unique_ptr<InjData>& injection();

private:
    data_injector(std::unique_ptr<InjData>& injection);

    std::unique_ptr<InjData>& injection_;
};



template <typename AgentID, typename Node, typename InjData, typename ObsData>
class interactive_system<AgentID, Node, InjData, ObsData>::data_observer
{
friend class interative_system;
public:
    std::unique_ptr<ObsData>& observation();

private:
    data_observer(std::unique_ptr<ObsData>& observation);

    std::unique_ptr<ObsData>& observation_;
};



template <typename AgentID, typename Node, typename InjData, typename ObsData>
interactive_system<AgentID, Node, InjData, ObsData>::interactive_system(const std::string& node_name, const node_context& external_context)
    : collection_node<int64, Node>(node_name, external_context)
    , injection_()
    , observation_()
    , injector_(new data_injector(injection_))
    , observer_(new data_observer(observation_))
{
    validate();
}


template <typename AgentID, typename Node, typename InjData, typename ObsData>
std::unique_ptr<typename interactive_system<AgentID, Node, InjData, ObsData>::data_injector> interactive_system<AgentID, Node, InjData, ObsData>::acquire_injector()
{
    return std::move(injector_);
}


template <typename AgentID, typename Node, typename InjData, typename ObsData>
std::unique_ptr<typename interactive_system<AgentID, Node, InjData, ObsData>::data_observer> interactive_system<AgentID, Node, InjData, ObsData>::acquire_observer()
{
    return std::move(observer_);
}


template <typename AgentID, typename Node, typename InjData, typename ObsData>
int64 interactive_system<AgentID, Node, InjData, ObsData>::frame_index() const
{
    return frame_index_;
}


template <typename AgentID, typename Node, typename InjData, typename ObsData>
void interactive_system<AgentID, Node, InjData, ObsData>::validate()
{
    if (external_IO().flow_input_port_count() != 0 ||
        external_IO().message_input_port_count() != 0 ||
        external_IO().message_output_port_count() != 0 ||
        external_IO().flow_output_port_count() != 0) {
        throw std::invalid_argument("Interactive node (" + full_name() + ") must have no ports");
    }
}


template <typename AgentID, typename Node, typename InjData, typename ObsData>
duration interactive_system<AgentID, Node, InjData, ObsData>::macro_initialization_event()
{
    auto planned_dt = initialize();
    return planned_dt;
}

    
template <typename AgentID, typename Node, typename InjData, typename ObsData>
duration interactive_system<AgentID, Node, InjData, ObsData>::macro_unplanned_event(duration elapsed_dt)
{
}


template <typename AgentID, typename Node, typename InjData, typename ObsData>
duration interactive_system<AgentID, Node, InjData, ObsData>::micro_planned_event(const AgentID& agent_id, duration elapsed_dt)
{
    update(agent_id, elapsed_dt);
    return planned_dt - elapsed_dt;
}

    
template <typename AgentID, typename Node, typename InjData, typename ObsData>
duration interactive_system<AgentID, Node, InjData, ObsData>::macro_planned_event(duration elapsed_dt)
{
    ++frame_index_;
    observation_.reset();
    auto planned_dt = interact(elapsed_dt, injection_, observation_);
    if (planned_dt <= 0_s) throw std::logic_error("Planned duration between interact events must be positive.");
    injection_.reset();
    return planned_dt;
}


template <typename AgentID, typename Node, typename InjData, typename ObsData>
void interactive_system<AgentID, Node, InjData, ObsData>::macro_finalization_event(duration elapsed_dt)
{
    finalize(elapsed_dt);
}

    
template <typename AgentID, typename Node, typename InjData, typename ObsData>
std::unique_ptr<InjData>& interactive_system<AgentID, Node, InjData, ObsData>::data_injector::injection()
{
    return injection_;
};


template <typename AgentID, typename Node, typename InjData, typename ObsData>
interactive_system<AgentID, Node, InjData, ObsData>::data_injector::data_injector(std::unique_ptr<InjData>& injection)
    : injection_(injection)
{
};


template <typename AgentID, typename Node, typename InjData, typename ObsData>
std::unique_ptr<ObsData>& interactive_system<AgentID, Node, InjData, ObsData>::data_observer::observation()
{
    return observation_;
};


template <typename AgentID, typename Node, typename InjData, typename ObsData>
interactive_system<AgentID, Node, InjData, ObsData>::data_observer::data_observer(std::unique_ptr<ObsData>& observation)
    : observation_(observation)
{
};


}  // namespace
}  // namespace

#endif
