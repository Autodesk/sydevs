#pragma once
#ifndef SYDEVS_SYSTEMS_STATISTIC_NODE_H_
#define SYDEVS_SYSTEMS_STATISTIC_NODE_H_

#include <sydevs/systems/function_node.h>

namespace sydevs {
namespace systems {


/**
 * @brief A class template for nodes which accept statistic values.
 *
 * @details
 * The `statistic_node` class template is instantiated by classes that 
 * accept a single value as a flow input. The value is accessible by the 
 * `value` member function.
 */
template<typename T>
class statistic_node : public function_node
{
public:
    /**
     * @brief Constructs a `statistic_node`.
     * 
     * @details
     * The statistic node is created along with its associated `node_interface`
     * object.
     *
     * @param node_name The name of the node within the encompassing context.
     * @param external_context The context in which the node is constructed.
     *
     * @tparam T The type of the statistic value.
     */
    statistic_node(const std::string& node_name, const node_context& external_context);

    port<flow, input, T> statistic;  ///< The flow input port on which the statistic value is obtained.

    const T& value() const;  ///< Returns the statistic value.

private:
    pointer val_;

    virtual void flow_event();
};


template<typename T>
inline statistic_node<T>::statistic_node(const std::string& node_name, const node_context& external_context)
    : function_node(node_name, external_context)
    , statistic("statistic", external_interface())
    , val_()
{
}


template<typename T>
inline const T& statistic_node<T>::value() const
{
    return val_.dereference<T>();
}


template<typename T>
inline void statistic_node<T>::flow_event()
{
    val_ = qualified_type<T>::copy(statistic.value());
}


}  // namespace
}  // namespace

#endif
