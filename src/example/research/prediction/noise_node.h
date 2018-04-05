#pragma once
#ifndef NOISE_NODE_H_
#define NOISE_NODE_H_

#include <sydevs/systems/atomic_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class noise_node : public atomic_node
{
public:
    // Constructor/Destructor:
    noise_node(const std::string& node_name, const node_context& external_context);
    virtual ~noise_node() = default;

    // Attributes:
    virtual scale time_precision() const { return femto; }

    // Ports:
    port<flow, input, duration> predictor_dt_input;
    port<flow, input, duration> avg_noise_dt_input;
    port<flow, input, float64> avg_noise_y_input;
    port<message, input, int64> occurrence_k_input;
    port<message, output, float64> noise_y_output;
    port<flow, output, int64> noise_count_output;

protected:
    // State Variables:
    duration predictor_dt;
    duration avg_noise_dt;
    float64 avg_noise_y;
    int64 noise_count;
    duration remaining_dt;
    duration planned_dt;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);

private:
    // Probability Distributions:
    duration sample_noise_dt();
    float64 sample_noise_y();
};


inline noise_node::noise_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , predictor_dt_input("predictor_dt_input", external_interface())
    , avg_noise_dt_input("avg_noise_dt_input", external_interface())
    , avg_noise_y_input("avg_noise_y_input", external_interface())
    , occurrence_k_input("occurrence_k_input", external_interface())
    , noise_y_output("noise_y_output", external_interface())
    , noise_count_output("noise_count_output", external_interface())
{
}


inline duration noise_node::initialization_event()
{
    predictor_dt = predictor_dt_input.value().fixed_at(time_precision());
    avg_noise_dt = avg_noise_dt_input.value().fixed_at(time_precision());
    avg_noise_y = avg_noise_y_input.value();
    noise_count = 0;
    remaining_dt = duration::inf();
    planned_dt = duration::inf();

    return planned_dt;
}


inline duration noise_node::unplanned_event(duration elapsed_dt)
{
    if (occurrence_k_input.received()) {
        if (planned_dt.finite()) {
            throw std::range_error("Noise node received input during predictor period");
        }
        remaining_dt = predictor_dt;
        planned_dt = 0_s;
    }

    return planned_dt;
}


inline duration noise_node::planned_event(duration elapsed_dt)
{
    float64 noise_y;

    if (planned_dt < remaining_dt) {
        noise_y = sample_noise_y();
        ++noise_count;
        remaining_dt -= planned_dt;
        planned_dt = sample_noise_dt();
        if (planned_dt > remaining_dt) {
            planned_dt = remaining_dt;
        }
    }
    else {
        noise_y = 0.0;
        remaining_dt = duration::inf();
        planned_dt = duration::inf();
    }
    noise_y_output.send(noise_y);

    return planned_dt;
}


inline void noise_node::finalization_event(duration elapsed_dt)
{
    noise_count_output.assign(noise_count);
}


inline duration noise_node::sample_noise_dt()
{
    static std::exponential_distribution<float64> dist(1.0);
    return dist(rng)*avg_noise_dt;
}


inline float64 noise_node::sample_noise_y()
{
    static std::uniform_real_distribution<float64> dist(0.0, 2.0);
    return dist(rng)*avg_noise_y;
};


#endif
