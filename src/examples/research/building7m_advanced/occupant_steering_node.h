#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_OCCUPANT_STEERING_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_OCCUPANT_STEERING_NODE_H_

#include <examples/research/building7m_advanced/building_occupant_ids.h>
#include <examples/research/building7m_advanced/building_layout_codes.h>
#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class occupant_steering_node : public atomic_node
{
public:
    // Constructor/Destructor:
    occupant_steering_node(const std::string& node_name, const node_context& external_context);
    virtual ~occupant_steering_node() = default;

    // Attributes:
    virtual scale time_precision() const { return nano; }

    // Ports:
    port<flow, input, std::pair<array2d<int64>, std::pair<distance, distance>>> building_layout_input;
    port<flow, input, quantity<decltype(_m/_s)>> walking_speed_input;
    port<flow, input, std::map<occupant_id, array1d<int64>>> initial_positions_input;
    port<message, input, std::pair<occupant_id, array1d<int64>>> occupant_destination_input;
    port<message, output, std::pair<occupant_id, array1d<int64>>> occupant_position_output;

protected:
    // State Variables:
    array2d<int64> L;                               // building layout
    int64 nx;                                       // number of cells in the x dimension
    int64 ny;                                       // number of cells in the y dimension
    distance d;                                     // distance between cells
    duration step_dt;                               // time_step
    std::array<array1d<int64>, 8> directions;       // array of directions;
    std::map<occupant_id, array1d<int64>> OP;       // occupant positions
    std::map<occupant_id, array1d<int64>> next_OP;  // next occupant positions
    std::map<occupant_id, array1d<int64>> OD;       // occupant destinations
    duration planned_dt;                            // duration until the next position change

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);

    // Helper Functions:
    float64 direction_score(array1d<int64> pos, array1d<int64> dest_pos);
};


inline occupant_steering_node::occupant_steering_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , building_layout_input("building_layout_input", external_interface())
    , walking_speed_input("walking_speed_input", external_interface())
    , initial_positions_input("initial_positions_input", external_interface())
    , occupant_destination_input("occupant_destination_input", external_interface())
    , occupant_position_output("occupant_position_output", external_interface())
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


inline duration occupant_steering_node::initialization_event()
{
    L = building_layout_input.value().first;
    d = building_layout_input.value().second.first;
    nx = L.dims()[0];
    ny = L.dims()[1];
    auto walking_speed = walking_speed_input.value();
    step_dt = (d/walking_speed).fixed_at(time_precision());
    OP = initial_positions_input.value();
    next_OP = OP;
    OD = std::map<occupant_id, array1d<int64>>();
    planned_dt = 0_s;
    return planned_dt;
}


inline duration occupant_steering_node::unplanned_event(duration elapsed_dt)
{
    if (occupant_destination_input.received()) {
        const auto& [occ_id, dest_pos] = occupant_destination_input.value();
        OD[occ_id] = dest_pos;
    }
    planned_dt -= elapsed_dt;
    return planned_dt;
}


inline duration occupant_steering_node::planned_event(duration elapsed_dt)
{
    OP = next_OP;
    for (const auto& [occ_id, pos] : OP) {
        occupant_position_output.send({ occ_id, pos });
        if (OD.find(occ_id) != std::end(OD)) {
            if (!all(pos == OD[occ_id])) {
                std::array<int64, 8> scores;
                for (int64 i = 0; i < 8; ++i) {
                    scores[i] = direction_score(pos + directions[i], OD[occ_id]);
                }
                auto direction_iter = std::min_element(std::begin(scores), std::end(scores));
                next_OP[occ_id] = pos + directions[std::distance(std::begin(scores), direction_iter)];
            }
        }
    }
    planned_dt = step_dt;
    return planned_dt;
}


inline void occupant_steering_node::finalization_event(duration elapsed_dt)
{
}


inline float64 occupant_steering_node::direction_score(array1d<int64> pos, array1d<int64> dest_pos)
{
    auto delta_pos = (pos - dest_pos);
    auto delta_pos_squared = delta_pos*delta_pos;
    return delta_pos_squared(0) + delta_pos_squared(1);
}


}  // namespace

#endif
