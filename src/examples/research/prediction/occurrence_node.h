#pragma once
#ifndef SYDEVS_EXAMPLES_OCCURRENCE_NODE_H_
#define SYDEVS_EXAMPLES_OCCURRENCE_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class occurrence_node : public atomic_node
{
public:
    // Constructor/Destructor:
    occurrence_node(const std::string& node_name, const node_context& external_context);
    virtual ~occurrence_node() = default;

    // Attributes:
    virtual scale time_precision() const { return milli; }

    // Ports:
    port<flow, input, int64> max_occurrence_count_input;
    port<flow, input, duration> avg_occurrence_dt_input;
    port<message, output, int64> occurrence_k_output;

protected:
    // State Variables:
    int64 max_occurrence_count;
    duration avg_occurrence_dt;
    int64 occurrence_count;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);

private:
    // Probability Distributions:
    duration sample_occurrence_dt();
    int64 sample_occurrence_k();
};


inline occurrence_node::occurrence_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , max_occurrence_count_input("max_occurrence_count_input", external_interface())
    , avg_occurrence_dt_input("avg_occurrence_dt_input", external_interface())
    , occurrence_k_output("occurrence_k_output", external_interface())
{
}


inline duration occurrence_node::initialization_event()
{
    max_occurrence_count = max_occurrence_count_input.value();
    avg_occurrence_dt = avg_occurrence_dt_input.value().fixed_at(time_precision());
    occurrence_count = 0;

    auto planned_dt = duration::inf();
    if (occurrence_count < max_occurrence_count) {
        planned_dt = sample_occurrence_dt();
    }

    return planned_dt;
}


inline duration occurrence_node::unplanned_event(duration elapsed_dt)
{
    return duration();
}


inline duration occurrence_node::planned_event(duration elapsed_dt)
{
    occurrence_k_output.send(sample_occurrence_k());
    ++occurrence_count;

    auto planned_dt = duration::inf();
    if (occurrence_count < max_occurrence_count) {
        planned_dt = sample_occurrence_dt();
    }

    return planned_dt;
}


inline void occurrence_node::finalization_event(duration elapsed_dt)
{
}


inline duration occurrence_node::sample_occurrence_dt()
{
    static std::uniform_real_distribution<float64> dist(0.5, 1.5);
    return dist(rng)*avg_occurrence_dt;
}


inline int64 occurrence_node::sample_occurrence_k()
{
    static std::uniform_int_distribution<int64> dist(0, 1);
    return dist(rng);
}


}  // namespace

#endif
