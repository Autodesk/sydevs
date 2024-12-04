#include <examples/demo/workers/workplace_node.h>

namespace sydevs_examples {


workplace_node::workplace_node(const std::string& node_name, const node_context& external_context)
    : collection_node<int64, worker_node>(node_name, external_context)
    , work_dt_input("work_dt_input", external_interface())
    , break_dt_input("break_dt_input", external_interface())
    , arrival_input("arrival_input", external_interface())
    , change_output("change_output", external_interface())
{
}


duration workplace_node::macro_initialization_event()
{
    // Initialize the work and break durations from the flow input ports, and
    // fix the time precision to that of the node (a recommended practice).
    work_dt = work_dt_input.value().fixed_at(time_precision());
    break_dt = break_dt_input.value().fixed_at(time_precision());

    // There are currently no workers at all, hence none on break.
    workers_on_break = std::set<int64>();

    // Wait indefinitely.
    return duration::inf();
}


duration workplace_node::macro_unplanned_event(duration elapsed_dt)
{
    // Handle the message input.
    if (arrival_input.received()) {
        // Get the received worker ID
        int64 agent_id = arrival_input.value();

        // Create the worker agent.
        get(prototype.work_dt_input) = work_dt;
        get(prototype.break_dt_input) = break_dt;
        create_agent(agent_id);
    }

    // Wait indefinitely.
    return duration::inf();
}


duration workplace_node::micro_planned_event(const int64& agent_id, duration elapsed_dt)
{
    // Handle the message output transmitted from the worker agent.
    if (transmitted(prototype.task_output)) {
        // Get the transmitted task.
        const std::string& task = get(prototype.task_output);

        // Add or remove the agent from the set of workers on break.
        int64 break_dn = 0;
        if (task == "break") {
            workers_on_break.insert(agent_id);
            break_dn = 1;
        }
        else if (task == "work") {
            workers_on_break.erase(agent_id);
            break_dn = -1;
        }

        // Notify the co-workers on break that a worker has started their break
        // or resumed working.
        for (int64 i : workers_on_break) {
            if (i != agent_id) {
                get(prototype.break_dn_input) = break_dn;
                affect_agent(i);
            }
        }

        // Send the worker ID and new task as a message output.
        change_output.send({ agent_id, task });
    }

    // Wait indefinitely.
    return duration::inf();
}


inline duration workplace_node::macro_planned_event(duration elapsed_dt)
{
    // This function should never be called, so trigger an error by returning
    // an invalid planned duration.
    return duration();
}


inline void workplace_node::macro_finalization_event(duration elapsed_dt)
{
}


}  // namespace
