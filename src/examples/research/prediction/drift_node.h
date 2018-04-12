#pragma once
#ifndef SYDEVS_EXAMPLES_DRIFT_NODE_H_
#define SYDEVS_EXAMPLES_DRIFT_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class drift_node : public atomic_node
{
public:
    // Constructor/Destructor:
    drift_node(const std::string& node_name, const node_context& external_context);
    virtual ~drift_node() = default;

    // Attributes:
    virtual scale time_precision() const { return femto; }

    // Ports:
    port<flow, input, duration> predictor_dt_input;
    port<flow, input, float64> avg_drift_y_input;
    port<message, input, int64> occurrence_k_input;
    port<message, output, float64> drift_y_output;

protected:
    // State Variables:
    duration predictor_dt;
    float64 avg_drift_y;
    duration planned_dt;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);

private:
    // Probability Distributions:
    float64 sample_drift_y();
};


inline drift_node::drift_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , predictor_dt_input("predictor_dt_input", external_interface())
    , avg_drift_y_input("avg_drift_y_input", external_interface())
    , occurrence_k_input("occurrence_k_input", external_interface())
    , drift_y_output("drift_y_output", external_interface())
{
}


inline duration drift_node::initialization_event()
{
    predictor_dt = predictor_dt_input.value().fixed_at(time_precision());
    avg_drift_y = avg_drift_y_input.value();
    planned_dt = duration::inf();

    return planned_dt;
}


inline duration drift_node::unplanned_event(duration elapsed_dt)
{
    if (occurrence_k_input.received()) {
        if (planned_dt.finite()) {
            throw std::range_error("Drift node received input during predictor period");
        }
        planned_dt = 0_s;
    }

    return planned_dt;
}


inline duration drift_node::planned_event(duration elapsed_dt)
{
    float64 drift_y;

    if (planned_dt == 0_s) {
        drift_y = sample_drift_y();
        planned_dt = predictor_dt;
    }
    else {
        drift_y = 0.0;
        planned_dt = duration::inf();
    }
    drift_y_output.send(drift_y);

    return planned_dt;
}


inline void drift_node::finalization_event(duration elapsed_dt)
{
}


inline float64 drift_node::sample_drift_y()
{
    static std::uniform_real_distribution<float64> dist(0.0, 2.0);
    return dist(rng)*avg_drift_y;
}


}  // namespace

#endif
