#pragma once
#ifndef SYDEVS_EXAMPLES_CELL_FUNCTION_NODE_H_
#define SYDEVS_EXAMPLES_CELL_FUNCTION_NODE_H_

#include <sydevs/systems/function_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class cell_function_node : public function_node
{
public:
    // Constructor/Destructor:
    cell_function_node(const std::string& node_name, const node_context& external_context);
    virtual ~cell_function_node() = default;

    // Ports:
    port<flow, input, array1d<int64>> coords_input;
    port<flow, output, int64> flow_output;

protected:
    // Event Handlers:
    void flow_event();
};


inline cell_function_node::cell_function_node(const std::string& node_name, const node_context& external_context)
    : function_node(node_name, external_context)
    , coords_input("coords_input", external_interface())
    , flow_output("flow_output", external_interface())
{
}


inline void cell_function_node::flow_event()
{
    const array1d<int64>& coords = coords_input.value();
    flow_output.assign(coords({0})*(coords({0}) + 1) + coords({1})*(coords({1}) + 1) + 10);
}


}  // namespace

#endif
