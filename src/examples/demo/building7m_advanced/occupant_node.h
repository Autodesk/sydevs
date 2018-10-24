#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_OCCUPANT_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_OCCUPANT_NODE_H_

#include <examples/demo/building7m_advanced/building_occupant_ids.h>
#include <examples/demo/building7m_advanced/building_layout_codes.h>
#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class occupant_node : public atomic_node
{
public:
    // Constructor/Destructor:
    occupant_node(const std::string& node_name, const node_context& external_context);
    virtual ~occupant_node() = default;

    // Attributes:
    virtual scale time_precision() const { return nano; }

    // Ports:
    port<flow, input, std::pair<array2d<int64>, distance>> building_layout_input;
    port<flow, input, quantity<decltype(_m/_s)>> walking_speed_input;
    port<flow, input, thermodynamic_temperature> initial_temperature_input;
    port<flow, input, std::map<occupant_id, array1d<int64>>> initial_positions_input;
    port<message, input, std::pair<occupant_id, thermodynamic_temperature>> occupant_temperature_input;
    port<message, output, std::pair<occupant_id, array1d<int64>>> occupant_position_output;

protected:
    // State Variables:
    array2d<int64> L;                                     // building layout
    int64 nx;                                             // number of cells in the x dimension
    int64 ny;                                             // number of cells in the y dimension
    distance d;                                           // distance between cells
    duration step_dt;                                     // time_step
    std::map<occupant_id, thermodynamic_temperature> OT;  // occupant temperatures
    std::map<occupant_id, array1d<int64>> OP;             // occupant positions
    std::map<occupant_id, array1d<int64>> next_OP;        // next occupant positions
    duration planned_dt;                                  // duration until the next position change

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);

    // Probability Distributions:
    array1d<int64> sample_position_change();
};


inline occupant_node::occupant_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , building_layout_input("building_layout_input", external_interface())
    , walking_speed_input("walking_speed_input", external_interface())
    , initial_temperature_input("initial_temperature_input", external_interface())
    , initial_positions_input("initial_positions_input", external_interface())
    , occupant_temperature_input("occupant_temperature_input", external_interface())
    , occupant_position_output("occupant_position_output", external_interface())
{
}


inline duration occupant_node::initialization_event()
{
    L = building_layout_input.value().first;
    nx = L.dims()[0];
    ny = L.dims()[1];
    d = building_layout_input.value().second;
    auto walking_speed = walking_speed_input.value();
    step_dt = (d/walking_speed).fixed_at(time_precision());
    thermodynamic_temperature T0 = initial_temperature_input.value();
    OP = initial_positions_input.value();
    next_OP = OP;
    for (auto& occ_pos : OP) {
        auto& occ_id = occ_pos.first;
        OT[occ_id] = T0;
    }
    planned_dt = 0_s;
    return planned_dt;
}


inline duration occupant_node::unplanned_event(duration elapsed_dt)
{
    if (occupant_temperature_input.received()) {
        const std::pair<occupant_id, thermodynamic_temperature>& occ_T = occupant_temperature_input.value();
        auto occ_id = occ_T.first;
        auto T = occ_T.second;
        OT[occ_id] = T;
    }
    planned_dt -= elapsed_dt;
    return planned_dt;
}


inline duration occupant_node::planned_event(duration elapsed_dt)
{
    // Update current position.
    OP = next_OP;
    for (auto& occ_pos : OP) {
        auto& occ_id = occ_pos.first;
        auto& pos = occ_pos.second;
        occupant_position_output.send(std::make_pair(occ_id, pos));
        auto next_pos = pos + sample_position_change();
        if (L(next_pos) == indoor_code) {
            next_OP[occ_id] = next_pos;
        }
    }
    planned_dt = step_dt;
    return planned_dt;
}


inline void occupant_node::finalization_event(duration elapsed_dt)
{
}


inline array1d<int64> occupant_node::sample_position_change()
{
    static std::uniform_int_distribution<int64> dist(0, 7);
    int64 angle = dist(rng);
    array1d<int64> delta({2}, {0, 0});
    switch (angle) {
    case 0:
        // Head east.
        delta(0) = 1;
        break;
    case 1:
        // Head north.
        delta(1) = 1;
        break;
    case 2:
        // Head west.
        delta(0) = -1;
        break;
    case 3:
        // Head south.
        delta(1) = -1;
        break;
    case 4:
        // Head northeast.
        delta(0) = 1;
        delta(1) = 1;
        break;
    case 5:
        // Head northwest.
        delta(0) = -1;
        delta(1) = 1;
        break;
    case 6:
        // Head southwest.
        delta(0) = -1;
        delta(1) = -1;
        break;
    case 7:
        // Head southeast.
        delta(0) = 1;
        delta(1) = -1;
        break;
    default:
        break;
    }
    return delta;
}


}  // namespace

#endif
