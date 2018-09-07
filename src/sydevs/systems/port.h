#pragma once
#ifndef SYDEVS_SYSTEMS_PORT_H_
#define SYDEVS_SYSTEMS_PORT_H_

#include <sydevs/systems/node_interface.h>

namespace sydevs {
namespace systems {


/**
 * @brief A base class template for all port classes.
 *
 * @details
 * `port_base` contains indices and references common to all types of ports.
 */
template<data_mode dmode, data_goal dgoal, typename T>
class port_base
{
public:
    port_base(const port_base&)            = delete;   ///< No copy constructor
    port_base& operator=(const port_base&) = delete;   ///< No copy assignment
    port_base(port_base&&)                 = default;  ///< Move constructor
    port_base& operator=(port_base&&)      = default;  ///< Move assignment
    virtual ~port_base()                   = default;  ///< Destructor

    const std::string& port_name() const;  ///< Returns the name of the port.

    int64 port_index() const;  ///< Returns the index of the port within the encompassing `node_interface` object.
    int64 node_index() const;  ///< Returns the index of the node within the encompassing `node_structure` object.

    const node_interface& external_interface() const;  ///< Returns the encomassing `node_interface` object.

protected:
    port_base(const std::string& port_name, int64 port_index, node_interface& external_interface);

private: 
    const std::string port_name_;
    const int64 port_index_;
    node_interface& external_interface_;
};


/**
 * @brief A generic port class template declaration.
 *
 * @details
 * `port<dmode, dgoal>` will never be instantiated, since all four possible
 * combinations of `dmode` and `dgoal` values have associated template
 * specializations.
 */
template<data_mode dmode, data_goal dgoal, typename T>
class port : public port_base<dmode, dgoal, T>
{
};


/**
 * @brief A class template for flow input ports.
 *
 * @details
 * A `port<flow, input>` object is used to supply a `system_node` object with
 * a parameter at the beginning of a simulation.
 *
 * @tparam T The type of data handled by the port.
 */
template<typename T>
class port<flow, input, T> : public port_base<flow, input, T>
{
public:
    /**
     * @brief Constructs a `port<flow, input>` object.
     * 
     * @details
     * The port is created and registered with the encompassing
     * `node_interface` object.
     *
     * @param port_name The name of the port.
     * @param external_interface The encompassing `node_interface` object
     *                           associated with the node.
     *
     * @tparam T The type of data handled by the port.
     */
    port(const std::string& port_name, const node_interface& external_interface);

    port(port&&)            = default;   ///< Move constructor
    port& operator=(port&&) = default;   ///< Move assignment
    ~port()                 = default;   ///< Destructor

    const T& value() const;  ///< Returns the current value attached to the port.

    void print_on_use(bool flag = true) const;  ///< If `flag` is `true`, all values communicated through this port are printed.
};


/**
 * @brief A class template for message input ports.
 *
 * @details
 * A `port<message, input>` object is used to supply a `system_node` object
 * with messages during a simulation.
 *
 * @tparam T The type of data handled by the port.
 */
template<typename T>
class port<message, input, T> : public port_base<message, input, T>
{
public:
    /**
     * @brief Constructs a `port<message, input>` object.
     * 
     * @details
     * The port is created and registered with the encompassing
     * `node_interface` object.
     *
     * @param port_name The name of the port.
     * @param external_interface The encompassing `node_interface` object
     *                           associated with the node.
     *
     * @tparam T The type of data handled by the port.
     */
    port(const std::string& port_name, const node_interface& external_interface);

    port(port&&)            = default;   ///< Move constructor
    port& operator=(port&&) = default;   ///< Move assignment
    ~port()                 = default;   ///< Destructor

    bool received() const;   ///< Returns `true` if the port was the receipient of the latest incoming message.
    const T& value() const;  ///< Returns the current value attached to the port.

    void print_on_use(bool flag = true) const;  ///< If `flag` is `true`, all values communicated through this port are printed.
};


/**
 * @brief A class template for message output ports.
 *
 * @details
 * A `port<message, output>` object is used to communicate messages from a
 * `system_node` object during a simulation.
 *
 * @tparam T The type of data handled by the port.
 */
template<typename T>
class port<message, output, T> : public port_base<message, output, T>
{
public:
    /**
     * @brief Constructs a `port<message, output>` object.
     * 
     * @details
     * The port is created and registered with the encompassing
     * `node_interface` object.
     *
     * @param port_name The name of the port.
     * @param external_interface The encompassing `node_interface` object
     *                           associated with the node.
     *
     * @tparam T The type of data handled by the port.
     */
    port(const std::string& port_name, const node_interface& external_interface);

