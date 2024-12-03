#pragma once
#ifndef SYDEVS_EXAMPLES_BUILDING_INFO_NODE_H_
#define SYDEVS_EXAMPLES_BUILDING_INFO_NODE_H_

#include <examples/demo/building7m/building_layout_codes.h>
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
    port<flow, output, std::pair<array2d<int64>, distance>> building_layout_output;
    port<flow, output, float64> wall_resistance_output;

private:
    // Event Handlers:
    virtual void flow_event();
};


inline building_info_node::building_info_node(const std::string& node_name, const node_context& external_context)
    : function_node(node_name, external_context)
    , building_layout_output("building_layout_output", external_interface())
    , wall_resistance_output("wall_resistance_output", external_interface())
{
}


inline void building_info_node::flow_event()
{
    int64 nx = 35;
    int64 ny = 35;
    distance d = 200_mm;
    int64 wall_R = 5;

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

    building_layout_output.assign({ L, d });
    wall_resistance_output.assign(wall_R);
}


}  // namespace

#endif
