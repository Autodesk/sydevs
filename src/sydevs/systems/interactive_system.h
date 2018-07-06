#pragma once
#ifndef SYDEVS_SYSTEMS_INTERACTIVE_SYSTEM_H_
#define SYDEVS_SYSTEMS_INTERACTIVE_SYSTEM_H_

#include <sydevs/systems/collection_node.h>

namespace sydevs {
namespace systems {


template <typename Node, typename Injection, typename Observation>
class interactive_system : public collection_node<int64, Node>
{
public:
    class data_injector;
    class data_observer;

    interactive_system(const std::string& node_name, const node_context& external_context);

    virtual ~interactive_system() = default;  ///< Destructor

    int64 frame_index() const;

    std::unique_ptr<data_injector> injector();
    std::unique_ptr<data_observer> observer();

private:
    std::unique_ptr<Injection> injection_ptr_;
    std::unique_ptr<Observation> observation_ptr_;
    int64 frame_index_;
    std::unique_ptr<data_injector> injector_;
    std::unique_ptr<data_observer> observer_;

    duration macro_planned_event(duration elapsed_dt);

    virtual std::pair<duration, Observation> interaction_event(duration elapsed_dt, Injection& injection) = 0;
};


template <typename Node, typename Injection, typename Observation>
class interactive_system<Node, Injection, Observation>::data_injector
{
friend class interative_system;
public:
    std::unique_ptr<Injection>& injection_ptr();

private:
    data_injector(std::unique_ptr<Injection>& injection_ptr);

    std::unique_ptr<Injection>& injection_ptr_;
};



template <typename Node, typename Injection, typename Observation>
class interactive_system<Node, Injection, Observation>::data_observer
{
friend class interative_system;
public:
    std::unique_ptr<Observation>& observation_ptr();

private:
    data_observer(std::unique_ptr<Observation>& observation_ptr);

    std::unique_ptr<Observation>& observation_ptr_;
};



template <typename Node, typename Injection, typename Observation>
interactive_system<Node, Injection, Observation>::interactive_system(const std::string& node_name, const node_context& external_context)
    : collection_node<int64, Node>(node_name, external_context)
    , injection_ptr_()
    , observation_ptr_()
    , injector_(new data_injector(injection_ptr_))
    , observer_(new data_observer(observation_ptr_))
{
}


template <typename Node, typename Injection, typename Observation>
int64 interactive_system<Node, Injection, Observation>::frame_index() const
{
    return frame_index_;
}


template <typename Node, typename Injection, typename Observation>
std::unique_ptr<typename interactive_system<Node, Injection, Observation>::data_injector> interactive_system<Node, Injection, Observation>::injector()
{
    return std::move(injector_);
}


template <typename Node, typename Injection, typename Observation>
std::unique_ptr<typename interactive_system<Node, Injection, Observation>::data_observer> interactive_system<Node, Injection, Observation>::observer()
{
    return std::move(observer_);
}


template <typename Node, typename Injection, typename Observation>
duration interactive_system<Node, Injection, Observation>::macro_planned_event(duration elapsed_dt)
{
    ++frame_index_;
    auto event_result = interaction_event(elapsed_dt, *injection_ptr_);
    interaction_ptr_.reset();
    observation_ptr_ = std::move(event_result.second);
    return event_result.first;
}


template <typename Node, typename Injection, typename Observation>
std::unique_ptr<Injection>& interactive_system<Node, Injection, Observation>::data_injector::injection_ptr()
{
    return injection_ptr_;
};


template <typename Node, typename Injection, typename Observation>
interactive_system<Node, Injection, Observation>::data_injector::data_injector(std::unique_ptr<Injection>& injection_ptr)
    : injection_ptr_(injection_ptr)
{
};


template <typename Node, typename Injection, typename Observation>
std::unique_ptr<Observation>& interactive_system<Node, Injection, Observation>::data_observer::observation_ptr()
{
    return observation_ptr_;
};


template <typename Node, typename Injection, typename Observation>
interactive_system<Node, Injection, Observation>::data_observer::data_observer(std::unique_ptr<Observation>& observation_ptr)
    : observation_ptr_(observation_ptr)
{
};


}  // namespace
}  // namespace

#endif
