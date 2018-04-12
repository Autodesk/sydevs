#pragma once
#ifndef SYDEVS_EXAMPLES_SORTING_2_NODE_H_
#define SYDEVS_EXAMPLES_SORTING_2_NODE_H_

#include <sydevs/systems/function_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class sorting_2_node : public function_node
{
public:
    // Constructor/Destructor:
    sorting_2_node(const std::string& node_name, const node_context& external_context);
    virtual ~sorting_2_node() = default;

    // Ports:
    port<flow, input, int64> a_input;
    port<flow, input, int64> b_input;
    port<flow, output, int64> a_output;
    port<flow, output, int64> b_output;

protected:
    // Event Handlers:
    void flow_event();
};


inline sorting_2_node::sorting_2_node(const std::string& node_name, const node_context& external_context)
    : function_node(node_name, external_context)
    , a_input("a_input", external_interface())
    , b_input("b_input", external_interface())
    , a_output("a_output", external_interface())
    , b_output("b_output", external_interface())
{
}


inline void sorting_2_node::flow_event()
{
    int64 a = a_input.value();
    int64 b = b_input.value();
    if (a <= b) {
        a_output.assign(a);
        b_output.assign(b);
    }
    else {
        a_output.assign(b);
        b_output.assign(a);
    }
}


}  // namespace

#endif
