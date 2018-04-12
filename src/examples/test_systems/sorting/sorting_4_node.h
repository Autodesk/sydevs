#pragma once
#ifndef SYDEVS_EXAMPLES_SORTING_4_NODE_H_
#define SYDEVS_EXAMPLES_SORTING_4_NODE_H_

#include <examples/test_systems/sorting/sorting_3_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class sorting_4_node : public composite_node
{
public:
    // Constructor/Destructor:
    sorting_4_node(const std::string& node_name, const node_context& external_context);
    virtual ~sorting_4_node() = default;

    // Ports:
    port<flow, input, int64> a_input;
    port<flow, input, int64> b_input;
    port<flow, input, int64> c_input;
    port<flow, input, int64> d_input;
    port<flow, output, int64> a_output;
    port<flow, output, int64> b_output;
    port<flow, output, int64> c_output;
    port<flow, output, int64> d_output;

    // Components:
    sorting_3_node A;
    sorting_2_node B;
    sorting_2_node C;
    sorting_2_node D;
};


sorting_4_node::sorting_4_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , a_input("a_input", external_interface())
    , b_input("b_input", external_interface())
    , c_input("c_input", external_interface())
    , d_input("d_input", external_interface())
    , a_output("a_output", external_interface())
    , b_output("b_output", external_interface())
    , c_output("c_output", external_interface())
    , d_output("d_output", external_interface())
    , A("A", internal_context())
    , B("B", internal_context())
    , C("C", internal_context())
    , D("D", internal_context())
{
    // Flow Links
    inward_link(a_input, A.a_input);
    inward_link(b_input, A.b_input);
    inward_link(c_input, A.c_input);
    inner_link(A.a_output, D.a_input);
    inner_link(A.b_output, C.a_input);
    inner_link(A.c_output, B.a_input);
    inward_link(d_input, B.b_input);
    inner_link(B.a_output, C.b_input);
    inner_link(C.a_output, D.b_input);
    outward_link(B.b_output, d_output);
    outward_link(C.b_output, c_output);
    outward_link(D.a_output, a_output);
    outward_link(D.b_output, b_output);
}


}  // namespace

#endif
