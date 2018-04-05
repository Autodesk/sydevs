#pragma once
#ifndef SORTING_4_TEST_NODE_H_
#define SORTING_4_TEST_NODE_H_

#include <example/test_systems/sorting/sorting_4_node.h>
#include <sydevs/systems/parameter_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class sorting_4_test_node : public composite_node
{
public:
    // Constructor/Destructor:
    sorting_4_test_node(const std::string& node_name, const node_context& external_context);
    virtual ~sorting_4_test_node() = default;

    // Ports:

    // Components:
    parameter_node<int64> a_parameter;
    parameter_node<int64> b_parameter;
    parameter_node<int64> c_parameter;
    parameter_node<int64> d_parameter;
    sorting_4_node sorter;
};


sorting_4_test_node::sorting_4_test_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , a_parameter("a_parameter", internal_context(), 3)
    , b_parameter("b_parameter", internal_context(), 2)
    , c_parameter("c_parameter", internal_context(), 4)
    , d_parameter("d_parameter", internal_context(), 1)
    , sorter("sorter", internal_context())
{
    // Flow Links
    inner_link(a_parameter.parameter, sorter.a_input);
    inner_link(b_parameter.parameter, sorter.b_input);
    inner_link(c_parameter.parameter, sorter.c_input);
    inner_link(d_parameter.parameter, sorter.d_input);
}


#endif
