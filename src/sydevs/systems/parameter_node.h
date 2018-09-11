#pragma once
#ifndef SYDEVS_SYSTEMS_PARAMETER_NODE_H_
#define SYDEVS_SYSTEMS_PARAMETER_NODE_H_

#include <sydevs/systems/function_node.h>

namespace sydevs {
namespace systems {


/**
 * @brief A class template for nodes which supply parameter values.
 *
 * @details
 * The `parameter_node` class template is instantiated by classes that 
 * provide a single value as a flow output. The value is specified on
 * construction or by the `set_value` member function.
 */
template<typename T>
class parameter_node : public function_node
{
public:
    /**
     * @brief Constructs a `parameter_node`.
     * 
     * @details
     * The parameter node is created along with its associated `node_interface`
     * object.
     *
     * @param node_name The name of the node within the encompassing context.
     * @param external_context The context in which the node is constructed.
     *
     * @tparam T The type of the parameter value.
     */
    parameter_node(const std::string& node_name, const node_context& external_context);

    /**
     * @brief Constructs a `parameter_node` with a parameter value.
     * 
     * @details
     * The parameter node is created along with its associated `node_interface`
     * object.
     *
     * @param node_name The name of the node within the encompassing context.
     * @param external_context The context in which the node is constructed.
     * @param X The parameter value.
     *
     * @tparam T The type of the parameter value.
     */
    parameter_node(const std::string& node_name, const node_context& external_context, const T& X);

    port<flow, output, T> parameter;  ///< The flow output port on which the parameter value is provided.

    const T& value() const;      ///< Returns the parameter value.
    void set_value(const T& X);  ///< Sets the parameter value to `X`.

private:
    pointer val_;

    virtual void flow_event();
};


template<typename T>
inline parameter_node<T>::parameter_node(const std::string& node_name, const node_context& external_context)
    : function_node(node_name, external_context)
    , parameter("parameter", external_interface())
    , val_()
{
}


template<typename T>
inline parameter_node<T>::parameter_node(const std::string& node_name, const node_context& external_context, const T& X)
    : function_node(node_name, external_context)
    , parameter("parameter", external_interface())
    , val_(qualified_type<T>::copy(X))
{
}


template<typename T>
inline const T& parameter_node<T>::value() const
{
    if (!val_) throw std::logic_error("No value provided for parameter node (" + full_name() + ")");
    return val_.dereference<T>();
}


template<typename T>
inline void parameter_node<T>::set_value(const T& X)
{
    val_ = qualified_type<T>::copy(X);
}

    
template<typename T>
inline void parameter_node<T>::flow_event()
{
    parameter.assign(value());
}


}  // namespace
}  // namespace

#endif
