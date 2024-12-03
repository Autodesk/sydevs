#pragma once
#ifndef SYDEVS_EXAMPLES_UPPER_CELLULAR_NODE_H_
#define SYDEVS_EXAMPLES_UPPER_CELLULAR_NODE_H_

#include <sydevs/systems/collection_node.h>
#include <examples/test_systems/cellular/lower_cellular_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class upper_cellular_node : public collection_node<array1d<int64>, lower_cellular_node>
{
public:
    // Constructor/Destructor:
    upper_cellular_node(const std::string& node_name, const node_context& external_context);
    virtual ~upper_cellular_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:

protected:
    // Event Handlers:
    virtual duration macro_initialization_event();
    virtual duration macro_unplanned_event(duration elapsed_dt);
    virtual duration micro_planned_event(const array1d<int64>& agent_id, duration elapsed_dt);
    virtual duration macro_planned_event(duration elapsed_dt);
    virtual void macro_finalization_event(duration elapsed_dt);
};


inline upper_cellular_node::upper_cellular_node(const std::string& node_name, const node_context& external_context)
    : collection_node<array1d<int64>, lower_cellular_node>(node_name, external_context)
{
}


inline duration upper_cellular_node::macro_initialization_event()
{
    for (int64 row = 0; row < 2; ++row) {
        for (int64 col = 0; col < 2; ++col) {
            array1d<int64> coords({2}, {2*row, 2*col});
            get(prototype.coords_input) = coords;
            create_agent(coords);
        }
    }
    return duration::inf();
}


inline duration upper_cellular_node::macro_unplanned_event(duration elapsed_dt)
{
    return duration();
}


inline duration upper_cellular_node::micro_planned_event(const array1d<int64>& agent_id, duration elapsed_dt)
{
    return duration();
}


inline duration upper_cellular_node::macro_planned_event(duration elapsed_dt)
{
    return duration();
}


inline void upper_cellular_node::macro_finalization_event(duration elapsed_dt)
{
    auto cellspace = array2d<int64>({4, 4}, 0);
    for (int64 row = 0; row < 2; ++row) {
        for (int64 col = 0; col < 2; ++col) {
            array1d<int64> coords({2}, {2*row, 2*col});
            remove_agent(coords);
            cellspace[range().start_at(2*row).stop_before(2*row + 2)]
                     [range().start_at(2*col).stop_before(2*col+ 2)] = get(prototype.cellspace);
        }
    }
    for (int64 i = 0; i < 4; ++i) {
        print(cellspace[i][range()]);
    }
}


}  // namespace

#endif
