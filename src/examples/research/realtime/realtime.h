#pragma once
#ifndef SYDEVS_EXAMPLES_REALTIME_H_
#define SYDEVS_EXAMPLES_REALTIME_H_

#include <examples/research/realtime/bouncing_ball_interactive_system.h>
#include <sydevs/systems/real_time_simulation.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class realtime {
public:
    realtime();

    void mainloop();

private:
    void observation_phase();
    void interaction_phase();

    void print_header();
    void print_footer();
    void print_frame(distance x);
    void print_menu();

    real_time_simulation<bouncing_ball_interactive_system> sim_;
    clock_time clock_t0_;
    duration t_;
    bool high_g_;
    bool fast_as_possible_;
    float64 t_syn_rate_;
};


}  // namespace

#endif
