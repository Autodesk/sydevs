#pragma once
#ifndef SYDEVS_EXAMPLES_PREDICTION_NODE_H_
#define SYDEVS_EXAMPLES_PREDICTION_NODE_H_

#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/function_node.h>
#include <sydevs/systems/parameter_node.h>
#include <examples/research/prediction/occurrence_node.h>
#include <examples/research/prediction/pulse_node.h>
#include <examples/research/prediction/drift_node.h>
#include <examples/research/prediction/noise_node.h>
#include <examples/research/prediction/waveform_node.h>
#include <examples/research/prediction/feature_node.h>
#include <examples/research/prediction/incident_node.h>
#include <examples/research/prediction/predictor_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class prediction_node : public composite_node
{
public:
    // Constructor/Destructor:
    prediction_node(const std::string& node_name, const node_context& external_context);
    virtual ~prediction_node() = default;

    // Ports:
    port<flow, input, duration> avg_occurrence_dt_input;
    port<flow, input, duration> avg_incident_dt_input;
    port<flow, output, int64> correct_count_output;
    port<flow, output, int64> noise_count_output;

    // Nested Classes:
    class durations_node : public function_node {
    public:
        durations_node(const std::string& node_name, const node_context& external_context);
        port<flow, input, duration> avg_incident_dt_input;
        port<flow, output, duration> predictor_dt_output;
        port<flow, output, duration> sparse_dt_output;
        port<flow, output, duration> decay_dt_output;
        port<flow, output, duration> dense_dt_output;
        port<flow, output, duration> avg_noise_dt_output;
    protected:
        virtual void flow_event();
    };

    // Components:
    parameter_node<int64> max_occurrence_count;
    parameter_node<float64> avg_pulse_y;
    parameter_node<float64> avg_drift_y;
    parameter_node<float64> avg_noise_y;
    durations_node durations;
    occurrence_node occurrence;
    pulse_node pulse;
    drift_node drift;
    noise_node noise;
    waveform_node waveform;
    feature_node feature;
    incident_node incident;
    predictor_node predictor;
};


prediction_node::prediction_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , avg_occurrence_dt_input("avg_occurrence_dt_input", external_interface())
    , avg_incident_dt_input("avg_incident_dt_input", external_interface())
    , correct_count_output("correct_count_output", external_interface())
    , noise_count_output("noise_count_output", external_interface())
    , max_occurrence_count("max_occurrence_count", internal_context(), 100)
    , avg_pulse_y("avg_pulse_y", internal_context(), 1.0)
    , avg_drift_y("avg_drift_y", internal_context(), 2.0)
    , avg_noise_y("avg_noise_y", internal_context(), 0.5)
    , durations("durations", internal_context())
    , occurrence("occurrence", internal_context())
    , pulse("pulse", internal_context())
    , drift("drift", internal_context())
    , noise("noise", internal_context())
    , waveform("waveform", internal_context())
    , feature("feature", internal_context())
    , incident("incident", internal_context())
    , predictor("predictor", internal_context())
{
    // Initialization Links
    inward_link(avg_occurrence_dt_input, occurrence.avg_occurrence_dt_input);
    inward_link(avg_incident_dt_input, incident.avg_incident_dt_input);
    inward_link(avg_incident_dt_input, durations.avg_incident_dt_input);
    inner_link(max_occurrence_count.parameter, occurrence.max_occurrence_count_input);
    inner_link(avg_pulse_y.parameter, pulse.avg_pulse_y_input);
    inner_link(avg_drift_y.parameter, drift.avg_drift_y_input);
    inner_link(avg_noise_y.parameter, noise.avg_noise_y_input);
    inner_link(durations.predictor_dt_output, drift.predictor_dt_input);
    inner_link(durations.predictor_dt_output, noise.predictor_dt_input);
    inner_link(durations.predictor_dt_output, waveform.predictor_dt_input);
    inner_link(durations.sparse_dt_output, waveform.sparse_dt_input);
    inner_link(durations.decay_dt_output, waveform.decay_dt_input);
    inner_link(durations.dense_dt_output, waveform.dense_dt_input);
    inner_link(durations.avg_noise_dt_output, noise.avg_noise_dt_input);
    
    // Simulation Links
    inner_link(occurrence.occurrence_k_output, pulse.occurrence_k_input);
    inner_link(occurrence.occurrence_k_output, drift.occurrence_k_input);
    inner_link(occurrence.occurrence_k_output, noise.occurrence_k_input);
    inner_link(occurrence.occurrence_k_output, incident.occurrence_k_input);
    inner_link(pulse.pulse_y_output, waveform.pulse_y_input);
    inner_link(drift.drift_y_output, waveform.drift_y_input);
    inner_link(noise.noise_y_output, waveform.noise_y_input);
    inner_link(waveform.waveform_y_output, feature.waveform_y_input);
    inner_link(feature.feature_y_output, predictor.feature_y_input);
    inner_link(incident.incident_k_output, predictor.incident_k_input);
    
    // Finalization Links
    outward_link(predictor.correct_count_output, correct_count_output);
    outward_link(noise.noise_count_output, noise_count_output);
}


prediction_node::durations_node::durations_node(const std::string& node_name, const node_context& external_context)
    : function_node(node_name, external_context)
    , avg_incident_dt_input("avg_incident_dt_input", external_interface())
    , predictor_dt_output("predictor_dt_output", external_interface())
    , sparse_dt_output("sparse_dt_output", external_interface())
    , decay_dt_output("decay_dt_output", external_interface())
    , dense_dt_output("dense_dt_output", external_interface())
    , avg_noise_dt_output("avg_noise_dt_output", external_interface())
{
}


void prediction_node::durations_node::flow_event()
{
    auto predictor_dt = (1.0/3.0)*avg_incident_dt_input.value();
    auto sparse_dt = (1.0/7.0)*predictor_dt;
    auto decay_dt = (1.0/3.0)*sparse_dt;
    auto dense_dt = (1.0/7.0)*sparse_dt;
    auto avg_noise_dt = (1.0/3.0)*dense_dt;
    predictor_dt_output.assign(predictor_dt);
    sparse_dt_output.assign(sparse_dt);
    decay_dt_output.assign(decay_dt);
    dense_dt_output.assign(dense_dt);
    avg_noise_dt_output.assign(avg_noise_dt);
}


}  // namespace

#endif
