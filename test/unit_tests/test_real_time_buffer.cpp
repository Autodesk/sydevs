#include <catch2/catch.hpp>
#include <sydevs/systems/real_time_buffer.h>
#include <sydevs/core/string_builder.h>

using std::begin;
using std::end;

namespace sydevs {
namespace systems {


TEST_CASE("test real time buffer")
{
    auto ms_from_clock_t = [](const clock_time& clock_t) {
        return std::chrono::duration_cast<std::chrono::microseconds>(clock_t.time_since_epoch()).count();
    };

    auto rt_buffer = real_time_buffer(1, 6);
    auto tp = time_point();
    auto ct = clock_time();

    rt_buffer.retain(tp += 50_ms, ct += std::chrono::milliseconds(50), 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 75000);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(30), 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 101987);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(20), 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 125000);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(30), 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 151038);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(20), 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 175000);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(30), 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 201038);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(20), 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 225000);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(20), 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 249827);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(30), 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 275000);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(25), 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 300000);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 388333);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(1), 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 420261);
}


}  // namespace
}  // namespace
