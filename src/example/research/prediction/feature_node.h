#pragma once
#ifndef FEATURE_NODE_H_
#define FEATURE_NODE_H_

#include <sydevs/systems/atomic_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class feature_node : public atomic_node
{
public:
    // Constructor/Destructor:
    feature_node(const std::string& node_name, const node_context& external_context);
    virtual ~feature_node() = default;

    // Attributes:
    virtual scale time_precision() const { return no_scale; }

    // Ports:
    port<message, input, float64> waveform_y_input;
    port<message, output, float64> feature_y_output;

protected:
    // State Variables:
    int64 point_count;
    float64 dense_y_sum;
    float64 sparse_y_sum;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline feature_node::feature_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , waveform_y_input("waveform_y_input", external_interface())
    , feature_y_output("feature_y_output", external_interface())
{
}


inline duration feature_node::initialization_event()
{
    point_count = 0;
    dense_y_sum = 0.0;
    sparse_y_sum = 0.0;

    return duration::inf();
}


inline duration feature_node::unplanned_event(duration elapsed_dt)
{
    auto planned_dt = duration();

    if (waveform_y_input.received()) {
        ++point_count;
        if (point_count <= 6) {
            dense_y_sum += waveform_y_input.value();
        }
        else if (point_count <= 12) {
            sparse_y_sum += waveform_y_input.value();
        }
        if (point_count < 12) {
            planned_dt = duration::inf();
        }
        else {
            planned_dt = 0_s;
        }
    }

    return planned_dt;
}


inline duration feature_node::planned_event(duration elapsed_dt)
{
    feature_y_output.send((dense_y_sum - sparse_y_sum)/6.0);
    point_count = 0;
    dense_y_sum = 0.0;
    sparse_y_sum = 0.0;

    return duration::inf();
}


inline void feature_node::finalization_event(duration elapsed_dt)
{
}


#endif
