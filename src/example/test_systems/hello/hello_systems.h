#pragma once
#ifndef HELLO_SYSTEMS_H_
#define HELLO_SYSTEMS_H_

#include <example/test_systems/hello/hello_at_init_node.h>
#include <example/test_systems/hello/hello_at_planned_node.h>
#include <example/test_systems/hello/hello_count_node.h>
#include <example/test_systems/hello/hello_poisson_node.h>
#include <sydevs/systems/simulation.h>
#include <iostream>


void hello_systems()
{
    try {
        {
            std::cout << "hello_at_init_node" << std::endl;
            sydevs::systems::simulation<hello_at_init_node> sim(10_s, 0, std::cout);
            sim.top.print_on_event();
            sim.process_remaining_events();
        }
        std::cout << std::endl;
        {
            std::cout << "hello_at_planned_node" << std::endl;
            sydevs::systems::simulation<hello_at_planned_node> sim(10_s, 0, std::cout);
            sim.top.print_on_event();
            sim.process_remaining_events();
        }
        std::cout << std::endl;
        {
            std::cout << "hello_count_node" << std::endl;
            sydevs::systems::simulation<hello_count_node> sim(10_s, 0, std::cout);
            sim.top.print_on_event();
            sim.process_remaining_events();
        }
        std::cout << std::endl;
        {
            std::cout << "hello_poisson_node" << std::endl;
            sydevs::systems::simulation<hello_poisson_node> sim(10_s, 0, std::cout);
            sim.top.print_on_event();
            sim.process_remaining_events();
        }
    }
    catch (const sydevs::systems::system_node::error& e) {
        std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "OTHER ERROR: " << e.what() << std::endl;
    }
}


#endif
