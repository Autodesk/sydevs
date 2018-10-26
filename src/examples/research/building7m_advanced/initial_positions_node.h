#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_INITIAL_POSITION_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_INITIAL_POSITION_NODE_H_

#include <examples/research/building7m_advanced/building_occupant_ids.h>
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
    port<flow, input, int64> occupant_count_input;
    port<flow, output, std::map<occupant_id, array1d<int64>>> initial_positions_output;

private:
    // Event Handlers:
    void flow_event();
};


inline initial_positions_node::initial_positions_node(const std::string& node_name, const node_context& external_context)
    : function_node(node_name, external_context)
    , building_layout_input("building_layout_input", external_interface())
    , occupant_count_input("occupant_count_input", external_interface())
    , initial_positions_output("initial_positions_output", external_interface())
{
}


inline void initial_positions_node::flow_event()
{
    array2d<int64> L = building_layout_input.value().first;
    int64 nx = L.dims()[0];
    int64 ny = L.dims()[1];
    int64 N = occupant_count_input.value();
    std::map<occupant_id, array1d<int64>> positions;
    bool done = (int64(positions.size()) == N);
    for (int64 iy = 0; !done && iy < ny; ++iy) {
        for (int64 ix = 0; !done && ix < nx; ++ix) {
            if (L(ix, iy) == 0) {
                positions[occupant_id(positions.size())] = array1d<int64>({2}, {ix, iy});
                done = (int64(positions.size()) == N);
            }
        }
    }
    if (!done) {
        throw std::domain_error("The building has insufficient indoor space.");
    }
    initial_positions_output.assign(positions);
}


}  // namespace

#endif
