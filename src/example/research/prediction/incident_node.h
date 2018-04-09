#pragma once
#ifndef INCIDENT_NODE_H_
#define INCIDENT_NODE_H_

#include <sydevs/systems/atomic_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class incident_node : public atomic_node
{
public:
    // Constructor/Destructor:
    incident_node(const std::string& node_name, const node_context& external_context);
    virtual ~incident_node() = default;

    // Attributes:
    virtual scale time_precision() const { return pico; }

    // Ports:
    port<flow, input, duration> avg_incident_dt_input;
    port<message, input, int64> occurrence_k_input;
    port<message, output, int64> incident_k_output;

protected:
    // State Variables:
    duration avg_incident_dt;
    int64 incident_k;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);

private:
    duration sample_incident_dt();
};


inline incident_node::incident_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , avg_incident_dt_input("avg_incident_dt_input", external_interface())
    , occurrence_k_input("occurrence_k_input", external_interface())
    , incident_k_output("incident_k_output", external_interface())
{
}


inline duration incident_node::initialization_event()
{
    avg_incident_dt = avg_incident_dt_input.value().fixed_at(time_precision());
    incident_k = -1;

    return duration::inf();
}


inline duration incident_node::unplanned_event(duration elapsed_dt)
{
    auto planned_dt = duration();
    if (occurrence_k_input.received()) {
        if (incident_k != -1) {
            throw std::range_error("Incident node received input during incident period");
        }
        incident_k = occurrence_k_input.value();
        planned_dt = sample_incident_dt();
    }

    return planned_dt;
}


inline duration incident_node::planned_event(duration elapsed_dt)
{
    incident_k_output.send(incident_k);
    incident_k = -1;

    return duration::inf();
}


inline void incident_node::finalization_event(duration elapsed_dt)
{
}


inline duration incident_node::sample_incident_dt()
{
    static std::uniform_real_distribution<float64> dist(0.5, 1.5);
    return dist(rng)*avg_incident_dt;
}


#endif
