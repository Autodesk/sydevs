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

    {
        rt_buffer.retain(tp += 50_ms, ct += std::chrono::milliseconds(50), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 75000);
    }
    {
        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(30), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 101824);

        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(20), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 125000);
    }
    {
        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(30), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 150866);

        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(20), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 175000);
    }
    {
        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(30), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 200866);

        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(20), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 225000);
    }
    {
        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(20), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 249758);

        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(30), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 275000);
    }
    {
        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(25), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 300000);
    }
    {
        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 381250);
    }
    {
        rt_buffer.update_time_advancement_depth(5);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 381250);

        rt_buffer.update_time_advancement_depth(4);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 381250);

        rt_buffer.update_time_advancement_depth(3);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 383333);

        rt_buffer.update_time_advancement_depth(2);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 387500);

        rt_buffer.update_time_advancement_depth(1);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 400000);

        rt_buffer.update_time_advancement_depth(0);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 0);

        rt_buffer.update_time_advancement_depth(6);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 381250);
    }
    {
        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(1), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 407294);

        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(25), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 432294);

        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(99), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 506250);
    }
    {
        rt_buffer.update_time_advancement_rate(1);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 525000);

        rt_buffer.update_time_advancement_rate(2);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 512500);

        rt_buffer.update_time_advancement_rate(5);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 505000);

        rt_buffer.update_time_advancement_rate(0.5);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 550000);

        rt_buffer.update_time_advancement_rate(0.25);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 600000);
    }
    {
        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 700000);

        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 800000);

        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 900000);

        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 1000000);

        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 1100000);

        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 1200000);

        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 1300000);
    }
    {
        rt_buffer.retain(tp += 25_ms, ct += std::chrono::milliseconds(200), 25_ms);
        CHECK(rt_buffer.time_advancement_depth() == 6);
        CHECK(rt_buffer.max_depth() == 4);
        CHECK(rt_buffer.cached_time() == time_point() + 575_ms);
        CHECK(rt_buffer.cached_time(0) == time_point() + 575_ms);
        CHECK(rt_buffer.cached_time(1) == time_point() + 550_ms);
        CHECK(rt_buffer.cached_time(2) == time_point() + 500_ms);
        CHECK(rt_buffer.cached_time(3) == time_point() + 400_ms);
        CHECK(ms_from_clock_t(rt_buffer.cached_clock_time()) == 1400000);
        CHECK(ms_from_clock_t(rt_buffer.cached_clock_time(0)) == 1400000);
        CHECK(ms_from_clock_t(rt_buffer.cached_clock_time(1)) == 1200000);
        CHECK(ms_from_clock_t(rt_buffer.cached_clock_time(2)) == 1000000);
        CHECK(ms_from_clock_t(rt_buffer.cached_clock_time(3)) == 600000);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 1425000);

        rt_buffer.update_time_advancement_depth(1);
        CHECK(rt_buffer.time_advancement_depth() == 1);
        CHECK(rt_buffer.max_depth() == 4);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 1500000);

        rt_buffer.update_time_advancement_depth(4);
        CHECK(rt_buffer.time_advancement_depth() == 4);
        CHECK(rt_buffer.max_depth() == 4);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 1425000);

        rt_buffer.update_time_advancement_rate(0.25);
        CHECK(rt_buffer.time_advancement_depth() == 4);
        CHECK(rt_buffer.max_depth() == 1);
        CHECK(ms_from_clock_t(rt_buffer.planned_clock_time()) == 1500000);
    }
}


}  // namespace
}  // namespace
