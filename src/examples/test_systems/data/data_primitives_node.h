#pragma once
#ifndef SYDEVS_EXAMPLES_DATA_PRIMITIVES_NODE_H_
#define SYDEVS_EXAMPLES_DATA_PRIMITIVES_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class data_primitives_node : public atomic_node
{
public:
    // Constructor/Destructor:
    data_primitives_node(const std::string& node_name, const node_context& external_context);
    virtual ~data_primitives_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, bool> fi_bool;
    port<flow, input, int64> fi_int64;
    port<flow, input, float64> fi_float64;
    port<flow, input, duration> fi_duration;
    port<flow, input, std::string> fi_string;
    port<message, input, bool> mi_bool;
    port<message, input, int64> mi_int64;
    port<message, input, float64> mi_float64;
    port<message, input, duration> mi_duration;
    port<message, input, std::string> mi_string;
    port<message, output, bool> mo_bool;
    port<message, output, int64> mo_int64;
    port<message, output, float64> mo_float64;
    port<message, output, duration> mo_duration;
    port<message, output, std::string> mo_string;
    port<flow, output, bool> fo_bool;
    port<flow, output, int64> fo_int64;
    port<flow, output, float64> fo_float64;
    port<flow, output, duration> fo_duration;
    port<flow, output, std::string> fo_string;

protected:
    // State Variables:
    bool sv_bool;
    int64 sv_int64;
    float64 sv_float64;
    duration sv_duration;
    std::string sv_string;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline data_primitives_node::data_primitives_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , fi_bool("fi_bool", external_interface())
    , fi_int64("fi_int64", external_interface())
    , fi_float64("fi_float64", external_interface())
    , fi_duration("fi_duration", external_interface())
    , fi_string("fi_string", external_interface())
    , mi_bool("mi_bool", external_interface())
    , mi_int64("mi_int64", external_interface())
    , mi_float64("mi_float64", external_interface())
    , mi_duration("mi_duration", external_interface())
    , mi_string("mi_string", external_interface())
    , mo_bool("mo_bool", external_interface())
    , mo_int64("mo_int64", external_interface())
    , mo_float64("mo_float64", external_interface())
    , mo_duration("mo_duration", external_interface())
    , mo_string("mo_string", external_interface())
    , fo_bool("fo_bool", external_interface())
    , fo_int64("fo_int64", external_interface())
    , fo_float64("fo_float64", external_interface())
    , fo_duration("fo_duration", external_interface())
    , fo_string("fo_string", external_interface())
{
}


inline duration data_primitives_node::initialization_event()
{
    sv_bool = fi_bool.value();
    sv_int64 = fi_int64.value();
    sv_float64 = fi_float64.value();
    sv_duration = fi_duration.value();
    sv_string = fi_string.value();
    return 0_s;
}


inline duration data_primitives_node::unplanned_event(duration elapsed_dt)
{
    if (mi_bool.received()) {
        sv_bool = mi_bool.value();
    }
    else if (mi_int64.received()) {
        sv_int64 = mi_int64.value();
    }
    else if (mi_float64.received()) {
        sv_float64 = mi_float64.value();
    }
    else if (mi_duration.received()) {
        sv_duration = mi_duration.value();
    }
    else if (mi_string.received()) {
        sv_string = mi_string.value();
    }
    sv_string += ";unplanned";
    print("sv_string = " + sv_string);
    return 0_s;
}


inline duration data_primitives_node::planned_event(duration elapsed_dt)
{
    mo_bool.send(sv_bool);
    mo_int64.send(sv_int64);
    mo_float64.send(sv_float64);
    mo_duration.send(sv_duration);
    mo_string.send(sv_string);
    sv_string += ";planned";
    print("sv_string = " + sv_string);
    return duration::inf();
}


inline void data_primitives_node::finalization_event(duration elapsed_dt)
{
    fo_bool.assign(sv_bool);
    fo_int64.assign(sv_int64);
    fo_float64.assign(sv_float64);
    fo_duration.assign(sv_duration);
    fo_string.assign(sv_string);
}


}  // namespace

#endif
