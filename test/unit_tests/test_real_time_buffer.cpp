#include <catch2/catch.hpp>
#include <sydevs/systems/real_time_buffer.h>
#include <sydevs/core/string_builder.h>

using std::begin;
using std::end;

namespace sydevs {
namespace systems {


TEST_CASE("test real time buffer")
{
    auto us_from_clock_t = [](const clock_time& clock_t) {
        return std::chrono::duration_cast<std::chrono::microseconds>(clock_t.time_since_epoch()).count();
    };

    auto rt_buffer = real_time_buffer(1.0, 1.0);
    auto tp = time_point();
    auto ct = clock_time();

    {
        rt_buffer.update_current_time(tp += 50_ms, ct += std::chrono::milliseconds(50), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 75000);
    }
    {
        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(30), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 103000);

        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(20), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 125000);
    }
    {
        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(30), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 153000);

        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(20), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 175000);
    }
    {
        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(30), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 203000);

        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(20), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 225000);
    }
    {
        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(20), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 246000);

        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(30), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 275000);
    }
    {
        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(25), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 300000);
    }
    {
        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 394000);
    }
    {
        rt_buffer.update_time_synchronization_rate(0.0);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 400000);

        rt_buffer.update_time_synchronization_rate(10.0);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 387000);

        rt_buffer.update_time_synchronization_rate(100.0);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 387000);

        rt_buffer.update_time_synchronization_rate(1000.0);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 387000);

        rt_buffer.update_time_synchronization_rate(1.0);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 394000);
    }
    {
        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(1), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 392000);

        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(25), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 416000);

        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(99), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 510000);
    }
    {
        rt_buffer.update_synchronization_time(rt_buffer.current_time(), rt_buffer.current_clock_time());

        rt_buffer.update_time_advancement_rate(1);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 525000);

        rt_buffer.update_time_advancement_rate(2);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 512000);

        rt_buffer.update_time_advancement_rate(5);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 505000);

        rt_buffer.update_time_advancement_rate(0.5);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 550000);

        rt_buffer.update_time_advancement_rate(0.25);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 600000);
    }
    {
        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 700000);

        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 800000);

        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 900000);

        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 1000000);

        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 1100000);

        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 1200000);

        rt_buffer.update_current_time(tp += 25_ms, ct += std::chrono::milliseconds(100), 25_ms);
        CHECK(us_from_clock_t(rt_buffer.planned_clock_time()) == 1300000);
    }
}


}  // namespace
}  // namespace
