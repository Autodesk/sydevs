#pragma once
#ifndef DATA_ARRAYNDS_NODE_H_
#define DATA_ARRAYNDS_NODE_H_

#include <sydevs/systems/atomic_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class data_arraynds_node : public atomic_node
{
public:
    // Constructor/Destructor:
    data_arraynds_node(const std::string& node_name, const node_context& external_context);
    virtual ~data_arraynds_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, array2d<int64>> fi_a2d;
    port<flow, input, array1d<int64>> fi_s1d;
    port<flow, input, pointer> fi_a2d_ptr;
    port<flow, input, pointer> fi_s1d_ptr;
    port<message, input, array2d<int64>> mi_a2d;
    port<message, input, array1d<int64>> mi_s1d;
    port<message, input, pointer> mi_a2d_ptr;
    port<message, input, pointer> mi_s1d_ptr;
    port<message, output, array2d<int64>> mo_a2d;
    port<message, output, array1d<int64>> mo_s1d;
    port<message, output, pointer> mo_a2d_ptr;
    port<message, output, pointer> mo_s1d_ptr;
    port<flow, output, array2d<int64>> fo_a2d;
    port<flow, output, array1d<int64>> fo_s1d;
    port<flow, output, pointer> fo_a2d_ptr;
    port<flow, output, pointer> fo_s1d_ptr;

protected:
    // State Variables:
    array2d<int64> sv_a2d;
    array1d<int64> sv_s1d;
    pointer sv_a2d_ptr;
    pointer sv_s1d_ptr;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline data_arraynds_node::data_arraynds_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , fi_a2d("fi_a2d", external_interface())
    , fi_s1d("fi_s1d", external_interface())
    , fi_a2d_ptr("fi_a2d_ptr", external_interface())
    , fi_s1d_ptr("fi_s1d_ptr", external_interface())
    , mi_a2d("mi_a2d", external_interface())
    , mi_s1d("mi_s1d", external_interface())
    , mi_a2d_ptr("mi_a2d_ptr", external_interface())
    , mi_s1d_ptr("mi_s1d_ptr", external_interface())
    , mo_a2d("mo_a2d", external_interface())
    , mo_s1d("mo_s1d", external_interface())
    , mo_a2d_ptr("mo_a2d_ptr", external_interface())
    , mo_s1d_ptr("mo_s1d_ptr", external_interface())
    , fo_a2d("fo_a2d", external_interface())
    , fo_s1d("fo_s1d", external_interface())
    , fo_a2d_ptr("fo_a2d_ptr", external_interface())
    , fo_s1d_ptr("fo_s1d_ptr", external_interface())
{
}


inline duration data_arraynds_node::initialization_event()
{
    sv_a2d = fi_a2d.value();
    sv_s1d = fi_s1d.value();
    sv_a2d_ptr = fi_a2d_ptr.value();
    sv_s1d_ptr = fi_s1d_ptr.value();

    auto& a2d_var = sv_a2d_ptr.dereference<array2d<int64>>();
    auto& s1d_var = sv_s1d_ptr.dereference<array1d<int64>>();

    print("sv_a2d  = " + tostring(sv_a2d));
    print("sv_s1d  = " + tostring(sv_s1d));
    print("a2d_var = " + tostring(a2d_var));
    print("s1d_var = " + tostring(s1d_var));

    sv_a2d({0, 0}) = 100;
    a2d_var({0, 0}) = 100;
    
    return 0_s;
}


inline duration data_arraynds_node::unplanned_event(duration elapsed_dt)
{
    if (mi_a2d.received()) {
        sv_a2d = mi_a2d.value();
        sv_a2d({1, 1}) = 200;
    }
    else if (mi_s1d.received()) {
        sv_s1d = mi_s1d.value();
        sv_s1d[2] = 300;
    }
    else if (mi_a2d_ptr.received()) {
        sv_a2d_ptr = mi_a2d_ptr.value();
        auto& a2d_var = sv_a2d_ptr.dereference<array2d<int64>>();
        a2d_var({1, 1}) = 200;
    }
    else if (mi_s1d_ptr.received()) {
        sv_s1d_ptr = mi_s1d_ptr.value();
        auto& s1d_var = sv_s1d_ptr.dereference<array1d<int64>>();
        s1d_var[2] = 300;
    }

    {
        auto& a2d_var = sv_a2d_ptr.dereference<array2d<int64>>();
        auto& s1d_var = sv_s1d_ptr.dereference<array1d<int64>>();

        print("sv_a2d  = " + tostring(sv_a2d));
        print("sv_s1d  = " + tostring(sv_s1d));
        print("a2d_var = " + tostring(a2d_var));
        print("s1d_var = " + tostring(s1d_var));
    }

    return 0_s;
}


inline duration data_arraynds_node::planned_event(duration elapsed_dt)
{
    auto& a2d_var = sv_a2d_ptr.dereference<array2d<int64>>();
    auto& s1d_var = sv_s1d_ptr.dereference<array1d<int64>>();

    mo_a2d.send(sv_a2d);
    mo_s1d.send(sv_a2d[range()][1]);
    mo_a2d_ptr.send(sv_a2d_ptr);
    mo_s1d_ptr.send(pointer(new array1d<int64>(a2d_var[range()][1])));

    print("a2d_var = " + tostring(a2d_var));
    print("s1d_var = " + tostring(s1d_var));

    return duration::inf();
}


inline void data_arraynds_node::finalization_event(duration elapsed_dt)
{
    fo_a2d.assign(sv_a2d);
    fo_s1d.assign(sv_s1d);
    fo_a2d_ptr.assign(sv_a2d_ptr);
    fo_s1d_ptr.assign(sv_s1d_ptr);

    auto& a2d_var = sv_a2d_ptr.dereference<array2d<int64>>();
    auto& s1d_var = sv_s1d_ptr.dereference<array1d<int64>>();

    print("a2d_var = " + tostring(a2d_var));
    print("s1d_var = " + tostring(s1d_var));
}


#endif
