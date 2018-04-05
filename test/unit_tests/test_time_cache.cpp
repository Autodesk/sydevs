#include <catch2/catch.hpp>
#include <sydevs/time/time_cache.h>
#include <sydevs/core/string_builder.h>
#include <sydevs/core/quantity.h>

using std::begin;
using std::end;

namespace sydevs {


void test_time_cache_basic_operations(const time_point& tp0)
{
    auto tc = time_cache(tp0);

    {
        CHECK(tc.empty() == true);
        CHECK(tc.size() == 0);
        CHECK(tc.current_time() == tp0);
    }

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 0);
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "duration::inf()");
    }

    tc.retain_event(0, yocto);

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 1);
        CHECK(ids.find(0) != end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "0_ys");
        CHECK((string_builder() << tc.duration_since(1)).str() == "duration::inf()");
    }

    tc.advance_time(duration(1, yocto - 1));

    {
        CHECK(tc.empty() == false);
        CHECK(tc.size() == 1);
        CHECK(tc.current_time() == tp0 + duration(1, scale(-9)));
    }

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 1);
        CHECK(ids.find(0) != end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "0_ys");
        CHECK((string_builder() << tc.duration_since(1)).str() == "duration::inf()");
    }

    tc.release_event(0);

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 0);
        CHECK(ids.find(0) == end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "duration::inf()");
    }

    tc.retain_event(0, yocto);
    tc.retain_event(1, micro);

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 2);
        CHECK(ids.find(0) != end(ids));
        CHECK(ids.find(1) != end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "0_ys");
        CHECK((string_builder() << tc.duration_since(1)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(2)).str() == "duration::inf()");
    }

    tc.advance_time(1_ps);

    {
        CHECK(tc.empty() == false);
        CHECK(tc.size() == 2);
        CHECK(tc.current_time() == tp0 + 1_ps);
    }

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 2);
        CHECK(ids.find(0) != end(ids));
        CHECK(ids.find(1) != end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "1000000000000_ys");
        CHECK((string_builder() << tc.duration_since(1)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(2)).str() == "duration::inf()");
    }

    tc.retain_event(2, micro);
    tc.advance_time(1_ps);

    {
        CHECK(tc.empty() == false);
        CHECK(tc.size() == 3);
        CHECK(tc.current_time() == tp0 + 2_ps);
    }

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 3);
        CHECK(ids.find(0) != end(ids));
        CHECK(ids.find(1) != end(ids));
        CHECK(ids.find(2) != end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "2000000000000_ys");
        CHECK((string_builder() << tc.duration_since(1)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(2)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(3)).str() == "duration::inf()");
    }

    tc.advance_time(997999999999998_ys);

    {
        CHECK(tc.empty() == false);
        CHECK(tc.size() == 3);
        CHECK(tc.current_time() == tp0 + 999_ps + 999_fs + 999_as + 999_zs + 998_ys);
    }

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 3);
        CHECK(ids.find(0) != end(ids));
        CHECK(ids.find(1) != end(ids));
        CHECK(ids.find(2) != end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "999999999999998_ys");
        CHECK((string_builder() << tc.duration_since(1)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(2)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(3)).str() == "duration::inf()");
    }

    tc.advance_time(duration(999, scale(-9)));

    {
        CHECK(tc.empty() == false);
        CHECK(tc.size() == 3);
        CHECK(tc.current_time() == tp0 + 999_ps + 999_fs + 999_as + 999_zs + 998_ys + duration(999, scale(-9)));
    }

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 3);
        CHECK(ids.find(0) != end(ids));
        CHECK(ids.find(1) != end(ids));
        CHECK(ids.find(2) != end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "999999999999998_ys");
        CHECK((string_builder() << tc.duration_since(1)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(2)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(3)).str() == "duration::inf()");
    }

    tc.advance_time(duration(1, scale(-9)));

    {
        CHECK(tc.empty() == false);
        CHECK(tc.size() == 3);
        CHECK(tc.current_time() == tp0 + 999_ps + 999_fs + 999_as + 999_zs + 999_ys);
    }

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 3);
        CHECK(ids.find(0) != end(ids));
        CHECK(ids.find(1) != end(ids));
        CHECK(ids.find(2) != end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "999999999999999_ys");
        CHECK((string_builder() << tc.duration_since(1)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(2)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(3)).str() == "duration::inf()");
    }

    tc.advance_time(duration(1, scale(-9)));

    {
        CHECK(tc.empty() == false);
        CHECK(tc.size() == 2);
        CHECK(tc.current_time() == tp0 + 999_ps + 999_fs + 999_as + 999_zs + 999_ys + duration(1, scale(-9)));
    }

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 2);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) != end(ids));
        CHECK(ids.find(2) != end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(1)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(2)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(3)).str() == "duration::inf()");
    }

    tc.advance_time(duration(999, scale(-9)));

    {
        CHECK(tc.empty() == false);
        CHECK(tc.size() == 2);
        CHECK(tc.current_time() == tp0 + 1_ns);
    }

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 2);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) != end(ids));
        CHECK(ids.find(2) != end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(1)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(2)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(3)).str() == "duration::inf()");
    }

    tc.retain_event(3, unit);
    tc.retain_event(4, kilo);
    tc.retain_event(5, micro);
    tc.retain_event(6, nano);
    tc.retain_event(2, femto);

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 6);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) != end(ids));
        CHECK(ids.find(2) != end(ids));
        CHECK(ids.find(3) != end(ids));
        CHECK(ids.find(4) != end(ids));
        CHECK(ids.find(5) != end(ids));
        CHECK(ids.find(6) != end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(1)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(2)).str() == "0_fs");
        CHECK((string_builder() << tc.duration_since(3)).str() == "0_s");
        CHECK((string_builder() << tc.duration_since(4)).str() == "0_ks");
        CHECK((string_builder() << tc.duration_since(5)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(6)).str() == "0_ns");
        CHECK((string_builder() << tc.duration_since(7)).str() == "duration::inf()");
    }

    tc.advance_time(1_fs);

    {
        CHECK(tc.empty() == false);
        CHECK(tc.size() == 6);
        CHECK(tc.current_time() == tp0 + 1_ns + 1_fs);
    }

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 6);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) != end(ids));
        CHECK(ids.find(2) != end(ids));
        CHECK(ids.find(3) != end(ids));
        CHECK(ids.find(4) != end(ids));
        CHECK(ids.find(5) != end(ids));
        CHECK(ids.find(6) != end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(1)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(2)).str() == "1_fs");
        CHECK((string_builder() << tc.duration_since(3)).str() == "0_s");
        CHECK((string_builder() << tc.duration_since(4)).str() == "0_ks");
        CHECK((string_builder() << tc.duration_since(5)).str() == "0_us");
        CHECK((string_builder() << tc.duration_since(6)).str() == "0_ns");
        CHECK((string_builder() << tc.duration_since(7)).str() == "duration::inf()");
    }

    tc.advance_time(1_yr);

    {
        CHECK(tc.empty() == false);
        CHECK(tc.size() == 4);
        CHECK(tc.current_time() == tp0 + 31_Ms + 536_ks);
    }

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 4);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) != end(ids));
        CHECK(ids.find(2) == end(ids));
        CHECK(ids.find(3) != end(ids));
        CHECK(ids.find(4) != end(ids));
        CHECK(ids.find(5) != end(ids));
        CHECK(ids.find(6) == end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(1)).str() == "31536000000000_us");
        CHECK((string_builder() << tc.duration_since(2)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(3)).str() == "31536000_s");
        CHECK((string_builder() << tc.duration_since(4)).str() == "31536_ks");
        CHECK((string_builder() << tc.duration_since(5)).str() == "31536000000000_us");
        CHECK((string_builder() << tc.duration_since(6)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(7)).str() == "duration::inf()");
    }

    tc.retain_event(3, unit);
    tc.advance_time(1_yr);

    {
        CHECK(tc.empty() == false);
        CHECK(tc.size() == 4);
        CHECK(tc.current_time() == tp0 + 63_Ms + 72_ks);
    }

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 4);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) != end(ids));
        CHECK(ids.find(2) == end(ids));
        CHECK(ids.find(3) != end(ids));
        CHECK(ids.find(4) != end(ids));
        CHECK(ids.find(5) != end(ids));
        CHECK(ids.find(6) == end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(1)).str() == "63072000000000_us");
        CHECK((string_builder() << tc.duration_since(2)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(3)).str() == "31536000_s");
        CHECK((string_builder() << tc.duration_since(4)).str() == "63072_ks");
        CHECK((string_builder() << tc.duration_since(5)).str() == "63072000000000_us");
        CHECK((string_builder() << tc.duration_since(6)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(7)).str() == "duration::inf()");
    }

    tc.advance_time(30_yr);

    {
        CHECK(tc.empty() == false);
        CHECK(tc.size() == 2);
        CHECK(tc.current_time() == tp0 + 1_Gs + 9_Ms + 152_ks);
    }

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 2);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) == end(ids));
        CHECK(ids.find(2) == end(ids));
        CHECK(ids.find(3) != end(ids));
        CHECK(ids.find(4) != end(ids));
        CHECK(ids.find(5) == end(ids));
        CHECK(ids.find(6) == end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(1)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(2)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(3)).str() == "977616000_s");
        CHECK((string_builder() << tc.duration_since(4)).str() == "1009152_ks");
        CHECK((string_builder() << tc.duration_since(5)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(6)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(7)).str() == "duration::inf()");
    }

    tc.advance_time(1_yr);

    {
        CHECK(tc.empty() == false);
        CHECK(tc.size() == 2);
        CHECK(tc.current_time() == tp0 + 1_Gs + 40_Ms + 688_ks);
    }

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 2);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) == end(ids));
        CHECK(ids.find(2) == end(ids));
        CHECK(ids.find(3) != end(ids));
        CHECK(ids.find(4) != end(ids));
        CHECK(ids.find(5) == end(ids));
        CHECK(ids.find(6) == end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(1)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(2)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(3)).str() == "1009152000_s");
        CHECK((string_builder() << tc.duration_since(4)).str() == "1040688_ks");
        CHECK((string_builder() << tc.duration_since(5)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(6)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(7)).str() == "duration::inf()");
    }

    tc.release_event(3);
    tc.release_event(5);

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 1);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) == end(ids));
        CHECK(ids.find(2) == end(ids));
        CHECK(ids.find(3) == end(ids));
        CHECK(ids.find(4) != end(ids));
        CHECK(ids.find(5) == end(ids));
        CHECK(ids.find(6) == end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(0)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(1)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(2)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(3)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(4)).str() == "1040688_ks");
        CHECK((string_builder() << tc.duration_since(5)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(6)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(7)).str() == "duration::inf()");
    }
    
    tc.retain_event(-1, milli);

    {
        auto ids = tc.event_ids();
        CHECK(ids.size() == 2);
        CHECK(ids.find(-1) != end(ids));
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) == end(ids));
        CHECK(ids.find(2) == end(ids));
        CHECK(ids.find(3) == end(ids));
        CHECK(ids.find(4) != end(ids));
        CHECK(ids.find(5) == end(ids));
        CHECK(ids.find(6) == end(ids));
    }

    {
        CHECK((string_builder() << tc.duration_since(-1)).str() == "0_ms");
        CHECK((string_builder() << tc.duration_since(0)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(1)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(2)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(3)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(4)).str() == "1040688_ks");
        CHECK((string_builder() << tc.duration_since(5)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(6)).str() == "duration::inf()");
        CHECK((string_builder() << tc.duration_since(7)).str() == "duration::inf()");
    }

    tc.advance_time(1_Es);
    
    if (tp0 <= time_point()) {

        {
            CHECK(tc.empty() == true);
            CHECK(tc.size() == 0);
            CHECK(tc.current_time() == 1_Es);
        }

        {
            auto ids = tc.event_ids();
            CHECK(ids.size() == 0);
            CHECK(ids.find(-1) == end(ids));
            CHECK(ids.find(0) == end(ids));
            CHECK(ids.find(1) == end(ids));
            CHECK(ids.find(2) == end(ids));
            CHECK(ids.find(3) == end(ids));
            CHECK(ids.find(4) == end(ids));
            CHECK(ids.find(5) == end(ids));
            CHECK(ids.find(6) == end(ids));
        }

        {
            CHECK((string_builder() << tc.duration_since(-1)).str() == "duration::inf()");
            CHECK((string_builder() << tc.duration_since(0)).str() == "duration::inf()");
            CHECK((string_builder() << tc.duration_since(1)).str() == "duration::inf()");
            CHECK((string_builder() << tc.duration_since(2)).str() == "duration::inf()");
            CHECK((string_builder() << tc.duration_since(3)).str() == "duration::inf()");
            CHECK((string_builder() << tc.duration_since(4)).str() == "duration::inf()");
            CHECK((string_builder() << tc.duration_since(5)).str() == "duration::inf()");
            CHECK((string_builder() << tc.duration_since(6)).str() == "duration::inf()");
            CHECK((string_builder() << tc.duration_since(7)).str() == "duration::inf()");
        }

    }
    else {

        {
            CHECK(tc.empty() == false);
            CHECK(tc.size() == 1);
            CHECK(tc.current_time() == 1_Es);
        }

        {
            auto ids = tc.event_ids();
            CHECK(ids.size() == 1);
            CHECK(ids.find(-1) == end(ids));
            CHECK(ids.find(0) == end(ids));
            CHECK(ids.find(1) == end(ids));
            CHECK(ids.find(2) == end(ids));
            CHECK(ids.find(3) == end(ids));
            CHECK(ids.find(4) != end(ids));
            CHECK(ids.find(5) == end(ids));
            CHECK(ids.find(6) == end(ids));
        }

        {
            CHECK((string_builder() << tc.duration_since(-1)).str() == "duration::inf()");
            CHECK((string_builder() << tc.duration_since(0)).str() == "duration::inf()");
            CHECK((string_builder() << tc.duration_since(1)).str() == "duration::inf()");
            CHECK((string_builder() << tc.duration_since(2)).str() == "duration::inf()");
            CHECK((string_builder() << tc.duration_since(3)).str() == "duration::inf()");
            CHECK((string_builder() << tc.duration_since(4)).str() == "999999999968464_ks");
            CHECK((string_builder() << tc.duration_since(5)).str() == "duration::inf()");
            CHECK((string_builder() << tc.duration_since(6)).str() == "duration::inf()");
            CHECK((string_builder() << tc.duration_since(7)).str() == "duration::inf()");
        }

    }
}
    
TEST_CASE("test time cache basic operations")
{
    test_time_cache_basic_operations(time_point());
}

TEST_CASE("test time cache basic operations with negative time points")
{
    test_time_cache_basic_operations(time_point() - 1_yr);
}


TEST_CASE("test time cache basic operations with positive time points")
{
    test_time_cache_basic_operations(time_point() + 1_yr);
}



}  // namespace
