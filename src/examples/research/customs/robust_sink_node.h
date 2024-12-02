#pragma once
#ifndef SYDEVS_EXAMPLES_ROBUST_SINK_NODE_H_
#define SYDEVS_EXAMPLES_ROBUST_SINK_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class robust_sink_node : public atomic_node
{
public:
    // Constructor/Destructor:
    robust_sink_node(const std::string& node_name, const node_context& external_context);
    virtual ~robust_sink_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, int64> id_input;
    port<message, input, int64> registration_input;
    port<message, input, std::tuple<int64, int64, int64>> item_input;
    port<message, output, std::pair<int64, int64>> acceptance_output;
    port<message, output, int64> item_output;

protected:
    // State Variables:
    int64 id_;
    std::vector<int64> queue_;
    std::set<int64> registered_src_ids_;
    int64 acceptance_src_id_;

    // Special IDs
    static const int64 null_id = -1;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);

    // Probability Distributions:
    int64 sample_registered_src();
};


inline robust_sink_node::robust_sink_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , id_input("id_input", external_interface())
    , registration_input("registration_input", external_interface())
    , item_input("item_input", external_interface())
    , acceptance_output("acceptance_output", external_interface())
    , item_output("item_output", external_interface())
{
}


inline duration robust_sink_node::initialization_event()
{
    id_ = id_input.value();
    queue_ = std::vector<int64>();
    registered_src_ids_ = std::set<int64>();
    acceptance_src_id_ = null_id;
    return duration::inf();
}


inline duration robust_sink_node::unplanned_event(duration elapsed_dt)
{
    if (registration_input.received()) {
        int64 src_id = registration_input.value();
        // Record that an item at node src_id is being registered with this node.
        registered_src_ids_.insert(src_id);
    }
    else if (item_input.received()) {
        const auto& [src_id, dst_id, item_id] = item_input.value();
        // Node src_id is sending an item to node dst_id.
        // Record that an item at node src_id is being deregistered from this node.
        registered_src_ids_.erase(src_id);
        // Queue the item if possible.
        if (src_id == acceptance_src_id_) {
            // Node src_id is the node with permission to send an item to this node.
            if (dst_id == id_) {
                // Node src_id is sending an item to this node.
                // Queue the item.
                queue_.push_back(item_id);  
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


inline duration robust_sink_node::planned_event(duration elapsed_dt)
{
    for (const int64& item_id : queue_) {
        item_output.send(item_id);
    }
    queue_.clear();
    if (!registered_src_ids_.empty()) {
        // There are items available.
        if (acceptance_src_id_ == null_id) {
            // None of these items have been accepted.
            // Randomly select a source node from which to accept an item.
            acceptance_src_id_ = sample_registered_src();
            acceptance_output.send(std::make_pair(acceptance_src_id_, id_));
        }
    }
    return duration::inf();
}


inline void robust_sink_node::finalization_event(duration elapsed_dt)
{
}


inline int64 robust_sink_node::sample_registered_src()
{
    std::uniform_int_distribution<int64> dist(0, registered_src_ids_.size() - 1);
    auto proposing_src_iter = std::begin(registered_src_ids_);
    std::advance(proposing_src_iter, dist(rng));
    return *proposing_src_iter;
}


}  // namespace

#endif
