#pragma once
#ifndef SYDEVS_EXAMPLES_ROBUST_SOURCE_NODE_H_
#define SYDEVS_EXAMPLES_ROBUST_SOURCE_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class robust_source_node : public atomic_node
{
public:
    // Constructor/Destructor:
    robust_source_node(const std::string& node_name, const node_context& external_context);
    virtual ~robust_source_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, int64> id_input;
    port<flow, input, duration> mean_dt_input;
    port<flow, input, duration> stdev_dt_input;
    port<flow, input, duration> inactive_dt_input;
    port<flow, input, duration> active_dt_input;
    port<flow, input, int64> start_item_id_input;
    port<message, input, std::pair<int64, int64>> acceptance_input;
    port<message, output, int64> registration_output;
    port<message, output, std::tuple<int64, int64, int64>> item_output;

protected:
    // State Variables:
    int64 id_;
    duration mean_dt_;
    duration stdev_dt_;
    duration inactive_dt_;
    duration active_dt_;
    int64 start_item_id_;
    int64 next_item_id_;
    std::vector<int64> queue_;
    std::set<int64> accepting_dst_ids_;
    bool registration_status_;
    std::unique_ptr<std::exponential_distribution<float64>> exp_dist_ptr;
    std::unique_ptr<std::gamma_distribution<float64>> gamma_dist_ptr;
    duration source_dt_;
    duration cumulative_dt_;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);

    // Probability Distributions:
    int64 sample_accepting_dst();
    duration sample_source_dt();
};


inline robust_source_node::robust_source_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , id_input("id_input", external_interface())
    , mean_dt_input("mean_dt_input", external_interface())
    , stdev_dt_input("stdev_dt_input", external_interface())
    , inactive_dt_input("inactive_dt_input", external_interface())
    , active_dt_input("active_dt_input", external_interface())
    , start_item_id_input("start_item_id_input", external_interface())
    , acceptance_input("acceptance_input", external_interface())
    , registration_output("registration_output", external_interface())
    , item_output("item_output", external_interface())
{
}


inline duration robust_source_node::initialization_event()
{
    id_ = id_input.value();
    mean_dt_ = mean_dt_input.value();
    stdev_dt_ = stdev_dt_input.value();
    inactive_dt_ = inactive_dt_input.value().fixed_at(time_precision());
    active_dt_ = active_dt_input.value().fixed_at(time_precision());
    start_item_id_ = start_item_id_input.value();
    next_item_id_ = start_item_id_;
    queue_ = std::vector<int64>();
    accepting_dst_ids_ = std::set<int64>();
    registration_status_ = false;
    exp_dist_ptr = nullptr;
    gamma_dist_ptr = nullptr;
    if (mean_dt_ > 0_s && stdev_dt_ > 0_s) {
        if (mean_dt_ == stdev_dt_) {
            float64 rate_param = 1_s/mean_dt_;
            exp_dist_ptr = std::make_unique<std::exponential_distribution<float64>>(rate_param);
        }
        else {
            float64 shape_param = (mean_dt_/stdev_dt_)*(mean_dt_/stdev_dt_);
            float64 scale_param = (stdev_dt_/mean_dt_)*(stdev_dt_/1_s);
            gamma_dist_ptr = std::make_unique<std::gamma_distribution<float64>>(shape_param, scale_param);
        }
    }
    auto dt = sample_source_dt();
    source_dt_ = inactive_dt_ + dt;
    cumulative_dt_ = dt;
    return source_dt_;
}


inline duration robust_source_node::unplanned_event(duration elapsed_dt)
{
    source_dt_ -= elapsed_dt;
    if (acceptance_input.received()) {
        const auto& [src_id, dst_id] = acceptance_input.value();
        // Node dst_id is accepting an item from node src_id.
        if (src_id == id_) {
            // Node dst_id is accepting an item from this node.
            // Record that node dst_id is accepting an item from this node.
            accepting_dst_ids_.insert(dst_id);
        }
        else {
            // Record that node dst_id is not accepting an item from this node.
            accepting_dst_ids_.erase(dst_id);
        }
    }
    auto planned_dt = source_dt_;
    if (!queue_.empty() && !accepting_dst_ids_.empty()) {
        // There are possible destination nodes waiting to accept an item from this node,
        // and there is an item available to send.
        planned_dt = 0_s;
    }
    return planned_dt;
}


inline duration robust_source_node::planned_event(duration elapsed_dt)
{
    source_dt_ -= elapsed_dt;
    if (source_dt_ == 0_s) {
        // Create a new item.
        queue_.push_back(next_item_id_);
        ++next_item_id_;
        source_dt_ = sample_source_dt();
        cumulative_dt_ += source_dt_;
        if (cumulative_dt_ > active_dt_) {
            source_dt_ = duration::inf();
        }
    }
    if (!accepting_dst_ids_.empty()) {
        // There are possible destination nodes waiting to accept an item from this node.
        // Randomly accept a destination node and send the item.
        // Note that there is an item available to send.
        int64 dst_id = sample_accepting_dst();
        item_output.send(std::make_tuple(id_, dst_id, queue_.front()));
        queue_.erase(std::begin(queue_));
        accepting_dst_ids_.clear();
        registration_status_ = false;
    }
    if (!queue_.empty()) {
        // There is at least one item in the queue.
        if (!registration_status_) {
            // Register the item with possible destination nodes.
            registration_output.send(id_);
            registration_status_ = true;
        }
    }
    return source_dt_;
}


inline void robust_source_node::finalization_event(duration elapsed_dt)
{
}


inline int64 robust_source_node::sample_accepting_dst()
{
    std::uniform_int_distribution<int64> dist(0, accepting_dst_ids_.size() - 1);
    auto accepting_dst_iter = std::begin(accepting_dst_ids_);
    std::advance(accepting_dst_iter, dist(rng));
    return *accepting_dst_iter;
}


inline duration robust_source_node::sample_source_dt()
{
    auto dt = mean_dt_;
    if (exp_dist_ptr) {
        dt = (*exp_dist_ptr)(rng)*1_s;
    }
    else if (gamma_dist_ptr) {
        dt = (*gamma_dist_ptr)(rng)*1_s;
    }
    return dt.fixed_at(time_precision());
}


}  // namespace

#endif
