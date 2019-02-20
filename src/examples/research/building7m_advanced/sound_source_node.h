#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_SOUND_SOURCE_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_SOUND_SOURCE_NODE_H_

#include <examples/research/building7m_advanced/building_occupant_ids.h>
#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class sound_source_node : public atomic_node
{
public:
    // Constructor/Destructor:
    sound_source_node(const std::string& node_name, const node_context& external_context);
    virtual ~sound_source_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, quantity<decltype(_g/_m/_s/_s)>> occupant_walking_sound_input;
    port<message, input, std::pair<occupant_id, array1d<int64>>> occupant_position_input;
    port<message, output, std::tuple<occupant_id, array1d<int64>, quantity<decltype(_g/_m/_s/_s)>>> sound_source_output;
 
protected:
    // State Variables:
    quantity<decltype(_g/_m/_s/_s)> walking_P;      // occupant walking sound
    std::map<occupant_id, array1d<int64>> OP;       // occupant positions
    std::map<occupant_id, array1d<int64>> next_OP;  // next occupant positions
    bool change_flag;                               // flag indicating change

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline sound_source_node::sound_source_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , occupant_walking_sound_input("occupant_walking_sound_input", external_interface())
    , occupant_position_input("occupant_position_input", external_interface())
    , sound_source_output("sound_source_output", external_interface())
{
}


inline duration sound_source_node::initialization_event()
{
    walking_P = occupant_walking_sound_input.value();
    OP = std::map<occupant_id, array1d<int64>>();
    change_flag = false;
    return duration::inf();
}


inline duration sound_source_node::unplanned_event(duration elapsed_dt)
{
    if (occupant_position_input.received()) {
        const std::pair<occupant_id, array1d<int64>>& occ_pos = occupant_position_input.value();
        auto& occ_id = occ_pos.first;
        auto& pos = occ_pos.second;
        next_OP[occ_id] = pos;
        if (OP.find(occ_id) == std::end(OP)) {
            OP[occ_id] = array1d<int64>();
        }
        if (!all(OP[occ_id] == next_OP[occ_id])) {
            change_flag = true;
        }
    }
    return change_flag ? 0_s : duration::inf();
}


inline duration sound_source_node::planned_event(duration elapsed_dt)
{
    for (auto& occ_pos : OP) {
        auto& occ_id = occ_pos.first;
        auto& pos = occ_pos.second;
        if (!all(pos == next_OP[occ_id])) {
            sound_source_output.send(std::make_tuple(occ_id, pos, walking_P));
        }
    }
    OP = next_OP;
    change_flag = false;
    return duration::inf();
}


inline void sound_source_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
