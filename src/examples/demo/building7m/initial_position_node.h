#pragma once
#ifndef SYDEVS_EXAMPLES_INITIAL_POSITION_NODE_H_
#define SYDEVS_EXAMPLES_INITIAL_POSITION_NODE_H_

#include <sydevs/systems/function_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class initial_position_node : public function_node
{
public:
    // Constructor/Destructor:
    initial_position_node(const std::string& node_name, const node_context& external_context);
    virtual ~initial_position_node() = default;

    // Ports:
    port<flow, input, std::pair<array2d<int64>, distance>> building_layout_input;
    port<flow, output, array1d<int64>> initial_position_output;

private:
    // Event Handlers:
    virtual void flow_event();
};


inline initial_position_node::initial_position_node(const std::string& node_name, const node_context& external_context)
    : function_node(node_name, external_context)
    , building_layout_input("building_layout_input", external_interface())
    , initial_position_output("initial_position_output", external_interface())
{
}


inline void initial_position_node::flow_event()
{
    const auto& [L, d] = building_layout_input.value();
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
    initial_position_output.assign(pos);
}


}  // namespace

#endif
