#pragma once
#ifndef SYDEVS_SYSTEMS_NODE_CONTEXT_H_
#define SYDEVS_SYSTEMS_NODE_CONTEXT_H_

#include <sydevs/systems/node_structure.h>
#include <sydevs/systems/discrete_event_time.h>
#include <memory>
#include <random>

namespace sydevs {
namespace systems {

class node_interface;


class node_context
{
public:
    node_context(const time_point& start_t, int64 seed, std::ostream& stream);
    node_context(node_interface* external_interface_ptr, node_context& parent_context);

    node_context(const node_context&)            = delete;   ///< No copy constructor
    node_context& operator=(const node_context&) = delete;   ///< No copy assignment
    node_context(node_context&&)                 = delete;   ///< No move constructor
    node_context& operator=(node_context&&)      = delete;   ///< No move assignment
    virtual ~node_context()                      = default;  ///< Destructor

    node_structure& internal_structure();
    node_interface* external_interface_ptr();
    discrete_event_time& event_time();
    std::mt19937& rng();
    bool& time_printed();
    std::ostream& stream();

private:
    void initialize_rng(int64 seed);

    node_structure internal_structure_;
    node_interface* external_interface_ptr_;
    std::unique_ptr<discrete_event_time> event_t_unique_ptr_;
    discrete_event_time* event_t_ptr_;
    std::unique_ptr<std::mt19937> rng_unique_ptr_;
    std::mt19937* rng_ptr_;
    std::unique_ptr<bool> t_printed_unique_ptr_;
    bool* t_printed_ptr_;
    std::ostream& stream_;
};


inline node_structure& node_context::internal_structure()
{
    return internal_structure_;
}


inline node_interface* node_context::external_interface_ptr()
{
    return external_interface_ptr_;
}


inline discrete_event_time& node_context::event_time()
{
    return *event_t_ptr_;
}


inline bool& node_context::time_printed()
{
    return *t_printed_ptr_;
}


inline std::mt19937& node_context::rng()
{
    return *rng_ptr_;
}


inline std::ostream& node_context::stream()
{
    return stream_;
}


}  // namespace
}  // namespace

#endif
