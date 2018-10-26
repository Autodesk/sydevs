#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_WEATHER_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_WEATHER_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class weather_node : public atomic_node
{
public:
    // Constructor/Destructor:
    weather_node(const std::string& node_name, const node_context& external_context);
    virtual ~weather_node() = default;

    // Attributes:
    virtual scale time_precision() const { return unit; }

    // Ports:
    port<flow, input, thermodynamic_temperature> outdoor_mean_temperature_input;
    port<flow, input, duration> outdoor_temperature_period_input;
    port<flow, input, duration> outdoor_temperature_time_step_input;
    port<flow, input, thermodynamic_temperature> initial_temperature_input;
    port<flow, input, quantity<decltype(_K/_s)>> initial_temperature_rate_input;
    port<message, output, thermodynamic_temperature> outdoor_temperature_output;

protected:
    // State Variables:
    thermodynamic_temperature mean_T;            // mean outdoor temperature
    duration period_dt;                          // duration of full outdoor temperature cycle
    quantity<decltype(_1/_s/_s)> omega_squared;  // angular frequency of outdoor temperature fluctuation, squared
    duration step_dt;                            // time step
    thermodynamic_temperature T0;                // initial outdoor temperature
    quantity<decltype(_K/_s)> rate0;             // initial rate of change in outdoor temperature
    thermodynamic_temperature T;                 // outdoor temperature
    quantity<decltype(_K/_s)> rate;              // rate of change in outdoor temperature

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline weather_node::weather_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , outdoor_mean_temperature_input("outdoor_mean_temperature_input", external_interface())
    , outdoor_temperature_period_input("outdoor_temperature_period_input", external_interface())
    , outdoor_temperature_time_step_input("outdoor_temperature_time_step_input", external_interface())
    , initial_temperature_input("initial_temperature_input", external_interface())
    , initial_temperature_rate_input("initial_temperature_rate_input", external_interface())
    , outdoor_temperature_output("outdoor_temperature_output", external_interface())
{
}


inline duration weather_node::initialization_event()
{
    mean_T = outdoor_mean_temperature_input.value();
    period_dt = outdoor_temperature_period_input.value();
    auto omega = 2*pi/period_dt;
    omega_squared = omega*omega;
    step_dt = outdoor_temperature_time_step_input.value().fixed_at(time_precision());
    T0 = initial_temperature_input.value();
    rate0 = initial_temperature_rate_input.value();
    T = T0;
    rate = rate0;
    return step_dt;
}


inline duration weather_node::unplanned_event(duration elapsed_dt)
{
    return duration();
}


inline duration weather_node::planned_event(duration elapsed_dt)
{
    T += rate*step_dt;
    rate -= omega_squared*(T - mean_T)*step_dt;

    outdoor_temperature_output.send(T);
    return step_dt;
}


inline void weather_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
