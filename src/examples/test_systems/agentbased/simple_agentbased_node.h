#pragma once
#ifndef SYDEVS_EXAMPLES_SIMPLE_AGENTBASED_NODE_H_
#define SYDEVS_EXAMPLES_SIMPLE_AGENTBASED_NODE_H_

#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/parameter_node.h>
#include <sydevs/systems/statistic_node.h>
#include <examples/test_systems/agentbased/simple_collection_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class simple_agentbased_node : public composite_node
{
public:
    // Constructor/Destructor:
    simple_agentbased_node(const std::string& node_name, const node_context& external_context);
    virtual ~simple_agentbased_node() = default;

    // Ports:

    // Components:
    parameter_node<int64> grid_extent;
    parameter_node<duration> avg_delay_dt;
    parameter_node<float64> invite_prob;
    simple_collection_node agents;
    statistic_node<float64> avg_acquaintances;
};


simple_agentbased_node::simple_agentbased_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , grid_extent("grid_extent", internal_context(), 1)
    , avg_delay_dt("avg_delay_dt", internal_context(), 10_s)
    , invite_prob("invite_prob", internal_context(), 0.2)
    , agents("agents", internal_context())
    , avg_acquaintances("avg_acquaintances", internal_context())
{
    // Initialization Links
    inner_link(grid_extent.parameter, agents.grid_extent_input);
    inner_link(avg_delay_dt.parameter, agents.avg_delay_dt_input);
    inner_link(invite_prob.parameter, agents.invite_prob_input);
     
    // Simulation Links
    
    // Finalization Links
    inner_link(agents.avg_acquaintances_output, avg_acquaintances.statistic);
}


}  // namespace

#endif
