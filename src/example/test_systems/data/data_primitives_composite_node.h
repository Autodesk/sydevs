#pragma once
#ifndef DATA_PRIMITIVES_COMPOSITE_NODE_H_
#define DATA_PRIMITIVES_COMPOSITE_NODE_H_

#include <example/test_systems/data/data_primitives_node.h>
#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/parameter_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class data_primitives_composite_node : public composite_node
{
public:
    // Constructor/Destructor:
    data_primitives_composite_node(const std::string& node_name, const node_context& external_context);
    virtual ~data_primitives_composite_node() = default;

    // Ports:

    // Parameters:
    parameter_node<bool> A_bool;
    parameter_node<int64> A_int64;
    parameter_node<float64> A_float64;
    parameter_node<duration> A_duration;
    parameter_node<std::string> A_string;
    parameter_node<bool> B_bool;
    parameter_node<int64> B_int64;
    parameter_node<float64> B_float64;
    parameter_node<duration> B_duration;
    parameter_node<std::string> B_string;

    // Flow Nodes:

    // Message Nodes:
    data_primitives_node A;
    data_primitives_node B;
};


data_primitives_composite_node::data_primitives_composite_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , A_bool("A_bool", internal_context(), true)
    , A_int64("A_int64", internal_context(), 5)
    , A_float64("A_float64", internal_context(), 5.0)
    , A_duration("A_duration", internal_context(), 5_s)
    , A_string("A_string", internal_context(), "five")
    , B_bool("B_bool", internal_context(), false)
    , B_int64("B_int64", internal_context(), 10)
    , B_float64("B_float64", internal_context(), 10.0)
    , B_duration("B_duration", internal_context(), 10_s)
    , B_string("B_string", internal_context(), "ten")
    , A("A", internal_context())
    , B("B", internal_context())
{
    // Initialization Links
    inner_link(A_bool.parameter, A.fi_bool);
    inner_link(A_int64.parameter, A.fi_int64);
    inner_link(A_float64.parameter, A.fi_float64);
    inner_link(A_duration.parameter, A.fi_duration);
    inner_link(A_string.parameter, A.fi_string);
    inner_link(B_bool.parameter, B.fi_bool);
    inner_link(B_int64.parameter, B.fi_int64);
    inner_link(B_float64.parameter, B.fi_float64);
    inner_link(B_duration.parameter, B.fi_duration);
    inner_link(B_string.parameter, B.fi_string);

    // Simulation Links
    inner_link(A.mo_bool, B.mi_bool);
    inner_link(A.mo_int64, B.mi_int64);
    inner_link(A.mo_float64, B.mi_float64);
    inner_link(A.mo_duration, B.mi_duration);
    inner_link(A.mo_string, B.mi_string);
 }


#endif
