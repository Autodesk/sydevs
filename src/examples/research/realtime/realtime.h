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
        auto clock_t0 = clock_time();
        auto t = 0_s;
        bool high_g = false;
        while (true) {
            std::cout << std::endl;
            auto pause_t = t + 5_s;
            auto done = false;
            auto prev_prev_x = distance();
            auto prev_x = distance();
            while (!done) {
                int64 event_count = sim.process_frame_if_time_reached();
                if (event_count > 0) {
                    if (sim.frame_index() == 0) {
                        clock_t0 = sim.frame_clock_time();
                    }
                    distance x = sim.observation();
                    int64 pos = int64(x/250_mm + 0.5);
                    int64 flight_chars = std::min(pos, int64(48));
                    int64 ball_chars = (flight_chars < 48 ? 1 : 0);
                    int64 animation_padding = 48 - flight_chars - ball_chars;
                    auto animation_str = std::string(flight_chars, '-') + 
                                         std::string(ball_chars, '0') +
                                         std::string(animation_padding, ' ') + "|";
                    t = sim.frame_time().gap(time_point()).rescaled(milli);
                    auto t_str = (string_builder() << t).str();
                    int64 t_padding = std::max(int64(0), int64(10 - t_str.length()));
                    t_str = std::string(t_padding, ' ') + t_str;
                    auto clock_t = sim.frame_clock_time();
                    auto ct = duration(std::chrono::duration_cast<std::chrono::milliseconds>(clock_t - clock_t0).count(), milli);
                    auto ct_str = (string_builder() << ct).str();
                    int64 ct_padding = std::max(int64(0), int64(10 - ct_str.length()));
                    ct_str = std::string(ct_padding, ' ') + ct_str;
                    std::cout << animation_str << " " << t_str << " " << ct_str << std::endl;
                    if (t >= pause_t) {
                        done = ((x > prev_x) && (prev_x <= prev_prev_x));
                    }
                    prev_prev_x = prev_x;
                    prev_x = x;
                }
            }
            std::cout << std::endl;
            std::cout << "Commands:" << std::endl;
            std::cout << "    g   | toggle high gravity)" << std::endl;
            std::cout << "    0-9 | log2(ta_depth + 1)" << std::endl;
            std::cout << "        | (ta_depth = time advancement depth)" << std::endl;
            std::cout << "Enter a command sequence: ";
            char input_ch;
            std::cin >> input_ch;
            if (input_ch == 'g') {
                high_g = !high_g;
                if (high_g) {
                    sim.injection() = 19620_mm/_s/_s;
                }
                else {
                    sim.injection() = 9810_mm/_s/_s;
                }
            }
            else if (isdigit(input_ch)) {
                int64 ta_depth_input = input_ch - '0';
                int64 ta_depth = exp2(ta_depth_input) - 1;
                sim.update_time_advancement_depth(ta_depth);
            }
            std::cout << std::endl;
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
