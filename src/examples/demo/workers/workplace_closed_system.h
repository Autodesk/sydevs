#pragma once
#ifndef SYDEVS_EXAMPLES_WORKPLACE_CLOSED_SYSTEM_H_
#define SYDEVS_EXAMPLES_WORKPLACE_CLOSED_SYSTEM_H_

#include <examples/demo/workers/workplace_node.h>
#include <examples/demo/workers/worker_arrival_node.h>
#include <examples/demo/workers/workplace_csv_output_node.h>
#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/parameter_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


/**
 * This node allows simulations to be performed on the "workplace_node", 
 * generating worker arrivals at a regular interval and providing access to the
 * parameters.
 */
class workplace_closed_system : public composite_node
{
public:
    // Constructor/Destructor:
    workplace_closed_system(const std::string& node_name, const node_context& external_context);
    virtual ~workplace_closed_system() = default;

    // Initialization Value Components:
    parameter_node<int64> worker_count;   // node supplying the total number of workers
    parameter_node<duration> arrival_dt;  // node supplying the worker arrival duration as a parameter
    parameter_node<duration> work_dt;     // node supplying the work duration as a parameter
    parameter_node<duration> break_dt;    // node supplying the break duration as a parameter
    parameter_node<duration> frame_dt;    // node supplying the output frame duration as a parameter

    // Initialization Object Components:
    parameter_node<std::shared_ptr<std::ostream>> ostream_ptr;  // node supplying the output stream

    // Simulation Components:
    worker_arrival_node worker_arrival;              // node supplying workers at a regular interval
    workplace_node workplace;                        // node representing the workplace
    workplace_csv_output_node workplace_csv_output;  // node outputting information in CSV format 
};


workplace_closed_system::workplace_closed_system(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , worker_count("worker_count", internal_context())
    , arrival_dt("arrival_dt", internal_context())
    , work_dt("work_dt", internal_context())
    , break_dt("break_dt", internal_context())
    , frame_dt("frame_dt", internal_context())
    , ostream_ptr("ostream_ptr", internal_context())
    , worker_arrival("worker_arrival", internal_context())
    , workplace("workplace", internal_context())
    , workplace_csv_output("workplace_csv_output", internal_context())
{
    // Flow Input Links:

    // Flow Micro Links:
    inner_link(worker_count.parameter, worker_arrival.worker_count_input);
    inner_link(arrival_dt.parameter, worker_arrival.arrival_dt_input);
    inner_link(work_dt.parameter, workplace.work_dt_input);
    inner_link(break_dt.parameter, workplace.break_dt_input);
    inner_link(worker_count.parameter, workplace_csv_output.worker_count_input);
    inner_link(frame_dt.parameter, workplace_csv_output.frame_dt_input);
    inner_link(ostream_ptr.parameter, workplace_csv_output.ostream_ptr_input);

    // Flow Output Links:

    // Message Input Links:

    // Message Micro Links:
    inner_link(worker_arrival.worker_id_output, workplace.arrival_input);
    inner_link(workplace.change_output, workplace_csv_output.change_input);

    // Message Output Links:
}


}  // namespace

#endif
