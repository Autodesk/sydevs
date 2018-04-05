#pragma once
#ifndef BUILDING_VIS_NODE_H_
#define BUILDING_VIS_NODE_H_

#include <sydevs/systems/atomic_node.h>
#include <iostream>

using namespace sydevs;
using namespace sydevs::systems;


class building_vis_node : public atomic_node
{
public:
    // Constructor/Destructor:
    building_vis_node(const std::string& node_name, const node_context& external_context);
    virtual ~building_vis_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, duration> frame_duration_input;
    port<message, input, array2d<thermodynamic_temperature>> temperature_field_input;
    port<message, input, array1d<int64>> occupant_position_input;

protected:
    // State Variables:
    duration frame_dt;                      // duration of simulated time between successive frames
    array2d<thermodynamic_temperature> TF;  // temperature field
    array1d<int64> pos;                     // occupant position
    duration planned_dt;                    // planned duration

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline building_vis_node::building_vis_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , frame_duration_input("frame_duration_input", external_interface())
    , temperature_field_input("temperature_field_input", external_interface())
    , occupant_position_input("occupant_position_input", external_interface())
{
}


inline duration building_vis_node::initialization_event()
{
    frame_dt = frame_duration_input.value();
    TF = array2d<thermodynamic_temperature>();
    pos = array1d<int64>({2}, {-1, -1});
    planned_dt = 0_s;
    return planned_dt;
}


inline duration building_vis_node::unplanned_event(duration elapsed_dt)
{
    if (temperature_field_input.received()) {
        TF = temperature_field_input.value();
    }
    else if (occupant_position_input.received()) {
        pos = occupant_position_input.value();
    }
    planned_dt -= elapsed_dt;
    return planned_dt;
}


inline duration building_vis_node::planned_event(duration elapsed_dt)
{
    // Print grid if the frame rate is finite and the temperature field is not empty.
    if (frame_dt.finite() && !TF.empty()) {
        int64 nx = TF.dims()[0];
        int64 ny = TF.dims()[1];
        std::cout << std::endl;
        for (int64 iy = ny - 1; iy >= 0; --iy) {
            std::string line("|");
            for (int64 ix = 0; ix < nx; ++ix) {
                if (pos(0) == ix && pos(1) == iy) {
                    line += "  ";
                }
                else {
                    int64 T_in_deg_C = int64((TF(ix, iy) - 273150_mK)/1_K + 0.5);
                    line += tostring(T_in_deg_C);
                }
                line += "|";
            }
            std::cout << line << std::endl;
        }
        std::cout << std::endl;
    }

    planned_dt = frame_dt;
    return planned_dt;
}


inline void building_vis_node::finalization_event(duration elapsed_dt)
{
}


#endif
