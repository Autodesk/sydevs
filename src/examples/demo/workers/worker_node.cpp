#include <examples/demo/workers/worker_node.h>

namespace sydevs_examples {


worker_node::worker_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , work_dt_input("work_dt_input", external_interface())
    , break_dt_input("break_dt_input", external_interface())
    , break_dn_input("break_dn_input", external_interface())
    , task_output("task_output", external_interface())
{
}


duration worker_node::initialization_event()
{
    // Initialize the work and break durations from the flow input ports, and
    // fix the time precision to that of the node (a recommended practice).
    work_dt = work_dt_input.value().fixed_at(time_precision());
    break_dt = break_dt_input.value().fixed_at(time_precision());

    // Set the task and planned duration so that the worker is initially working
    // for a duration of "work_dt".
    task = "work";
    planned_dt = work_dt;
    return planned_dt;
}


duration worker_node::unplanned_event(duration elapsed_dt)
{
    // Handle the message input.
    if (break_dn_input.received()) {
        // Get the received change in the number of workers on break
        int64 break_dn = break_dn_input.value();

        // Check if the current task is working or taking a break
        if (task == "work") {
            // If the current task is working, reschedule the break by
            // subtracting the elapsed duration.
            planned_dt -= elapsed_dt;
        }
        else if (task == "break") {
            // If the current task is taking a break, reschedule resumption of
            // work, delay it, or make it more imminent depending on how the
            // number of workers on break is changing.
            if (break_dn == 0) {
                planned_dt -= elapsed_dt;
            }
            else {
                auto pdt = double(planned_dt/duration(1, time_precision()));
                auto edt = double(elapsed_dt/duration(1, time_precision()));
                if (break_dn > 0) {
                    planned_dt = duration(int64(sqrt(pdt*pdt - edt*edt)), time_precision());
                }
                else if (break_dn < 0) {
                    planned_dt = duration(int64(pdt - sqrt(2*pdt*edt - edt*edt)), time_precision());
                }
            }
        }
    }

    // Schedule the planned event.
    return planned_dt;
}


duration worker_node::planned_event(duration elapsed_dt)
{
    // Change the task from working to taking a break, or vice versa, and set
    // the planned duration accordingly.
    if (task == "work") {
        task = "break";
        planned_dt = break_dt;
    }
    else if (task == "break") {
        task = "work";
        planned_dt = work_dt;
    }

    // Send the new task as a message output.
    task_output.send(task);

    // Schedule the planned event.
    return planned_dt;
}


inline void worker_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace
