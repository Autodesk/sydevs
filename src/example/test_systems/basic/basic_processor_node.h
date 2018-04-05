#pragma once
#ifndef BASIC_PROCESSOR_NODE_H_
#define BASIC_PROCESSOR_NODE_H_

#include <sydevs/systems/atomic_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class basic_processor_node : public atomic_node
{
public:
    // Constructor/Destructor:
    basic_processor_node(const std::string& node_name, const node_context& external_context);
    virtual ~basic_processor_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, duration> response_dt_input;
    port<message, input, int64> job_id_input;
    port<message, output, int64> job_id_output;
    port<flow, output, int64> miss_count_output;

protected:
    // State Variables:
    duration response_dt;
    int64 job_id;
    duration planned_dt;
    int64 miss_count;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline basic_processor_node::basic_processor_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , response_dt_input("response_dt_input", external_interface())
    , job_id_input("job_id_input", external_interface())
    , job_id_output("job_id_output", external_interface())
    , miss_count_output("miss_count_output", external_interface())
{
}


inline duration basic_processor_node::initialization_event()
{
    response_dt = response_dt_input.value().fixed_at(time_precision());
    job_id = -1;
    planned_dt = duration::inf();
    miss_count = 0;
    return planned_dt;
}


inline duration basic_processor_node::unplanned_event(duration elapsed_dt)
{
    if (planned_dt.finite()) {
        planned_dt -= elapsed_dt;
        ++miss_count;
        print("miss_count = " + tostring(miss_count));
    }
    else if (job_id_input.received()) {
        job_id = job_id_input.value();
        planned_dt = response_dt;
    }
    return planned_dt;
}


inline duration basic_processor_node::planned_event(duration elapsed_dt)
{
    job_id_output.send(job_id);

    job_id = -1;
    planned_dt = duration::inf();
    return planned_dt;
}


inline void basic_processor_node::finalization_event(duration elapsed_dt)
{
    miss_count_output.assign(miss_count);
}


#endif
