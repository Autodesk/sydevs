#pragma once
#ifndef SYDEVS_SYSTEMS_COMPOSITE_NODE_H_
#define SYDEVS_SYSTEMS_COMPOSITE_NODE_H_

#include <sydevs/systems/system_node.h>
#include <sydevs/time/time_queue.h>
#include <sydevs/time/time_cache.h>

namespace sydevs {
namespace systems {


/**
 * @brief A base class for all nodes defined as fixed-structure compositions of
 *        different types of nodes.
 *
 * @details
 * The `composite_node` base class is inherited by classes that represent
 * system behavior using networks of component nodes connected by links. The
 * component nodes can be of an assortment of different types. The network
 * itself has a fixed structure than cannot change duing a simulation.
 */
class composite_node : public system_node
{
public:
    /**
     * @brief Constructs a `composite_node`.
     * 
     * @details
     * The composite node is created along with its associated `node_interface`
     * object.
     *
     * @param node_name The name of the node within the encompassing context.
     * @param external_context The context in which the node is constructed.
     */
    composite_node(const std::string& node_name, const node_context& external_context);

    virtual ~composite_node() = default;  ///< Destructor

    data_mode node_dmode() const;  ///< Returns `flow` if all component nodes have data flow elements only, and `message` otherwise.
    scale time_precision() const;  ///< Returns `no_scale` for all composite nodes.

    duration handle_initialization_event();                ///< Invoked at the beginning of a simulation, calls `process_initialization_event` on all component nodes.
    duration handle_unplanned_event(duration elapsed_dt);  ///< Invoked whenever a message is received; calls `process_unplanned_event` on receiving component nodes.
    duration handle_planned_event(duration elapsed_dt);    ///< Invoked when the planned duration elapses; calls `process_planned_event` on the source node and propagates messages.
    void handle_finalization_event(duration elapsed_dt);   ///< Invoked at the end of a simulation; calls `process_finalization_event` on all component nodes.

protected:
    const node_context& internal_context();  ///< Returns a representation of the internal context, which is also the external context of the component nodes.

    /**
     * @brief Constructs an inward link.
     * 
     * @details
     * An inwward link is created from an input port on the interface of the
     * composite node to an input port on one of the component nodes.
     *
     * @param src_port The source port on the interface of the composite node.
     * @param dst_port The destination port on one of the component nodes.
     */
    template<data_mode dmode, typename T>
    void inward_link(const port<dmode, input, T>& src_port, 
                     const port<dmode, input, T>& dst_port);

    /**
     * @brief Constructs an inner link.
     * 
     * @details
     * An inner link is created from an output port on one of the component
     * nodes to an input port on another component node.
     *
     * @param src_port The source port on one of the component nodes.
     * @param dst_port The destination port on one of the component nodes.
     */
    template<data_mode dmode, typename T>
    void inner_link(const port<dmode, output, T>& src_port, 
                    const port<dmode, input, T>& dst_port);

    /**
     * @brief Constructs an outward link.
     * 
     * @details
     * An outward link is created from an output port on one of the component
     * nodes to an output port on the interface of the composite node.
     *
     * @param src_port The source port on one of the component nodes.
     * @param dst_port The destination port on the interface of the composite node.
     */
    template<data_mode dmode, typename T>
    void outward_link(const port<dmode, output, T>& src_port, 
                      const port<dmode, output, T>& dst_port);

private:
    node_structure& internal_structure();
    node_interface& component_IO(const system_node& node) const;
    discrete_event_time& event_time();
    
    void categorize_node_indices();
    void process_flow_nodes(bool finalize);
    void handle_initialization_events();
    system_node& handle_src_event(int64 src_index);
    void handle_dst_events(const std::set<std::pair<int64, int64>>& dsts, const pointer& val);
    void handle_finalization_events();

    void activate_flow_inward_links();
    void activate_flow_inner_links(int64 node_index, system_node& node);
    void activate_flow_outward_links(int64 node_index, system_node& node);

    const std::set<std::pair<int64, int64>>& traverse_message_inward_links(int64 port_index);
    const std::set<std::pair<int64, int64>>& traverse_message_inner_links(int64 node_index, int64 port_index);
    const std::set<int64>& traverse_message_outward_links(int64 node_index, int64 port_index);

    void adopt_component_print_flags(const system_node& node) const;

