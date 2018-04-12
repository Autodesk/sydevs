#pragma once
#ifndef SYDEVS_EXAMPLES_BASIC_GENERATOR_NODE_H_
#define SYDEVS_EXAMPLES_BASIC_GENERATOR_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class basic_generator_node : public atomic_node
{
public:
    // Constructor/Destructor:
    basic_generator_node(const std::string& node_name, const node_context& external_context);
    virtual ~basic_generator_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, duration> mean_dt_input;
    port<message, output, int64> count_output;

protected:
    // State Variables:
    duration mean_dt;
    int64 count;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline basic_generator_node::basic_generator_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , mean_dt_input("mean_dt_input", external_interface())
    , count_output("count_output", external_interface())
{
}


inline duration basic_generator_node::initialization_event()
{
    static auto dist = std::exponential_distribution<float64>(1.0);

    mean_dt = mean_dt_input.value().fixed_at(time_precision());
    count = 0;

    return dist(rng)*mean_dt;
}


inline duration basic_generator_node::unplanned_event(duration elapsed_dt)
{
    return duration();
}


inline duration basic_generator_node::planned_event(duration elapsed_dt)
{
    static auto dist = std::exponential_distribution<float64>(1.0);

    ++count;

    count_output.send(count);

    return dist(rng)*mean_dt;
}


inline void basic_generator_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
