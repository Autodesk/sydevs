#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_VIS_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_VIS_NODE_H_

#include <examples/research/building7m_advanced/building_occupant_ids.h>
#include <sydevs/systems/atomic_node.h>
#include <iostream>

namespace sydevs_examples {

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
    port<flow, input, std::pair<array2d<int64>, distance>> building_layout_input;
    port<message, input, array2d<thermodynamic_temperature>> temperature_field_input;
    port<message, input, std::pair<occupant_id, array1d<int64>>> occupant_position_input;

protected:
    // State Variables:
    duration frame_dt;                         // duration of simulated time between successive frames
    array2d<int64> L;                          // building layout
    int64 nx;                                  // number of cells in the x dimension
    int64 ny;                                  // number of cells in the y dimension
    int64 frame_index;                         // index of frame
    array2d<thermodynamic_temperature> TF;     // temperature field
    std::map<occupant_id, array1d<int64>> OP;  // occupant positions
    duration planned_dt;                       // planned duration

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline building_vis_node::building_vis_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , frame_duration_input("frame_duration_input", external_interface())
    , building_layout_input("building_layout_input", external_interface())
    , temperature_field_input("temperature_field_input", external_interface())
    , occupant_position_input("occupant_position_input", external_interface())
{
}


inline duration building_vis_node::initialization_event()
{
    frame_dt = frame_duration_input.value();
    L = building_layout_input.value().first;
    nx = L.dims()[0];
    ny = L.dims()[1];
    frame_index = -1;
    TF = array2d<thermodynamic_temperature>();
    OP = std::map<occupant_id, array1d<int64>>();
    planned_dt = 0_s;
    return planned_dt;
}


inline duration building_vis_node::unplanned_event(duration elapsed_dt)
{
    if (temperature_field_input.received()) {
        TF = temperature_field_input.value();
    }
    else if (occupant_position_input.received()) {
        const std::pair<occupant_id, array1d<int64>>& occ_pos = occupant_position_input.value();
        auto occ_id = occ_pos.first;
        auto pos = occ_pos.second;
        OP[occ_id] = pos;
    }
    planned_dt -= elapsed_dt;
    return planned_dt;
}


inline duration building_vis_node::planned_event(duration elapsed_dt)
{
    // Print grid if the frame rate is finite and the temperature field is not empty.
    if (frame_dt.finite() && !TF.empty()) {
        ++frame_index;
        print("Frame " + tostring(frame_index));
        std::cout << std::endl;
        int64 nx = TF.dims()[0];
        int64 ny = TF.dims()[1];
        for (int64 iy = ny - 1; iy >= 0; --iy) {
            std::string line("|");
            for (int64 ix = 0; ix < nx; ++ix) {
                if (L(ix, iy) == wall_code) {
                    line += "@@";
                }
                else {
                    bool occupied = false;
                    for (auto occ_pos = std::begin(OP); !occupied && occ_pos != std::end(OP); ++occ_pos) {
                        const auto& pos = occ_pos->second;
                        occupied = (pos(0) == ix && pos(1) == iy);
                    }
                    if (occupied) {
                        line += "  ";
                    }
                    else {
                        int64 T_in_deg_C = int64((TF(ix, iy) - 273150_mK)/1_K + 0.5);
                        line += tostring(T_in_deg_C);
                    }
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


}  // namespace

#endif
