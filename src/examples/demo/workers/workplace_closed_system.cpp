#include <examples/demo/workers/workplace_closed_system.h>

namespace sydevs_examples {


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
