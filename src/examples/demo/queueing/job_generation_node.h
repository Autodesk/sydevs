#pragma once
#ifndef SYDEVS_EXAMPLES_JOB_GENERATION_NODE_H_
#define SYDEVS_EXAMPLES_JOB_GENERATION_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


/**
 * This node generates jobs with an incrementing IDs at intervals specified by
 * the job generation duration flow input.
 */
class job_generation_node : public atomic_node
{
public:
    // Constructor/Destructor:
    job_generation_node(const std::string& node_name, const node_context& external_context);
    virtual ~job_generation_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, duration> job_gen_dt_input;  // job generation duration
    port<message, output, int64> job_id_output;    // job ID

protected:
    // State Variables:
    duration job_gen_dt;  // job generation duration (constant)
    int64 job_id;         // job ID (incrementing)

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline job_generation_node::job_generation_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , job_gen_dt_input("job_gen_dt_input", external_interface())
    , job_id_output("job_id_output", external_interface())
{
}


inline duration job_generation_node::initialization_event()
{
    // Initialize the job generation duration from the flow input port, and fix
    // the time precision level at that of the node (a recommended practice).
    job_gen_dt = job_gen_dt_input.value().fixed_at(time_precision());  

    // Set the ID of the first job.
    job_id = 0;

    // Schedule a planned event when the job generation duration elapses.
    return job_gen_dt;
}


inline duration job_generation_node::unplanned_event(duration elapsed_dt)
{
    // This function should never be called, so trigger an error by returning
    // an invalid planned duration.
    return duration();
}


inline duration job_generation_node::planned_event(duration elapsed_dt)
{
    // Generate a job by sending the ID, then increment the ID.
    job_id_output.send(job_id);
    ++job_id;

    // Schedule a planned event when the job generation duration elapses.
    return job_gen_dt;
}


inline void job_generation_node::finalization_event(duration elapsed_dt)
{
    // There are no flow output ports, so do nothing.
}


}  // namespace

#endif
