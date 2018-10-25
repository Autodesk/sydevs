#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_OCCUPANT_PLANNING_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_OCCUPANT_PLANNING_NODE_H_

#include <examples/demo/building7m_advanced/building_occupant_ids.h>
#include <examples/demo/building7m_advanced/building_layout_codes.h>
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
    port<flow, input, std::pair<array2d<int64>, distance>> building_layout_input;
    port<flow, input, thermodynamic_temperature> high_temperature_input;
    port<flow, input, thermodynamic_temperature> initial_temperature_input;
    port<flow, input, std::map<occupant_id, array1d<int64>>> initial_positions_input;
    port<message, input, std::pair<occupant_id, array1d<int64>>> occupant_position_input;
    port<message, input, std::pair<occupant_id, thermodynamic_temperature>> occupant_temperature_input;
    port<message, output, std::pair<occupant_id, array1d<int64>>> occupant_destination_output;

protected:
    // State Variables:
    array2d<int64> L;                                     // building layout
    int64 nx;                                             // number of cells in the x dimension
    int64 ny;                                             // number of cells in the y dimension
    distance d;                                           // distance between cells
    thermodynamic_temperature high_T;                     // high temperature
    std::array<array1d<int64>, 8> directions;             // array of directions;
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
    float64 direction_score(float64 angle, array1d<int64> pos0, array1d<int64> pos);
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
    directions[0] = array1d<int64>({2}, {1, 0});
    directions[1] = array1d<int64>({2}, {1, 1});
    directions[2] = array1d<int64>({2}, {0, 1});
    directions[3] = array1d<int64>({2}, {-1, 1});
    directions[4] = array1d<int64>({2}, {-1, 0});
    directions[5] = array1d<int64>({2}, {-1, -1});
    directions[6] = array1d<int64>({2}, {0, -1});
    directions[7] = array1d<int64>({2}, {1, -1});
}


inline duration occupant_planning_node::initialization_event()
{
    L = building_layout_input.value().first;
    nx = L.dims()[0];
    ny = L.dims()[1];
    d = building_layout_input.value().second;
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
    for (auto& occ_pos : OP) {
        auto& occ_id = occ_pos.first;
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
    auto angle = 2.0*pi*angle_dist(rng);
    auto pos0 = OP[occ_id];
    auto pos = pos0;
    auto done = false;
    auto destinations = std::vector<array1d<int64>>();
    while (!done) {
        std::array<int64, 8> scores;
        for (int64 i = 0; i < 8; ++i) {
            scores[i] = direction_score(angle, pos0, pos + directions[i]);
        }
        auto direction_iter = std::min_element(std::begin(scores), std::end(scores));
        pos = pos + directions[std::distance(std::begin(scores), direction_iter)];
        if (L(pos) == indoor_code) {
            destinations.push_back(pos);
        }
        else {
            done = true;
        }
    }
    auto dest_pos = pos;
    if (!destinations.empty()) {
        static std::uniform_int_distribution<int64> dest_dist(0, int64(destinations.size()) - 1);
        auto dest_index = dest_dist(rng);
        dest_pos = destinations[dest_index];
    }
    return dest_pos;
}


inline float64 occupant_planning_node::direction_score(float64 angle, array1d<int64> pos0, array1d<int64> pos)
{
    auto v = pos - pos0;
    auto a = std::atan2(v(1), v(0));
    return std::abs(a - angle);
}


}  // namespace

#endif