    node_context internal_context_;
    std::vector<int64> unprocessed_flow_node_indices_;
    std::vector<int64> processed_flow_node_indices_;
    std::vector<int64> uninitialized_message_node_indices_;
    std::vector<int64> initialized_message_node_indices_;
    time_queue t_queue_;
    time_cache t_cache_;
    std::map<int64, std::set<std::pair<int64, int64>>> inward_link_dsts_;                   // Reorders link destinations
    std::map<std::pair<int64, int64>, std::set<std::pair<int64, int64>>> inner_link_dsts_;  // Reorders link destinations
    std::map<std::pair<int64, int64>, std::set<int64>> outward_link_dsts_;                  // Reorders link destinations
    mutable data_mode composite_dmode_;
    mutable bool composite_dmode_calculated_;
    bool initialized_;
    bool finalized_;
};


inline composite_node::composite_node(const std::string& node_name, const node_context& external_context)
    : system_node(node_name, external_context)
    , internal_context_(&const_cast<node_interface&>(external_interface()),
                        const_cast<node_context&>(external_context))
    , unprocessed_flow_node_indices_()
    , processed_flow_node_indices_()
    , uninitialized_message_node_indices_()
    , initialized_message_node_indices_()
    , t_queue_()
    , t_cache_()
    , inward_link_dsts_()
    , inner_link_dsts_()
    , outward_link_dsts_()
    , composite_dmode_(flow)
    , composite_dmode_calculated_(false)
    , initialized_(false)
    , finalized_(false)
{
}


inline scale composite_node::time_precision() const
{
    return no_scale;
}


inline const node_context& composite_node::internal_context()
{
    return const_cast<const node_context&>(internal_context_);
}


template<data_mode dmode, typename T>
inline void composite_node::inward_link(const port<dmode, input, T>& src_port, 
                                        const port<dmode, input, T>& dst_port)
{
    if (&src_port.external_interface() != &external_interface()) {
        throw std::logic_error("Attempt to create inward link on composite node (" + full_name() + ") that does not originate from one of the composite node's ports");
    }
    if (&(const_cast<node_interface&>(dst_port.external_interface()).external_context()) != &internal_context_) {
        throw std::logic_error("Attempt to create inward link on composite node (" + full_name() + ") that does not end up at one of the composite node's components");
    }
    switch (dmode) {
    case flow:
        internal_structure().add_flow_inward_link(src_port.port_index(), 
                                                  dst_port.node_index(), dst_port.port_index());
        break;
    case message:
        internal_structure().add_message_inward_link(src_port.port_index(), 
                                                     dst_port.node_index(), dst_port.port_index());
        break;          
    }
}


template<data_mode dmode, typename T>
inline void composite_node::inner_link(const port<dmode, output, T>& src_port, 
                                       const port<dmode, input, T>& dst_port)
{
    if (&(const_cast<node_interface&>(src_port.external_interface()).external_context()) != &internal_context_) {
        throw std::logic_error("Attempt to create inner link on composite node (" + full_name() + ") that does not originate from one of the composite node's components");
    }
    if (&(const_cast<node_interface&>(dst_port.external_interface()).external_context()) != &internal_context_) {
        throw std::logic_error("Attempt to create inner link on composite node (" + full_name() + ") that does not end up at one of the composite node's components");
    }
    switch (dmode) {
    case flow:
        internal_structure().add_flow_inner_link(src_port.node_index(), src_port.port_index(), 
                                                 dst_port.node_index(), dst_port.port_index());
        break;
    case message:
        internal_structure().add_message_inner_link(src_port.node_index(), src_port.port_index(), 
                                                    dst_port.node_index(), dst_port.port_index());
        break;          
    }
}


template<data_mode dmode, typename T>
inline void composite_node::outward_link(const port<dmode, output, T>& src_port, 
                                         const port<dmode, output, T>& dst_port)
{
    if (&(const_cast<node_interface&>(src_port.external_interface()).external_context()) != &internal_context_) {
        throw std::logic_error("Attempt to create outward link on composite node (" + full_name() + ") that does not originate from one of the composite node's components");
    }
    if (&dst_port.external_interface() != &external_interface()) {
        throw std::logic_error("Attempt to create outward link on composite node (" + full_name() + ") that does not end up at one of the composite node's ports");
    }
    switch (dmode) {
    case flow:
        internal_structure().add_flow_outward_link(src_port.node_index(), src_port.port_index(), 
                                                   dst_port.port_index());
        break;
    case message:
        internal_structure().add_message_outward_link(src_port.node_index(), src_port.port_index(), 
                                                      dst_port.port_index());
        break;          
    }
}


inline node_structure& composite_node::internal_structure()
{
    return internal_context_.internal_structure();
}


inline node_interface& composite_node::component_IO(const system_node& node) const
{
    return const_cast<node_interface&>(const_cast<system_node&>(node).external_interface());
}


inline discrete_event_time& composite_node::event_time()
{
    return external_IO().external_context().event_time();
}


}  // namespace
}  // namespace

#endif
