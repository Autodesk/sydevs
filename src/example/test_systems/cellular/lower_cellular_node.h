#pragma once
#ifndef LOWER_CELLULAR_NODE_H_
#define LOWER_CELLULAR_NODE_H_

#include <sydevs/systems/collection_node.h>
#include <example/test_systems/cellular/cell_function_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class lower_cellular_node : public collection_node<array1d<int64>, cell_function_node>
{
public:
    // Constructor/Destructor:
    lower_cellular_node(const std::string& node_name, const node_context& external_context);
    virtual ~lower_cellular_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, array1d<int64>> coords_input;
    port<flow, output, array2d<int64>> cellspace;

protected:
    // State Variables:
    array2d<int64> cellspace_;

    // Event Handlers:
    virtual duration macro_initialization_event();
    virtual duration macro_unplanned_event(duration elapsed_dt);
    virtual duration micro_planned_event(const array1d<int64>& agent_id, duration elapsed_dt);
    virtual duration macro_planned_event(duration elapsed_dt);
    virtual void macro_finalization_event(duration elapsed_dt);
};


inline lower_cellular_node::lower_cellular_node(const std::string& node_name, const node_context& external_context)
    : collection_node<array1d<int64>, cell_function_node>(node_name, external_context)
    , coords_input("coords_input", external_interface())
    , cellspace("cellspace", external_interface())
{
}


inline duration lower_cellular_node::macro_initialization_event()
{
    const array1d<int64>& coords = coords_input.value();
    cellspace_ = array2d<int64>({2, 2}, 0);

    for (int64 row = 0; row < 2; ++row) {
        for (int64 col = 0; col < 2; ++col) {
            auto offset = array1d<int64>({2}, {row, col});
            auto cell_coords = coords + offset;
            access(prototype.coords_input) = cell_coords;
            invoke_agent(cell_coords);
            cellspace_(offset) = access(prototype.flow_output);
        }
    }
    return duration::inf();
}


inline duration lower_cellular_node::macro_unplanned_event(duration elapsed_dt)
{
    return duration();
}


inline duration lower_cellular_node::micro_planned_event(const array1d<int64>& agent_id, duration elapsed_dt)
{
    return duration();
}


inline duration lower_cellular_node::macro_planned_event(duration elapsed_dt)
{
    return duration();
}


inline void lower_cellular_node::macro_finalization_event(duration elapsed_dt)
{
    cellspace.assign(cellspace_);
}


#endif
