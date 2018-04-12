#pragma once
#ifndef SYDEVS_EXAMPLES_DATA_TUPLES_NODE_H_
#define SYDEVS_EXAMPLES_DATA_TUPLES_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class data_tuples_node : public atomic_node
{
public:
    // Constructor/Destructor:
    data_tuples_node(const std::string& node_name, const node_context& external_context);
    virtual ~data_tuples_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Local Aliases:
    using speed = quantity<decltype(_m/_s)>;

    // Ports:
    port<flow, input, std::tuple<int64>> fi_tuple_int64;
    port<flow, input, std::tuple<std::string, bool, float64>> fi_tuple_string_bool_float64;
    port<flow, input, std::tuple<mass, std::pair<speed, std::pair<bool, bool>>>> fi_tuple_mass_speed_pair_bool_bool;
    port<message, input, std::tuple<int64>> mi_tuple_int64;
    port<message, input, std::tuple<std::string, bool, float64>> mi_tuple_string_bool_float64;
    port<message, input, std::tuple<mass, std::pair<speed, std::pair<bool, bool>>>> mi_tuple_mass_speed_pair_bool_bool;
    port<message, output, std::tuple<int64>> mo_tuple_int64;
    port<message, output, std::tuple<std::string, bool, float64>> mo_tuple_string_bool_float64;
    port<message, output, std::tuple<mass, std::pair<speed, std::pair<bool, bool>>>> mo_tuple_mass_speed_pair_bool_bool;
    port<flow, output, std::tuple<int64>> fo_tuple_int64;
    port<flow, output, std::tuple<std::string, bool, float64>> fo_tuple_string_bool_float64;
    port<flow, output, std::tuple<mass, std::pair<speed, std::pair<bool, bool>>>> fo_tuple_mass_speed_pair_bool_bool;

protected:
    // State Variables:
    std::tuple<int64> sv_tuple_int64;
    std::tuple<std::string, bool, float64> sv_tuple_string_bool_float64;
    std::tuple<mass, std::pair<speed, std::pair<bool, bool>>> sv_tuple_mass_speed_pair_bool_bool;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline data_tuples_node::data_tuples_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , fi_tuple_int64("fi_tuple_int64", external_interface())
    , fi_tuple_string_bool_float64("fi_tuple_string_bool_float64", external_interface())
    , fi_tuple_mass_speed_pair_bool_bool("fi_tuple_mass_speed_pair_bool_bool", external_interface())
    , mi_tuple_int64("mi_tuple_int64", external_interface())
    , mi_tuple_string_bool_float64("mi_tuple_string_bool_float64", external_interface())
    , mi_tuple_mass_speed_pair_bool_bool("mi_tuple_mass_speed_pair_bool_bool", external_interface())
    , mo_tuple_int64("mo_tuple_int64", external_interface())
    , mo_tuple_string_bool_float64("mo_tuple_string_bool_float64", external_interface())
    , mo_tuple_mass_speed_pair_bool_bool("mo_tuple_mass_speed_pair_bool_bool", external_interface())
    , fo_tuple_int64("fo_tuple_int64", external_interface())
    , fo_tuple_string_bool_float64("fo_tuple_string_bool_float64", external_interface())
    , fo_tuple_mass_speed_pair_bool_bool("fo_tuple_mass_speed_pair_bool_bool", external_interface())
{
}


inline duration data_tuples_node::initialization_event()
{
    sv_tuple_int64 = fi_tuple_int64.value();
    sv_tuple_string_bool_float64 = fi_tuple_string_bool_float64.value();
    sv_tuple_mass_speed_pair_bool_bool = fi_tuple_mass_speed_pair_bool_bool.value();

    return 0_s;
}


inline duration data_tuples_node::unplanned_event(duration elapsed_dt)
{
    if (mi_tuple_int64.received()) {
        sv_tuple_int64 = mi_tuple_int64.value();
    }
    else if (mi_tuple_string_bool_float64.received()) {
        sv_tuple_string_bool_float64 = mi_tuple_string_bool_float64.value();
    }
    else if (mi_tuple_mass_speed_pair_bool_bool.received()) {
        sv_tuple_mass_speed_pair_bool_bool = mi_tuple_mass_speed_pair_bool_bool.value();
    }

    ++std::get<0>(sv_tuple_int64);
    std::get<0>(sv_tuple_string_bool_float64) += ";unplanned";
    std::get<0>(std::get<1>(sv_tuple_mass_speed_pair_bool_bool)) = 299792458_m/_s;

    return 0_s;
}


inline duration data_tuples_node::planned_event(duration elapsed_dt)
{
    std::get<0>(sv_tuple_int64) += 1000;
    std::get<2>(sv_tuple_string_bool_float64) = 3.14159;
    std::get<0>(sv_tuple_mass_speed_pair_bool_bool) = 1_Mg;

    mo_tuple_int64.send(sv_tuple_int64);
    mo_tuple_string_bool_float64.send(sv_tuple_string_bool_float64);
    mo_tuple_mass_speed_pair_bool_bool.send(sv_tuple_mass_speed_pair_bool_bool);

    return duration::inf();
}


inline void data_tuples_node::finalization_event(duration elapsed_dt)
{
    std::get<0>(sv_tuple_int64) += 2000000;
    std::get<1>(sv_tuple_string_bool_float64) = !std::get<1>(sv_tuple_string_bool_float64);
    std::get<1>(std::get<1>(sv_tuple_mass_speed_pair_bool_bool)).second = !std::get<1>(std::get<1>(sv_tuple_mass_speed_pair_bool_bool)).second;

    fo_tuple_int64.assign(sv_tuple_int64);
    fo_tuple_string_bool_float64.assign(sv_tuple_string_bool_float64);
    fo_tuple_mass_speed_pair_bool_bool.assign(sv_tuple_mass_speed_pair_bool_bool);
}


}  // namespace

#endif
