#pragma once
#ifndef SYDEVS_EXAMPLES_ROBUST_LANE_NODE_H_
#define SYDEVS_EXAMPLES_ROBUST_LANE_NODE_H_

#include <sydevs/systems/composite_node.h>
#include <examples/research/customs/robust_queue_node.h>
#include <examples/research/customs/robust_server_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class robust_lane_node : public composite_node
{
public:
    // Constructor/Destructor:
    robust_lane_node(const std::string& node_name, const node_context& external_context);
    virtual ~robust_lane_node() = default;

    // Ports:
    port<flow, input, int64> id_input;
    port<flow, input, int64> capacity_input;
    port<flow, input, duration> mean_dt_input;
    port<flow, input, duration> stdev_dt_input;
    port<message, input, int64> registration_input;
    port<message, input, std::pair<int64, int64>> acceptance_input;
    port<message, input, std::tuple<int64, int64, int64>> item_input;
    port<message, output, int64> registration_output;
    port<message, output, std::pair<int64, int64>> acceptance_output;
    port<message, output, std::tuple<int64, int64, int64>> item_output;

    // Components:
    robust_queue_node queue;
    robust_server_node server;
};


robust_lane_node::robust_lane_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , id_input("id_input", external_interface())
    , capacity_input("capacity_input", external_interface())
    , mean_dt_input("mean_dt_input", external_interface())
    , stdev_dt_input("stdev_dt_input", external_interface())
    , registration_input("registration_input", external_interface())
    , acceptance_input("acceptance_input", external_interface())
    , item_input("item_input", external_interface())
    , registration_output("registration_output", external_interface())
    , acceptance_output("acceptance_output", external_interface())
    , item_output("item_output", external_interface())
    , queue("queue", internal_context())
    , server("server", internal_context())
{
    // Initialization Links
    inward_link(id_input, queue.id_input);
    inward_link(id_input, server.id_input);
    inward_link(capacity_input, queue.capacity_input);
    inward_link(mean_dt_input, server.mean_dt_input);
    inward_link(stdev_dt_input, server.stdev_dt_input);

    // Simulation Links
    inward_link(registration_input, queue.registration_input);
    inward_link(acceptance_input, server.acceptance_input);
    inward_link(item_input, queue.item_input);
    inner_link(queue.registration_output, server.registration_input);
    inner_link(server.acceptance_output, queue.acceptance_input);
    inner_link(queue.item_output, server.item_input);
    outward_link(server.registration_output, registration_output);
    outward_link(queue.acceptance_output, acceptance_output);
    outward_link(server.item_output, item_output);
}


}  // namespace

#endif
