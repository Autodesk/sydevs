#pragma once
#ifndef DATA_ARRAYNDS_COMPOSITE_NODE_H_
#define DATA_ARRAYNDS_COMPOSITE_NODE_H_

#include <example/test_systems/data/data_arraynds_node.h>
#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/parameter_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class data_arraynds_composite_node : public composite_node
{
public:
    // Constructor/Destructor:
    data_arraynds_composite_node(const std::string& node_name, const node_context& external_context);
    virtual ~data_arraynds_composite_node() = default;

    // Ports:

    // Parameters:
    parameter_node<array2d<int64>> A_a2d;
    parameter_node<array1d<int64>> A_s1d;
    parameter_node<pointer> A_a2d_ptr;
    parameter_node<pointer> A_s1d_ptr;
    parameter_node<array2d<int64>> B_a2d;
    parameter_node<array1d<int64>> B_s1d;
    parameter_node<pointer> B_a2d_ptr;
    parameter_node<pointer> B_s1d_ptr;

    // Flow Nodes:

    // Message Nodes:
    data_arraynds_node A;
    data_arraynds_node B;
};


data_arraynds_composite_node::data_arraynds_composite_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , A_a2d("A_a2d", internal_context(), array2d<int64>())
    , A_s1d("A_s1d", internal_context(), array1d<int64>())
    , A_a2d_ptr("A_a2d_ptr", internal_context(), pointer())
    , A_s1d_ptr("A_s1d_ptr", internal_context(), pointer())
    , B_a2d("B_a2d", internal_context(), array2d<int64>())
    , B_s1d("B_s1d", internal_context(), array1d<int64>())
    , B_a2d_ptr("B_a2d_ptr", internal_context(), pointer())
    , B_s1d_ptr("B_s1d_ptr", internal_context(), pointer())
    , A("A", internal_context())
    , B("B", internal_context())
{
    {
        auto a2d = array2d<int64>({4, 3}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
        A_a2d.set_value(a2d);
        A_s1d.set_value(a2d[range()][0]);
        A_a2d_ptr.set_value(pointer(new array2d<int64>(a2d.copy())));
        auto& a2d_var = A_a2d_ptr.value().dereference<array2d<int64>>();
        A_s1d_ptr.set_value(pointer(new array1d<int64>(a2d_var[range()][0])));
    }
    {
        auto a2d = array2d<int64>({4, 3}, {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11});
        B_a2d.set_value(a2d);
        B_s1d.set_value(a2d[range()][0]);
        B_a2d_ptr.set_value(pointer(new array2d<int64>(a2d.copy())));
        auto& a2d_var = B_a2d_ptr.value().dereference<array2d<int64>>();
        B_s1d_ptr.set_value(pointer(new array1d<int64>(a2d_var[range()][0])));
    }

    // Initialization Links
    inner_link(A_a2d.parameter, A.fi_a2d);
    inner_link(A_s1d.parameter, A.fi_s1d);
    inner_link(A_a2d_ptr.parameter, A.fi_a2d_ptr);
    inner_link(A_s1d_ptr.parameter, A.fi_s1d_ptr);
    inner_link(B_a2d.parameter, B.fi_a2d);
    inner_link(B_s1d.parameter, B.fi_s1d);
    inner_link(B_a2d_ptr.parameter, B.fi_a2d_ptr);
    inner_link(B_s1d_ptr.parameter, B.fi_s1d_ptr);

    // Simulation Links
    inner_link(A.mo_a2d, B.mi_a2d);
    inner_link(A.mo_s1d, B.mi_s1d);
    inner_link(A.mo_a2d_ptr, B.mi_a2d_ptr);
    inner_link(A.mo_s1d_ptr, B.mi_s1d_ptr);
 }


#endif
