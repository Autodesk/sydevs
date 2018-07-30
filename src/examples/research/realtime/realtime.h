#pragma once
#ifndef SYDEVS_EXAMPLES_REALTIME_H_
#define SYDEVS_EXAMPLES_REALTIME_H_

#include <examples/research/realtime/bouncing_ball_interactive_system.h>
#include <sydevs/systems/real_time_simulation.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


void realtime()
{
    try {
        real_time_simulation<bouncing_ball_interactive_system> sim(duration::inf(), 0, std::cout);
        sim.update_time_advancement_rate(1);
        sim.update_time_advancement_depth(1);
        while (true) {
            int64 event_count = sim.process_frame_if_time_reached();
            if (event_count > 0) {
                distance x = sim.observation();
                int64 pos = int64(x/250_mm + 0.5);
                int64 flight_chars = std::min(pos, int64(48));
                int64 ball_chars = (flight_chars < 48 ? 1 : 0);
                int64 blank_chars = 48 - flight_chars - ball_chars;
                auto animation_str = std::string(flight_chars, '-') + 
                                     std::string(ball_chars, '0') +
                                     std::string(blank_chars, ' ') + "|";
                auto t = sim.frame_time().gap(time_point());
                auto t_str = (string_builder() << t).str();
                std::cout << animation_str << " " << t_str << std::endl;
            }
        }
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
