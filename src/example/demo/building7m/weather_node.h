#pragma once
#ifndef WEATHER_NODE_H_
#define WEATHER_NODE_H_

#include <sydevs/systems/atomic_node.h>

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
    port<flow, input, thermodynamic_temperature> initial_temperature_input;
    port<message, output, thermodynamic_temperature> outdoor_temperature_output;

protected:
    // State Variables:
    thermodynamic_temperature T0;
    thermodynamic_temperature T;
    quantity<decltype(_K/_s)> rate;
    quantity<decltype(_1/_s)> omega;
    duration dt;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline weather_node::weather_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , initial_temperature_input("initial_temperature_input", external_interface())
    , outdoor_temperature_output("outdoor_temperature_output", external_interface())
{
}


inline duration weather_node::initialization_event()
{
    constexpr float64 pi = 3.14159265358979323846;
    T0 = initial_temperature_input.value();
    T = T0.fixed_at(milli);

    rate = 200_mK/_s;    // initial rate of change in outdoor temperature
    omega = 2*pi/5_min;  // angular frequency of outdoor temperature fluctuation
    dt = 15_s;           // time step

    return dt;
}


inline duration weather_node::unplanned_event(duration elapsed_dt)
{
    return duration();
}


inline duration weather_node::planned_event(duration elapsed_dt)
{
    T += rate*dt;                     // numerical integration
    rate -= omega*omega*(T - T0)*dt;  // basic differential equation for sinewaves.

    outdoor_temperature_output.send(T);
    return dt;
}


inline void weather_node::finalization_event(duration elapsed_dt)
{
}


#endif
