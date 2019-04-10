#pragma once
#ifndef SYDEVS_EXAMPLES_WORKER_ARRIVAL_NODE_H_
#define SYDEVS_EXAMPLES_WORKER_ARRIVAL_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


/**
 * This node generates workers with incrementing IDs at a regular interval.
 */
class worker_arrival_node : public atomic_node
{
public:
    // Constructor/Destructor:
    worker_arrival_node(const std::string& node_name, const node_context& external_context);
    virtual ~worker_arrival_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, int64> worker_count_input;    // number of workers
    port<flow, input, duration> arrival_dt_input;   // worker arrival duration
    port<message, output, int64> worker_id_output;  // worker ID

protected:
    // State Variables:
    int64 worker_count;   // number of workers (constant)
    duration arrival_dt;  // worker arrival duration (constant)
    int64 worker_id;      // worker ID (incrementing)

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


}  // namespace

#endif
