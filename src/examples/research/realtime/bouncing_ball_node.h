#pragma once
#ifndef SYDEVS_EXAMPLES_BOUNCING_BALL_NODE_H_
#define SYDEVS_EXAMPLES_BOUNCING_BALL_NODE_H_

#include <examples/research/realtime/motion_physics_types.h>
#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class bouncing_ball_node : public atomic_node
{
public:
    // Constructor/Destructor:
    bouncing_ball_node(const std::string& node_name, const node_context& external_context);
    virtual ~bouncing_ball_node() = default;

    // Attributes:
    virtual scale time_precision() const { return nano; }

    // Ports:
    port<flow, input, trajectory> X0_input;      // initial trajectory
    port<message, input, acceleration> g_input;  // new gravitational acceleration
    port<message, output, trajectory> X_output;  // current trajectory

protected:
    // State Variables:
    distance y;          // height
    velocity v;          // speed
    acceleration g;      // gravitational acceleration (e.g. 9.81_m/_s/_s)
    duration bounce_dt;  // time until next bounce (at y = 0)
    bool midair_change;  // true if trajectory changing prior to bouncing

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline bouncing_ball_node::bouncing_ball_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , X0_input("X0_input", external_interface())
    , g_input("g_input", external_interface())
    , X_output("X_output", external_interface())
{
}


inline duration bouncing_ball_node::initialization_event()
{
    const auto& [y, v, g] = X0_input.value();
    if (y < 0_m) throw std::invalid_argument("y must be non-negative");
    if (g < 0_m/_s/_s) throw std::invalid_argument("g must be non-negative");
    if (y == 0_m && v > 0_m/_s) {
        bounce_dt = (2*v/g).fixed_at(time_precision());
    }
    else {
        auto d = float64((v*v + 2*y*g)/(_m*_m/_s/_s));
        bounce_dt = ((v + sqrt(d)*(1_m/_s))/g).fixed_at(time_precision());
    }
    midair_change = true;
    return 0_s;
}


inline duration bouncing_ball_node::unplanned_event(duration elapsed_dt)
{
    y += (v*elapsed_dt - 0.5*g*elapsed_dt*elapsed_dt);
    v -= g*elapsed_dt;
    if (g_input.received()) {
        g = g_input.value();
        auto d = float64((v*v + 2*y*g)/(_m*_m/_s/_s));
        bounce_dt = ((v + sqrt(d)*(1_m/_s))/g).fixed_at(time_precision());
        midair_change = true;
    }
    return 0_s;
}


inline duration bouncing_ball_node::planned_event(duration elapsed_dt)
{
    if (midair_change) {
        midair_change = false;
    }
    else {
        if (y != 0_m || v < 0_m/_s) {
            v = -(v - g*elapsed_dt);
        }
        y = 0_m;
        bounce_dt = (2*v/g).fixed_at(time_precision());
    }
    X_output.send(std::make_tuple(y, v, -g));
    return bounce_dt;
}


inline void bouncing_ball_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
