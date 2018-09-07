#pragma once
#ifndef SYDEVS_EXAMPLES_DATA_STRUCTURES_NODE_H_
#define SYDEVS_EXAMPLES_DATA_STRUCTURES_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class data_structures_node : public atomic_node
{
public:
    // Constructor/Destructor:
    data_structures_node(const std::string& node_name, const node_context& external_context);
    virtual ~data_structures_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, std::vector<bool>> fi_vector_bool;
    port<flow, input, std::vector<int64>> fi_vector_int64;
    port<flow, input, std::vector<std::vector<duration>>> fi_vector_vector_duration;
    port<flow, input, std::map<duration, float64>> fi_map_duration_float64;
    port<flow, input, std::set<std::pair<float64, std::string>>> fi_set_pair_float64_string;
    port<flow, input, std::shared_ptr<quantity<decltype(_1/_s)>>> fi_rate_ptr;
    port<flow, input, std::shared_ptr<std::ostringstream>> fi_ostringstream_ptr;
    port<flow, input, std::shared_ptr<std::vector<std::array<float64, 3>>>> fi_vector_array_ptr;
    port<message, input, std::vector<bool>> mi_vector_bool;
    port<message, input, std::vector<int64>> mi_vector_int64;
    port<message, input, std::vector<std::vector<duration>>> mi_vector_vector_duration;
    port<message, input, std::map<duration, float64>> mi_map_duration_float64;
    port<message, input, std::set<std::pair<float64, std::string>>> mi_set_pair_float64_string;
    port<message, input, std::shared_ptr<quantity<decltype(_1/_s)>>> mi_rate_ptr;
    port<message, input, std::shared_ptr<std::ostringstream>> mi_ostringstream_ptr;
    port<message, input, std::shared_ptr<std::vector<std::array<float64, 3>>>> mi_vector_array_ptr;
    port<message, output, std::vector<bool>> mo_vector_bool;
    port<message, output, std::vector<int64>> mo_vector_int64;
    port<message, output, std::vector<std::vector<duration>>> mo_vector_vector_duration;
    port<message, output, std::map<duration, float64>> mo_map_duration_float64;
    port<message, output, std::set<std::pair<float64, std::string>>> mo_set_pair_float64_string;
    port<message, output, std::shared_ptr<quantity<decltype(_1/_s)>>> mo_rate_ptr;
    port<message, output, std::shared_ptr<std::ostringstream>> mo_ostringstream_ptr;
    port<message, output, std::shared_ptr<std::vector<std::array<float64, 3>>>> mo_vector_array_ptr;
    port<flow, output, std::vector<bool>> fo_vector_bool;
    port<flow, output, std::vector<int64>> fo_vector_int64;
    port<flow, output, std::vector<std::vector<duration>>> fo_vector_vector_duration;
    port<flow, output, std::map<duration, float64>> fo_map_duration_float64;
    port<flow, output, std::set<std::pair<float64, std::string>>> fo_set_pair_float64_string;
    port<flow, output, std::shared_ptr<quantity<decltype(_1/_s)>>> fo_rate_ptr;
    port<flow, output, std::shared_ptr<std::ostringstream>> fo_ostringstream_ptr;
    port<flow, output, std::shared_ptr<std::vector<std::array<float64, 3>>>> fo_vector_array_ptr;

protected:
    // State Variables:
    std::vector<bool> sv_vector_bool;
    std::vector<int64> sv_vector_int64;
    std::vector<std::vector<duration>> sv_vector_vector_duration;
    std::map<duration, float64> sv_map_duration_float64;
    std::set<std::pair<float64, std::string>> sv_set_pair_float64_string;
    std::shared_ptr<quantity<decltype(_1/_s)>> sv_rate_ptr;
    std::shared_ptr<std::ostringstream> sv_ostringstream_ptr;
    std::shared_ptr<std::vector<std::array<float64, 3>>> sv_vector_array_ptr;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline data_structures_node::data_structures_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , fi_vector_bool("fi_vector_bool", external_interface())
    , fi_vector_int64("fi_vector_int64", external_interface())
    , fi_vector_vector_duration("fi_vector_vector_duration", external_interface())
    , fi_map_duration_float64("fi_map_duration_float64", external_interface())
    , fi_set_pair_float64_string("fi_set_pair_float64_string", external_interface())
    , fi_rate_ptr("fi_rate_ptr", external_interface())
    , fi_ostringstream_ptr("fi_ostringstream_ptr", external_interface())
    , fi_vector_array_ptr("fi_vector_array_ptr", external_interface())
    , mi_vector_bool("mi_vector_bool", external_interface())
    , mi_vector_int64("mi_vector_int64", external_interface())
    , mi_vector_vector_duration("mi_vector_vector_duration", external_interface())
    , mi_map_duration_float64("mi_map_duration_float64", external_interface())
    , mi_set_pair_float64_string("mi_set_pair_float64_string", external_interface())
    , mi_rate_ptr("mi_rate_ptr", external_interface())
    , mi_ostringstream_ptr("mi_ostringstream_ptr", external_interface())
    , mi_vector_array_ptr("mi_vector_array_ptr", external_interface())
    , mo_vector_bool("mo_vector_bool", external_interface())
    , mo_vector_int64("mo_vector_int64", external_interface())
    , mo_vector_vector_duration("mo_vector_vector_duration", external_interface())
    , mo_map_duration_float64("mo_map_duration_float64", external_interface())
    , mo_set_pair_float64_string("mo_set_pair_float64_string", external_interface())
    , mo_rate_ptr("mo_rate_ptr", external_interface())
    , mo_ostringstream_ptr("mo_ostringstream_ptr", external_interface())
    , mo_vector_array_ptr("mo_vector_array_ptr", external_interface())
    , fo_vector_bool("fo_vector_bool", external_interface())
    , fo_vector_int64("fo_vector_int64", external_interface())
    , fo_vector_vector_duration("fo_vector_vector_duration", external_interface())
    , fo_map_duration_float64("fo_map_duration_float64", external_interface())
    , fo_set_pair_float64_string("fo_set_pair_float64_string", external_interface())
    , fo_rate_ptr("fo_rate_ptr", external_interface())
    , fo_ostringstream_ptr("fo_ostringstream_ptr", external_interface())
    , fo_vector_array_ptr("fo_vector_array_ptr", external_interface())
{
}


inline duration data_structures_node::initialization_event()
{
    sv_vector_bool = fi_vector_bool.value();
    sv_vector_int64 = fi_vector_int64.value();
    sv_vector_vector_duration = fi_vector_vector_duration.value();
    sv_map_duration_float64 = fi_map_duration_float64.value();
    sv_set_pair_float64_string = fi_set_pair_float64_string.value();
    sv_rate_ptr = fi_rate_ptr.value();
    sv_ostringstream_ptr = fi_ostringstream_ptr.value();
    sv_vector_array_ptr = fi_vector_array_ptr.value();

    auto& ostringstream_var = *sv_ostringstream_ptr;
    auto& vector_array_var = *sv_vector_array_ptr;

    ostringstream_var << "initialized;";

    print("ostringstream_var.str() = " + ostringstream_var.str());
    print((string_builder() << "vector_array_var.size() = " << vector_array_var.size()).str());

    return 0_s;
}


inline duration data_structures_node::unplanned_event(duration elapsed_dt)
{
    if (mi_vector_bool.received()) {
        sv_vector_bool = mi_vector_bool.value();
    }
    else if (mi_vector_int64.received()) {
        sv_vector_int64 = mi_vector_int64.value();
    }
    else if (mi_vector_vector_duration.received()) {
        sv_vector_vector_duration = mi_vector_vector_duration.value();
    }
    else if (mi_map_duration_float64.received()) {
        sv_map_duration_float64 = mi_map_duration_float64.value();
    }
    else if (mi_set_pair_float64_string.received()) {
        sv_set_pair_float64_string = mi_set_pair_float64_string.value();
    }
    else if (mi_rate_ptr.received()) {
        sv_rate_ptr = mi_rate_ptr.value();
    }
    else if (mi_ostringstream_ptr.received()) {
        sv_ostringstream_ptr = mi_ostringstream_ptr.value();
    }
    else if (mi_vector_array_ptr.received()) {
        sv_vector_array_ptr = mi_vector_array_ptr.value();
    }

    auto& ostringstream_var = *sv_ostringstream_ptr;
    auto& vector_array_var = *sv_vector_array_ptr;

    sv_vector_bool[1] = !sv_vector_bool[1];
    ++sv_vector_int64.back();
    sv_vector_vector_duration[0][0] *= 1.1;
    sv_map_duration_float64[987654321_s] = -10;
    sv_set_pair_float64_string.emplace(9.7, "nine point seven");
    *sv_rate_ptr = 1/0_s;
    ostringstream_var << "received;";

    print("ostringstream_var.str() = " + ostringstream_var.str());
    print((string_builder() << "vector_array_var.size() = " << vector_array_var.size()).str());

    return 0_s;
}


inline duration data_structures_node::planned_event(duration elapsed_dt)
{
    auto& ostringstream_var = *sv_ostringstream_ptr;
    auto& vector_array_var = *sv_vector_array_ptr;

    sv_vector_bool[2] = !sv_vector_bool[2];
    sv_vector_int64.push_back(100);
    sv_vector_vector_duration[0][0] *= 3;
    sv_map_duration_float64[123456789_s] = 10;
    *sv_rate_ptr += 1/1_s;
    ostringstream_var << "sent;";

    print("ostringstream_var.str() = " + ostringstream_var.str());
    print((string_builder() << "vector_array_var.size() = " << vector_array_var.size()).str());

    mo_vector_bool.send(sv_vector_bool);
    mo_vector_int64.send(sv_vector_int64);
    mo_vector_vector_duration.send(sv_vector_vector_duration);
    mo_map_duration_float64.send(sv_map_duration_float64);
    mo_set_pair_float64_string.send(sv_set_pair_float64_string);
    mo_rate_ptr.send(sv_rate_ptr);
    mo_ostringstream_ptr.send(sv_ostringstream_ptr);
    mo_vector_array_ptr.send(sv_vector_array_ptr);

    return duration::inf();
}


inline void data_structures_node::finalization_event(duration elapsed_dt)
{
    auto& ostringstream_var = *sv_ostringstream_ptr;
    auto& vector_array_var = *sv_vector_array_ptr;

    sv_vector_bool[3] = !sv_vector_bool[3];
    sv_vector_int64.clear();
    ostringstream_var << "finalized;";
    vector_array_var.push_back({{1.0, 2.0, 3.0}});

    print("ostringstream_var.str() = " + ostringstream_var.str());
    print((string_builder() << "vector_array_var.size() = " << vector_array_var.size()).str());

    fo_vector_bool.assign(sv_vector_bool);
    fo_vector_int64.assign(sv_vector_int64);
    fo_vector_vector_duration.assign(sv_vector_vector_duration);
    fo_map_duration_float64.assign(sv_map_duration_float64);
    fo_set_pair_float64_string.assign(sv_set_pair_float64_string);
    fo_rate_ptr.assign(sv_rate_ptr);
    fo_ostringstream_ptr.assign(sv_ostringstream_ptr);
    fo_vector_array_ptr.assign(sv_vector_array_ptr);
}


}  // namespace

#endif
