#pragma once
#ifndef SYDEVS_EXAMPLES_ROBUST_SERVER_NODE_H_
#define SYDEVS_EXAMPLES_ROBUST_SERVER_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class robust_server_node : public atomic_node
{
public:
    // Constructor/Destructor:
    robust_server_node(const std::string& node_name, const node_context& external_context);
    virtual ~robust_server_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, int64> id_input;
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
    duration mean_dt_;
    duration stdev_dt_;
    int64 item_id_;
    bool served_;
    duration service_dt_;
    std::set<int64> registered_src_ids_;
    std::set<int64> accepting_dst_ids_;
    bool registration_status_;
    int64 acceptance_src_id_;
    std::unique_ptr<std::exponential_distribution<float64>> exp_dist_ptr;
    std::unique_ptr<std::gamma_distribution<float64>> gamma_dist_ptr;

    // Special IDs
    static const int64 null_id = -1;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);

    // Probability Distributions:
    int64 sample_registered_src();
    int64 sample_accepting_dst();
    duration sample_service_dt();
};


inline robust_server_node::robust_server_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , id_input("id_input", external_interface())
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


inline duration robust_server_node::initialization_event()
{
    id_ = id_input.value();
    mean_dt_ = mean_dt_input.value();
    stdev_dt_ = stdev_dt_input.value();
    item_id_ = null_id;
    served_ = false;
    service_dt_ = duration::inf();
    registered_src_ids_ = std::set<int64>();
    accepting_dst_ids_ = std::set<int64>();
    registration_status_ = false;
    acceptance_src_id_ = null_id;
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
    return duration::inf();
}


inline duration robust_server_node::unplanned_event(duration elapsed_dt)
{
    service_dt_ -= elapsed_dt;
    if (registration_input.received()) {
        int64 src_id = registration_input.value();
        // Record that an item at node src_id is being registered with this node.
        registered_src_ids_.insert(src_id);
    }
    else if (acceptance_input.received()) {
        const std::pair<int64, int64>& acceptance_value = acceptance_input.value();
        int64 src_id = acceptance_value.first;
        int64 dst_id = acceptance_value.second;
        // Node dst_id is accepting an item from node src_id.
        if (src_id == id_) {
            // Node dst_id is accepting an item from this node.
            if (item_id_ == null_id || !served_) {
                // Node dst_id should have been informed that there is no available item at this node.
                throw std::logic_error("Received an acceptance notification with no item available.");
            }
            // Record that node dst_id is accepting an item from this node.
            accepting_dst_ids_.insert(dst_id);
        }
        else {
            // Record that node dst_id is not accepting an item from this node.
            accepting_dst_ids_.erase(dst_id);
        }
    }
    else if (item_input.received()) {
        const auto& [src_id, dst_id, item_id] = item_input.value();
        // Node src_id is sending an item to node dst_id.
        // Record that an item at node src_id is being deregistered from this node.
        registered_src_ids_.erase(src_id);
        // Serve the item if possible.
        if (src_id == acceptance_src_id_) {
            // Node src_id is the node with permission to send an item to this node.
            if (dst_id == id_) {
                // Node src_id is sending an item to this node.
                // Serve the item.
                item_id_ = item_id;
                served_ = false;
                service_dt_ = sample_service_dt();
            }
            acceptance_src_id_ = null_id;
        }
        else {
            if (dst_id == id_) {
                throw std::logic_error("Received an item without permission.");
            }
        }
    }
    return 0_s;
}


inline duration robust_server_node::planned_event(duration elapsed_dt)
{
    service_dt_ -= elapsed_dt;
    if (!accepting_dst_ids_.empty()) {
        // There are possible destination nodes waiting to accept an item from this node.
        // Randomly accept a destination node and send the item.
        // Note that there is an item available to send.
        int64 dst_id = sample_accepting_dst();
        item_output.send(std::make_tuple(id_, dst_id, item_id_));
        item_id_ = null_id;
        served_ = false;
        accepting_dst_ids_.clear();
        registration_status_ = false;
    }
    if (!registered_src_ids_.empty()) {
        // There are items available.
        if (acceptance_src_id_ == null_id) {
            // None of these items have been accepted.
            if (item_id_ == null_id) {
                // The server is free.
                // Randomly select a source node from which to accept an item.
                acceptance_src_id_ = sample_registered_src();
                acceptance_output.send(std::make_pair(acceptance_src_id_, id_));
            }
        }
    }
    if (!served_ && service_dt_ == 0_s) {
        // The service duration has just elapsed.
        // Record that the item has been served.
        served_ = true;
        service_dt_ = duration::inf();
        // Register the item with possible destination nodes.
        registration_output.send(id_);
        registration_status_ = true;
    }
    return service_dt_;
}


inline void robust_server_node::finalization_event(duration elapsed_dt)
{
}


inline int64 robust_server_node::sample_registered_src()
{
    std::uniform_int_distribution<int64> dist(0, registered_src_ids_.size() - 1);
    auto proposing_src_iter = std::begin(registered_src_ids_);
    std::advance(proposing_src_iter, dist(rng));
    return *proposing_src_iter;
}


inline int64 robust_server_node::sample_accepting_dst()
{
    std::uniform_int_distribution<int64> dist(0, accepting_dst_ids_.size() - 1);
    auto accepting_dst_iter = std::begin(accepting_dst_ids_);
    std::advance(accepting_dst_iter, dist(rng));
    return *accepting_dst_iter;
}


inline duration robust_server_node::sample_service_dt()
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
