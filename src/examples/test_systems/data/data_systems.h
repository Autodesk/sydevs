#pragma once
#ifndef SYDEVS_EXAMPLES_DATA_SYSTEMS_H_
#define SYDEVS_EXAMPLES_DATA_SYSTEMS_H_

#include <examples/test_systems/data/data_primitives_composite_node.h>
#include <examples/test_systems/data/data_structures_composite_node.h>
#include <examples/test_systems/data/data_arraynds_composite_node.h>
#include <examples/test_systems/data/data_tuples_composite_node.h>
#include <sydevs/systems/simulation.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


void data_systems()
{
    {
        std::cout << "data_primitives_composite_node" << std::endl;
        simulation<data_primitives_composite_node> sim(10_s, 0, std::cout);
        sim.top.print_on_event();
        sim.top.A.print_on_event();
        sim.top.A.print_on_elapsed_duration();
        sim.top.A.print_on_planned_duration();
        sim.top.B.print_on_event();
        sim.top.B.print_on_elapsed_duration();
        sim.top.B.print_on_planned_duration();
        sim.top.A.mo_bool.print_on_use();
        sim.top.A.mo_int64.print_on_use();
        sim.top.A.mo_float64.print_on_use();
        sim.top.A.mo_duration.print_on_use();
        sim.top.A.mo_string.print_on_use();
        sim.top.B.mo_bool.print_on_use();
        sim.top.B.mo_int64.print_on_use();
        sim.top.B.mo_float64.print_on_use();
        sim.top.B.mo_duration.print_on_use();
        sim.top.B.mo_string.print_on_use();
        sim.top.A.fo_bool.print_on_use();
        sim.top.A.fo_int64.print_on_use();
        sim.top.A.fo_float64.print_on_use();
        sim.top.A.fo_duration.print_on_use();
        sim.top.A.fo_string.print_on_use();
        sim.top.B.fo_bool.print_on_use();
        sim.top.B.fo_int64.print_on_use();
        sim.top.B.fo_float64.print_on_use();
        sim.top.B.fo_duration.print_on_use();
        sim.top.B.fo_string.print_on_use();
        try {
            sim.process_remaining_events();
        }
        catch (const system_node::error& e) {
            std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "OTHER ERROR: " << e.what() << std::endl;
        }
    }
    std::cout << std::endl;
    {
        std::cout << "data_structures_composite_node" << std::endl;
        simulation<data_structures_composite_node> sim(10_s, 0, std::cout);
        sim.top.print_on_event();
        sim.top.A.print_on_event();
        sim.top.A.print_on_elapsed_duration();
        sim.top.A.print_on_planned_duration();
        sim.top.B.print_on_event();
        sim.top.B.print_on_elapsed_duration();
        sim.top.B.print_on_planned_duration();
        sim.top.A.mo_vector_bool.print_on_use();
        sim.top.A.mo_vector_int64.print_on_use();
        sim.top.A.mo_vector_vector_duration.print_on_use();
        sim.top.A.mo_map_duration_float64.print_on_use();
        sim.top.A.mo_set_pair_float64_string.print_on_use();
        sim.top.A.mo_rate_ptr.print_on_use();
        sim.top.A.mo_ostringstream_ptr.print_on_use();
        sim.top.A.mo_vector_array_ptr.print_on_use();
        sim.top.B.mo_vector_bool.print_on_use();
        sim.top.B.mo_vector_int64.print_on_use();
        sim.top.B.mo_vector_vector_duration.print_on_use();
        sim.top.B.mo_map_duration_float64.print_on_use();
        sim.top.B.mo_set_pair_float64_string.print_on_use();
        sim.top.B.mo_rate_ptr.print_on_use();
        sim.top.B.mo_ostringstream_ptr.print_on_use();
        sim.top.B.mo_vector_array_ptr.print_on_use();
        sim.top.A.fo_vector_bool.print_on_use();
        sim.top.A.fo_vector_int64.print_on_use();
        sim.top.A.fo_vector_vector_duration.print_on_use();
        sim.top.A.fo_map_duration_float64.print_on_use();
        sim.top.A.fo_set_pair_float64_string.print_on_use();
        sim.top.A.fo_rate_ptr.print_on_use();
        sim.top.A.fo_ostringstream_ptr.print_on_use();
        sim.top.A.fo_vector_array_ptr.print_on_use();
        sim.top.B.fo_vector_bool.print_on_use();
        sim.top.B.fo_vector_int64.print_on_use();
        sim.top.B.fo_vector_vector_duration.print_on_use();
        sim.top.B.fo_map_duration_float64.print_on_use();
        sim.top.B.fo_set_pair_float64_string.print_on_use();
        sim.top.B.fo_rate_ptr.print_on_use();
        sim.top.B.fo_ostringstream_ptr.print_on_use();
        sim.top.B.fo_vector_array_ptr.print_on_use();
        try {
            sim.process_remaining_events();
        }
        catch (const system_node::error& e) {
            std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "OTHER ERROR: " << e.what() << std::endl;
        }
    }
    std::cout << std::endl;
    {
        std::cout << "data_arraynds_composite_node" << std::endl;
        simulation<data_arraynds_composite_node> sim(10_s, 0, std::cout);
        sim.top.print_on_event();
        sim.top.A.print_on_event();
        sim.top.A.print_on_elapsed_duration();
        sim.top.A.print_on_planned_duration();
        sim.top.B.print_on_event();
        sim.top.B.print_on_elapsed_duration();
        sim.top.B.print_on_planned_duration();
        sim.top.A.mo_a2d.print_on_use();
        sim.top.A.mo_s1d.print_on_use();
        sim.top.A.mo_a2d_ptr.print_on_use();
        sim.top.A.mo_s1d_ptr.print_on_use();
        sim.top.B.mo_a2d.print_on_use();
        sim.top.B.mo_s1d.print_on_use();
        sim.top.B.mo_a2d_ptr.print_on_use();
        sim.top.B.mo_s1d_ptr.print_on_use();
        sim.top.A.fo_a2d.print_on_use();
        sim.top.A.fo_s1d.print_on_use();
        sim.top.A.fo_a2d_ptr.print_on_use();
        sim.top.A.fo_s1d_ptr.print_on_use();
        sim.top.B.fo_a2d.print_on_use();
        sim.top.B.fo_s1d.print_on_use();
        sim.top.B.fo_a2d_ptr.print_on_use();
        sim.top.B.fo_s1d_ptr.print_on_use();
        try {
            sim.process_remaining_events();
        }
        catch (const system_node::error& e) {
            std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "OTHER ERROR: " << e.what() << std::endl;
        }
    }
    std::cout << std::endl;
    {
        std::cout << "data_tuples_composite_node" << std::endl;
        simulation<data_tuples_composite_node> sim(10_s, 0, std::cout);
        sim.top.print_on_event();
        sim.top.A.print_on_event();
        sim.top.A.print_on_elapsed_duration();
        sim.top.A.print_on_planned_duration();
        sim.top.B.print_on_event();
        sim.top.B.print_on_elapsed_duration();
        sim.top.B.print_on_planned_duration();
        sim.top.A.mo_tuple_int64.print_on_use();
        sim.top.A.mo_tuple_string_bool_float64.print_on_use();
        sim.top.A.mo_tuple_mass_speed_pair_bool_bool.print_on_use();
        sim.top.B.mo_tuple_int64.print_on_use();
        sim.top.B.mo_tuple_string_bool_float64.print_on_use();
        sim.top.B.mo_tuple_mass_speed_pair_bool_bool.print_on_use();
        sim.top.A.fo_tuple_int64.print_on_use();
        sim.top.A.fo_tuple_string_bool_float64.print_on_use();
        sim.top.A.fo_tuple_mass_speed_pair_bool_bool.print_on_use();
        sim.top.B.fo_tuple_int64.print_on_use();
        sim.top.B.fo_tuple_string_bool_float64.print_on_use();
        sim.top.B.fo_tuple_mass_speed_pair_bool_bool.print_on_use();
        try {
            sim.process_remaining_events();
        }
        catch (const system_node::error& e) {
            std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "OTHER ERROR: " << e.what() << std::endl;
        }
    }
}


}  // namespace

#endif
