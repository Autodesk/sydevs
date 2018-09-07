#pragma once
#ifndef SYDEVS_EXAMPLES_DATA_CUSTOM_NODE_H_
#define SYDEVS_EXAMPLES_DATA_CUSTOM_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs {


class foo
{
public:
    int64 n;
};

template<>
struct qualified_type<foo> {
    static constexpr bool valid = true;
    static constexpr bool valid_and_sortable = false;
    static std::string tostring(const foo& X);
    static pointer copy(const foo& X);
};

inline std::string qualified_type<foo>::tostring(const foo& X)
{
    return "{" + qualified_type<int64>::tostring(X.n) + "}";
}

inline pointer qualified_type<foo>::copy(const foo& X)
{
    auto ptr = pointer(new foo());
    ptr.dereference<foo>().n = X.n;
    return ptr;
}

}

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class apple_unit;
using apple_id = identity<apple_unit>;


class data_custom_node : public atomic_node
{
public:
    // Constructor/Destructor:
    data_custom_node(const std::string& node_name, const node_context& external_context);
    virtual ~data_custom_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, foo> fi_foo;
    port<flow, input, apple_id> fi_apple_id;
    port<message, input, foo> mi_foo;
    port<message, input, apple_id> mi_apple_id;
    port<message, output, foo> mo_foo;
    port<message, output, apple_id> mo_apple_id;
    port<flow, output, foo> fo_foo;
    port<flow, output, apple_id> fo_apple_id;

protected:
    // State Variables:
    foo sv_foo;
    apple_id sv_apple_id;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline data_custom_node::data_custom_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , fi_foo("fi_foo", external_interface())
    , fi_apple_id("fi_apple_id", external_interface())
    , mi_foo("mi_foo", external_interface())
    , mi_apple_id("mi_apple_id", external_interface())
    , mo_foo("mo_foo", external_interface())
    , mo_apple_id("mo_apple_id", external_interface())
    , fo_foo("fo_foo", external_interface())
    , fo_apple_id("fo_apple_id", external_interface())
{
}


inline duration data_custom_node::initialization_event()
{
    sv_foo = fi_foo.value();
    sv_apple_id = fi_apple_id.value();
    print(sv_foo);
    print(sv_apple_id);
    return 0_s;
}


inline duration data_custom_node::unplanned_event(duration elapsed_dt)
{
    if (mi_foo.received()) {
        sv_foo = mi_foo.value();
    }
    else if (mi_apple_id.received()) {
        sv_apple_id = mi_apple_id.value();
    }
    return 0_s;
}


inline duration data_custom_node::planned_event(duration elapsed_dt)
{
    ++sv_foo.n;
    ++sv_apple_id;
    mo_foo.send(sv_foo);
    mo_apple_id.send(sv_apple_id);
    print(sv_foo);
    print(sv_apple_id);
    return duration::inf();
}


inline void data_custom_node::finalization_event(duration elapsed_dt)
{
    fo_foo.assign(sv_foo);
    fo_apple_id.assign(sv_apple_id);
}


}  // namespace

#endif
