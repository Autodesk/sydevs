#include <sydevs/systems/node_context.h>
#include <sydevs/core/string_builder.h>
#include <array>
#include <algorithm>

namespace sydevs {
namespace systems {


node_context::node_context(const time_point& start_t, int64 seed, std::ostream& stream)
    : internal_structure_()
    , external_interface_ptr_(nullptr)
    , event_t_unique_ptr_(new discrete_event_time(start_t))
    , event_t_ptr_(nullptr)
    , rng_unique_ptr_()
    , rng_ptr_(nullptr)
    , t_printed_unique_ptr_(new bool(false))
    , t_printed_ptr_(nullptr)
    , stream_(stream)
{
    event_t_ptr_ = event_t_unique_ptr_.get();
    initialize_rng(seed);
    rng_ptr_ = rng_unique_ptr_.get();
    t_printed_ptr_ = t_printed_unique_ptr_.get();
}


node_context::node_context(node_interface* external_interface_ptr, node_context& parent_context)
    : internal_structure_()
    , external_interface_ptr_(external_interface_ptr)
    , event_t_unique_ptr_()
    , event_t_ptr_(&parent_context.event_time())
    , rng_unique_ptr_()
    , rng_ptr_(&parent_context.rng())
    , t_printed_unique_ptr_()
    , t_printed_ptr_(&parent_context.time_printed())
    , stream_(parent_context.stream())
{
}


void node_context::initialize_rng(int64 seed)
{
    const auto state_size = std::mt19937::state_size;
    const auto lc_discard_count = 10;
    const auto mt_discard_count = state_size;
    const auto seed_limit = 4000000000;
    if (seed < 0) throw std::invalid_argument("Random seed must be at least zero");
    if (seed >= seed_limit) throw std::invalid_argument((string_builder() << "Random seed must be less than " << seed_limit).str());
    auto adjusted_seed = std::numeric_limits<std::uint32_t>::max() - seed_limit + seed;
    auto lc_generator = std::minstd_rand0(adjusted_seed);
    auto seed_data = std::array<std::uint32_t, state_size>();
    lc_generator.discard(lc_discard_count);
    std::generate_n(seed_data.data(), state_size, lc_generator);
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    rng_unique_ptr_.reset(new std::mt19937(seq));
    rng_unique_ptr_->discard(mt_discard_count);
}


}  // namespace
}  // namespace
