#pragma once
#ifndef SYDEVS_SYSTEMS_NODE_STRUCTURE_H_
#define SYDEVS_SYSTEMS_NODE_STRUCTURE_H_

#include <sydevs/core/number_types.h>
#include <vector>
#include <map>

namespace sydevs {
namespace systems {

class system_node;


class node_structure
{
public:
    node_structure();

    node_structure(const node_structure&)            = delete;   ///< No copy constructor
    node_structure& operator=(const node_structure&) = delete;   ///< No copy assignment
    node_structure(node_structure&&)                 = delete;   ///< No move constructor
    node_structure& operator=(node_structure&&)      = delete;   ///< No move assignment
    virtual ~node_structure()                        = default;  ///< Destructor
    
    int64 node_count() const;
    system_node& node(int64 node_index) const;
    
    int64 add_node(system_node* node_ptr);
    void erase_node(int64 node_index);

    void add_flow_inward_link(int64 src_port_index, 
                              int64 dst_node_index, int64 dst_port_index);
    const std::vector<std::pair<int64, int64>>& flow_inward_links(int64 src_port_index) const;

    void add_message_inward_link(int64 src_port_index, 
                                 int64 dst_node_index, int64 dst_port_index);
    const std::vector<std::pair<int64, int64>>& message_inward_links(int64 src_port_index) const;

    void add_flow_inner_link(int64 src_node_index, int64 src_port_index, 
                             int64 dst_node_index, int64 dst_port_index);
    const std::vector<std::pair<int64, int64>>& flow_inner_links(int64 src_node_index, int64 src_port_index) const;

    void add_message_inner_link(int64 src_node_index, int64 src_port_index, 
                                int64 dst_node_index, int64 dst_port_index);
    const std::vector<std::pair<int64, int64>>& message_inner_links(int64 src_node_index, int64 src_port_index) const;

    void add_flow_outward_link(int64 src_node_index, int64 src_port_index, 
                               int64 dst_port_index);
    const std::vector<int64>& flow_outward_links(int64 src_node_index, int64 src_port_index) const;

