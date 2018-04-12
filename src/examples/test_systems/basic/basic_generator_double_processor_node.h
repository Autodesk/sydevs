#pragma once
#ifndef SYDEVS_EXAMPLES_BASIC_GENERATOR_DOUBLE_PROCESSOR_NODE_H_
#define SYDEVS_EXAMPLES_BASIC_GENERATOR_DOUBLE_PROCESSOR_NODE_H_

#include <examples/test_systems/basic/basic_generator_node.h>
#include <examples/test_systems/basic/basic_double_processor_node.h>
#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/parameter_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class basic_generator_double_processor_node : public composite_node
{
public:
    // Constructor/Destructor:
    basic_generator_double_processor_node(const std::string& node_name, const node_context& external_context);
    virtual ~basic_generator_double_processor_node() = default;

    // Components:
    parameter_node<duration> mean_dt_parameter;
    parameter_node<duration> response_dt_A_parameter;
    parameter_node<duration> response_dt_B_parameter;
    basic_generator_node generator;
    basic_double_processor_node processor;
};


basic_generator_double_processor_node::basic_generator_double_processor_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , mean_dt_parameter("mean_dt_parameter", internal_context(), 1_s)
    , response_dt_A_parameter("response_dt_A_parameter", internal_context(), 250_ms)
    , response_dt_B_parameter("response_dt_B_parameter", internal_context(), 500_ms)
    , generator("generator", internal_context())
    , processor("processor", internal_context())
{
    // Initialization Links
    inner_link(mean_dt_parameter.parameter, generator.mean_dt_input);
    inner_link(response_dt_A_parameter.parameter, processor.response_dt_A_input);
    inner_link(response_dt_B_parameter.parameter, processor.response_dt_B_input);

    // Simulation Links
    inner_link(generator.count_output, processor.job_id_input);
}


}  // namespace

#endif
