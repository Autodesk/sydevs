#pragma once
#ifndef TWO_STAGE_QUEUEING_SIMULATION_NODE_H_
#define TWO_STAGE_QUEUEING_SIMULATION_NODE_H_

#include <example/demo/queueing/two_stage_queueing_node.h>
#include <example/demo/queueing/job_generation_node.h>
#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/parameter_node.h>
#include <sydevs/systems/statistic_node.h>

using namespace sydevs;
using namespace sydevs::systems;


/**
 * This node allows simulations to be performed on the 
 * "two_stage_queueing_node" composite class, supplying jobs at regular
 * intervals and providing access to the parameters and statistics.
 */
class two_stage_queueing_simulation_node : public composite_node
{
public:
    // Constructor/Destructor:
    two_stage_queueing_simulation_node(const std::string& node_name, const node_context& external_context);
    virtual ~two_stage_queueing_simulation_node() = default;

    // Components:

    parameter_node<duration> job_gen_dt;  // node supplying the job generation duration as a parameter
    parameter_node<duration> serv_dt;     // node supplying the service duration as a parameter
    job_generation_node job_generation;   // node supplying jobs at regular intervals
    two_stage_queueing_node queue;        // node representing the two sequential queues
    statistic_node<duration> idle_dt;     // node accessing the idle duration as a statistic
};


two_stage_queueing_simulation_node::two_stage_queueing_simulation_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , job_gen_dt("job_gen_dt", internal_context())
    , serv_dt("serv_dt", internal_context())
    , job_generation("job_generation", internal_context())
    , queue("queue", internal_context())
    , idle_dt("idle_dt", internal_context())
{
    // Flow Input Links:

    // Flow Micro Links:
    inner_link(job_gen_dt.parameter, job_generation.job_gen_dt_input);
    inner_link(serv_dt.parameter, queue.serv_dt_input);
    inner_link(queue.idle_dt_output, idle_dt.statistic);

    // Flow Output Links:

    // Message Input Links:

    // Message Micro Links:
    inner_link(job_generation.job_id_output, queue.job_id_input);

    // Message Output Links:
}


#endif
