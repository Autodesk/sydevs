#pragma once
#ifndef SYDEVS_EXAMPLES_DATA_CUSTOM_COMPOSITE_NODE_H_
#define SYDEVS_EXAMPLES_DATA_CUSTOM_COMPOSITE_NODE_H_

#include <examples/test_systems/data/data_custom_node.h>
#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/parameter_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class data_custom_composite_node : public composite_node
{
public:
    // Constructor/Destructor:
    data_custom_composite_node(const std::string& node_name, const node_context& external_context);
    virtual ~data_custom_composite_node() = default;

    // Ports:

    // Parameters:
    parameter_node<foo> A_foo;
    parameter_node<foo> B_foo;

    // Flow Nodes:

    // Message Nodes:
    data_custom_node A;
    data_custom_node B;
};


data_custom_composite_node::data_custom_composite_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , A_foo("A_foo", internal_context())
    , B_foo("B_foo", internal_context())
    , A("A", internal_context())
    , B("B", internal_context())
{
    foo foo_A;
    foo foo_B;

    foo_A.n = 10;
    foo_B.n = 25;

    A_foo.set_value(foo_A);
    B_foo.set_value(foo_B);

    // Initialization Links
    inner_link(A_foo.parameter, A.fi_foo);
    inner_link(B_foo.parameter, B.fi_foo);

    // Simulation Links
    inner_link(A.mo_foo, B.mi_foo);
 }


}  // namespace

#endif
