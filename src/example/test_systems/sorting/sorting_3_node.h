#pragma once
#ifndef SORTING_3_NODE_H_
#define SORTING_3_NODE_H_

#include <example/test_systems/sorting/sorting_2_node.h>
#include <sydevs/systems/composite_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class sorting_3_node : public composite_node
{
public:
    // Constructor/Destructor:
    sorting_3_node(const std::string& node_name, const node_context& external_context);
    virtual ~sorting_3_node() = default;

    // Ports:
    port<flow, input, int64> a_input;
    port<flow, input, int64> b_input;
    port<flow, input, int64> c_input;
    port<flow, output, int64> a_output;
    port<flow, output, int64> b_output;
    port<flow, output, int64> c_output;

    // Components:
    sorting_2_node A;
    sorting_2_node B;
    sorting_2_node C;
};


sorting_3_node::sorting_3_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , a_input("a_input", external_interface())
    , b_input("b_input", external_interface())
    , c_input("c_input", external_interface())
    , a_output("a_output", external_interface())
    , b_output("b_output", external_interface())
    , c_output("c_output", external_interface())
    , A("A", internal_context())
    , B("B", internal_context())
    , C("C", internal_context())
{
    // Flow Links
    inward_link(a_input, A.a_input);
    inward_link(b_input, A.b_input);
    inner_link(A.a_output, C.a_input);
    inner_link(A.b_output, B.a_input);
    inward_link(c_input, B.b_input);
    inner_link(B.a_output, C.b_input);
    outward_link(B.b_output, c_output);
    outward_link(C.a_output, a_output);
    outward_link(C.b_output, b_output);
}


#endif
