#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_INITIAL_POSITION_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_INITIAL_POSITION_NODE_H_

#include <examples/demo/building7m_advanced/building_occupant_ids.h>
#include <sydevs/systems/function_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class initial_positions_node : public function_node
{
public:
    // Constructor/Destructor:
    initial_positions_node(const std::string& node_name, const node_context& external_context);
    virtual ~initial_positions_node() = default;

    // Ports:
    port<flow, input, std::pair<array2d<int64>, distance>> building_layout_input;
    port<flow, output, std::map<occupant_id, array1d<int64>>> initial_positions_output;

private:
    // Event Handlers:
    void flow_event();
};


inline initial_positions_node::initial_positions_node(const std::string& node_name, const node_context& external_context)
    : function_node(node_name, external_context)
    , building_layout_input("building_layout_input", external_interface())
    , initial_positions_output("initial_positions_output", external_interface())
{
}


inline void initial_positions_node::flow_event()
{
    array2d<int64> L = building_layout_input.value().first;
    int64 nx = L.dims()[0];
    int64 ny = L.dims()[1];
    array1d<int64> pos({2}, {0, 0});
    bool done = false;
    for (int64 iy = 0; !done && iy < ny; ++iy) {
        for (int64 ix = 0; !done && ix < nx; ++ix) {
            if (L(ix, iy) == 0) {
                // The first indoor grid cell has been found.
                pos(0) = ix;
                pos(1) = iy;
                done = true;
            }
        }
    }
    if (!done) {
        throw std::domain_error("The building has no indoor space.");
    }
    std::map<occupant_id, array1d<int64>> positions;
    positions[occupant_id(0)] = pos;
    initial_positions_output.assign(positions);
}


}  // namespace

#endif
