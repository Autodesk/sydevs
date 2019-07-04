#include <examples/demo/workers/workers.h>

namespace sydevs_examples {


void workers()
{
    bool output_to_file = false;
    bool print_timing_stats = false;

    std::shared_ptr<std::fstream> os_ptr;
    if (output_to_file) {
        os_ptr = std::make_shared<std::fstream>();
        os_ptr->open("workers.csv", std::fstream::out);
    }

    try {
        timer sim_timer;
        sim_timer.start();
        simulation<workplace_closed_system> sim(7_hr, 0, std::cout);
        sim.top.worker_count.set_value(12);
        sim.top.arrival_dt.set_value(5_min);
        sim.top.work_dt.set_value(1_hr);
        sim.top.break_dt.set_value(10_min);
        sim.top.frame_dt.set_value(5_s);
        sim.top.ostream_ptr.set_value(os_ptr);
        sim.top.workplace.arrival_input.print_on_use();
        sim.top.workplace.change_output.print_on_use();
        sim.process_remaining_events();
        sim_timer.stop();
        if (print_timing_stats) {
            std::cout << std::endl;
            std::cout << "Timing Statistics" << std::endl;
            std::cout << "(overall simulation):         " << sim_timer.cumulative_duration() << std::endl;
            std::cout << "sim:                          " << sim.event_timer().cumulative_duration() << std::endl;
            std::cout << "sim.top:                      " << sim.top.event_timer().cumulative_duration() << std::endl;
            std::cout << "sim.top.worker_arrival:       " << sim.top.worker_arrival.event_timer().cumulative_duration() << std::endl;
            std::cout << "sim.top.workplace:            " << sim.top.workplace.event_timer().cumulative_duration() << std::endl;
            std::cout << "sim.top.workplace.prototype:  " << sim.top.workplace.prototype.event_timer().cumulative_duration() << std::endl;
            std::cout << "sim.top.workplace_csv_output: " << sim.top.workplace_csv_output.event_timer().cumulative_duration() << std::endl;
        }
    }
    catch (const system_node::error& e) {
        std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "OTHER ERROR: " << e.what() << std::endl;
    }

    if (output_to_file) {
        os_ptr->close();
    }
}


}  // namespace
