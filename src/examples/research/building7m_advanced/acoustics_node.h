#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_ACOUSTICS_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_ACOUSTICS_NODE_H_

#include <examples/research/building7m_advanced/building_occupant_ids.h>
#include <examples/research/building7m_advanced/building_layout_codes.h>
#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class acoustics_node : public atomic_node
{
public:
    // Constructor/Destructor:
    acoustics_node(const std::string& node_name, const node_context& external_context);
    virtual ~acoustics_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, std::pair<array2d<int64>, distance>> building_layout_input;
    port<message, input, std::tuple<occupant_id, array1d<int64>, quantity<decltype(_g/_m/_s/_s)>>> sound_source_input;
    port<message, output, array2d<quantity<decltype(_g/_m/_s/_s)>>> sound_field_output;

protected:
    // State Variables:
    array2d<int64> L;                                                                      // building layout
    int64 nx;                                                                              // number of cells in the x dimension
    int64 ny;                                                                              // number of cells in the y dimension
    float64 wall_R;                                                                        // wall resistance
    array2d<quantity<decltype(_g/_m/_s/_s)>> SF;                                           // sound field
    std::map<occupant_id, std::pair<array1d<int64>, quantity<decltype(_g/_m/_s/_s)>>> SS;  // sound sources
    duration step_dt;                                                                      // time step
    duration planned_dt;                                                                   // planned duration

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline acoustics_node::acoustics_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , building_layout_input("building_layout_input", external_interface())
    , sound_source_input("sound_source_input", external_interface())
    , sound_field_output("sound_field_output", external_interface())
{
}


inline duration acoustics_node::initialization_event()
{
    L = building_layout_input.value().first;
    nx = L.dims()[0];
    ny = L.dims()[1];
    wall_R = 5.0;
    SF = array2d<quantity<decltype(_g/_m/_s/_s)>>({nx, ny}, 0_g/_m/_s/_s);
    SS = std::map<occupant_id, std::pair<array1d<int64>, quantity<decltype(_g/_m/_s/_s)>>>();
    step_dt = 50_ms;
    planned_dt = 0_s;
    return planned_dt;
}


inline duration acoustics_node::unplanned_event(duration elapsed_dt)
{
    if (sound_source_input.received()) {
        const std::tuple<occupant_id, array1d<int64>, quantity<decltype(_g/_m/_s/_s)>>& sound_source = sound_source_input.value();
        const auto& occ_id = std::get<0>(sound_source);
        const auto& pos = std::get<1>(sound_source);
        const auto& P = std::get<2>(sound_source);
        SS[occ_id] = std::make_pair(pos, P);
    }
    planned_dt -= elapsed_dt;
    return planned_dt;
}


inline duration acoustics_node::planned_event(duration elapsed_dt)
{
    // Add pressure from point sources
    for (const auto& sound_source : SS) {
        const auto& pos = sound_source.second.first;
        const auto& P = sound_source.second.second;
        SF(pos) += P;
    }
    SS.clear();

    // Calculate the new sound of each non-outdoor, non-border cell.
    array2d<quantity<decltype(_g/_m/_s/_s)>> prev_SF = SF.copy();
    for (int64 ix = 1; ix < nx - 1; ++ix) {
        for (int64 iy = 1; iy < ny - 1; ++iy) {
            if (L(ix, iy) != outdoor_code) {
                // The cell is not an outdoor cell.

                // Obtain thermal resistances in neighborhood.
                float64 r__ = (L(ix, iy) == wall_code ? wall_R : 0.0);
                float64 r0_ = (L(ix - 1, iy) == wall_code ? wall_R : 0.0);
                float64 r1_ = (L(ix + 1, iy) == wall_code ? wall_R : 0.0);
                float64 r_0 = (L(ix, iy - 1) == wall_code ? wall_R : 0.0);
                float64 r_1 = (L(ix, iy + 1) == wall_code ? wall_R : 0.0);

                // Calculate diffusion coefficients.
                // (Note: This is not actually based on physics.)
                float64 c0_ = (1.0/4.0)*pow(2, -(r__ + r0_));
                float64 c1_ = (1.0/4.0)*pow(2, -(r__ + r1_));
                float64 c_0 = (1.0/4.0)*pow(2, -(r__ + r_0));
                float64 c_1 = (1.0/4.0)*pow(2, -(r__ + r_1));
                float64 c__ = 1.0 - c0_ - c1_ -  c_0 - c_1;

                // Apply diffusion to obtain new cell sound.
                SF(ix, iy) = 
                    c__*prev_SF(ix, iy) + 
                    c0_*prev_SF(ix - 1, iy) + 
                    c1_*prev_SF(ix + 1, iy) +
                    c_0*prev_SF(ix, iy - 1) + 
                    c_1*prev_SF(ix, iy + 1);
            }
        }
    }

    sound_field_output.send(SF);
    planned_dt = step_dt;
    return planned_dt;
}


inline void acoustics_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
