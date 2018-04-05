#pragma once
#ifndef DATA_STRUCTURES_COMPOSITE_NODE_H_
#define DATA_STRUCTURES_COMPOSITE_NODE_H_

#include <example/test_systems/data/data_structures_node.h>
#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/parameter_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class data_structures_composite_node : public composite_node
{
public:
    // Constructor/Destructor:
    data_structures_composite_node(const std::string& node_name, const node_context& external_context);
    virtual ~data_structures_composite_node() = default;

    // Ports:

    // Parameters:
    parameter_node<std::vector<bool>> A_vector_bool;
    parameter_node<std::vector<int64>> A_vector_int64;
    parameter_node<std::vector<std::vector<duration>>> A_vector_vector_duration;
    parameter_node<std::map<duration, float64>> A_map_duration_float64;
    parameter_node<std::set<std::pair<float64, std::string>>> A_set_pair_float64_string;
    parameter_node<std::shared_ptr<quantity<decltype(_1/_s)>>> A_rate_ptr;
    parameter_node<std::shared_ptr<std::ostringstream>> A_ostringstream_ptr;
    parameter_node<pointer> A_vector_array_ptr;
    parameter_node<std::vector<bool>> B_vector_bool;
    parameter_node<std::vector<int64>> B_vector_int64;
    parameter_node<std::vector<std::vector<duration>>> B_vector_vector_duration;
    parameter_node<std::map<duration, float64>> B_map_duration_float64;
    parameter_node<std::set<std::pair<float64, std::string>>> B_set_pair_float64_string;
    parameter_node<std::shared_ptr<quantity<decltype(_1/_s)>>> B_rate_ptr;
    parameter_node<std::shared_ptr<std::ostringstream>> B_ostringstream_ptr;
    parameter_node<pointer> B_vector_array_ptr;

    // Flow Nodes:

    // Message Nodes:
    data_structures_node A;
    data_structures_node B;
};


data_structures_composite_node::data_structures_composite_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , A_vector_bool("A_vector_bool", internal_context(), {true, true, true, true, true, true, true, false})
    , A_vector_int64("A_vector_int64", internal_context(), {5, 5, 5, 5, 5})
    , A_vector_vector_duration("A_vector_vector_duration", internal_context(), {{5_us, 5_ms}, {5_s}, {5_ks, 5_Ms, duration::inf()}})
    , A_map_duration_float64("A_map_duration_float64", internal_context(), {{5_s, 5.0}, {-duration::inf(), -555555.0}, {5_ks, 5000.0}, {duration::inf(), 555555.0}, {5_ms, 0.005}})
    , A_set_pair_float64_string("A_set_pair_float64_string", internal_context(), {{5.0, "five"}, {5.0, "zero"}, {0.0, "five"}})
    , A_rate_ptr("A_rate_ptr", internal_context())
    , A_ostringstream_ptr("A_ostringstream_ptr", internal_context())
    , A_vector_array_ptr("A_vector_array_ptr", internal_context(), pointer())
    , B_vector_bool("B_vector_bool", internal_context(), {false, false, false, false, true, true, true, true})
    , B_vector_int64("B_vector_int64", internal_context(), {10, 10, 10, 10, 10})
    , B_vector_vector_duration("B_vector_vector_duration", internal_context(), {{10_us, 10_ms}, {10_s}, {10_ks, 10_Ms, -duration::inf()}})
    , B_map_duration_float64("B_map_duration_float64", internal_context(), {{10_s, 10.0}, {-duration::inf(), -101010101010.0}, {10_ks, 10000.0}, {duration::inf(), 101010101010.0}, {10_ms, 0.01}})
    , B_set_pair_float64_string("B_set_pair_float64_string", internal_context(), {{10.0, "ten"}, {10.0, "eleven"}, {20.0, "ten"}})
    , B_rate_ptr("B_rate_ptr", internal_context())
    , B_ostringstream_ptr("B_ostringstream_ptr", internal_context())
    , B_vector_array_ptr("B_vector_array_ptr", internal_context(), pointer())
    , A("A", internal_context())
    , B("B", internal_context())
{
    // Parameter Pointer Values
    {
        auto rate_ptr = std::make_shared<quantity<decltype(_1/_s)>>(1/1_s);
        A_rate_ptr.set_value(rate_ptr);
        auto ostringstream_ptr = std::make_shared<std::ostringstream>();
        (*ostringstream_ptr) << "5 created;";
        A_ostringstream_ptr.set_value(ostringstream_ptr);
        auto* vector_array_ptr = new std::vector<std::array<float64, 3>>();
        vector_array_ptr->push_back({{5.0, 5.0, 5.0}});
        vector_array_ptr->push_back({{5.0, 5.0, 5.0}});
        A_vector_array_ptr.set_value(pointer(vector_array_ptr));
    }
    {
        auto rate_ptr = std::make_shared<quantity<decltype(_1/_s)>>(-10/1_s);
        B_rate_ptr.set_value(rate_ptr);
        auto ostringstream_ptr = std::make_shared<std::ostringstream>();
        (*ostringstream_ptr) << "10 created;";
        B_ostringstream_ptr.set_value(ostringstream_ptr);
        auto* vector_array_ptr = new std::vector<std::array<float64, 3>>();
        vector_array_ptr->push_back({{10.0, 10.0, 10.0}});
        vector_array_ptr->push_back({{10.0, 10.0, 10.0}});
        B_vector_array_ptr.set_value(pointer(vector_array_ptr));
    }

    // Initialization Links
    inner_link(A_vector_bool.parameter, A.fi_vector_bool);
    inner_link(A_vector_int64.parameter, A.fi_vector_int64);
    inner_link(A_vector_vector_duration.parameter, A.fi_vector_vector_duration);
    inner_link(A_map_duration_float64.parameter, A.fi_map_duration_float64);
    inner_link(A_set_pair_float64_string.parameter, A.fi_set_pair_float64_string);
    inner_link(A_rate_ptr.parameter, A.fi_rate_ptr);
    inner_link(A_ostringstream_ptr.parameter, A.fi_ostringstream_ptr);
    inner_link(A_vector_array_ptr.parameter, A.fi_vector_array_ptr);
    inner_link(B_vector_bool.parameter, B.fi_vector_bool);
    inner_link(B_vector_int64.parameter, B.fi_vector_int64);
    inner_link(B_vector_vector_duration.parameter, B.fi_vector_vector_duration);
    inner_link(B_map_duration_float64.parameter, B.fi_map_duration_float64);
    inner_link(B_set_pair_float64_string.parameter, B.fi_set_pair_float64_string);
    inner_link(B_rate_ptr.parameter, B.fi_rate_ptr);
    inner_link(B_ostringstream_ptr.parameter, B.fi_ostringstream_ptr);
    inner_link(B_vector_array_ptr.parameter, B.fi_vector_array_ptr);

    // Simulation Links
    inner_link(A.mo_vector_bool, B.mi_vector_bool);
    inner_link(A.mo_vector_int64, B.mi_vector_int64);
    inner_link(A.mo_vector_vector_duration, B.mi_vector_vector_duration);
    inner_link(A.mo_map_duration_float64, B.mi_map_duration_float64);
    inner_link(A.mo_map_duration_float64, B.mi_map_duration_float64);
    inner_link(A.mo_set_pair_float64_string, B.mi_set_pair_float64_string);
    inner_link(A.mo_rate_ptr, B.mi_rate_ptr);
    inner_link(A.mo_ostringstream_ptr, B.mi_ostringstream_ptr);
    inner_link(A.mo_vector_array_ptr, B.mi_vector_array_ptr);
}


#endif
