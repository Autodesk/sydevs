#pragma once
#ifndef SYDEVS_EXAMPLES_WORKPLACE_NODE_H_
#define SYDEVS_EXAMPLES_WORKPLACE_NODE_H_

#include <examples/demo/workers/worker_node.h>
#include <sydevs/systems/collection_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


/**
 * This node represents a workplace consisting of multiple workers who may
 * arrive at different times and then alterative between two tasks: working
 * ("work") and taking a break ("break"). Workers are influenced by the actions
 * of their co-workers. Specifically, when they are taking a break and are
 * joined by a co-worker, they extend the duration of their break. When they are
 * taking break and a co-worker leaves to resume working, they shorten the
 * duration of their break.
 */
class workplace_node : public collection_node<int64, worker_node>
{
public:
    // Constructor/Destructor:
    workplace_node(const std::string& node_name, const node_context& external_context);
    virtual ~workplace_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, duration> work_dt_input;                           // work duration
    port<flow, input, duration> break_dt_input;                          // break duration
    port<message, input, int64> arrival_input;                           // ID of arriving worker
    port<message, output, std::pair<int64, std::string>> change_output;  // ID of worker and their new task

protected:
    // State Variables:
    duration work_dt;                  // work duration (constant)
    duration break_dt;                 // break duration (constant)
    std::set<int64> workers_on_break;  // IDs of workers on break

    // Event Handlers:
    virtual duration macro_initialization_event();
    virtual duration macro_unplanned_event(duration elapsed_dt);
    virtual duration micro_planned_event(const int64& agent_id, duration elapsed_dt);
    virtual duration macro_planned_event(duration elapsed_dt);
    virtual void macro_finalization_event(duration elapsed_dt);
};


}  // namespace

#endif
