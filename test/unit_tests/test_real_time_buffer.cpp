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

    rt_buffer.retain(tp += 50_ms, ct += std::chrono::milliseconds(50), 0, 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 75000);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(30), 0, 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 101923);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(20), 0, 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 125000);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(30), 0, 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 150862);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(20), 0, 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 175000);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(30), 0, 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 200675);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(20), 0, 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 225000);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(20), 0, 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 249719);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(30), 0, 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 275000);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(25), 0, 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 300000);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(100), 0, 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 400000);

    rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(1), 0, 25_ms);

    CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 420121);
}


}  // namespace
}  // namespace
