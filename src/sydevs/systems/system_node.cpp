#include <sydevs/systems/system_node.h>

namespace sydevs {
namespace systems {


void system_node::adopt_print_flags(const system_node& node) const
{
    auto& IO = external_IO();
    auto& node_IO = node.external_IO();
    IO.print_on_event(node_IO.print_on_event_flag());
    IO.print_on_elapsed_duration(node_IO.print_on_elapsed_duration_flag());
    IO.print_on_planned_duration(node_IO.print_on_planned_duration_flag());
    {
        int64 port_count = node_IO.flow_input_port_count();
        for (int64 port_index = 0; port_index < port_count; ++port_index) {
            auto tostring_func = node_IO.flow_input_tostring_func(port_index);
            IO.set_flow_input_printable(port_index, tostring_func);
        }
    }
    {
        int64 port_count = node_IO.message_input_port_count();
        for (int64 port_index = 0; port_index < port_count; ++port_index) {
            auto tostring_func = node_IO.message_input_tostring_func(port_index);
            IO.set_message_input_printable(port_index, tostring_func);
        }
    }
    {
        int64 port_count = node_IO.message_output_port_count();
        for (int64 port_index = 0; port_index < port_count; ++port_index) {
            auto tostring_func = node_IO.message_output_tostring_func(port_index);
            IO.set_message_output_printable(port_index, tostring_func);
        }
    }
    {
        int64 port_count = node_IO.flow_output_port_count();
        for (int64 port_index = 0; port_index < port_count; ++port_index) {
            auto tostring_func = node_IO.flow_output_tostring_func(port_index);
            IO.set_flow_output_printable(port_index, tostring_func);
        }
    }
    adopt_component_print_flags(node);
}


duration system_node::scale_planned_dt(duration planned_dt) const
{
    auto dt = planned_dt;
    if (!dt.valid()) throw std::logic_error("Planned duration for atomic node must be valid");
    if (dt < 0_s) throw std::logic_error("Planned duration for atomic node must be non-negative");
    if (time_precision() == no_scale) {
        if (dt != 0_s && dt.finite()) throw std::logic_error("Planned duration for atomic node with no time scale must be either zero or infinity");
    }
    else {
        dt = dt.fixed_at(time_precision());
        if (dt != planned_dt) throw std::logic_error("Planned duration must not lose precision when automatically scaled to match the model's time precision");
    }
    return dt;
}


}  // namespace
}  // namespace


