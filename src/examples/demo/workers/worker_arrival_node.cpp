#include <examples/demo/workers/worker_arrival_node.h>

namespace sydevs_examples {


worker_arrival_node::worker_arrival_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , worker_count_input("worker_count_input", external_interface())
    , arrival_dt_input("arrival_dt_input", external_interface())
    , worker_id_output("worker_id_output", external_interface())
{
}


duration worker_arrival_node::initialization_event()
{
    // Initialize the worker count from the flow input port.
    worker_count = worker_count_input.value();

    // Initialize the worker arrival duration from the flow input port, and fix
    // the time precision to that of the node (a recommended practice).
    arrival_dt = arrival_dt_input.value().fixed_at(time_precision());  

    // Set the ID of the first worker.
    worker_id = 0;

    // If there are any workers, schedule a planned event when the worker
    // arrival duration elapses.
    return worker_id < worker_count ? arrival_dt : duration::inf();
}


duration worker_arrival_node::unplanned_event(duration elapsed_dt)
{
    // This function should never be called, so trigger an error by returning
    // an invalid planned duration.
    return duration();
}


duration worker_arrival_node::planned_event(duration elapsed_dt)
{
    // Generate a worker by sending the ID, then increment the ID.
    worker_id_output.send(worker_id);
    ++worker_id;

    // If there are any workers, schedule a planned event when the worker
    // arrival duration elapses.
    return worker_id < worker_count ? arrival_dt : duration::inf();
}


void worker_arrival_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace
