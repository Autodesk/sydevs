#pragma once
#ifndef SYDEVS_EXAMPLES_WAVEFORM_NODE_H_
#define SYDEVS_EXAMPLES_WAVEFORM_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class waveform_node : public atomic_node
{
public:
    // Constructor/Destructor:
    waveform_node(const std::string& node_name, const node_context& external_context);
    virtual ~waveform_node() = default;

    // Attributes:
    virtual scale time_precision() const { return femto; }

    // Ports:
    port<flow, input, duration> predictor_dt_input;
    port<flow, input, duration> decay_dt_input;
    port<flow, input, duration> dense_dt_input;
    port<flow, input, duration> sparse_dt_input;
    port<message, input, float64> pulse_y_input;
    port<message, input, float64> drift_y_input;
    port<message, input, float64> noise_y_input;
    port<message, output, float64> waveform_y_output;

protected:
    // State Variables:
    duration predictor_dt;
    duration decay_dt;
    duration dense_dt;
    duration sparse_dt;
    int64 point_count;
    float64 decay_y;
    float64 drift_y;
    float64 noise_y;
    duration planned_dt;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline waveform_node::waveform_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , predictor_dt_input("predictor_dt_input", external_interface())
    , decay_dt_input("decay_dt_input", external_interface())
    , dense_dt_input("dense_dt_input", external_interface())
    , sparse_dt_input("sparse_dt_input", external_interface())
    , pulse_y_input("pulse_y_input", external_interface())
    , drift_y_input("drift_y_input", external_interface())
    , noise_y_input("noise_y_input", external_interface())
    , waveform_y_output("waveform_y_output", external_interface())
{
}


inline duration waveform_node::initialization_event()
{
    predictor_dt = predictor_dt_input.value().fixed_at(time_precision());
    decay_dt = decay_dt_input.value().fixed_at(time_precision());
    dense_dt = dense_dt_input.value().fixed_at(time_precision());
    sparse_dt = sparse_dt_input.value().fixed_at(time_precision());
    point_count = 0;
    decay_y = 0.0;
    drift_y = 0.0;
    noise_y = 0.0;
    planned_dt = duration::inf();

    return planned_dt;
}


inline duration waveform_node::unplanned_event(duration elapsed_dt)
{
    if (pulse_y_input.received()) {
        if (planned_dt.finite()) {
            throw std::range_error("Waveform node received input during predictor period");
        }
        decay_y = pulse_y_input.value();
        point_count = 0;
        planned_dt = dense_dt;
    }
    else if (drift_y_input.received()) {
        drift_y = drift_y_input.value();
        decay_y *= std::exp(-elapsed_dt/decay_dt);
        planned_dt -= elapsed_dt;
    }
    else if (noise_y_input.received()) {
        noise_y = noise_y_input.value();
        decay_y *= std::exp(-elapsed_dt/decay_dt);
        planned_dt -= elapsed_dt;
    }

    return planned_dt;
}


inline duration waveform_node::planned_event(duration elapsed_dt)
{
    ++point_count;
    decay_y *= std::exp(-planned_dt/decay_dt);
    float64 waveform_y = decay_y + drift_y + noise_y;
    waveform_y_output.send(waveform_y);
    if (point_count < 6) {
        planned_dt = dense_dt;
    }
    else if (point_count < 12) {
        planned_dt = sparse_dt;
    }
    else {
        planned_dt = duration::inf();
    }

    return planned_dt;
}


inline void waveform_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
