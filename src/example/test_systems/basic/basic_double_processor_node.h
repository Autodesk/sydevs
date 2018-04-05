#pragma once
#ifndef BASIC_DOUBLE_PROCESSOR_NODE_H_
#define BASIC_DOUBLE_PROCESSOR_NODE_H_

#include <example/test_systems/basic/basic_processor_node.h>
#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/function_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class basic_double_processor_node : public composite_node
{
public:
    // Constructor/Destructor:
    basic_double_processor_node(const std::string& node_name, const node_context& external_context);
    virtual ~basic_double_processor_node() = default;

    // Ports:
    port<flow, input, duration> response_dt_A_input;
    port<flow, input, duration> response_dt_B_input;
    port<message, input, int64> job_id_input;
    port<message, output, int64> job_id_output;
    port<flow, output, int64> miss_count_output;

    // Nested Classes:
    class miss_count_adder_node : public function_node {
    public:
        miss_count_adder_node(const std::string& node_name, const node_context& external_context);
        port<flow, input, int64> miss_count_A_input;
        port<flow, input, int64> miss_count_B_input;
        port<flow, output, int64> miss_count_output;
    protected:
        void flow_event();
    };

    // Components:
    basic_processor_node processor_A;
    basic_processor_node processor_B;
    miss_count_adder_node adder;
};


basic_double_processor_node::basic_double_processor_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , response_dt_A_input("response_dt_A_input", external_interface())
    , response_dt_B_input("response_dt_B_input", external_interface())
    , job_id_input("job_id_input", external_interface())
    , job_id_output("job_id_output", external_interface())
    , miss_count_output("miss_count_output", external_interface())
    , processor_A("processor_A", internal_context())
    , processor_B("processor_B", internal_context())
    , adder("adder", internal_context())
{
    // Initialization Links
    inward_link(response_dt_A_input, processor_A.response_dt_input);
    inward_link(response_dt_B_input, processor_B.response_dt_input);

    // Simulation Links
    inward_link(job_id_input, processor_A.job_id_input);
    inner_link(processor_A.job_id_output, processor_B.job_id_input);
    outward_link(processor_B.job_id_output, job_id_output);
  
    // Finalization Links
    inner_link(processor_A.miss_count_output, adder.miss_count_A_input);
    inner_link(processor_B.miss_count_output, adder.miss_count_B_input);
    outward_link(adder.miss_count_output, miss_count_output);
}


basic_double_processor_node::miss_count_adder_node::miss_count_adder_node(const std::string& node_name, const node_context& external_context)
    : function_node(node_name, external_context)
    , miss_count_A_input("miss_count_A_input", external_interface())
    , miss_count_B_input("miss_count_B_input", external_interface())
    , miss_count_output("miss_count_output", external_interface())
{
}


void basic_double_processor_node::miss_count_adder_node::flow_event()
{
    int64 miss_count = miss_count_A_input.value() + miss_count_B_input.value();

    miss_count_output.assign(miss_count);
}


#endif
