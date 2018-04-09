#pragma once
#ifndef PULSE_NODE_H_
#define PULSE_NODE_H_

#include <sydevs/systems/atomic_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class pulse_node : public atomic_node
{
public:
    // Constructor/Destructor:
    pulse_node(const std::string& node_name, const node_context& external_context);
    virtual ~pulse_node() = default;

    // Attributes:
    virtual scale time_precision() const { return no_scale; }

    // Ports:
    port<flow, input, float64> avg_pulse_y_input;
    port<message, input, int64> occurrence_k_input;
    port<message, output, float64> pulse_y_output;

protected:
    // State Variables:
    float64 avg_pulse_y;
    int64 occurrence_k;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);

private:
    // Probability Distributions:
    float64 sample_pulse_y();
};


inline pulse_node::pulse_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , avg_pulse_y_input("avg_pulse_y_input", external_interface())
    , occurrence_k_input("occurrence_k_input", external_interface())
    , pulse_y_output("pulse_y_output", external_interface())
{
}


inline duration pulse_node::initialization_event()
{
    avg_pulse_y = avg_pulse_y_input.value();
    occurrence_k = -1;

    return duration::inf();
}


inline duration pulse_node::unplanned_event(duration elapsed_dt)
{
    auto planned_dt = duration();

    if (occurrence_k_input.received()) {
        if (occurrence_k != -1) {
            throw std::range_error("Pulse node received simultaneous inputs");
        }
        occurrence_k = occurrence_k_input.value();
        planned_dt = 0_s;
    }

    return planned_dt;
}


inline duration pulse_node::planned_event(duration elapsed_dt)
{
    pulse_y_output.send(sample_pulse_y());
    occurrence_k = -1;

    return duration::inf();
}


inline void pulse_node::finalization_event(duration elapsed_dt)
{
}


inline float64 pulse_node::sample_pulse_y()
{
    static std::uniform_real_distribution<float64> dist(0.0, 1.0);
    return (dist(rng) + occurrence_k)*avg_pulse_y;
}


#endif
