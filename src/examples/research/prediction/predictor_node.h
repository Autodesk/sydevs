#pragma once
#ifndef SYDEVS_EXAMPLES_PREDICTOR_NODE_H_
#define SYDEVS_EXAMPLES_PREDICTOR_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class predictor_node : public atomic_node
{
public:
    // Constructor/Destructor:
    predictor_node(const std::string& node_name, const node_context& external_context);
    virtual ~predictor_node() = default;

    // Attributes:
    virtual scale time_precision() const { return no_scale; }

    // Ports:
    port<message, input, float64> feature_y_input;
    port<message, input, int64> incident_k_input;
    port<message, output, int64> predictor_k_output;
    port<flow, output, int64> correct_count_output;

protected:
    // State Variables:
    int64 correct_count;
    float64 sum_0;
    float64 sum_1;
    int64 count_0;
    int64 count_1;
    float64 threshold;
    float64 feature_y;
    int64 predictor_k;
    duration planned_dt;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline predictor_node::predictor_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , feature_y_input("feature_y_input", external_interface())
    , incident_k_input("incident_k_input", external_interface())
    , predictor_k_output("predictor_k_output", external_interface())
    , correct_count_output("correct_count_output", external_interface())
{
}


inline duration predictor_node::initialization_event()
{
    correct_count = 0;
    sum_0 = 0.0;
    sum_1 = 0.0;
    count_0 = 0;
    count_1 = 0;
    threshold = std::numeric_limits<float64>::infinity();
    feature_y = -std::numeric_limits<float64>::max();
    predictor_k = -1;
    planned_dt = duration::inf();

    return planned_dt;
}


inline duration predictor_node::unplanned_event(duration elapsed_dt)
{
    if (feature_y_input.received()) {
        if (feature_y != -std::numeric_limits<float64>::max()) {
            throw std::range_error("Predictor node received feature input with old feature unclassified");
        }
        feature_y = feature_y_input.value();
        planned_dt = 0_s;
    }
    else if (incident_k_input.received()) {
        if (planned_dt.finite()) {
            throw std::range_error("Predictor node received k input with output imminent");
        }
        if (feature_y == -1) {
            throw std::range_error("Predictor node received k input with no feature unclassified");
        }
        auto incident_k = incident_k_input.value();
        correct_count += 1*(predictor_k == incident_k);
        if (incident_k == 0) {
            sum_0 += feature_y;
            ++count_0;
        }
        else if (incident_k == 1) {
            sum_1 += feature_y;
            ++count_1;
        }
        if (count_0 > 0 && count_1 > 0) {
            threshold = 0.5*(sum_0/count_0 + sum_1/count_1);
        }
        feature_y = -std::numeric_limits<float64>::max();
        predictor_k = -1;
        planned_dt = duration::inf();
    }

    return planned_dt;
}


inline duration predictor_node::planned_event(duration elapsed_dt)
{
    predictor_k = 1*(feature_y >= threshold);
    predictor_k_output.send(predictor_k);
    planned_dt = duration::inf();

    return planned_dt;
}


inline void predictor_node::finalization_event(duration elapsed_dt)
{
    correct_count_output.assign(correct_count);
}


}  // namespace

#endif