    void add_message_outward_link(int64 src_node_index, int64 src_port_index, 
                                  int64 dst_port_index);
    const std::vector<int64>& message_outward_links(int64 src_node_index, int64 src_port_index) const;

private:
    int64 next_node_index_;
    std::map<int64, system_node*> node_ptrs_;
    std::map<int64, std::vector<std::pair<int64, int64>>> flow_inward_links_;
    std::map<int64, std::vector<std::pair<int64, int64>>> message_inward_links_;
    std::map<std::pair<int64, int64>, std::vector<std::pair<int64, int64>>> flow_inner_links_;
    std::map<std::pair<int64, int64>, std::vector<std::pair<int64, int64>>> message_inner_links_;
    std::map<std::pair<int64, int64>, std::vector<int64>> flow_outward_links_;
    std::map<std::pair<int64, int64>, std::vector<int64>> message_outward_links_;
};


inline node_structure::node_structure()
    : next_node_index_(0)
    , node_ptrs_()
    , flow_inward_links_()
    , message_inward_links_()
    , flow_inner_links_()
    , message_inner_links_()
    , flow_outward_links_()
    , message_outward_links_()
{
}


inline int64 node_structure::node_count() const
{
    return node_ptrs_.size();
}


inline system_node& node_structure::node(int64 node_index) const
{    
    return *node_ptrs_.at(node_index);
}

    
inline int64 node_structure::add_node(system_node* node_ptr)
{
    node_ptrs_[next_node_index_] = node_ptr;
    ++next_node_index_;
    return next_node_index_ - 1;
}


inline void node_structure::erase_node(int64 node_index)
{
    node_ptrs_.erase(node_index);
}

    
inline void node_structure::add_flow_inward_link(int64 src_port_index, 
                                                 int64 dst_node_index, int64 dst_port_index)
{
    flow_inward_links_[src_port_index].emplace_back(dst_node_index, dst_port_index);
}


inline const std::vector<std::pair<int64, int64>>& node_structure::flow_inward_links(int64 src_port_index) const
{
    static const auto no_links = std::vector<std::pair<int64, int64>>();
    auto* links_ptr = &no_links;
    auto links_iter = flow_inward_links_.find(src_port_index);
    if (links_iter != std::end(flow_inward_links_)) {
        links_ptr = &(links_iter->second);
    }
    return *links_ptr;
}


inline void node_structure::add_message_inward_link(int64 src_port_index, 
                                                    int64 dst_node_index, int64 dst_port_index)
{
    message_inward_links_[src_port_index].emplace_back(dst_node_index, dst_port_index);
}


inline const std::vector<std::pair<int64, int64>>& node_structure::message_inward_links(int64 src_port_index) const
{
    static const auto no_links = std::vector<std::pair<int64, int64>>();
    auto* links_ptr = &no_links;
    auto links_iter = message_inward_links_.find(src_port_index);
    if (links_iter != std::end(message_inward_links_)) {
        links_ptr = &(links_iter->second);
    }
    return *links_ptr;

}


inline void node_structure::add_flow_inner_link(int64 src_node_index, int64 src_port_index, 
                                                int64 dst_node_index, int64 dst_port_index)
{
    flow_inner_links_[std::make_pair(src_node_index, src_port_index)].emplace_back(dst_node_index, dst_port_index);
}


inline const std::vector<std::pair<int64, int64>>& node_structure::flow_inner_links(int64 src_node_index, int64 src_port_index) const
{
    static const auto no_links = std::vector<std::pair<int64, int64>>();
    auto* links_ptr = &no_links;
    auto links_iter = flow_inner_links_.find(std::make_pair(src_node_index, src_port_index));
    if (links_iter != std::end(flow_inner_links_)) {
        links_ptr = &(links_iter->second);
    }
    return *links_ptr;
}


inline void node_structure::add_message_inner_link(int64 src_node_index, int64 src_port_index, 
                                                   int64 dst_node_index, int64 dst_port_index)
{
    message_inner_links_[std::make_pair(src_node_index, src_port_index)].emplace_back(dst_node_index, dst_port_index);
}


inline const std::vector<std::pair<int64, int64>>& node_structure::message_inner_links(int64 src_node_index, int64 src_port_index) const
{
    static const auto no_links = std::vector<std::pair<int64, int64>>();
    auto* links_ptr = &no_links;
    auto links_iter = message_inner_links_.find(std::make_pair(src_node_index, src_port_index));
    if (links_iter != std::end(message_inner_links_)) {
        links_ptr = &(links_iter->second);
    }
    return *links_ptr;
}


inline void node_structure::add_flow_outward_link(int64 src_node_index, int64 src_port_index, 
                                                  int64 dst_port_index)
{
    flow_outward_links_[std::make_pair(src_node_index, src_port_index)].emplace_back(dst_port_index);
}


inline const std::vector<int64>& node_structure::flow_outward_links(int64 src_node_index, int64 src_port_index) const
{
    static const auto no_links = std::vector<int64>();
    auto* links_ptr = &no_links;
    auto links_iter = flow_outward_links_.find(std::make_pair(src_node_index, src_port_index));
    if (links_iter != std::end(flow_outward_links_)) {
        links_ptr = &(links_iter->second);
    }
    return *links_ptr;
}


inline void node_structure::add_message_outward_link(int64 src_node_index, int64 src_port_index, 
                                                     int64 dst_port_index)
{
    message_outward_links_[std::make_pair(src_node_index, src_port_index)].emplace_back(dst_port_index);
}


inline const std::vector<int64>& node_structure::message_outward_links(int64 src_node_index, int64 src_port_index) const
{
    static const auto no_links = std::vector<int64>();
    auto* links_ptr = &no_links;
    auto links_iter = message_outward_links_.find(std::make_pair(src_node_index, src_port_index));
    if (links_iter != std::end(message_outward_links_)) {
        links_ptr = &(links_iter->second);
    }
    return *links_ptr;
}


}  // namespace
}  // namespace

#endif
