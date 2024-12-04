#pragma once
#ifndef SYDEVS_EXAMPLES_BOUNCING_INTERACTIVE_SYSTEM_H_
#define SYDEVS_EXAMPLES_BOUNCING_INTERACTIVE_SYSTEM_H_

#include <examples/research/realtime/bouncing_ball_node.h>
#include <sydevs/systems/interactive_system.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class bouncing_ball_interactive_system : public interactive_system<int64, bouncing_ball_node, acceleration, distance>
{
public:
    bouncing_ball_interactive_system(const std::string& node_name, const node_context& external_context);
    virtual ~bouncing_ball_interactive_system() = default;

    // Attributes:
    virtual scale time_precision() const { return nano; }

private:
    // State Variables:
    distance x;
    velocity v;
    acceleration a;
    duration frame_dt;
    duration motion_dt;

    // Event Handlers:
    virtual duration macro_initialization_update(acceleration& injection);
    virtual void micro_planned_update(const int64& agent_id, duration elapsed_dt);
    virtual duration macro_planned_update(duration elapsed_dt, const acceleration& injection, distance& observation);
    virtual void macro_finalization_update(duration elapsed_dt);
};


inline bouncing_ball_interactive_system::bouncing_ball_interactive_system(const std::string& node_name, const node_context& external_context)
    : interactive_system<int64, bouncing_ball_node, acceleration, distance>(node_name, external_context)
{
}


inline duration bouncing_ball_interactive_system::macro_initialization_update(acceleration& injection)
{
    x = 0_m;
    v = 15000_mm/_s;
    a = -9810_mm/_s/_s;
    injection = -a;
    get(prototype.X0_input) = std::make_tuple(x, v, -a);
    create_agent(0);
    frame_dt = 100_ms;
    motion_dt = 0_s;
    return 0_s;
}


inline void bouncing_ball_interactive_system::micro_planned_update(const int64& agent_id, duration elapsed_dt)
{
    if (transmitted(prototype.X_output)) {
        std::tie(x, v, a) = get(prototype.X_output);
        motion_dt = 0_s;
    }
}


inline duration bouncing_ball_interactive_system::macro_planned_update(duration elapsed_dt, const acceleration& injection, distance& observation)
{
    motion_dt += elapsed_dt;
    observation = x + v*motion_dt + 0.5*a*motion_dt*motion_dt;
    if (injection != -a) {
        get(prototype.g_input) = injection;
        affect_agent(0);
    }
    return frame_dt;
}


inline void bouncing_ball_interactive_system::macro_finalization_update(duration elapsed_dt)
{
}


}  // namespace

#endif
