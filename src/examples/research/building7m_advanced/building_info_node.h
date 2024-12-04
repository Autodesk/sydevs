#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_INFO_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_INFO_NODE_H_

#include <examples/research/building7m_advanced/building_layout_codes.h>
#include <sydevs/systems/function_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class building_info_node : public function_node
{
public:
    // Constructor/Destructor:
    building_info_node(const std::string& node_name, const node_context& external_context);
    virtual ~building_info_node() = default;

    // Ports:
    port<flow, output, std::pair<array2d<int64>, std::pair<distance, distance>>> building_layout_output;
    port<flow, output, float64> wall_resistance_output;
    port<flow, output, float64> wall_sound_absorption_output;
    port<flow, output, float64> floor_sound_absorption_output;
    port<flow, output, float64> ceiling_sound_absorption_output;

private:
    // Event Handlers:
    virtual void flow_event();
};


inline building_info_node::building_info_node(const std::string& node_name, const node_context& external_context)
    : function_node(node_name, external_context)
    , building_layout_output("building_layout_output", external_interface())
    , wall_resistance_output("wall_resistance_output", external_interface())
    , wall_sound_absorption_output("wall_sound_absorption_output", external_interface())
    , floor_sound_absorption_output("floor_sound_absorption_output", external_interface())
    , ceiling_sound_absorption_output("ceiling_sound_absorption_output", external_interface())
{
}


inline void building_info_node::flow_event()
{
    int64 nx = 35;
    int64 ny = 35;
    distance d = 200_mm;
    distance h = 3000_mm;
    float64 wall_R = 4.0;
    float64 wall_alpha = 0.1;
    float64 floor_alpha = 0.1;
    float64 ceiling_alpha = 0.1;

    // Starting with indoor cells everywhere.
    array2d<int64> L({nx, ny}, indoor_code);

    // Placing outdoor cells on the two southern and northern rows.
    for (int64 ix = 0; ix < nx; ++ix) {
        L(ix, 0) = outdoor_code;
        L(ix, 1) = outdoor_code;
        L(ix, ny - 2) = outdoor_code;
        L(ix, ny - 1) = outdoor_code;
    }

    // Placing outdoor cells on the two western and eastern rows.
    for (int64 iy = 2; iy < ny - 2; ++iy) {
        L(0, iy) = outdoor_code;
        L(1, iy) = outdoor_code;
        L(nx - 2, iy) = outdoor_code;
        L(nx - 1, iy) = outdoor_code;
    }

    // Placing wall cells on the southern and northern facade.
    for (int64 ix = 2; ix < nx - 2; ++ix) {
        L(ix, 2) = wall_code;
        L(ix, ny - 3) = wall_code;
    }

    // Placing wall cells on the western and eastern facade.
    for (int64 iy = 3; iy < ny - 3; ++iy) {
        L(2, iy) = wall_code;
        L(nx - 3, iy) = wall_code;
    }

    // Placing interior wall cells.
    int64 ix_wall = 3 + (nx - 6)/3;
    for (int64 iy = 3; iy < ny - 8; ++iy) {
        L(ix_wall, iy) = wall_code;
    }

    building_layout_output.assign({ L, {d, h} });
    wall_resistance_output.assign(wall_R);
    wall_sound_absorption_output.assign(wall_alpha);
    floor_sound_absorption_output.assign(floor_alpha);
    ceiling_sound_absorption_output.assign(ceiling_alpha);
}


}  // namespace

#endif
