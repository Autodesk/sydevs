#pragma once
#ifndef SYDEVS_EXAMPLES_TWO_STAGE_QUEUEING_NODE_H_
#define SYDEVS_EXAMPLES_TWO_STAGE_QUEUEING_NODE_H_

#include <examples/demo/queueing/queueing_node.h>
#include <examples/demo/queueing/plus_node.h>
#include <sydevs/systems/composite_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


/**
 * This node places two queueing nodes end-on-end, so that incoming jobs must
 * pass through two queues in order to be processed. The idle duration of this
 * composite node is the sum of the idle durations of the two queueing nodes.
 */
class two_stage_queueing_node : public composite_node
{
public:
    // Constructor/Destructor:
    two_stage_queueing_node(const std::string& node_name, const node_context& external_context);
    virtual ~two_stage_queueing_node() = default;

    // Ports:
    port<flow, input, duration> serv_dt_input;    // service duration
    port<message, input, int64> job_id_input;     // job ID (input)
    port<message, output, int64> job_id_output;   // job ID (output)
    port<flow, output, duration> idle_dt_output;  // idle duration

    // Components:
    queueing_node queue_A;     // node representing the 1st stage (queue A)
    queueing_node queue_B;     // node representing the 2nd stage (queue B)
    plus_node<duration> plus;  // node adding the idle times of both queues A and B
};


two_stage_queueing_node::two_stage_queueing_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , serv_dt_input("serv_dt_input", external_interface())
    , job_id_input("job_id_input", external_interface())
    , job_id_output("job_id_output", external_interface())
    , idle_dt_output("idle_dt_output", external_interface())
    , queue_A("queue_A", internal_context())
    , queue_B("queue_B", internal_context())
    , plus("plus", internal_context())
{
    // Flow Input Links:
    inward_link(serv_dt_input, queue_A.serv_dt_input);
    inward_link(serv_dt_input, queue_B.serv_dt_input);

    // Flow Micro Links:
    inner_link(queue_A.idle_dt_output, plus.a_input);
    inner_link(queue_B.idle_dt_output, plus.b_input);

    // Flow Output Links:
    outward_link(plus.c_output, idle_dt_output);

    // Message Input Links:
    inward_link(job_id_input, queue_A.job_id_input);

    // Message Micro Links:
    inner_link(queue_A.job_id_output, queue_B.job_id_input);

    // Message Output Links:
    outward_link(queue_B.job_id_output, job_id_output);
}


}  // namespace

#endif