    port(port&&)            = default;   ///< Move constructor
    port& operator=(port&&) = default;   ///< Move assignment
    ~port()                 = default;   ///< Destructor

    void send(const T& val);  ///< Sends the outgoing message `val` through this port.

    void print_on_use(bool flag = true) const;  ///< If `flag` is `true`, all values communicated through this port are printed.
};


/**
 * @brief A class template for flow output ports.
 *
 * @details
 * A `port<flow, output>` object is used to communicate a statistic from a
 * `system_node` object at the end of a simulation.
 *
 * @tparam T The type of data handled by the port.
 */
template<typename T>
class port<flow, output, T> : public port_base<flow, output, T>
{
public:
    /**
     * @brief Constructs a `port<flow, output>` object.
     * 
     * @details
     * The port is created and registered with the encompassing
     * `node_interface` object.
     *
     * @param port_name The name of the port.
     * @param external_interface The encompassing `node_interface` object
     *                           associated with the node.
     *
     * @tparam T The type of data handled by the port.
     */
    port(const std::string& port_name, const node_interface& external_interface);

    port(port&&)            = default;   ///< Move constructor
    port& operator=(port&&) = default;   ///< Move assignment
    ~port()                 = default;   ///< Destructor

    void assign(const T& val);  ///< Attaches the value `val` to the port.

