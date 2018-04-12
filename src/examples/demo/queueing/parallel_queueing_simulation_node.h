#pragma once
#ifndef SYDEVS_EXAMPLES_PARALLEL_QUEUEING_SIMULATION_NODE_H_
#define SYDEVS_EXAMPLES_PARALLEL_QUEUEING_SIMULATION_NODE_H_

#include <examples/demo/queueing/parallel_queueing_node.h>
#include <examples/demo/queueing/job_generation_node.h>
#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/parameter_node.h>
#include <sydevs/systems/statistic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


/**
 * This node allows simulations to be performed on the "parallel_queueing_node"
 * collection class, supplying jobs at regular intervals and providing access
 * to the parameters and statistics.
 */
class parallel_queueing_simulation_node : public composite_node
{
public:
    // Constructor/Destructor:
    parallel_queueing_simulation_node(const std::string& node_name, const node_context& external_context);
    virtual ~parallel_queueing_simulation_node() = default;

    // Components:
    parameter_node<duration> job_gen_dt;  // node supplying the job generation duration as a parameter
    parameter_node<duration> serv_dt;     // node supplying the service duration as a parameter
    parameter_node<int64> max_n;          // node supplying the maximum number of jobs in a queue as a parameter
    job_generation_node job_generation;   // node supplying jobs at regular intervals
    parallel_queueing_node queue;         // node representing the collection of queues
    statistic_node<duration> idle_dt;     // node accessing the idle duration as a statistic
};


parallel_queueing_simulation_node::parallel_queueing_simulation_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , job_gen_dt("job_gen_dt", internal_context())
    , serv_dt("serv_dt", internal_context())
    , max_n("max_n", internal_context())
    , job_generation("job_generation", internal_context())
    , queue("queue", internal_context())
    , idle_dt("idle_dt", internal_context())
{
    // Flow Input Links:

    // Flow Micro Links:
    inner_link(job_gen_dt.parameter, job_generation.job_gen_dt_input);
    inner_link(serv_dt.parameter, queue.serv_dt_input);
    inner_link(max_n.parameter, queue.max_n_input);
    inner_link(queue.idle_dt_output, idle_dt.statistic);

    // Flow Output Links:

    // Message Input Links:

    // Message Micro Links:
    inner_link(job_generation.job_id_output, queue.job_id_input);

    // Message Output Links:
}


}  // namespace

#endif
