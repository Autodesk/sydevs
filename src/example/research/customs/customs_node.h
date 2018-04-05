#pragma once
#ifndef CUSTOMS_NODE_H_
#define CUSTOMS_NODE_H_

#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/parameter_node.h>
#include <example/research/customs/robust_source_node.h>
#include <example/research/customs/robust_queue_node.h>
#include <example/research/customs/robust_server_node.h>
#include <example/research/customs/robust_queue_array_node.h>
#include <example/research/customs/robust_server_array_node.h>
#include <example/research/customs/robust_variable_lane_node.h>
#include <example/research/customs/robust_sink_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class customs_node : public composite_node
{
public:
    // Constructor/Destructor:
    customs_node(const std::string& node_name, const node_context& external_context);
    virtual ~customs_node() = default;

    // Ports:

    // Components:
    parameter_node<int64> source_id;
    parameter_node<duration> source_dt;
    parameter_node<duration> source_inactive_dt;
    parameter_node<duration> source_active_dt;
    parameter_node<int64> source_start_id;
    robust_source_node source;

    parameter_node<int64> kiosk_queue_id;
    parameter_node<int64> kiosk_queue_capacity;
    robust_queue_node kiosk_queue;

    parameter_node<int64> kiosk_array_id;
    parameter_node<int64> kiosk_count;
    parameter_node<duration> kiosk_dt;
    parameter_node<duration> kiosk_zero_dt;
    robust_server_array_node kiosk_array;

    parameter_node<int64> central_queue_id;
    parameter_node<int64> central_queue_count;
    parameter_node<int64> central_queue_capacity;
    robust_queue_array_node central_queue;

    parameter_node<int64> booths_id;
    parameter_node<int64> booths_queue_capacity;
    parameter_node<duration> booths_server_mean_dt;
    parameter_node<duration> booths_server_stdev_dt;
    parameter_node<duration> booths_lane_creation_dt;
    robust_variable_lane_node booths;

    parameter_node<int64> sink_id;
    robust_sink_node sink;
};


