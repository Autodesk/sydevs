#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_OCCUPANT_PLANNING_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_OCCUPANT_PLANNING_NODE_H_

#include <examples/research/building7m_advanced/building_occupant_ids.h>
#include <examples/research/building7m_advanced/building_layout_codes.h>
#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class occupant_planning_node : public atomic_node
{
public:
    // Constructor/Destructor:
    occupant_planning_node(const std::string& node_name, const node_context& external_context);
    virtual ~occupant_planning_node() = default;

    // Attributes:
    virtual scale time_precision() const { return nano; }

    // Ports:
    port<flow, input, std::pair<array2d<int64>, std::pair<distance, distance>>> building_layout_input;
    port<flow, input, thermodynamic_temperature> high_temperature_input;
    port<flow, input, thermodynamic_temperature> initial_temperature_input;
    port<flow, input, std::map<occupant_id, array1d<int64>>> initial_positions_input;
    port<message, input, std::pair<occupant_id, array1d<int64>>> occupant_position_input;
    port<message, input, std::pair<occupant_id, thermodynamic_temperature>> occupant_temperature_input;
    port<message, output, std::pair<occupant_id, array1d<int64>>> occupant_destination_output;

protected:
    // State Variables:
    std::array<array1d<int64>, 8> directions;             // array of directions;
    array2d<int64> L;                                     // building layout
    int64 nx;                                             // number of cells in the x dimension
    int64 ny;                                             // number of cells in the y dimension
    distance d;                                           // distance between cells
    thermodynamic_temperature high_T;                     // high temperature
    std::map<occupant_id, array1d<int64>> OP;             // occupant positions
    std::map<occupant_id, array1d<int64>> OD;             // occupant destinations
    std::map<occupant_id, array1d<int64>> next_OD;        // next occupant destinations
    std::map<occupant_id, thermodynamic_temperature> OT;  // occupant temperatures
    bool change_flag;                                     // flag indicating change

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);

    // Helper Functions:
    array1d<int64> sample_destination(occupant_id occ_id);
    float64 direction_score(float64 angle, array1d<int64> pos0, array1d<int64> pos, array1d<int64> next_pos);
};


inline occupant_planning_node::occupant_planning_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , building_layout_input("building_layout_input", external_interface())
    , high_temperature_input("high_temperature_input", external_interface())
    , initial_temperature_input("initial_temperature_input", external_interface())
    , initial_positions_input("initial_positions_input", external_interface())
    , occupant_position_input("occupant_position_input", external_interface())
    , occupant_temperature_input("occupant_temperature_input", external_interface())
    , occupant_destination_output("occupant_destination_output", external_interface())
{
}


inline duration occupant_planning_node::initialization_event()
{
    directions[0] = array1d<int64>({ 2 }, { 1, 0 });
    directions[1] = array1d<int64>({ 2 }, { 1, 1 });
    directions[2] = array1d<int64>({ 2 }, { 0, 1 });
    directions[3] = array1d<int64>({ 2 }, { -1, 1 });
    directions[4] = array1d<int64>({ 2 }, { -1, 0 });
    directions[5] = array1d<int64>({ 2 }, { -1, -1 });
    directions[6] = array1d<int64>({ 2 }, { 0, -1 });
    directions[7] = array1d<int64>({ 2 }, { 1, -1 });

    L = building_layout_input.value().first;
    d = building_layout_input.value().second.first;
    nx = L.dims()[0];
    ny = L.dims()[1];
    high_T = high_temperature_input.value();

    thermodynamic_temperature T0 = initial_temperature_input.value();
    OP = initial_positions_input.value();

    OD = OP;
    next_OD = OP;
    OT = std::map<occupant_id, thermodynamic_temperature>();
    for (auto& occ_pos : OP) {
        auto& occ_id = occ_pos.first;
        OT[occ_id] = T0;
    }
    change_flag = false;
    return duration::inf();
}


inline duration occupant_planning_node::unplanned_event(duration elapsed_dt)
{
    auto occ_id = occupant_id();
    if (occupant_temperature_input.received()) {
        const std::pair<occupant_id, thermodynamic_temperature>& occ_T = occupant_temperature_input.value();
        occ_id = occ_T.first;
        auto& T = occ_T.second;
        OT[occ_id] = T;
    }
    else if (occupant_position_input.received()) {
        const std::pair<occupant_id, array1d<int64>>& occ_pos = occupant_position_input.value();
        occ_id = occ_pos.first;
        auto& pos = occ_pos.second;
        OP[occ_id] = pos;
    }
    if (occ_id.valid()) {
        if (all(OP[occ_id] == OD[occ_id])) {
            if (OT[occ_id] >= high_T) {
                next_OD[occ_id] = sample_destination(occ_id);
                change_flag = true;
            }
        }
    }
    return change_flag ? 0_s : duration::inf();
}


inline duration occupant_planning_node::planned_event(duration elapsed_dt)
{
    for (const auto& [occ_id, pos] : OP) {
        if (!all(OD[occ_id] == next_OD[occ_id])) {
            OD[occ_id] = next_OD[occ_id];
            occupant_destination_output.send(std::make_pair(occ_id, OD[occ_id]));
        }
    }
    change_flag = false;
    return duration::inf();
}


inline void occupant_planning_node::finalization_event(duration elapsed_dt)
{
}


inline array1d<int64> occupant_planning_node::sample_destination(occupant_id occ_id)
{
    static std::uniform_real_distribution<float64> angle_dist;
    auto pos0 = OP[occ_id];
    auto destinations = std::vector<array1d<int64>>();
    for (int64 attempt_index = 0; destinations.empty() && attempt_index < 10000; ++attempt_index) {
        auto angle = 2.0*pi*angle_dist(rng) - pi;
        auto pos = pos0;
        auto done = false;
        while (!done) {
            float64 min_score = pi;
            int64 direction_index = -1;
            for (int64 i = 0; i < 8; ++i) {
                float64 score = direction_score(angle, pos0, pos, pos + directions[i]);
                if (score < min_score) {
                    min_score = score;
                    direction_index = i;
                }
            }
            pos = pos + directions[direction_index];
            done = true;
            if ((pos(0) >= 0) && (pos(0) < nx) && (pos(1) >= 0) && (pos(1) < ny)) {
                if (L(pos) == indoor_code) {
                    destinations.push_back(pos);
                    done = false;
                }
            }
        }
    }
    auto dest_pos = pos0;
    if (destinations.empty()) {
        throw std::range_error("Failed to find a walkable destination");
    }
    else {
        std::uniform_int_distribution<int64> dest_dist(0, int64(destinations.size()) - 1);
        auto dest_index = dest_dist(rng);
        dest_pos = destinations[dest_index];
    }
    return dest_pos;
}


inline float64 occupant_planning_node::direction_score(float64 angle, array1d<int64> pos0, array1d<int64> pos, array1d<int64> next_pos)
{
    auto score = pi;
    auto delta_pos = (pos - pos0);
    auto delta_pos_squared = delta_pos*delta_pos;
    auto delta_squared = delta_pos_squared(0) + delta_pos_squared(1);
    auto delta_next_pos = (next_pos - pos0);
    auto delta_next_pos_squared = delta_next_pos*delta_next_pos;
    auto delta_next_squared = delta_next_pos_squared(0) + delta_next_pos_squared(1);
    if (delta_next_squared > delta_squared) {
        auto a = std::abs(std::atan2(delta_next_pos(1), delta_next_pos(0)) - angle);
        score = std::min(a, 2*pi - a);
    }
    return score;
}


}  // namespace

#endif
