#pragma once
#ifndef SYDEVS_EXAMPLES_PREDICTION_CLOSED_SYSTEM_H_
#define SYDEVS_EXAMPLES_PREDICTION_CLOSED_SYSTEM_H_

#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/statistic_node.h>
#include <examples/research/prediction/prediction_node.h>
#include <sydevs/systems/simulation.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class prediction_closed_system : public composite_node
{
public:
    // Constructor/Destructor:
    prediction_closed_system(const std::string& node_name, const node_context& external_context);
    virtual ~prediction_closed_system() = default;

    // Components:
    parameter_node<duration> avg_occurrence_dt;
    parameter_node<duration> avg_incident_dt;
    prediction_node prediction;
    statistic_node<int64> correct_count;
    statistic_node<int64> noise_count;
};


prediction_closed_system::prediction_closed_system(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , avg_occurrence_dt("avg_occurrence_dt", internal_context())
    , avg_incident_dt("avg_incident_dt", internal_context())
    , prediction("prediction", internal_context())
    , correct_count("correct_count", internal_context())
    , noise_count("noise_count", internal_context())
{
    // Flow Inner Links
    inner_link(avg_occurrence_dt.parameter, prediction.avg_occurrence_dt_input);
    inner_link(avg_incident_dt.parameter, prediction.avg_incident_dt_input);
    inner_link(prediction.correct_count_output, correct_count.statistic);
    inner_link(prediction.noise_count_output, noise_count.statistic);
}


std::pair<int64, int64> simulate_prediction_system(int64 upper_scale, int64 lower_scale, int64 seed, bool print_all)
{
    auto stats = std::pair<int64, int64>(0, 0);

    auto avg_occurrence_dt = duration(pow(10, upper_scale + 1));
    auto avg_incident_dt = duration(pow(10, 3*(unit - pico) - lower_scale), pico);

    try {
        simulation<prediction_closed_system> sim(duration::inf(), seed, std::cout);
        sim.top.avg_occurrence_dt.set_value(avg_occurrence_dt);
        sim.top.avg_incident_dt.set_value(avg_incident_dt);
        if (print_all) {
            sim.top.avg_occurrence_dt.parameter.print_on_use();
            sim.top.avg_incident_dt.parameter.print_on_use();
            sim.top.prediction.occurrence.occurrence_k_output.print_on_use();
            sim.top.prediction.occurrence.print_on_elapsed_duration();
            sim.top.prediction.occurrence.print_on_planned_duration();
            sim.top.prediction.pulse.pulse_y_output.print_on_use();
            sim.top.prediction.drift.drift_y_output.print_on_use();
            sim.top.prediction.incident.incident_k_output.print_on_use();
            sim.top.prediction.incident.print_on_elapsed_duration();
            sim.top.prediction.incident.print_on_planned_duration();
            sim.top.prediction.feature.feature_y_output.print_on_use();
            sim.top.prediction.predictor.predictor_k_output.print_on_use();
            sim.top.prediction.correct_count_output.print_on_use();
            sim.top.prediction.noise_count_output.print_on_use();
            sim.top.correct_count.statistic.print_on_use();
            sim.top.noise_count.statistic.print_on_use();
        }
        sim.process_remaining_events();
        stats.first = sim.top.correct_count.value();
        stats.second = sim.top.noise_count.value();
    }
    catch (const system_node::error& e) {
        std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "OTHER ERROR: " << e.what() << std::endl;
    }

    return stats;
}


}  // namespace

#endif
