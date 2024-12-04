#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_UI_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_UI_NODE_H_

#include <sydevs/systems/atomic_node.h>


namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class building_ui_node : public atomic_node
{
public:
    // Constructor/Destructor:
    building_ui_node(const std::string& node_name, const node_context& external_context);
    virtual ~building_ui_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, duration> frame_duration_input;
    port<flow, input, std::pair<array2d<int64>, distance>> building_layout_input;
    port<message, input, array2d<thermodynamic_temperature>> temperature_field_input;
    port<message, input, array1d<int64>> occupant_position_input;

    // State Functions:
    int64 frame_index() { return frame_index_; }
    array2d<int64> L() const { return L_; }
    distance d() { return d_; }
    array2d<thermodynamic_temperature> TF() const { return TF_; }
    array1d<int64> pos() const { return pos_; }

protected:
    // State Variables:
    int64 frame_index_;                      // counter of past frames
    duration frame_dt_;                      // duration of simulated time between successive frames
    array2d<int64> L_;                       // building layout
    distance d_;                             // distance between cells
    array2d<thermodynamic_temperature> TF_;  // temperature field
    array1d<int64> pos_;                     // occupant position
    duration planned_dt_;                    // planned duration

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline building_ui_node::building_ui_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , frame_duration_input("frame_duration_input", external_interface())
    , building_layout_input("building_layout_input", external_interface())
    , temperature_field_input("temperature_field_input", external_interface())
    , occupant_position_input("occupant_position_input", external_interface())
{
}


inline duration building_ui_node::initialization_event()
{
    frame_index_ = -1;
    frame_dt_ = frame_duration_input.value().fixed_at(time_precision());
    std::tie(L_, d_) = building_layout_input.value();
    TF_ = array2d<thermodynamic_temperature>();
    pos_ = array1d<int64>({2}, {-1, -1});
    planned_dt_ = 0_s;
    return planned_dt_;
}


inline duration building_ui_node::unplanned_event(duration elapsed_dt)
{
    if (temperature_field_input.received()) {
        TF_ = temperature_field_input.value();
    }
    else if (occupant_position_input.received()) {
        pos_ = occupant_position_input.value();
    }
    planned_dt_ -= elapsed_dt;
    return planned_dt_;
}


inline duration building_ui_node::planned_event(duration elapsed_dt)
{
    ++frame_index_;
    planned_dt_ = frame_dt_;
    return planned_dt_;
}


inline void building_ui_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
