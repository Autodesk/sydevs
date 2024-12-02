#pragma once
#ifndef SYDEVS_EXAMPLES_ROBUST_SERVER_ARRAY_NODE_H_
#define SYDEVS_EXAMPLES_ROBUST_SERVER_ARRAY_NODE_H_

#include <sydevs/systems/collection_node.h>
#include <examples/research/customs/robust_server_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class robust_server_array_node : public collection_node<int64, robust_server_node>
{
public:
    // Constructor/Destructor:
    robust_server_array_node(const std::string& node_name, const node_context& external_context);
    virtual ~robust_server_array_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, int64> id_input;
    port<flow, input, int64> server_count_input;
    port<flow, input, duration> mean_dt_input;
    port<flow, input, duration> stdev_dt_input;
    port<message, input, int64> registration_input;
    port<message, input, std::pair<int64, int64>> acceptance_input;
    port<message, input, std::tuple<int64, int64, int64>> item_input;
    port<message, output, int64> registration_output;
    port<message, output, std::pair<int64, int64>> acceptance_output;
    port<message, output, std::tuple<int64, int64, int64>> item_output;

protected:
    // State Variables:
    int64 id_;
    int64 server_count_;
    duration mean_dt_;
    duration stdev_dt_;
    std::vector<std::pair<int64, int64>> acceptance_values_;

    // Event Handlers:
    virtual duration macro_initialization_event();
    virtual duration macro_unplanned_event(duration elapsed_dt);
    virtual duration micro_planned_event(const int64& agent_id, duration elapsed_dt);
    virtual duration macro_planned_event(duration elapsed_dt);
    virtual void macro_finalization_event(duration elapsed_dt);
};


inline robust_server_array_node::robust_server_array_node(const std::string& node_name, const node_context& external_context)
    : collection_node<int64, robust_server_node>(node_name, external_context)
    , id_input("id_input", external_interface())
    , server_count_input("server_count_input", external_interface())
    , mean_dt_input("mean_dt_input", external_interface())
    , stdev_dt_input("stdev_dt_input", external_interface())
    , registration_input("registration_input", external_interface())
    , acceptance_input("acceptance_input", external_interface())
    , item_input("item_input", external_interface())
    , registration_output("registration_output", external_interface())
    , acceptance_output("acceptance_output", external_interface())
    , item_output("item_output", external_interface())
{
}


inline duration robust_server_array_node::macro_initialization_event()
{
    // Store parameter values.
    id_ = id_input.value();
    server_count_ = server_count_input.value();
    mean_dt_ = mean_dt_input.value();
    stdev_dt_ = stdev_dt_input.value();
    acceptance_values_ = std::vector<std::pair<int64, int64>>();

    // Create the server agents.
    access(prototype.mean_dt_input) = mean_dt_;
    access(prototype.stdev_dt_input) = stdev_dt_;
    for (int64 i = 0; i < server_count_; ++i) {
        int64 server_id = id_ + i + 1;
        access(prototype.id_input) = server_id;
        create_agent(server_id);
    }

    // Wait indefinitely for activity.
    return duration::inf();
}


inline duration robust_server_array_node::macro_unplanned_event(duration elapsed_dt)
{
    if (registration_input.received()) {
        int64 src_id = registration_input.value();
        for (auto agent_iter = agent_begin(); agent_iter != agent_end(); ++agent_iter) {
            access(prototype.registration_input) = src_id;
            affect_agent(*agent_iter);
        }
    }
    else if (acceptance_input.received()) {
        const std::pair<int64, int64>& acceptance_value = acceptance_input.value();
        for (auto agent_iter = agent_begin(); agent_iter != agent_end(); ++agent_iter) {
            access(prototype.acceptance_input) = acceptance_value;
            affect_agent(*agent_iter);
        }
    }
    else if (item_input.received()) {
        const auto& [src_id, dst_id, item_id] = item_input.value();
        for (auto agent_iter = agent_begin(); agent_iter != agent_end(); ++agent_iter) {
            access(prototype.item_input) = { src_id, dst_id, item_id };
            affect_agent(*agent_iter);
        }
        auto acceptance_iter = std::remove_if(std::begin(acceptance_values_), 
                                              std::end(acceptance_values_), 
                                              [src_id](const std::pair<int64, int64>& acceptance_value){
            return acceptance_value.first == src_id;
        });
        acceptance_values_.erase(acceptance_iter, std::end(acceptance_values_));
    }
    return !acceptance_values_.empty() ? 0_s : duration::inf();
}


inline duration robust_server_array_node::micro_planned_event(const int64& agent_id, duration elapsed_dt)
{
    if (transmitted(prototype.registration_output)) {
        int64 src_id = access(prototype.registration_output);
        registration_output.send(src_id);
    }
    else if (transmitted(prototype.acceptance_output)) {
        const std::pair<int64, int64>& acceptance_value = access(prototype.acceptance_output);
        acceptance_values_.push_back(acceptance_value);
    }
    else if (transmitted(prototype.item_output)) {
        const std::tuple<int64, int64, int64>& item_value = access(prototype.item_output);
        item_output.send(item_value);
    }
    return !acceptance_values_.empty() ? 0_s : duration::inf();
}


inline duration robust_server_array_node::macro_planned_event(duration elapsed_dt)
{
    std::uniform_int_distribution<int64> dist(0, acceptance_values_.size() - 1);
    int64 index = dist(rng);
    acceptance_output.send(acceptance_values_[index]);
    acceptance_values_.erase(std::begin(acceptance_values_) + index);
    return !acceptance_values_.empty() ? 0_s : duration::inf();
}


inline void robust_server_array_node::macro_finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
