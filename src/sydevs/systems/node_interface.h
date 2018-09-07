#pragma once
#ifndef SYDEVS_SYSTEMS_NODE_INTERFACE_H_
#define SYDEVS_SYSTEMS_NODE_INTERFACE_H_

#include <sydevs/systems/node_context.h>
#include <sydevs/core/qualified_type.h>
#include <sydevs/systems/data_mode.h>
#include <sydevs/systems/data_goal.h>

namespace sydevs {
namespace systems {

class system_node;


class node_interface
{
public:
    node_interface(const std::string& node_name, system_node* node_ptr, const node_context& external_context);

    node_interface(const node_interface&)            = delete;   ///< No copy constructor
    node_interface& operator=(const node_interface&) = delete;   ///< No copy assignment
    node_interface(node_interface&&)                 = delete;   ///< No move constructor
    node_interface& operator=(node_interface&&)      = delete;   ///< No move assignment
    virtual ~node_interface()                        = default;  ///< Destructor

    const std::string& node_name();
    const std::string& full_name();
    int64 node_index();

    node_context& external_context();

    int64 add_flow_input_port(const std::string& port_name);
    int64 flow_input_port_count();
    const std::string& flow_input_port_name(int64 port_index);
    const pointer& flow_input_port_value(int64 port_index);
    void clear_flow_inputs();
    void assign_flow_input(int64 port_index, const pointer& val);
    tostring_function flow_input_tostring_func(int64 port_index);
    void set_flow_input_printable(int64 port_index, tostring_function tostring_func);

    int64 add_message_input_port(const std::string& port_name);
    int64 message_input_port_count();
    const std::string& message_input_port_name(int64 port_index);
    const pointer& message_input_port_value(int64 port_index);
    int64 message_input_port_index();
    void clear_message_input();
    void set_message_input(int64 port_index, const pointer& val);
    tostring_function message_input_tostring_func(int64 port_index);
    void set_message_input_printable(int64 port_index, tostring_function tostring_func);

    int64 add_message_output_port(const std::string& port_name);
    int64 message_output_port_count();
    const std::string& message_output_port_name(int64 port_index);
    int64 message_output_list_size();
    int64 message_output_index(int64 list_index);
    const pointer& message_output_value(int64 list_index);
    void clear_message_outputs();
    void append_message_output(int64 port_index, const pointer& val);
    tostring_function message_output_tostring_func(int64 port_index);
    void set_message_output_printable(int64 port_index, tostring_function tostring_func);

    int64 add_flow_output_port(const std::string& port_name);
    int64 flow_output_port_count();
    const std::string& flow_output_port_name(int64 port_index);
    const pointer& flow_output_port_value(int64 port_index);
    void clear_flow_outputs();
    void assign_flow_output(int64 port_index, const pointer& val);
    tostring_function flow_output_tostring_func(int64 port_index);
    void set_flow_output_printable(int64 port_index, tostring_function tostring_func);

    void activate(data_mode dmode, data_goal dgoal);
    void deactivate();

    bool active() const;
    data_mode dmode() const;
    data_goal dgoal() const;

    int64 missing_flow_input();
    int64 missing_flow_output();

    void print_port_value(const std::string& port_name, const std::string& val_string);
    void print_flow_input(int64 port_index);
    void print_message_input(int64 port_index);
    void print_message_output(int64 list_index, int64 port_index);
    void print_flow_output(int64 port_index);

    void print_command_text(const std::string& command, const std::string& text);

    bool print_on_event_flag();
    void print_on_event(bool flag = true);
    void print_event(const std::string& text);

    bool print_on_elapsed_duration_flag();
    void print_on_elapsed_duration(bool flag);
    void print_elapsed_duration(duration elapsed_dt);

    bool print_on_planned_duration_flag();
    void print_on_planned_duration(bool flag);
    void print_planned_duration(duration planned_dt);

private:
    void print(const std::string& text);

    std::string node_name_;
    std::string full_name_;
    int64 node_index_;

    node_context& external_context_;

    std::vector<std::string> flow_input_port_names_;
    std::vector<tostring_function> flow_input_port_tostring_funcs_;
    std::vector<pointer> flow_input_port_vals_;

    std::vector<std::string> message_input_port_names_;
    std::vector<tostring_function> message_input_port_tostring_funcs_;
    std::vector<pointer> message_input_port_vals_;
    int64 message_input_port_index_;

    std::vector<std::string> message_output_port_names_;
    std::vector<tostring_function> message_output_port_tostring_funcs_;
    std::vector<int64> message_output_indices_;
    std::vector<pointer> message_output_vals_;

    std::vector<std::string> flow_output_port_names_;
    std::vector<tostring_function> flow_output_port_tostring_funcs_;
    std::vector<pointer> flow_output_port_vals_;

    bool active_;
    data_mode dmode_;
    data_goal dgoal_;

    bool print_on_event_;
    bool print_on_elapsed_duration_;
    bool print_on_planned_duration_;
};


inline const std::string& node_interface::node_name()
{
    return node_name_;
}


inline const std::string& node_interface::full_name()
{
    return full_name_;
}

    
inline int64 node_interface::node_index()
{
    return node_index_;
}


inline node_context& node_interface::external_context()
{
    return external_context_;
}


inline void node_interface::activate(data_mode dmode, data_goal dgoal)
{
    active_ = true;
    dmode_ = dmode;
    dgoal_ = dgoal;
}


inline void node_interface::deactivate()
{
    active_ = false;
}


inline bool node_interface::active() const
{
    return active_;
}


inline data_mode node_interface::dmode() const
{
    return dmode_;
}


inline data_goal node_interface::dgoal() const
{
    return dgoal_;
}


}  // namespace
}  // namespace

#endif
