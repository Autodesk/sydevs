#include <sydevs/systems/node_interface.h>
#include <ostream>

namespace sydevs {
namespace systems {


node_interface::node_interface(const std::string& node_name, system_node* node_ptr, const node_context& external_context)
    : node_name_(node_name)
    , full_name_()
    , node_index_(const_cast<node_context&>(external_context).internal_structure().add_node(node_ptr))
    , external_context_(const_cast<node_context&>(external_context))
    , flow_input_port_names_()
    , flow_input_port_tostring_funcs_()
    , flow_input_port_vals_()
    , message_input_port_names_()
    , message_input_port_tostring_funcs_()
    , message_input_port_vals_()
    , message_input_port_index_(-1)
    , message_output_port_names_()
    , message_output_port_tostring_funcs_()
    , message_output_indices_()
    , message_output_vals_()
    , flow_output_port_names_()
    , flow_output_port_tostring_funcs_()
    , flow_output_port_vals_()
    , active_(false)
    , dmode_(flow)
    , dgoal_(input)
    , print_on_event_(false)
    , print_on_elapsed_duration_(false)
    , print_on_planned_duration_(false)
{
    if (external_context_.external_interface_ptr()) {
        full_name_ += external_context_.external_interface_ptr()->full_name() + ".";
    }
    full_name_ += node_name;
}


int64 node_interface::add_flow_input_port(const std::string& port_name)
{
    flow_input_port_names_.push_back(port_name);
    flow_input_port_tostring_funcs_.push_back(nullptr);
    flow_input_port_vals_.emplace_back();
    return flow_input_port_names_.size() - 1;
}


int64 node_interface::flow_input_port_count()
{
    return flow_input_port_vals_.size();
}


const std::string& node_interface::flow_input_port_name(int64 port_index)
{
    return flow_input_port_names_[port_index];
}

    
const pointer& node_interface::flow_input_port_value(int64 port_index)
{
    return flow_input_port_vals_[port_index];
}


void node_interface::clear_flow_inputs()
{
    flow_input_port_vals_.assign(flow_input_port_vals_.size(), pointer());
}

    
void node_interface::assign_flow_input(int64 port_index, const pointer& val)
{
   flow_input_port_vals_[port_index] = val;
}


tostring_function node_interface::flow_input_tostring_func(int64 port_index)
{
    return flow_input_port_tostring_funcs_[port_index];
}

    
void node_interface::set_flow_input_printable(int64 port_index, tostring_function tostring_func)
{
    flow_input_port_tostring_funcs_[port_index] = tostring_func;
}

        
int64 node_interface::add_message_input_port(const std::string& port_name)
{
    message_input_port_names_.push_back(port_name);
    message_input_port_tostring_funcs_.push_back(nullptr);
    message_input_port_vals_.emplace_back();
    return message_input_port_names_.size() - 1;
}


int64 node_interface::message_input_port_count()
{
    return message_input_port_vals_.size();
}


const std::string& node_interface::message_input_port_name(int64 port_index)
{
    return message_input_port_names_[port_index];
}

    
const pointer& node_interface::message_input_port_value(int64 port_index)
{
    return message_input_port_vals_[port_index];
}


int64 node_interface::message_input_port_index()
{
    return message_input_port_index_;
}


void node_interface::clear_message_input()
{
    if (message_input_port_index_ != -1) {
        message_input_port_vals_[message_input_port_index_].reset();
        message_input_port_index_ = -1;
    }
}


void node_interface::set_message_input(int64 port_index, const pointer& val)
{
    message_input_port_index_ = port_index;
    message_input_port_vals_[message_input_port_index_] = val;
}


tostring_function node_interface::message_input_tostring_func(int64 port_index)
{
    return message_input_port_tostring_funcs_[port_index];
}

    
void node_interface::set_message_input_printable(int64 port_index, tostring_function tostring_func)
{
    message_input_port_tostring_funcs_[port_index] = tostring_func;
}

        
int64 node_interface::add_message_output_port(const std::string& port_name)
{
    message_output_port_names_.push_back(port_name);
    message_output_port_tostring_funcs_.push_back(nullptr);
    return message_output_port_names_.size() - 1;
}


int64 node_interface::message_output_port_count()
{
    return message_output_port_names_.size();
}


const std::string& node_interface::message_output_port_name(int64 port_index)
{
    return message_output_port_names_[port_index];
}

    
int64 node_interface::message_output_list_size()
{
    return message_output_indices_.size();
}


int64 node_interface::message_output_index(int64 list_index)
{
    return message_output_indices_[list_index];
}


const pointer& node_interface::message_output_value(int64 list_index)
{
    return message_output_vals_[list_index];
}


void node_interface::clear_message_outputs()
{
    message_output_indices_.clear();
    message_output_vals_.clear();
}


void node_interface::append_message_output(int64 port_index, const pointer& val)
{
    message_output_indices_.push_back(port_index);
    message_output_vals_.push_back(val);
}


tostring_function node_interface::message_output_tostring_func(int64 port_index)
{
    return message_output_port_tostring_funcs_[port_index];
}

    
void node_interface::set_message_output_printable(int64 port_index, tostring_function tostring_func)
{
    message_output_port_tostring_funcs_[port_index] = tostring_func;
}

        
int64 node_interface::add_flow_output_port(const std::string& port_name)
{
    flow_output_port_names_.push_back(port_name);
    flow_output_port_tostring_funcs_.push_back(nullptr);
    flow_output_port_vals_.emplace_back();
    return flow_output_port_names_.size() - 1;
}


int64 node_interface::flow_output_port_count()
{
    return flow_output_port_vals_.size();
}


const std::string& node_interface::flow_output_port_name(int64 port_index)
{
    return flow_output_port_names_[port_index];
}

    
const pointer& node_interface::flow_output_port_value(int64 port_index)
{
    return flow_output_port_vals_[port_index];
}


void node_interface::clear_flow_outputs()
{
    flow_output_port_vals_.assign(flow_output_port_vals_.size(), pointer());
}

    
void node_interface::assign_flow_output(int64 port_index, const pointer& val)
{
    flow_output_port_vals_[port_index] = val;
}


tostring_function node_interface::flow_output_tostring_func(int64 port_index)
{
    return flow_output_port_tostring_funcs_[port_index];
}

    
void node_interface::set_flow_output_printable(int64 port_index, tostring_function tostring_func)
{
    flow_output_port_tostring_funcs_[port_index] = tostring_func;
}


int64 node_interface::missing_flow_input()
{
    int64 missing_index = -1;
    int64 port_count = flow_input_port_count();
    for (int64 port_index = 0; missing_index == -1 && port_index < port_count; ++port_index) {
        if (!flow_input_port_value(port_index)) {
            missing_index = port_index;
        }
    }
    return missing_index;
}


int64 node_interface::missing_flow_output()
{
    int64 missing_index = -1;
    int64 port_count = flow_output_port_count();
    for (int64 port_index = 0; missing_index == -1 && port_index < port_count; ++port_index) {
        if (!flow_output_port_value(port_index)) {
            missing_index = port_index;
        }
    }
    return missing_index;
}

    
void node_interface::print_port_value(const std::string& port_name, const std::string& val_string)
{    
    print("#" + port_name + ":" + val_string);
}

    
void node_interface::print_flow_input(int64 port_index)
{
    const auto& tostring_func = flow_input_port_tostring_funcs_[port_index];
    if (tostring_func) {
        const std::string& port_name = flow_input_port_names_[port_index];
        const auto& val = flow_input_port_vals_[port_index];
        print_port_value(port_name, tostring_func(val));
    }
}

 
void node_interface::print_message_input(int64 port_index)
{
    const auto& tostring_func = message_input_port_tostring_funcs_[port_index];
    if (tostring_func) {
        const std::string& port_name = message_input_port_names_[port_index];
        const auto& val = message_input_port_vals_[port_index];
        print_port_value(port_name, tostring_func(val));
    }
}

 
void node_interface::print_message_output(int64 list_index, int64 port_index)
{
    const auto& tostring_func = message_output_port_tostring_funcs_[port_index];
    if (tostring_func) {
        const std::string& port_name = message_output_port_names_[port_index];
        const auto& val = message_output_vals_[list_index];
        print_port_value(port_name, tostring_func(val));
    }
}


void node_interface::print_flow_output(int64 port_index)
{
    const auto& tostring_func = flow_output_port_tostring_funcs_[port_index];
    if (tostring_func) {
        const std::string& port_name = flow_output_port_names_[port_index];
        const auto& val = flow_output_port_vals_[port_index];
        print_port_value(port_name, tostring_func(val));
    }
}


void node_interface::print_command_text(const std::string& command, const std::string& text)
{
    print("$" + command + ":" + text);
}


bool node_interface::print_on_event_flag()
{
    return print_on_event_;
}


void node_interface::print_on_event(bool flag)
{
    print_on_event_ = flag;
}


void node_interface::print_event(const std::string& text)
{
    if (print_on_event_) {
        print("$" + text);
    }
}

    
bool node_interface::print_on_elapsed_duration_flag()
{
    return print_on_elapsed_duration_;
}


void node_interface::print_on_elapsed_duration(bool flag)
{
    print_on_elapsed_duration_ = flag;
}


void node_interface::print_elapsed_duration(duration elapsed_dt)
{
    if (print_on_elapsed_duration_) {
        print_command_text("elapsed_dt", core_type<duration>::tostring(elapsed_dt));
    }
}

    
bool node_interface::print_on_planned_duration_flag()
{
    return print_on_planned_duration_;
}


void node_interface::print_on_planned_duration(bool flag)
{
    print_on_planned_duration_ = flag;
}


void node_interface::print_planned_duration(duration planned_dt)
{
    if (print_on_planned_duration_) {
        print_command_text("planned_dt", core_type<duration>::tostring(planned_dt));
    }
}

    
void node_interface::print(const std::string& text)
{
    int64 t_index = external_context_.event_time().t_index();
    if (!external_context_.time_printed()) {
        external_context_.stream() << t_index << "|0|$time:" << 
                                      external_context_.event_time().t() << std::endl;
        external_context_.time_printed() = true;
    }
    int64 c = external_context_.event_time().c();
    external_context_.stream() << t_index << "|" << c << "|" << full_name() << text << std::endl;
}


}  // namespace
}  // namespace
