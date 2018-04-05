#pragma once
#ifndef DATA_TUPLES_COMPOSITE_NODE_H_
#define DATA_TUPLES_COMPOSITE_NODE_H_

#include <example/test_systems/data/data_tuples_node.h>
#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/parameter_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class data_tuples_composite_node : public composite_node
{
public:
    // Constructor/Destructor:
    data_tuples_composite_node(const std::string& node_name, const node_context& external_context);
    virtual ~data_tuples_composite_node() = default;

    // Ports:

    // Parameters:
    parameter_node<std::tuple<int64>> A_tuple_int64;
    parameter_node<std::tuple<std::string, bool, float64>> A_tuple_string_bool_float64;
    parameter_node<std::tuple<mass, std::pair<data_tuples_node::speed, std::pair<bool, bool>>>> A_tuple_mass_speed_pair_bool_bool;
    parameter_node<std::tuple<int64>> B_tuple_int64;
    parameter_node<std::tuple<std::string, bool, float64>> B_tuple_string_bool_float64;
    parameter_node<std::tuple<mass, std::pair<data_tuples_node::speed, std::pair<bool, bool>>>> B_tuple_mass_speed_pair_bool_bool;

    // Flow Nodes:

    // Message Nodes:
    data_tuples_node A;
    data_tuples_node B;
};


data_tuples_composite_node::data_tuples_composite_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , A_tuple_int64("A_tuple_int64", internal_context(), {7})
    , A_tuple_string_bool_float64("A_tuple_string_bool_float64", internal_context(), {"a string within tuple A", false, 1.25})
, A_tuple_mass_speed_pair_bool_bool("A_tuple_mass_speed_pair_bool_bool", internal_context(), std::tuple<mass, std::pair<data_tuples_node::speed, std::pair<bool, bool>>>{3_kg, {15_m/_s, {true, false}}})
    , B_tuple_int64("B_tuple_int64", internal_context(), {42})
    , B_tuple_string_bool_float64("B_tuple_string_bool_float64", internal_context(), {"a string within tuple B", true, -9.5})
    , B_tuple_mass_speed_pair_bool_bool("B_tuple_mass_speed_pair_bool_bool", internal_context(), std::tuple<mass, std::pair<data_tuples_node::speed, std::pair<bool, bool>>>{3_kg, {15_m/_s, {true, true}}})
    , A("A", internal_context())
    , B("B", internal_context())
{
    // Initialization Links
    inner_link(A_tuple_int64.parameter, A.fi_tuple_int64);
    inner_link(A_tuple_string_bool_float64.parameter, A.fi_tuple_string_bool_float64);
    inner_link(A_tuple_mass_speed_pair_bool_bool.parameter, A.fi_tuple_mass_speed_pair_bool_bool);
    inner_link(B_tuple_int64.parameter, B.fi_tuple_int64);
    inner_link(B_tuple_string_bool_float64.parameter, B.fi_tuple_string_bool_float64);
    inner_link(B_tuple_mass_speed_pair_bool_bool.parameter, B.fi_tuple_mass_speed_pair_bool_bool);

    // Simulation Links
    inner_link(A.mo_tuple_int64, B.mi_tuple_int64);
    inner_link(A.mo_tuple_string_bool_float64, B.mi_tuple_string_bool_float64);
    inner_link(A.mo_tuple_mass_speed_pair_bool_bool, B.mi_tuple_mass_speed_pair_bool_bool);
}


#endif
