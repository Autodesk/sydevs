#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_HEARING_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_HEARING_NODE_H_

#include <examples/research/building7m_advanced/building_occupant_ids.h>
#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class hearing_node : public atomic_node
{
public:
    // Constructor/Destructor:
    hearing_node(const std::string& node_name, const node_context& external_context);
    virtual ~hearing_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<message, input, array2d<quantity<decltype(_g/_m/_s/_s)>>> sound_field_input;
    port<message, input, std::pair<occupant_id, array1d<int64>>> occupant_position_input;
    port<message, output, std::pair<occupant_id, quantity<decltype(_g/_m/_s/_s)>>> occupant_sound_output;
 
protected:
    // State Variables:
    array2d<quantity<decltype(_g/_m/_s/_s)>> SF;                     // sound field
    std::map<occupant_id, array1d<int64>> OP;                        // occupant positions
    std::map<occupant_id, quantity<decltype(_g/_m/_s/_s)>> OS;       // occupant sounds
    std::map<occupant_id, quantity<decltype(_g/_m/_s/_s)>> next_OS;  // next occupant sounds
    bool change_flag;                                                // flag indicating change

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline hearing_node::hearing_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , sound_field_input("sound_field_input", external_interface())
    , occupant_position_input("occupant_position_input", external_interface())
    , occupant_sound_output("occupant_sound_output", external_interface())
{
}


inline duration hearing_node::initialization_event()
{
    SF = array2d<quantity<decltype(_g/_m/_s/_s)>>();
    OP = std::map<occupant_id, array1d<int64>>();
    OS = std::map<occupant_id, quantity<decltype(_g/_m/_s/_s)>>();
    next_OS = std::map<occupant_id, quantity<decltype(_g/_m/_s/_s)>>();
    change_flag = false;
    return duration::inf();
}


inline duration hearing_node::unplanned_event(duration elapsed_dt)
{
    if (sound_field_input.received()) {
        SF = sound_field_input.value();
        for (auto& occ_pos : OP) {
            auto& occ_id = occ_pos.first;
            auto& pos = occ_pos.second;
            next_OS[occ_id] = SF(pos);
            if (OS[occ_id] != next_OS[occ_id]) {
                change_flag = true;
            }
        }
    }
    else if (occupant_position_input.received()) {
        const std::pair<occupant_id, array1d<int64>>& occ_pos = occupant_position_input.value();
        auto& occ_id = occ_pos.first;
        auto& pos = occ_pos.second;
        OP[occ_id] = pos;
        if (OS.find(occ_id) == std::end(OS)) {
            OS[occ_id] = quantity<decltype(_g/_m/_s/_s)>();
        }
        if (SF.empty()) {
            next_OS[occ_id] = 0_g/_m/_s/_s;
        }
        else {
            next_OS[occ_id] = SF(pos);
        }
        if (OS[occ_id] != next_OS[occ_id]) {
            change_flag = true;
        }
    }
    return change_flag ? 0_s : duration::inf();
}


inline duration hearing_node::planned_event(duration elapsed_dt)
{
    for (auto& occ_pos : OP) {
        auto& occ_id = occ_pos.first;
        if (OS[occ_id] != next_OS[occ_id]) {
            OS[occ_id] = next_OS[occ_id];
            occupant_sound_output.send(std::make_pair(occ_id, OS[occ_id]));
        }
    }
    change_flag = false;
    return duration::inf();
}


inline void hearing_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
