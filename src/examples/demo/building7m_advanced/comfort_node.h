#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_COMFORT_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_COMFORT_NODE_H_

#include <examples/demo/building7m_advanced/building_occupant_ids.h>
#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class comfort_node : public atomic_node
{
public:
    // Constructor/Destructor:
    comfort_node(const std::string& node_name, const node_context& external_context);
    virtual ~comfort_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, thermodynamic_temperature> initial_temperature_input;
    port<message, input, array2d<thermodynamic_temperature>> temperature_field_input;
    port<message, input, std::pair<occupant_id, array1d<int64>>> occupant_position_input;
    port<message, output, std::pair<occupant_id, thermodynamic_temperature>> occupant_temperature_output;
 
protected:
    // State Variables:
    thermodynamic_temperature prev_T;       // previous temperature
    thermodynamic_temperature curr_T;       // previous temperature
    array2d<thermodynamic_temperature> TF;  // temperature field
    array1d<int64> pos;                     // occupant position

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline comfort_node::comfort_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , initial_temperature_input("initial_temperature_input", external_interface())
    , temperature_field_input("temperature_field_input", external_interface())
    , occupant_position_input("occupant_position_input", external_interface())
    , occupant_temperature_output("occupant_temperature_output", external_interface())
{
}


inline duration comfort_node::initialization_event()
{
    prev_T = initial_temperature_input.value();
    curr_T = prev_T;
    TF = array2d<thermodynamic_temperature>();
    pos = array1d<int64>();
    return duration::inf();
}


inline duration comfort_node::unplanned_event(duration elapsed_dt)
{
    if (temperature_field_input.received()) {
        TF = temperature_field_input.value();
        if (!pos.empty()) {
            curr_T = TF(pos);
        }
    }
    else if (occupant_position_input.received()) {
        pos = occupant_position_input.value().second;
        if (!TF.empty()) {
            curr_T = TF(pos);
        }
    }
    return curr_T != prev_T ? 0_s : duration::inf();
}


inline duration comfort_node::planned_event(duration elapsed_dt)
{
    occupant_temperature_output.send(std::make_pair(occupant_id(0), curr_T));
    prev_T = curr_T;
    return duration::inf();
}


inline void comfort_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
