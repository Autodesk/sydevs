#include <examples/demo/workers/workplace_csv_output_node.h>

namespace sydevs_examples {


workplace_csv_output_node::workplace_csv_output_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , worker_count_input("worker_count_input", external_interface())
    , frame_dt_input("frame_dt_input", external_interface())
    , ostream_ptr_input("ostream_ptr_input", external_interface())
    , change_input("change_input", external_interface())
{
}


duration workplace_csv_output_node::initialization_event()
{
    // Initialize the frame duration from the flow input port, and fix the time
    // precision to that of the node (a recommended practice).
    frame_dt = frame_dt_input.value().fixed_at(time_precision());

    // Initialize the output stream pointer from the flow input port.
    os_ptr = ostream_ptr_input.value();

    // Initialize the flags indicating which workers are working.
    int64 worker_count = worker_count_input.value();
    break_flags = array1d<bool>({worker_count}, false);

    // Output a frame immediately.
    return 0_s;
}


duration workplace_csv_output_node::unplanned_event(duration elapsed_dt)
{
    // Handle the message input.
    if (change_input.received()) {
        // Get the received change in a worker's task.
        const auto& [worker_id, task] = change_input.value();

        // Update the flag indicating whether the worker is on break.
        break_flags[worker_id] = (task == "break");
    }

    // Schedule the planned event.
    planned_dt -= elapsed_dt;
    return planned_dt;
}


duration workplace_csv_output_node::planned_event(duration elapsed_dt)
{
    // Output the frame by inserting a line onto the output stream.
    if (os_ptr) {
        auto& os = *os_ptr;
        std::string line;
        for (int64 worker_id = 0; worker_id < int64(break_flags.size()); ++worker_id) {
            if (worker_id > 0) {
                line += ",";
            }
            line += (break_flags[worker_id] ? "1" : "0");
        }
        os << line << std::endl;
    }

    // Schedule the planned event.
    planned_dt = frame_dt;
    return planned_dt;
}


inline void workplace_csv_output_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace
