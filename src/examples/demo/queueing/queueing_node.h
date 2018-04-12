#pragma once
#ifndef SYDEVS_EXAMPLES_QUEUEING_NODE_H_
#define SYDEVS_EXAMPLES_QUEUEING_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


/**
 * This node processes jobs one at a time. The time required to process a job
 * is determined by the service duration flow input. Any job that arrives while
 * another is being processed is placed in a queue and processed later. The
 * total time spend in an idle state, with no jobs to be processed, is tracked
 * and reported as a flow output.
 */
class queueing_node : public atomic_node
{
public:
    // Constructor/Destructor:
    queueing_node(const std::string& node_name, const node_context& external_context);
    virtual ~queueing_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, duration> serv_dt_input;    // service duration
    port<message, input, int64> job_id_input;     // job ID (input)
    port<message, output, int64> job_id_output;   // job ID (output)
    port<flow, output, duration> idle_dt_output;  // idle duration

protected:
    // State Variables:
    duration serv_dt;      // service duration (constant)
    std::vector<int64> Q;  // queue of IDs of jobs waiting to be processed
    duration idle_dt;      // idle duration (accumulating)
    duration planned_dt;   // planned duration

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline queueing_node::queueing_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , serv_dt_input("serv_dt_input", external_interface())
    , job_id_input("job_id_input", external_interface())
    , job_id_output("job_id_output", external_interface())
    , idle_dt_output("idle_dt_output", external_interface())
{
}


inline duration queueing_node::initialization_event()
{
    // Initialize the service duration from the flow input port, and fix the
    // time precision level at that of the node (a recommended practice).
    serv_dt = serv_dt_input.value().fixed_at(time_precision());  

    // Initialize the empty queue and accumulated idle time.
    Q = std::vector<int64>();
    idle_dt = 0_s;

    // Wait indefinitely for a message input (unplanned event) or the end of
    // the simulation (finalization event).
    planned_dt = duration::inf();
    return planned_dt;
}


inline duration queueing_node::unplanned_event(duration elapsed_dt)
{
    // If the queue is empty, record the elapsed duration as idle time.
    if (Q.empty()) {
        idle_dt += elapsed_dt;
    }

    // Handle the message input.
    if (job_id_input.received()) {
        // Get the received job ID
        int64 job_id = job_id_input.value();

        // Add the job ID to the queue.
        Q.push_back(job_id);

        // If the only queued job is the one just added, process it for the
        // service duration; otherwise continue processing a preexisting job
        // for the same duration as before minus the elapsed duration.
        if (Q.size() == 1) {
            planned_dt = serv_dt;
        }
        else {
            planned_dt -= elapsed_dt;
        }
    }

    // Schedule a planned event when the job at the front of the queue is
    // processed.
    return planned_dt;
}


inline duration queueing_node::planned_event(duration elapsed_dt)
{
    // The first job in the queue has been processed, so send the ID as a
    // message output and remove it from the queue.
    int64 job_id = Q[0];
    job_id_output.send(job_id);
    Q.erase(std::begin(Q));

    // If the queue is now empty, wait indefinitely; otherwise process the next
    // job for the service duration.
    if (Q.empty()) {
        planned_dt = duration::inf();
    }
    else {
        planned_dt = serv_dt;
    }
    return planned_dt;
}


inline void queueing_node::finalization_event(duration elapsed_dt)
{
    // If the queue is empty, record the elapsed duration as idle time.
    if (Q.empty()) {
        idle_dt += elapsed_dt;
    }

    // Assign the accumulated idle time to the flow output port.
    idle_dt_output.assign(idle_dt);
}


}  // namespace

#endif
