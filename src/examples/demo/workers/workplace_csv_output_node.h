#pragma once
#ifndef SYDEVS_EXAMPLES_WORKPLACE_CSV_OUTPUT_NODE_H_
#define SYDEVS_EXAMPLES_WORKPLACE_CSV_OUTPUT_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


/**
 * This node outputs, at regular time intervals, a line of comma-separated
 * variable (CSV) text indicating which workers are taking a break. The 
 * information is inserted onto the output stream, which could be a text file.
 */
class workplace_csv_output_node : public atomic_node
{
public:
    // Constructor/Destructor:
    workplace_csv_output_node(const std::string& node_name, const node_context& external_context);
    virtual ~workplace_csv_output_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, int64> worker_count_input;                         // number of workers
    port<flow, input, duration> frame_dt_input;                          // frame duration
    port<flow, input, std::shared_ptr<std::ostream>> ostream_ptr_input;  // output stream pointer
    port<message, input, std::pair<int64, std::string>> change_input;    // ID of worker and their new task

protected:
    // State Variables:
    duration frame_dt;                     // frame duration
    std::shared_ptr<std::ostream> os_ptr;  // output stream pointer
    array1d<bool> break_flags;             // flags indicating which workers are on break
    duration planned_dt;                   // planned duration

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


}  // namespace

#endif
