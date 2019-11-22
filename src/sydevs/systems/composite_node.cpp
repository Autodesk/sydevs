#include <sydevs/systems/composite_node.h>

namespace sydevs {
namespace systems {


data_mode composite_node::node_dmode() const
{
    if (!composite_dmode_calculated_) {
        auto& internal_structure = const_cast<node_context&>(internal_context_).internal_structure();
        int64 node_count = internal_structure.node_count();
        for (int64 node_index = 0; composite_dmode_ == flow && node_index < node_count; ++node_index) {
            auto& node = internal_structure.node(node_index);
            if (node.node_dmode() == message) {
                composite_dmode_ = message;
            }
        }
        composite_dmode_calculated_ = true;
    }
    return composite_dmode_;
}


duration composite_node::handle_initialization_event()
{
    if (initialized_) throw std::logic_error("Attempt to initialize composite node (" + full_name() + ") more than once");
    auto& current_t = event_time().t();
    t_queue_ = time_queue(current_t);
    t_cache_ = time_cache(current_t);
    categorize_node_indices();
    activate_flow_inward_links();
    process_flow_nodes(node_dmode() == flow);
    handle_initialization_events();
    initialized_ = true;
    if (node_dmode() == flow) {
        event_time().advance();
    }
    return t_queue_.imminent_duration();
}


duration composite_node::handle_unplanned_event(duration elapsed_dt)
{
    auto& current_t = event_time().t();
    t_queue_.advance_time(current_t);
    t_cache_.advance_time(current_t);
    int64 port_index = external_IO().message_input_port_index();
    const auto& val = external_IO().message_input_port_value(port_index);
    const auto& inward_dsts = traverse_message_inward_links(port_index);
    handle_dst_events(inward_dsts, val);
    return t_queue_.imminent_duration();
}


duration composite_node::handle_planned_event(duration elapsed_dt)
{
    auto& current_t = event_time().t();
    t_queue_.advance_time(current_t);
    t_cache_.advance_time(current_t);
    if (t_queue_.imminent_duration() != 0_s) throw std::logic_error("Unexpected error while advancing time to that of next planned event");
    int64 src_index = *std::begin(t_queue_.imminent_event_ids());
    auto& src_node = handle_src_event(src_index);
    int64 list_size = component_IO(src_node).message_output_list_size();
    for (int64 list_index = 0; list_index < list_size; ++list_index) {
        int64 port_index = component_IO(src_node).message_output_index(list_index);
        const auto& val = component_IO(src_node).message_output_value(list_index);
        const auto& inner_dsts = traverse_message_inner_links(src_index, port_index);
        handle_dst_events(inner_dsts, val);
        const auto& outward_dsts = traverse_message_outward_links(src_index, port_index);
        for (int64 outward_port_index : outward_dsts) {
            external_IO().append_message_output(outward_port_index, val);
        }
    }
    component_IO(src_node).clear_message_outputs();
    event_time().advance();
    return t_queue_.imminent_duration();
}


void composite_node::handle_finalization_event(duration elapsed_dt)
{
    if (finalized_) throw std::logic_error("Attempt to finalize composite node (" + full_name() + ") more than once");
    auto& current_t = event_time().t();
    t_queue_.advance_time(current_t);
    t_cache_.advance_time(current_t);
    handle_finalization_events();
    process_flow_nodes(true);
    finalized_ = true;
    event_time().advance();
}


void composite_node::categorize_node_indices()
{
    int64 node_count = internal_structure().node_count();
    for (int64 node_index = 0; node_index < node_count; ++node_index) {
        auto& node = internal_structure().node(node_index);
        if (node.node_dmode() == flow) {
            unprocessed_flow_node_indices_.push_back(node_index);
        }
        else {
            uninitialized_message_node_indices_.push_back(node_index);
        }
    }
}


void composite_node::process_flow_nodes(bool finalize)
{
    auto index_iter = std::begin(unprocessed_flow_node_indices_);
    while (index_iter != std::end(unprocessed_flow_node_indices_)) {
        int64 node_index = *index_iter;
        auto& node = internal_structure().node(node_index);
        int64 missing_input = component_IO(node).missing_flow_input();
        if (missing_input != -1) {
            if (finalize) {
                throw std::logic_error("Flow input port (" + component_IO(node).flow_input_port_name(missing_input) + ") " + 
                                       "of node (" + node.full_name() + ") has no value");
            }
            else {
                ++index_iter;
            }
        }
        else {
            event_time().advance();
            component_IO(node).print_event("flow");
            int64 input_port_count = component_IO(node).flow_input_port_count();
            for (int64 port_index = 0; port_index < input_port_count; ++port_index) {
                component_IO(node).print_flow_input(port_index);
            }
            component_IO(node).activate(flow, input);
            ET().start();
            node.process_initialization_event();
            ET().stop();
            component_IO(node).deactivate();
            int64 missing_output = component_IO(node).missing_flow_output();
            if (missing_output != -1) {
                throw std::logic_error("Flow output port (" + component_IO(node).flow_output_port_name(missing_output) + ") " +
                                       "of flow node (" + node.full_name() + ") not assigned");
            }
            int64 output_port_count = component_IO(node).flow_output_port_count();
            for (int64 port_index = 0; port_index < output_port_count; ++port_index) {
                component_IO(node).print_flow_output(port_index);
            }
            activate_flow_inner_links(node_index, node);
            activate_flow_outward_links(node_index, node);
            processed_flow_node_indices_.push_back(node_index);
            unprocessed_flow_node_indices_.erase(index_iter);
            index_iter = std::begin(unprocessed_flow_node_indices_);
        }
    }
}


void composite_node::handle_initialization_events()
{
    while (!uninitialized_message_node_indices_.empty()) {
        event_time().advance();
        int64 node_index = uninitialized_message_node_indices_.front();
        auto& node = internal_structure().node(node_index);
        component_IO(node).print_event("initialization");
        int64 missing_input = component_IO(node).missing_flow_input();
        if (missing_input != -1) {
            throw std::logic_error("Flow input port (" + component_IO(node).flow_input_port_name(missing_input) + ") " +
                                   "of message node (" + node.full_name() + ") has no value");
        }
        int64 port_count = component_IO(node).flow_input_port_count();
        for (int64 port_index = 0; port_index < port_count; ++port_index) {
            component_IO(node).print_flow_input(port_index);
        }
        component_IO(node).activate(flow, input);
        ET().start();
        auto planned_dt = node.process_initialization_event();
        ET().stop();
        component_IO(node).deactivate();
        if (planned_dt.finite()) {
            t_queue_.plan_event(node_index, planned_dt);
        }
        if (node.time_precision() != no_scale) {
            t_cache_.retain_event(node_index, node.time_precision());
        }
        initialized_message_node_indices_.push_back(node_index);
        uninitialized_message_node_indices_.erase(std::begin(uninitialized_message_node_indices_));
    }
}


system_node& composite_node::handle_src_event(int64 src_index)
{
    event_time().advance();
    auto& src_node = internal_structure().node(src_index);
    component_IO(src_node).print_event("planned");
    auto elapsed_dt = duration();
    if (src_node.time_precision() != no_scale) {
        elapsed_dt = t_cache_.duration_since(src_index).fixed_at(src_node.time_precision());
    }
    component_IO(src_node).activate(message, output);
    ET().start();
    auto planned_dt = src_node.process_planned_event(elapsed_dt);
    ET().stop();
    component_IO(src_node).deactivate();
    if (planned_dt.finite()) {
        t_queue_.plan_event(src_index, planned_dt);
    }
    else {
        t_queue_.pop_imminent_event(src_index);
    }
    if (src_node.time_precision() != no_scale) {
        t_cache_.retain_event(src_index, src_node.time_precision());
    }
    int64 list_size = component_IO(src_node).message_output_list_size();
    for (int64 list_index = 0; list_index < list_size; ++list_index) {
        int64 port_index = component_IO(src_node).message_output_index(list_index);
        component_IO(src_node).print_message_output(list_index, port_index);
    }
    return src_node;
}


void composite_node::handle_dst_events(const std::set<std::pair<int64, int64>>& dsts, const pointer& val)
{
    for (const auto& dst : dsts) {
        event_time().advance();
        auto& dst_node = internal_structure().node(dst.first);
        component_IO(dst_node).print_event("unplanned");
        component_IO(dst_node).set_message_input(dst.second, val);
        component_IO(dst_node).print_message_input(dst.second);
        auto elapsed_dt = duration();
        if (dst_node.time_precision() != no_scale) {
            elapsed_dt = t_cache_.duration_since(dst.first).fixed_at(dst_node.time_precision());
        }
        component_IO(dst_node).activate(message, input);
        ET().start();
        auto planned_dt = dst_node.process_unplanned_event(elapsed_dt);
        ET().stop();
        component_IO(dst_node).deactivate();
        if (planned_dt.finite()) {
            t_queue_.plan_event(dst.first, planned_dt);
        }
        else {
            t_queue_.cancel_event(dst.first);
        }
        if (dst_node.time_precision() != no_scale) {
            t_cache_.retain_event(dst.first, dst_node.time_precision());
        }
        component_IO(dst_node).clear_message_input();
    }
}


void composite_node::handle_finalization_events()
{
    for (int64 node_index : initialized_message_node_indices_) {
        event_time().advance();
        auto& node = internal_structure().node(node_index);
        component_IO(node).print_event("finalization");
        auto elapsed_dt = duration();
        if (node.time_precision() != no_scale) {
            elapsed_dt = t_cache_.duration_since(node_index).fixed_at(node.time_precision());
        }
        component_IO(node).activate(flow, output);
        ET().start();
        node.process_finalization_event(elapsed_dt);
        ET().stop();
        component_IO(node).deactivate();
        int64 missing_output = component_IO(node).missing_flow_output();
        if (missing_output != -1) {
            throw std::logic_error("Flow output port (" + component_IO(node).flow_input_port_name(missing_output) + ") " +
                                   "of message node (" + node.full_name() + ") not assigned");
        }
        int64 port_count = component_IO(node).flow_output_port_count();
        for (int64 port_index = 0; port_index < port_count; ++port_index) {
            component_IO(node).print_flow_output(port_index);
        }
        activate_flow_inner_links(node_index, node);
        activate_flow_outward_links(node_index, node);
    }
}


void composite_node::activate_flow_inward_links()
{
    int64 port_count = external_IO().flow_input_port_count();
    for (int64 port_index = 0; port_index < port_count; ++port_index) {
        const auto& val = external_IO().flow_input_port_value(port_index);
        const auto& links = internal_structure().flow_inward_links(port_index);
        for (const auto& dst : links) {
            auto& dst_node = internal_structure().node(dst.first);
            const auto& dst_val = component_IO(dst_node).flow_input_port_value(dst.second);
            if (dst_val) throw std::logic_error("Flow input port (" + component_IO(dst_node).flow_input_port_name(dst.second) + ") " +
                                                "of node (" + dst_node.full_name() + ") receiving multiple values");
            component_IO(dst_node).assign_flow_input(dst.second, val);
        }
    }
}


void composite_node::activate_flow_inner_links(int64 node_index, system_node& node)
{
    int64 port_count = component_IO(node).flow_output_port_count();
    for (int64 port_index = 0; port_index < port_count; ++port_index) {
        const auto& val = component_IO(node).flow_output_port_value(port_index);
        const auto& links = internal_structure().flow_inner_links(node_index, port_index);
        for (const auto& dst : links) {
            auto& dst_node = internal_structure().node(dst.first);
            const auto& dst_val = component_IO(dst_node).flow_input_port_value(dst.second);
            if (dst_val) throw std::logic_error("Flow input port (" + component_IO(dst_node).flow_input_port_name(dst.second) + ") " +
                                                "of node (" + dst_node.full_name() + ") receiving multiple values");
            component_IO(dst_node).assign_flow_input(dst.second, val);
        }
    }
}


void composite_node::activate_flow_outward_links(int64 node_index, system_node& node)
{
    int64 port_count = component_IO(node).flow_output_port_count();
    for (int64 port_index = 0; port_index < port_count; ++port_index) {
        const auto& val = component_IO(node).flow_output_port_value(port_index);
        const auto& links = internal_structure().flow_outward_links(node_index, port_index);
        for (const auto& dst : links) {
            auto& dst_node = internal_structure().node(dst);
            const auto& dst_val = external_IO().flow_output_port_value(dst);
            if (dst_val) throw std::logic_error("Flow output port (" + component_IO(dst_node).flow_output_port_name(dst) + ") " + 
                                                "of composite node (" + full_name() + ") receiving multiple values");
            external_IO().assign_flow_output(dst, val);
        }
    }
}


const std::set<std::pair<int64, int64>>& composite_node::traverse_message_inward_links(int64 port_index)
{
    auto dsts_iter = inward_link_dsts_.find(port_index);
    if (dsts_iter == std::end(inward_link_dsts_)) {
        auto& dsts = inward_link_dsts_[port_index];
        const auto& links = internal_structure().message_inward_links(port_index);
        for (const auto& dst : links) {
            dsts.insert(dst);
        }
        dsts_iter = inward_link_dsts_.find(port_index);
    }
    return dsts_iter->second;
}


const std::set<std::pair<int64, int64>>& composite_node::traverse_message_inner_links(int64 node_index, int64 port_index)
{
    auto src = std::pair<int64, int64>(node_index, port_index);
    auto dsts_iter = inner_link_dsts_.find(src);
    if (dsts_iter == std::end(inner_link_dsts_)) {
        auto& dsts = inner_link_dsts_[src];
        const auto& links = internal_structure().message_inner_links(node_index, port_index);
        for (const auto& dst : links) {
            dsts.insert(dst);
        }
        dsts_iter = inner_link_dsts_.find(src);
    }
    return dsts_iter->second;
}


const std::set<int64>& composite_node::traverse_message_outward_links(int64 node_index, int64 port_index)
{
    auto src = std::pair<int64, int64>(node_index, port_index);
    auto dsts_iter = outward_link_dsts_.find(src);
    if (dsts_iter == std::end(outward_link_dsts_)) {
        auto& dsts = outward_link_dsts_[src];
        const auto& links = internal_structure().message_outward_links(node_index, port_index);
        for (const auto& dst : links) {
            dsts.insert(dst);
        }
        dsts_iter = outward_link_dsts_.find(src);
    }
    return dsts_iter->second;
}


void composite_node::adopt_component_print_flags(const system_node& node) const
{
    const auto node_ptr = dynamic_cast<const composite_node*>(&node);
    if (!node_ptr) {
        throw std::logic_error("Attempt to transfer print flags from node (" + node.full_name() + ")" +
                               "to node (" + full_name() + "), but the nodes are of different types");
    }
    auto& other_node = const_cast<composite_node&>(*node_ptr);
    auto& structure = const_cast<composite_node&>(*this).internal_structure();
    int64 component_count = structure.node_count();
    for (int64 component_index = 0; component_index < component_count; ++component_index) {
        auto& component = structure.node(component_index);
        auto& other_component = other_node.internal_structure().node(component_index);
        component.adopt_print_flags(other_component);
    }
}


}  // namespace
}  // namespace
