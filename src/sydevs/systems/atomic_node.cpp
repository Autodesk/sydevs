#include <sydevs/systems/atomic_node.h>

namespace sydevs {
namespace systems {


duration atomic_node::handle_initialization_event()
{
    if (initialized_) throw std::logic_error("Attempt to initalize atomic node (" + full_name() + ") more than once");
    ET().start();
    auto dt = initialization_event();
    ET().stop();
    auto planned_dt = scale_planned_dt(dt);
    external_IO().print_planned_duration(planned_dt);
    initialized_ = true;
    return planned_dt;
}


duration atomic_node::handle_unplanned_event(duration elapsed_dt)
{
    external_IO().print_elapsed_duration(elapsed_dt);
    ET().start();
    auto dt = unplanned_event(elapsed_dt);
    ET().stop();
    auto planned_dt = scale_planned_dt(dt);
    external_IO().print_planned_duration(planned_dt);
    return planned_dt;
}


duration atomic_node::handle_planned_event(duration elapsed_dt)
{
    external_IO().print_elapsed_duration(elapsed_dt);
    ET().start();
    auto dt = planned_event(elapsed_dt);
    ET().stop();
    auto planned_dt = scale_planned_dt(dt);
    external_IO().print_planned_duration(planned_dt);
    return planned_dt;
}


void atomic_node::handle_finalization_event(duration elapsed_dt)
{
    if (finalized_) throw std::logic_error("Attempt to finalize atomic node (" + full_name() + ") more than once");
    external_IO().print_elapsed_duration(elapsed_dt);
    ET().start();
    finalization_event(elapsed_dt);
    ET().stop();
    finalized_ = true;
}


}  // namespace
}  // namespace


