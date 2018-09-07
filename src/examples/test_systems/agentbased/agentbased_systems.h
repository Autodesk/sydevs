#pragma once
#ifndef SYDEVS_EXAMPLES_AGENTBASED_SYSTEMS_H_
#define SYDEVS_EXAMPLES_AGENTBASED_SYSTEMS_H_

#include <examples/test_systems/agentbased/simple_agent_node.h>
#include <examples/test_systems/agentbased/simple_collection_node.h>
#include <examples/test_systems/agentbased/simple_agentbased_node.h>
#include <sydevs/systems/simulation.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


void agentbased_systems()
{
    try {
        simulation<simple_agentbased_node> sim(600_s, 0, std::cout);
        sim.top.grid_extent.print_on_event();
        sim.top.grid_extent.parameter.print_on_use();
        sim.top.avg_delay_dt.print_on_event();
        sim.top.avg_delay_dt.parameter.print_on_use();
        sim.top.invite_prob.print_on_event();
        sim.top.invite_prob.parameter.print_on_use();
        sim.top.print_on_event();
        sim.top.agents.print_on_event();
        sim.top.agents.avg_acquaintances_output.print_on_use();
        sim.top.avg_acquaintances.print_on_event();
        sim.process_remaining_events();
    }
    catch (const system_node::error& e) {
        std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "OTHER ERROR: " << e.what() << std::endl;
    }
}


}  // namespace

#endif
