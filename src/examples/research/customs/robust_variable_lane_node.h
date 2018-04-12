#pragma once
#ifndef SYDEVS_EXAMPLES_ROBUST_VARIABLE_LANE_NODE_H_
#define SYDEVS_EXAMPLES_ROBUST_VARIABLE_LANE_NODE_H_

#include <sydevs/systems/collection_node.h>
#include <examples/research/customs/robust_lane_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class robust_variable_lane_node : public collection_node<int64, robust_lane_node>
{
public:
    // Constructor/Destructor:
    robust_variable_lane_node(const std::string& node_name, const node_context& external_context);
    virtual ~robust_variable_lane_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, int64> id_input;
    port<flow, input, int64> queue_capacity_input;
    port<flow, input, duration> server_mean_dt_input;
    port<flow, input, duration> server_stdev_dt_input;
    port<flow, input, duration> lane_creation_dt_input;
    port<message, input, int64> registration_input;
    port<message, input, std::pair<int64, int64>> acceptance_input;
    port<message, input, std::tuple<int64, int64, int64>> item_input;
    port<message, output, int64> registration_output;
    port<message, output, std::pair<int64, int64>> acceptance_output;
    port<message, output, std::tuple<int64, int64, int64>> item_output;
    port<message, output, int64> lane_creation_output;
    port<message, output, int64> lane_removal_output;

protected:
    // State Variables:
    int64 id_;
    int64 queue_capacity_;
    duration server_mean_dt_;
    duration server_stdev_dt_;
    duration lane_creation_dt_;
    std::map<int64, int64> item_counts_;
    int64 total_item_count_;
    std::set<int64> registration_src_ids_;
    std::vector<std::pair<int64, int64>> acceptance_values_;
    duration remaining_lane_creation_dt_;

    // Event Handlers:
    virtual duration macro_initialization_event();
    virtual duration macro_unplanned_event(duration elapsed_dt);
    virtual duration micro_planned_event(const int64& agent_id, duration elapsed_dt);
    virtual duration macro_planned_event(duration elapsed_dt);
    virtual void macro_finalization_event(duration elapsed_dt);
};


inline robust_variable_lane_node::robust_variable_lane_node(const std::string& node_name, const node_context& external_context)
    : collection_node<int64, robust_lane_node>(node_name, external_context)
    , id_input("id_input", external_interface())
    , queue_capacity_input("queue_capacity_input", external_interface())
    , server_mean_dt_input("server_mean_dt_input", external_interface())
    , server_stdev_dt_input("server_stdev_dt_input", external_interface())
    , lane_creation_dt_input("lane_creation_dt_input", external_interface())
    , registration_input("registration_input", external_interface())
    , acceptance_input("acceptance_input", external_interface())
    , item_input("item_input", external_interface())
    , registration_output("registration_output", external_interface())
    , acceptance_output("acceptance_output", external_interface())
    , item_output("item_output", external_interface())
    , lane_creation_output("lane_creation_output", external_interface())
    , lane_removal_output("lane_removal_output", external_interface())
{
}


inline duration robust_variable_lane_node::macro_initialization_event()
{
    // Store parameter values.
    id_ = id_input.value();
    queue_capacity_ = queue_capacity_input.value();
    server_mean_dt_ = server_mean_dt_input.value();
    server_stdev_dt_ = server_stdev_dt_input.value();
    lane_creation_dt_ = lane_creation_dt_input.value();

    // Initialize variables.
    item_counts_ = std::map<int64, int64>();
    total_item_count_ = 0;
    registration_src_ids_ = std::set<int64>();
    acceptance_values_ = std::vector<std::pair<int64, int64>>();
    remaining_lane_creation_dt_ = duration::inf();

    // Create a lane after lane creation dt expires.
    return remaining_lane_creation_dt_;
}


inline duration robust_variable_lane_node::macro_unplanned_event(duration elapsed_dt)
{
    remaining_lane_creation_dt_ -= elapsed_dt;
    if (registration_input.received()) {
        int64 src_id = registration_input.value();
        for (auto agent_iter = agent_begin(); agent_iter != agent_end(); ++agent_iter) {
            access(prototype.registration_input) = src_id;
            affect_agent(*agent_iter);
        }
        registration_src_ids_.insert(src_id);
        if (total_item_count_ == item_counts_.size()*(queue_capacity_ + 1)) {
            if (remaining_lane_creation_dt_ == duration::inf()) {
                remaining_lane_creation_dt_ = lane_creation_dt_;
            }
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
        const std::tuple<int64, int64, int64>& item_value = item_input.value();
        for (auto agent_iter = agent_begin(); agent_iter != agent_end(); ++agent_iter) {
            access(prototype.item_input) = item_value;
            affect_agent(*agent_iter);
        }
        int64 src_id = std::get<0>(item_value);
        int64 dst_id = std::get<1>(item_value);
        ++item_counts_[dst_id];
        ++total_item_count_;
        registration_src_ids_.erase(src_id);
        auto acceptance_iter = std::remove_if(std::begin(acceptance_values_), 
                                              std::end(acceptance_values_), 
                                              [src_id](const std::pair<int64, int64>& acceptance_value){
            return acceptance_value.first == src_id;
        });
        acceptance_values_.erase(acceptance_iter, std::end(acceptance_values_));
    }
    return !acceptance_values_.empty() ? 0_s : remaining_lane_creation_dt_;
}


inline duration robust_variable_lane_node::micro_planned_event(const int64& agent_id, duration elapsed_dt)
{
    remaining_lane_creation_dt_ -= elapsed_dt;
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
        int64 src_id = std::get<0>(item_value);
        --item_counts_[src_id];
        --total_item_count_;
        if (item_counts_[src_id] == 0) {
            remove_agent(src_id);
            lane_removal_output.send(src_id);
            item_counts_.erase(src_id);
        }
    }
    return !acceptance_values_.empty() ? 0_s : remaining_lane_creation_dt_;
}


inline duration robust_variable_lane_node::macro_planned_event(duration elapsed_dt)
{
    if (!acceptance_values_.empty()) {
        std::uniform_int_distribution<int64> dist(0, acceptance_values_.size() - 1);
        int64 index = dist(rng);
        acceptance_output.send(acceptance_values_[index]);
        acceptance_values_.erase(std::begin(acceptance_values_) + index);
    }
    else {
        int64 lane_id = id_ + 1;
        bool done = false;
        for (auto agent_iter = agent_begin(); !done && (agent_iter != agent_end()); ++agent_iter) {
            if (*agent_iter == lane_id) {
                ++lane_id;
            }
            else {
                done = true;
            }
        }
        access(prototype.id_input) = lane_id;
        access(prototype.capacity_input) = queue_capacity_;
        access(prototype.mean_dt_input) = server_mean_dt_;
        access(prototype.stdev_dt_input) = server_stdev_dt_;
        create_agent(lane_id);
        lane_creation_output.send(lane_id);
        item_counts_[lane_id] = 0;
        for (int64 src_id : registration_src_ids_) {
            access(prototype.registration_input) = src_id;
            affect_agent(lane_id);
        }
        remaining_lane_creation_dt_ = duration::inf();
    }
    return !acceptance_values_.empty() ? 0_s : remaining_lane_creation_dt_;
}


inline void robust_variable_lane_node::macro_finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
