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
    port<flow, input, thermodynamic_temperature> initial_temperature_input;
    port<flow, input, std::pair<array2d<int64>, distance>> building_layout_input;
    port<flow, input, array1d<int64>> initial_position_input;
    port<flow, input, quantity<decltype(_m/_s)>> walking_speed_input;
    port<message, input, std::pair<occupant_id, thermodynamic_temperature>> occupant_temperature_input;
    port<message, output, std::pair<occupant_id, array1d<int64>>> occupant_position_output;

protected:
    // State Variables:
    array2d<int64> L;                         // building layout
    int64 nx;                                 // number of cells in the x dimension
    int64 ny;                                 // number of cells in the y dimension
    distance d;                               // distance between cells
    thermodynamic_temperature T;              // occupant temperature
    array1d<int64> pos;                       // occupant position
    array1d<int64> dest_pos;                  // destination occupant position
    quantity<decltype(_m/_s)> walking_speed;  // occupant walking speed
    duration planned_dt;                      // duration until the next position change

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
    , initial_temperature_input("initial_temperature_input", external_interface())
    , building_layout_input("building_layout_input", external_interface())
    , initial_position_input("initial_position_input", external_interface())
    , walking_speed_input("walking_speed_input", external_interface())
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
    T = initial_temperature_input.value();
    pos = initial_position_input.value();
    dest_pos = pos;
    walking_speed = walking_speed_input.value();
    planned_dt = 0_s;
    return planned_dt;
}


inline duration occupant_node::unplanned_event(duration elapsed_dt)
{
    if (occupant_temperature_input.received()) {
        T = occupant_temperature_input.value().second;
    }
    planned_dt -= elapsed_dt;
    return planned_dt;
}


inline duration occupant_node::planned_event(duration elapsed_dt)
{
    // Update current position.
    pos = dest_pos;
    occupant_position_output.send(std::make_pair(occupant_id(0), pos));

    // Determine potential destination and travel time.
    array1d<int64> delta = sample_position_change();               // random change in position
    dest_pos = pos + delta;                                        // potential destination
    distance r = d*sqrt(std::abs(delta(0)) + std::abs(delta(1)));  // distance to potential destination
    planned_dt = (r/walking_speed).fixed_at(time_precision());     // time before arrival at destination

    // Ensure that the destination is valid.
    if (dest_pos(0) <= -1 || dest_pos(0) >= nx || dest_pos(1) <= -1 || dest_pos(1) >= ny) {
        // dest_pos is off the grid, so remain stationary.
        dest_pos = pos;
    }
    else if (L(dest_pos) == wall_code) {
        // dest_pos is in a wall, so remain stationary.
        dest_pos = pos;
    }

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
