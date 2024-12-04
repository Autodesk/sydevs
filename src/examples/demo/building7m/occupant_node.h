#pragma once
#ifndef SYDEVS_EXAMPLES_OCCUPANT_NODE_H_
#define SYDEVS_EXAMPLES_OCCUPANT_NODE_H_

#include <examples/demo/building7m/building_layout_codes.h>
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
    port<message, input, thermodynamic_temperature> occupant_temperature_input;
    port<message, output, array1d<int64>> occupant_position_output;

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
    quantity<decltype(_K*_s)> cummulative_T;  // cummulative occupant temperature times time
    duration cummulative_dt;                  // cummulative duration elapsed 
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
    std::tie(L, d) = building_layout_input.value();
    nx = L.dims()[0];
    ny = L.dims()[1];
    T = initial_temperature_input.value();
    pos = initial_position_input.value();
    dest_pos = pos;
    walking_speed = walking_speed_input.value();
    cummulative_T = (0_K*_s).fixed_at(unit);
    cummulative_dt = (0_s).fixed_at(micro);
    planned_dt = 0_s;
    print(cummulative_T);
    print(cummulative_dt);
    return planned_dt;
}


inline duration occupant_node::unplanned_event(duration elapsed_dt)
{
    cummulative_T += elapsed_dt*T;
    cummulative_dt += elapsed_dt.fixed_at(micro);
    if (occupant_temperature_input.received()) {
        T = occupant_temperature_input.value();
    }
    planned_dt -= elapsed_dt;
    return planned_dt;
}


inline duration occupant_node::planned_event(duration elapsed_dt)
{
    cummulative_T += elapsed_dt*T;
    cummulative_dt += elapsed_dt.fixed_at(micro);

    // Update current position.
    pos = dest_pos;
    occupant_position_output.send(pos);

    // Determine potential destination and travel time.
    array1d<int64> delta = sample_position_change();            // random change in position
    dest_pos = pos + delta;                                     // potential destination
    planned_dt = (d/walking_speed).fixed_at(time_precision());  // time before arrival at destination

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
    cummulative_T += elapsed_dt*T;
    cummulative_dt += elapsed_dt.fixed_at(micro);
    thermodynamic_temperature average_T = (cummulative_T/cummulative_dt).fixed_at(milli);
}


inline array1d<int64> occupant_node::sample_position_change()
{
    static std::uniform_int_distribution<int64> dist(0, 3);
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
    default:
        break;
    }
    return delta;
}


}  // namespace

#endif
