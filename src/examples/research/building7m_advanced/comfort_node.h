#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_COMFORT_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_COMFORT_NODE_H_

#include <examples/research/building7m_advanced/building_occupant_ids.h>
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
    thermodynamic_temperature T0;                              // initial temperature
    array2d<thermodynamic_temperature> TF;                     // temperature field
    std::map<occupant_id, array1d<int64>> OP;                  // occupant positions
    std::map<occupant_id, thermodynamic_temperature> OT;       // occupant temperatures
    std::map<occupant_id, thermodynamic_temperature> next_OT;  // next occupant temperatures
    bool change_flag;                                          // flag indicating change

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
    T0 = initial_temperature_input.value();
    TF = array2d<thermodynamic_temperature>();
    OP = std::map<occupant_id, array1d<int64>>();
    OT = std::map<occupant_id, thermodynamic_temperature>();
    next_OT = std::map<occupant_id, thermodynamic_temperature>();
    change_flag = false;
    return duration::inf();
}


inline duration comfort_node::unplanned_event(duration elapsed_dt)
{
    if (temperature_field_input.received()) {
        TF = temperature_field_input.value();
        for (const auto& [occ_id, pos] : OP) {
            next_OT[occ_id] = TF(pos);
            if (OT[occ_id] != next_OT[occ_id]) {
                change_flag = true;
            }
        }
    }
    else if (occupant_position_input.received()) {
        const auto& [occ_id, pos] = occupant_position_input.value();
        OP[occ_id] = pos;
        if (OT.find(occ_id) == std::end(OT)) {
            OT[occ_id] = thermodynamic_temperature();
        }
        if (TF.empty()) {
            next_OT[occ_id] = T0;
        }
        else {
            next_OT[occ_id] = TF(pos);
        }
        if (OT[occ_id] != next_OT[occ_id]) {
            change_flag = true;
        }
    }
    return change_flag ? 0_s : duration::inf();
}


inline duration comfort_node::planned_event(duration elapsed_dt)
{
    for (const auto& [occ_id, pos] : OP) {
        if (OT[occ_id] != next_OT[occ_id]) {
            OT[occ_id] = next_OT[occ_id];
            occupant_temperature_output.send(std::make_pair(occ_id, OT[occ_id]));
        }
    }
    change_flag = false;
    return duration::inf();
}


inline void comfort_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
