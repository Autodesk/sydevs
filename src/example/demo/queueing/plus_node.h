#pragma once
#ifndef PLUS_NODE_H_
#define PLUS_NODE_H_

#include <sydevs/systems/function_node.h>

using namespace sydevs;
using namespace sydevs::systems;


/**
 * This node adds flow inputs "a" and "b" to produce flow output "c".
 */
template<typename T>
class plus_node : public function_node
{
public:
    // Constructor/Destructor:
    plus_node(const std::string& node_name, const node_context& external_context);
    virtual ~plus_node() = default;

    // Ports:
    port<flow, input, T> a_input;
    port<flow, input, T> b_input;
    port<flow, output, T> c_output;

private:
    // Event Handlers:
    void flow_event();
};


template<typename T>
inline plus_node<T>::plus_node(const std::string& node_name, const node_context& external_context)
    : function_node(node_name, external_context)
    , a_input("a_input", external_interface())
    , b_input("b_input", external_interface())
    , c_output("c_output", external_interface())
{
}


template<typename T>
inline void plus_node<T>::flow_event()
{
    // Get the two flow input values, add them together, and assign the result
    // to the flow output port.
    const T& a = a_input.value();
    const T& b = b_input.value();
    T c = a + b;
    c_output.assign(c);
}


#endif
