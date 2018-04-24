#pragma once
#ifndef SYDEVS_EXAMPLES_PARALLEL_QUEUEING_NODE_H_
#define SYDEVS_EXAMPLES_PARALLEL_QUEUEING_NODE_H_

#include <examples/demo/queueing/queueing_node.h>
#include <sydevs/systems/collection_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


/**
 * This node initially directs incoming jobs to a single queue, but more queues
 * are added in parallel when needed. Normally, a job will go into the first
 * queue with fewer than the maximum number of jobs. But if a job is received
 * while the current queues are all full, then a new queue is created. The
 * variable number of queues are modeled as agents in a collection. The idle
 * duration of this collection node is the sum of the idle durations of all
 * queueing node agents.
 */
class parallel_queueing_node : public collection_node<int64, queueing_node>
{
public:
    // Constructor/Destructor:
    parallel_queueing_node(const std::string& node_name, const node_context& external_context);
    virtual ~parallel_queueing_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, duration> serv_dt_input;    // service duration
    port<flow, input, int64> max_n_input;         // maximum number of jobs in a queue
    port<message, input, int64> job_id_input;     // job ID (input)
    port<message, output, int64> job_id_output;   // job ID (output)
    port<flow, output, duration> idle_dt_output;  // idle duration

protected:
    // State Variables:
    duration serv_dt;      // service duration (constant)
    int64 max_n;           // maximum number of jobs in a queue (constant)
    std::vector<int64> N;  // number of jobs in each queue

    // Event Handlers:
    virtual duration macro_initialization_event();
    virtual duration macro_unplanned_event(duration elapsed_dt);
    virtual duration micro_planned_event(const int64& agent_id, duration elapsed_dt);
    virtual duration macro_planned_event(duration elapsed_dt);
    virtual void macro_finalization_event(duration elapsed_dt);
};


inline parallel_queueing_node::parallel_queueing_node(const std::string& node_name, const node_context& external_context)
    : collection_node<int64, queueing_node>(node_name, external_context)
    , serv_dt_input("serv_dt_input", external_interface())
    , max_n_input("max_n_input", external_interface())
    , job_id_input("job_id_input", external_interface())
    , job_id_output("job_id_output", external_interface())
    , idle_dt_output("idle_dt_output", external_interface())
{
}


inline duration parallel_queueing_node::macro_initialization_event()
{
    // Initialize the state variables.
    serv_dt = serv_dt_input.value().fixed_at(time_precision());  
    max_n = max_n_input.value();
    N = std::vector<int64>();

    // Add the first queue.
    access(prototype.serv_dt_input) = serv_dt;  // Set the queueing node flow input value.
    create_agent(0);                            // Create the queueing node agent.
    N.push_back(0);                             // Record that the queueing node has no jobs.

    // Wait indefinitely.
    return duration::inf();
}


inline duration parallel_queueing_node::macro_unplanned_event(duration elapsed_dt)
{
    // Handle the message input.
    if (job_id_input.received()) {
        // Get the received job ID
        int64 job_id = job_id_input.value();

        // Find the ID of the first queue with space for the job.
        int64 agent_id = 0;
        auto found = false;
        while (!found && agent_id < int64(N.size())) {
            if (N[agent_id] < max_n) {
                found = true;
            }
            else {
                ++agent_id;
            }
        }

        // Add a new queue if needed and update the number of jobs in each queue.
        if (agent_id == int64(N.size())) {
            access(prototype.serv_dt_input) = serv_dt;  // Set the queueing node flow input value.
            create_agent(agent_id);                     // Create the queueing node agent.
            N.push_back(1);                             // Record that the queueing node has one job.
        }
        else {
            ++N[agent_id];  // Record that the selected queueing node has one additional job.
        }

        // Send the received job ID to the selected queueing node.
        access(prototype.job_id_input) = job_id;
        affect_agent(agent_id);
    }

    // Wait indefinitely.
    return duration::inf();
}


inline duration parallel_queueing_node::micro_planned_event(const int64& agent_id, duration elapsed_dt)
{
    // Handle the message output transmitted from the queueing node.
    if (transmitted(prototype.job_id_output)) {
        // Get the transmitted job ID.
        int64 job_id = access(prototype.job_id_output);

        // Send the job ID as a message output.
        job_id_output.send(job_id);

        // Record that the transmitting queueing node has one fewer job.
        --N[agent_id];
    }

    // Wait indefinitely.
    return duration::inf();
}


inline duration parallel_queueing_node::macro_planned_event(duration elapsed_dt)
{
    // This function should never be called, so trigger an error by returning
    // an invalid planned duration.
    return duration();
}


inline void parallel_queueing_node::macro_finalization_event(duration elapsed_dt)
{
    // Add the idle times of all queueing nodes, and assign the result to the
    // flow output port.
    auto idle_dt = 0_s;
    while (agent_count() > 0) {
        int64 agent_id = *agent_begin();
        remove_agent(agent_id);
        idle_dt += access(prototype.idle_dt_output);
    }
    idle_dt_output.assign(idle_dt);
}


}  // namespace

#endif