    void print_on_use(bool flag = true) const;  ///< If `flag` is `true`, all values communicated through this port are printed.
};


template<data_mode dmode, data_goal dgoal, typename T>
const std::string& port_base<dmode, dgoal, T>::port_name() const
{
    return port_name_;
}

    
template<data_mode dmode, data_goal dgoal, typename T>
int64 port_base<dmode, dgoal, T>::port_index() const
{
    return port_index_;
}


template<data_mode dmode, data_goal dgoal, typename T>
int64 port_base<dmode, dgoal, T>::node_index() const
{
    return external_interface_.node_index();
}


template<data_mode dmode, data_goal dgoal, typename T>
const node_interface& port_base<dmode, dgoal, T>::external_interface() const
{
    return external_interface_;
}


template<data_mode dmode, data_goal dgoal, typename T>
port_base<dmode, dgoal, T>::port_base(const std::string& port_name, int64 port_index, node_interface& external_interface)
    : port_name_(port_name)
    , port_index_(port_index)
    , external_interface_(external_interface)
{
    if (!qualified_type<T>::valid) {
        const auto& full_node_name = const_cast<node_interface&>(this->external_interface()).full_name();
        throw std::logic_error("Invalid data type for port (" + this->port_name() + ") " +
                               "of node (" + full_node_name + ")");
    }
}


template<typename T>
port<flow, input, T>::port(const std::string& port_name, const node_interface& external_interface)
    : port_base<flow, input, T>(
        port_name,
        const_cast<node_interface&>(external_interface).add_flow_input_port(port_name), 
        const_cast<node_interface&>(external_interface))
{
}


template<typename T>
const T& port<flow, input, T>::value() const
{
    if (!this->external_interface().active()) {
        const auto& full_node_name = const_cast<node_interface&>(this->external_interface()).full_name();
        throw std::logic_error("Attempt to get value on flow input port (" + this->port_name() + ") " +
                               "of inactive node (" + full_node_name + ")");
    }
    const auto& val = const_cast<node_interface&>(this->external_interface()).flow_input_port_value(this->port_index());
    return const_cast<const T&>(val.template dereference<T>());
}


template<typename T>
void port<flow, input, T>::print_on_use(bool flag) const
{
    auto tostring_func = flag ? tostring_converter<T>() : nullptr;
    const_cast<node_interface&>(this->external_interface()).set_flow_input_printable(this->port_index(), tostring_func);
}

    
template<typename T>
port<message, input, T>::port(const std::string& port_name, const node_interface& external_interface)
    : port_base<message, input, T>(
        port_name,
        const_cast<node_interface&>(external_interface).add_message_input_port(port_name),
        const_cast<node_interface&>(external_interface))
{
}


template<typename T>
bool port<message, input, T>::received() const
{
    if (!this->external_interface().active()) {
        const auto& full_node_name = const_cast<node_interface&>(this->external_interface()).full_name();
        throw std::logic_error("Attempt to check message input port (" + this->port_name() + ") " +
                               "of inactive node (" + full_node_name + ")");
    }
    if (this->external_interface().dmode() != message || this->external_interface().dgoal() != input) {
        const auto& full_node_name = const_cast<node_interface&>(this->external_interface()).full_name();
        throw std::logic_error("Attempt to check message input port (" + this->port_name() + ") " +
                               "of node (" + full_node_name + ") outside of unplanned event");
    }
    return const_cast<node_interface&>(this->external_interface()).message_input_port_index() == this->port_index();
}


template<typename T>
const T& port<message, input, T>::value() const
{
    if (!this->external_interface().active()) {
        const auto& full_node_name = const_cast<node_interface&>(this->external_interface()).full_name();
        throw std::logic_error("Attempt to get value on message input port (" + this->port_name() + ") " +
                               "of inactive node (" + full_node_name + ")");
    }
    if (this->external_interface().dmode() != message || this->external_interface().dgoal() != input) {
        const auto& full_node_name = const_cast<node_interface&>(this->external_interface()).full_name();
        throw std::logic_error("Attempt to get value on message input port (" + this->port_name() + ") " +
                               "of node (" + full_node_name + ") outside of unplanned event");
    }
    const auto& val = const_cast<node_interface&>(this->external_interface()).message_input_port_value(this->port_index());
    return const_cast<const T&>(val.template dereference<T>());
}


template<typename T>
void port<message, input, T>::print_on_use(bool flag) const
{
    auto tostring_func = flag ? tostring_converter<T>() : nullptr;
    const_cast<node_interface&>(this->external_interface()).set_message_input_printable(this->port_index(), tostring_func);
}

    
template<typename T>
port<message, output, T>::port(const std::string& port_name, const node_interface& external_interface)
    : port_base<message, output, T>(
        port_name,
        const_cast<node_interface&>(external_interface).add_message_output_port(port_name),
        const_cast<node_interface&>(external_interface))
{
}


template<typename T>
void port<message, output, T>::send(const T& val)
{
    if (!this->external_interface().active()) {
        const auto& full_node_name = const_cast<node_interface&>(this->external_interface()).full_name();
        throw std::logic_error("Attempt to send value on message output port (" + this->port_name() + ") " +
                               "of inactive node (" + full_node_name + ")");
    }
    if (this->external_interface().dmode() != message || this->external_interface().dgoal() != output) {
        const auto& full_node_name = const_cast<node_interface&>(this->external_interface()).full_name();
        throw std::logic_error("Attempt to send value on message output port (" + this->port_name() + ") " +
                               "of node (" + full_node_name + ") outside of planned event");
    }
    const_cast<node_interface&>(this->external_interface()).append_message_output(this->port_index(), qualified_type<T>::copy(val));
}


template<typename T>
void port<message, output, T>::print_on_use(bool flag) const
{
    auto tostring_func = flag ? tostring_converter<T>() : nullptr;
    const_cast<node_interface&>(this->external_interface()).set_message_output_printable(this->port_index(), tostring_func);
}

    
template<typename T>
port<flow, output, T>::port(const std::string& port_name, const node_interface& external_interface)
    : port_base<flow, output, T>(
        port_name,
        const_cast<node_interface&>(external_interface).add_flow_output_port(port_name),
        const_cast<node_interface&>(external_interface))
{
}


template<typename T>
void port<flow, output, T>::assign(const T& val)
{
    if (!this->external_interface().active()) {
        const auto& full_node_name = const_cast<node_interface&>(this->external_interface()).full_name();
        throw std::logic_error("Attempt to assign value on flow output port (" + this->port_name() + ") " +
                               "of inactive node (" + full_node_name + ")");
    }
    if (this->external_interface().dmode() != flow) {
        const auto& full_node_name = const_cast<node_interface&>(this->external_interface()).full_name();
        throw std::logic_error("Attempt to assign value on flow output port (" + this->port_name() + ") " +
                               "of node (" + full_node_name + ") outside of initialization or finalization event");
    }
    const_cast<node_interface&>(this->external_interface()).assign_flow_output(this->port_index(), qualified_type<T>::copy(val));
}


template<typename T>
void port<flow, output, T>::print_on_use(bool flag) const
{
    auto tostring_func = flag ? tostring_converter<T>() : nullptr;
    const_cast<node_interface&>(this->external_interface()).set_flow_output_printable(this->port_index(), tostring_func);
}

    
}  // namespace
}  // namespace

#endif
