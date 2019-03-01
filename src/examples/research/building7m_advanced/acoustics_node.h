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
    port<flow, input, std::pair<array2d<int64>, std::pair<distance, distance>>> building_layout_input;
    port<flow, input, float64> wall_sound_absorption_input;
    port<flow, input, float64> floor_sound_absorption_input;
    port<flow, input, float64> ceiling_sound_absorption_input;
    port<message, input, std::tuple<occupant_id, array1d<int64>, quantity<decltype(_g/_m/_s/_s)>>> sound_source_input;
    port<message, output, array2d<quantity<decltype(_g/_m/_s/_s)>>> sound_field_output;

protected:
    // State Variables:
    std::array<array1d<int64>, 4> directions;                                             // array of directions
    array2d<int64> L;                                                                     // building layout
    int64 nx;                                                                             // number of cells in the x dimension
    int64 ny;                                                                             // number of cells in the y dimension
    distance d;                                                                           // distance between cells
    distance h;                                                                           // height of ceiling
    float64 wall_alpha;                                                                   // wall sound absorption coefficient
    float64 floor_alpha;                                                                  // floor sound absorption coefficient
    float64 ceiling_alpha;                                                                // ceiling sound absorption coefficient
    array2d<quantity<decltype(_g/_m/_s/_s)>> SF;                                          // sound field
    std::map<occupant_id, std::pair<array1d<int64>, quantity<decltype(_g/_m/_s/_s)>>> S;  // sound sources
    array2d<std::array<float64, 4>> TLM;                                                  // transmission line matrix grid
    duration step_dt;                                                                     // time step
    int64 inner_step_count;                                                               // number of TLM iterations per time step
    duration planned_dt;                                                                  // planned duration

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline acoustics_node::acoustics_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , building_layout_input("building_layout_input", external_interface())
    , wall_sound_absorption_input("wall_sound_absorption_input", external_interface())
    , floor_sound_absorption_input("floor_sound_absorption_input", external_interface())
    , ceiling_sound_absorption_input("ceiling_sound_absorption_input", external_interface())
    , sound_source_input("sound_source_input", external_interface())
    , sound_field_output("sound_field_output", external_interface())
{
}


inline duration acoustics_node::initialization_event()
{
    directions[0] = array1d<int64>({2}, {1, 0});
    directions[1] = array1d<int64>({2}, {0, 1});
    directions[2] = array1d<int64>({2}, {-1, 0});
    directions[3] = array1d<int64>({2}, {0, -1});

    L = building_layout_input.value().first;

    nx = L.dims()[0];
    ny = L.dims()[1];
    d = building_layout_input.value().second.first;
    h = building_layout_input.value().second.second;
    wall_alpha = wall_sound_absorption_input.value();
    floor_alpha = floor_sound_absorption_input.value();
    ceiling_alpha = ceiling_sound_absorption_input.value();
    SF = array2d<quantity<decltype(_g/_m/_s/_s)>>({nx, ny}, 0_g/_m/_s/_s);
    S = std::map<occupant_id, std::pair<array1d<int64>, quantity<decltype(_g/_m/_s/_s)>>>();
    TLM = array2d<std::array<float64, 4>>({nx, ny}, {0.0, 0.0, 0.0, 0.0});
    step_dt = 100_ms;
    inner_step_count = 5;
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
        S[occ_id] = std::make_pair(pos, P);
    }
    planned_dt -= elapsed_dt;
    return planned_dt;
}


inline duration acoustics_node::planned_event(duration elapsed_dt)
{
    // Add pressure from point sources
    for (const auto& sound_source : S) {
        const auto& pos = sound_source.second.first;
        const auto& P = sound_source.second.second;
        float64 dPx = 0.25*(P/(1_g/_m/_s/_s));
        for (auto& Px : TLM(pos)) {
            Px += dPx;
        }
    }
    S.clear();

    // Calculate the sound field
    SF.fill(0_g/_m/_s/_s);
    for (int64 inner_step = 0; inner_step < inner_step_count; ++inner_step) {

        // Update the TLM pressure values
        auto next_TLM = array2d<std::array<float64, 4>>({ nx, ny }, { 0.0, 0.0, 0.0, 0.0 });
        float64 wall_G = 1.0 - wall_alpha;
        float64 walkable_G = std::max(0.0, 1.0 - (floor_alpha + ceiling_alpha)*d/(2.0*h));
        for (int64 ix = 0; ix < nx; ++ix) {
            for (int64 iy = 0; iy < ny; ++iy) {
                array1d<int64> pos({ 2 }, { ix, iy });
                auto& Pxs = TLM(pos);
                for (int64 idir = 0; idir < 4; ++idir) {
                    auto dir = directions[idir];
                    auto nbr_pos = pos + dir;
                    if ((nbr_pos(0) >= 0) && (nbr_pos(0) < nx) && (nbr_pos(1) >= 0) && (nbr_pos(1) < ny)) {
                        if (L(nbr_pos) == wall_code) {
                            auto& next_Pxs = next_TLM(pos);
                            auto dPx = wall_G*Pxs[idir]/2.0;
                            next_Pxs[idir] -= dPx;
                            next_Pxs[(idir + 1)%4] += dPx;
                            next_Pxs[(idir + 2)%4] += dPx;
                            next_Pxs[(idir + 3)%4] += dPx;
                        }
                        else {
                            auto& next_Pxs = next_TLM(nbr_pos);
                            auto dPx = walkable_G*Pxs[idir]/2.0;
                            next_Pxs[idir] += dPx;
                            next_Pxs[(idir + 1)%4] += dPx;
                            next_Pxs[(idir + 2)%4] -= dPx;
                            next_Pxs[(idir + 3)%4] += dPx;
                        }
                    }
                }
            }
        }
        TLM = std::move(next_TLM);

        // Update the sound field.
        double scale = 1.0/inner_step_count;
        for (int64 ix = 0; ix < nx; ++ix) {
            for (int64 iy = 0; iy < ny; ++iy) {
                const auto& Pxs = TLM(ix, iy);
                auto Px = std::abs(Pxs[0]) + std::abs(Pxs[1]) + std::abs(Pxs[2]) + std::abs(Pxs[3]);
                SF(ix, iy) += scale*Px*(1_g/_m/_s/_s);
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
