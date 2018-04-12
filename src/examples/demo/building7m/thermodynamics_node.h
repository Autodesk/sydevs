#pragma once
#ifndef SYDEVS_EXAMPLES_THERMODYNAMICS_NODE_H_
#define SYDEVS_EXAMPLES_THERMODYNAMICS_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class thermodynamics_node : public atomic_node
{
public:
    // Constructor/Destructor:
    thermodynamics_node(const std::string& node_name, const node_context& external_context);
    virtual ~thermodynamics_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, thermodynamic_temperature> initial_temperature_input;
    port<flow, input, std::pair<array2d<int64>, distance>> building_layout_input;
    port<flow, input, float64> wall_resistance_input;
    port<message, input, thermodynamic_temperature> outdoor_temperature_input;
    port<message, output, array2d<thermodynamic_temperature>> temperature_field_output;

protected:
    // State Variables:
    array2d<int64> L;                       // building layout
    int64 nx;                               // number of cells in the x dimension
    int64 ny;                               // number of cells in the y dimension
    float64 wall_R;                         // wall resistance
    array2d<thermodynamic_temperature> TF;  // temperature field
    duration step_dt;                       // time step
    duration planned_dt;                    // planned duration

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline thermodynamics_node::thermodynamics_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , initial_temperature_input("initial_temperature_input", external_interface())
    , building_layout_input("building_layout_input", external_interface())
    , wall_resistance_input("wall_resistance_input", external_interface())
    , outdoor_temperature_input("outdoor_temperature_input", external_interface())
    , temperature_field_output("temperature_field_output", external_interface())
{
}


inline duration thermodynamics_node::initialization_event()
{
    L = building_layout_input.value().first;
    nx = L.dims()[0];
    ny = L.dims()[1];
    wall_R = wall_resistance_input.value();
    thermodynamic_temperature T0 = initial_temperature_input.value();
    TF = array2d<thermodynamic_temperature>({nx, ny}, T0);
    step_dt = 250_ms;
    planned_dt = 0_s;
    return planned_dt;
}


inline duration thermodynamics_node::unplanned_event(duration elapsed_dt)
{
    if (outdoor_temperature_input.received()) {
        thermodynamic_temperature T = outdoor_temperature_input.value();
        TF = replace(TF, L == -1, T);
    }
    planned_dt -= elapsed_dt;
    return planned_dt;
}


inline duration thermodynamics_node::planned_event(duration elapsed_dt)
{
    // Calculate the new temperature of each non-outdoor, non-border cell.
    array2d<thermodynamic_temperature> prev_TF = TF.copy();
    for (int64 ix = 1; ix < nx - 1; ++ix) {
        for (int64 iy = 1; iy < ny - 1; ++iy) {
            if (L(ix, iy) >= 0) {
                // The cell is not an outdoor cell.

                // Obtain thermal resistances in neighborhood.
                float64 r__ = (L(ix, iy) == 1 ? wall_R : 0.0);
                float64 r0_ = (L(ix - 1, iy) == 1 ? wall_R : 0.0);
                float64 r1_ = (L(ix + 1, iy) == 1 ? wall_R : 0.0);
                float64 r_0 = (L(ix, iy - 1) == 1 ? wall_R : 0.0);
                float64 r_1 = (L(ix, iy + 1) == 1 ? wall_R : 0.0);

                // Calculate diffusion coefficients.
                // (Note: This is not actually based on physics.)
                float64 c0_ = (1.0/4.0)*pow(2, -(r__ + r0_));
                float64 c1_ = (1.0/4.0)*pow(2, -(r__ + r1_));
                float64 c_0 = (1.0/4.0)*pow(2, -(r__ + r_0));
                float64 c_1 = (1.0/4.0)*pow(2, -(r__ + r_1));
                float64 c__ = 1.0 - c0_ - c1_ -  c_0 - c_1;

                // Apply diffusion to obtain new cell temperature.
                TF(ix, iy) = 
                    c__*prev_TF(ix, iy) + 
                    c0_*prev_TF(ix - 1, iy) + 
                    c1_*prev_TF(ix + 1, iy) +
                    c_0*prev_TF(ix, iy - 1) + 
                    c_1*prev_TF(ix, iy + 1);
            }
        }
    }

    temperature_field_output.send(TF);
    planned_dt = step_dt;
    return planned_dt;
}


inline void thermodynamics_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
