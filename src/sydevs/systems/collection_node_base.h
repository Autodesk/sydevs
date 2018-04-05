#pragma once
#ifndef SYDEVS_SYSTEMS_COLLECTION_NODE_BASE_H_
#define SYDEVS_SYSTEMS_COLLECTION_NODE_BASE_H_

#include <sydevs/systems/system_node.h>

namespace sydevs {
namespace systems {


/**
 * @brief A base class for the collection node class template.
 *
 * @details
 * `collection_node_base` defines and provides access to a context
 * representation specifically intended for the prototype node. The prototype
 * node is defined in the `collection_node` class template, which derives from
 * this base class. This arrangement ensures that the prototype's context is
 * initialized prior to the prototype node itself.
 */
class collection_node_base : public system_node
{
public:
    virtual ~collection_node_base() = default;  ///< Destructor

    data_mode node_dmode() const;  ///< Returns `message` for all collection nodes.

protected: 
    /**
     * @brief Constructs a `collection_node_base`.
     * 
     * @details
     * Constructs the collection node base and its associated `node_interface`
     * object. Also initializes the prototype context.
     *
     * @param node_name The name of the node within the encompassing context.
     * @param external_context The context in which the node is constructed.
     */
    collection_node_base(const std::string& node_name, const node_context& external_context);

    const node_context& prototype_context();  ///< Returns a const reference to the prototype node's context object.
    node_structure& prototype_structure();    ///< Returns a non-const reference to the prototype node's encompassing structure object.

private: 
    node_context prototype_context_;
};

    
inline data_mode collection_node_base::node_dmode() const
{
    return message;
}


inline collection_node_base::collection_node_base(const std::string& node_name, const node_context& external_context)
    : system_node(node_name, external_context)
    , prototype_context_(&const_cast<node_interface&>(external_interface()),
                         const_cast<node_context&>(external_context))
{
}


inline const node_context& collection_node_base::prototype_context()
{
    return const_cast<const node_context&>(prototype_context_);
}


inline node_structure& collection_node_base::prototype_structure()
{
    return prototype_context_.internal_structure();
}


}  // namespace
}  // namespace

#endif