customs_node::customs_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)

    , source_id("source_id", internal_context(), 1000)                    // ID of the node that generates arrivals
    , source_dt("source_dt", internal_context(), 15_s)                    // Average time between people arriving (connected to mean_dt_input
                                                                          // and stdev_dt_input ports to create exponential distribution)
    , source_inactive_dt("source_inactive_dt", internal_context(), 0_hr)  // Time before people begin arriving
    , source_active_dt("source_inactive_dt", internal_context(), 1_hr)    // Time after no more people arrive
    , source_start_id("source_start_id", internal_context(), 0)           // ID of the first person to arrive
    , source("source", internal_context())                                // Node that generates arrivals

    , kiosk_queue_id("kiosk_queue_id", internal_context(), 2000)                    // ID of the node representing people lining up for a kiosk
    , kiosk_queue_capacity("kiosk_queue_capacity", internal_context(), 1000000000)  // Maximum number of people lining up for a kiosk
    , kiosk_queue("kiosk_queue", internal_context())                                // Node representing people lining up for a kiosk

    , kiosk_array_id("kiosk_array_id", internal_context(), 3000)  // ID of the node representing the kiosks
    , kiosk_count("kiosk_count", internal_context(), 10)          // Number of kiosks
    , kiosk_dt("kiosk_dt", internal_context(), 2_min)             // Time required to be served by kiosk
    , kiosk_zero_dt("kiosk_zero_dt", internal_context(), 0_s)     // Zero duration (connected to stdev_dt_input port
                                                                  // to indicate that kiosk service time does not vary)
    , kiosk_array("kiosk_array", internal_context())              // Node representing the kiosks

    , central_queue_id("central_queue_id", internal_context(), 4000)            // ID of the node representing people lining up between the kiosks and the booths
    , central_queue_count("central_queue_count", internal_context(), 2)         // Number of central queues
    , central_queue_capacity("central_queue_capacity", internal_context(), 25)  // Maximum number of people lining up in each central queue
    , central_queue("central_queue", internal_context())                        // Node representing people lining up between the kiosks and the booths

    , booths_id("booths_id", internal_context(), 5000)                               // ID if the node representing booths and associated queues
    , booths_queue_capacity("booths_queue_capacity", internal_context(), 5)          // Maximum number of people lining up for a single booth
    , booths_server_mean_dt("booths_server_mean_dt", internal_context(), 90_s)       // Average time to serve a person at a booth
    , booths_server_stdev_dt("booths_server_stdev_dt", internal_context(), 45_s)     // Standard deviation of time to serve a person at a booth
    , booths_lane_creation_dt("booths_lane_creation_dt", internal_context(), 3_min)  // Time required to open a new booth and associated queue
    , booths("booths", internal_context())                                           // Node representing booths and associated queues

    , sink_id("sink_id", internal_context(), 6000)  // ID of the node that handles departures
    , sink("sink", internal_context())              // Node that handles departures
{
    // Initialization Links - Source
    inner_link(source_id.parameter, source.id_input);
    inner_link(source_dt.parameter, source.mean_dt_input);
    inner_link(source_dt.parameter, source.stdev_dt_input); 
    inner_link(source_inactive_dt.parameter, source.inactive_dt_input);
    inner_link(source_active_dt.parameter, source.active_dt_input);
    inner_link(source_start_id.parameter, source.start_item_id_input);

    // Initialization Links - Kiosk Queue
    inner_link(kiosk_queue_id.parameter, kiosk_queue.id_input);
    inner_link(kiosk_queue_capacity.parameter, kiosk_queue.capacity_input);
     
    // Initialization Links - Kiosk Array
    inner_link(kiosk_array_id.parameter, kiosk_array.id_input);
    inner_link(kiosk_count.parameter, kiosk_array.server_count_input);
    inner_link(kiosk_dt.parameter, kiosk_array.mean_dt_input);
    inner_link(kiosk_zero_dt.parameter, kiosk_array.stdev_dt_input); 

    // Initialization Links - Main Queues
    inner_link(central_queue_id.parameter, central_queue.id_input);
    inner_link(central_queue_count.parameter, central_queue.queue_count_input);
    inner_link(central_queue_capacity.parameter, central_queue.capacity_input);

    // Initialization Links - Booths
    inner_link(booths_id.parameter, booths.id_input);
    inner_link(booths_queue_capacity.parameter, booths.queue_capacity_input);
    inner_link(booths_server_mean_dt.parameter, booths.server_mean_dt_input);
    inner_link(booths_server_stdev_dt.parameter, booths.server_stdev_dt_input);
    inner_link(booths_lane_creation_dt.parameter, booths.lane_creation_dt_input);

    // Initialization Links - Sink
    inner_link(sink_id.parameter, sink.id_input);

    // Simulation Links
    inner_link(source.registration_output, kiosk_queue.registration_input);
    inner_link(kiosk_queue.acceptance_output, source.acceptance_input);
    inner_link(source.item_output, kiosk_queue.item_input);

    inner_link(kiosk_queue.registration_output, kiosk_array.registration_input);
    inner_link(kiosk_array.acceptance_output, kiosk_queue.acceptance_input);
    inner_link(kiosk_queue.item_output, kiosk_array.item_input);

    inner_link(kiosk_array.registration_output, central_queue.registration_input);
    inner_link(central_queue.acceptance_output, kiosk_array.acceptance_input);
    inner_link(kiosk_array.item_output, central_queue.item_input);

    inner_link(central_queue.registration_output, booths.registration_input);
    inner_link(booths.acceptance_output, central_queue.acceptance_input);
    inner_link(central_queue.item_output, booths.item_input);

    inner_link(booths.registration_output, sink.registration_input);
    inner_link(sink.acceptance_output, booths.acceptance_input);
    inner_link(booths.item_output, sink.item_input);
}


#endif
