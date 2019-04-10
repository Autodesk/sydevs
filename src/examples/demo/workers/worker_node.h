#pragma once
#ifndef SYDEVS_EXAMPLES_WORKER_NODE_H_
#define SYDEVS_EXAMPLES_WORKER_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


/**
 * This node represents a worker who alteratives between two tasks: working
 * ("work") and taking a break ("break"). Workers are influenced by the actions
 * of their co-workers. Specifically, when they are taking a break and are
 * joined by a co-worker, they extend the duration of their break. When they are
 * taking break and a co-worker leaves to resume working, they shorten the
 * duration of their break.
 */
class worker_node : public atomic_node
{
public:
    // Constructor/Destructor:
    worker_node(const std::string& node_name, const node_context& external_context);
    virtual ~worker_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, duration> work_dt_input;       // work duration
    port<flow, input, duration> break_dt_input;      // break duration
    port<message, input, int64> break_dn_input;      // change in the number of workers on break
    port<message, output, std::string> task_output;  // new task (either "work" or "break")

protected:
    // State Variables:
    duration work_dt;     // work duration (constant)
    duration break_dt;    // break duration (constant)
    std::string task;     // current task
    duration planned_dt;  // planned duration

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


}  // namespace

#endif
