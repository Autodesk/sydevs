#include <catch2/catch.hpp>
#include <sydevs/time/time_queue.h>
#include <sydevs/core/string_builder.h>

using std::begin;
using std::end;

namespace sydevs {


void test_time_queue_basic_operations(const time_point& tp0)
{
    auto tq = time_queue(tp0);

    {
        CHECK(tq.empty() == true);
        CHECK(tq.size() == 0);
        CHECK(tq.time_count() == 0);
        CHECK(tq.current_time() == tp0);
        CHECK(tq.imminent_duration() == duration::inf());
    }

    {
        auto errmsg = std::string();
        try {
            tq.imminent_event_ids();
        }
        catch (const std::out_of_range& e) {
            errmsg = e.what();
        }
        CHECK(errmsg == "No imminent events exist");
    }

    {
        CHECK(tq.duration_until(0) == duration::inf());
    }

    tq.plan_event(0, 3_min + 15_s);

    {
        CHECK(tq.empty() == false);
        CHECK(tq.size() == 1);
        CHECK(tq.time_count() == 1);
        CHECK(tq.current_time() == tp0);
        CHECK(tq.imminent_duration() == 195_s);
    }

    {
        auto ids = tq.imminent_event_ids();
        CHECK(ids.size() == 1);
        CHECK(ids.find(0) != end(ids));
    }

    {
        CHECK(tq.duration_until(0) == 195_s);
        CHECK(tq.duration_until(1) == duration::inf());
    }

    tq.plan_event(1, 10_Ms);

    {
        CHECK(tq.empty() == false);
        CHECK(tq.size() == 2);
        CHECK(tq.time_count() == 2);
        CHECK(tq.current_time() == tp0);
        CHECK(tq.imminent_duration() == 195_s);
    }

    {
        auto ids = tq.imminent_event_ids();
        CHECK(ids.size() == 1);
        CHECK(ids.find(0) != end(ids));
        CHECK(ids.find(1) == end(ids));
    }

    {
        CHECK(tq.duration_until(0) == 195_s);
        CHECK(tq.duration_until(1) == 10_Ms);
        CHECK(tq.duration_until(2) == duration::inf());
    }

    tq.plan_event(2, 1_min);

    {
        CHECK(tq.empty() == false);
        CHECK(tq.size() == 3);
        CHECK(tq.time_count() == 3);
        CHECK(tq.current_time() == tp0);
        CHECK(tq.imminent_duration() == 60_s);
    }

    {
        auto ids = tq.imminent_event_ids();
        CHECK(ids.size() == 1);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) == end(ids));
        CHECK(ids.find(2) != end(ids));
    }

    {
        CHECK(tq.duration_until(0) == 195_s);
        CHECK(tq.duration_until(1) == 10_Ms);
        CHECK(tq.duration_until(2) == 60_s);
        CHECK(tq.duration_until(3) == duration::inf());
    }

    tq.plan_event(3, 3_min + 15_s);

    {
        CHECK(tq.empty() == false);
        CHECK(tq.size() == 4);
        CHECK(tq.time_count() == 3);
        CHECK(tq.current_time() == tp0);
        CHECK(tq.imminent_duration() == 60_s);
    }

    {
        auto ids = tq.imminent_event_ids();
        CHECK(ids.size() == 1);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) == end(ids));
        CHECK(ids.find(2) != end(ids));
        CHECK(ids.find(3) == end(ids));
    }

    {
        CHECK(tq.duration_until(0) == 195_s);
        CHECK(tq.duration_until(1) == 10_Ms);
        CHECK(tq.duration_until(2) == 60_s);
        CHECK(tq.duration_until(3) == 195_s);
        CHECK(tq.duration_until(4) == duration::inf());
    }

    tq.advance_time(1_ys);

    {
        CHECK(tq.current_time() == tp0 + 1_ys);
        CHECK(tq.imminent_duration() == 60_s);
    }

    {
        auto ids = tq.imminent_event_ids();
        CHECK(ids.size() == 1);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) == end(ids));
        CHECK(ids.find(2) != end(ids));
        CHECK(ids.find(3) == end(ids));
    }

    {
        CHECK(tq.duration_until(0) == 195_s);
        CHECK(tq.duration_until(1) == 10_Ms);
        CHECK(tq.duration_until(2) == 60_s);
        CHECK(tq.duration_until(3) == 195_s);
        CHECK(tq.duration_until(4) == duration::inf());
    }

    tq.advance_time();

    {
        CHECK(tq.current_time() == tp0 + 60_s);
        CHECK(tq.imminent_duration() == 0_s);
    }

    {
        auto ids = tq.imminent_event_ids();
        CHECK(ids.size() == 1);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) == end(ids));
        CHECK(ids.find(2) != end(ids));
        CHECK(ids.find(3) == end(ids));
    }

    {
        CHECK(tq.duration_until(0) == 135_s);
        CHECK(tq.duration_until(1) == 10_Ms);
        CHECK(tq.duration_until(2) == 0_s);
        CHECK(tq.duration_until(3) == 135_s);
        CHECK(tq.duration_until(4) == duration::inf());
    }

    tq.pop_imminent_event(2);

    {
        CHECK(tq.empty() == false);
        CHECK(tq.size() == 3);
        CHECK(tq.time_count() == 2);
        CHECK(tq.current_time() == tp0 + 60_s);
        CHECK(tq.imminent_duration() == 135_s);
    }

    {
        auto ids = tq.imminent_event_ids();
        CHECK(ids.size() == 2);
        CHECK(ids.find(0) != end(ids));
        CHECK(ids.find(1) == end(ids));
        CHECK(ids.find(2) == end(ids));
        CHECK(ids.find(3) != end(ids));
    }

    {
        CHECK(tq.duration_until(0) == 135_s);
        CHECK(tq.duration_until(1) == 10_Ms);
        CHECK(tq.duration_until(2) == duration::inf());
        CHECK(tq.duration_until(3) == 135_s);
        CHECK(tq.duration_until(4) == duration::inf());
    }

    tq.plan_event(4, 10_Ms);
    tq.plan_event(5, 1_Ts);
    tq.pop_imminent_event(3);
    tq.pop_imminent_event(0);

    {
        CHECK(tq.empty() == false);
        CHECK(tq.size() == 3);
        CHECK(tq.time_count() == 2);
        CHECK(tq.current_time() == tp0 + 60_s);
        CHECK(tq.imminent_duration() == 10_Ms);
    }

    {
        auto ids = tq.imminent_event_ids();
        CHECK(ids.size() == 2);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) != end(ids));
        CHECK(ids.find(2) == end(ids));
        CHECK(ids.find(3) == end(ids));
        CHECK(ids.find(4) != end(ids));
        CHECK(ids.find(5) == end(ids));
    }

    {
        CHECK(tq.duration_until(0) == duration::inf());
        CHECK(tq.duration_until(1) == 10_Ms);
        CHECK(tq.duration_until(2) == duration::inf());
        CHECK(tq.duration_until(3) == duration::inf());
        CHECK(tq.duration_until(4) == 10_Ms);
        CHECK(tq.duration_until(5) == 1_Ts);
        CHECK(tq.duration_until(6) == duration::inf());
    }

    tq.advance_time(tq.imminent_duration());

    {
        CHECK(tq.current_time() == tp0 + 10_Ms);
        CHECK(tq.imminent_duration() == 0_s);
    }

    {
        auto ids = tq.imminent_event_ids();
        CHECK(ids.size() == 2);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) != end(ids));
        CHECK(ids.find(2) == end(ids));
        CHECK(ids.find(3) == end(ids));
        CHECK(ids.find(4) != end(ids));
        CHECK(ids.find(5) == end(ids));
    }

    {
        CHECK(tq.duration_until(0) == duration::inf());
        CHECK(tq.duration_until(1) == 0_Ms);
        CHECK(tq.duration_until(2) == duration::inf());
        CHECK(tq.duration_until(3) == duration::inf());
        CHECK(tq.duration_until(4) == 0_Ms);
        CHECK(tq.duration_until(5) == 1_Ts);
        CHECK(tq.duration_until(6) == duration::inf());
    }

    tq.pop_imminent_events();

    {
        CHECK(tq.empty() == false);
        CHECK(tq.size() == 1);
        CHECK(tq.time_count() == 1);
        CHECK(tq.current_time() == tp0 + 10_Ms);
        if (tp0.sign() == 1) {
            CHECK(tq.imminent_duration() == 1_Ts);
        }
        else {
            CHECK(tq.imminent_duration() == 1_Ps);
        }
    }

    {
        auto ids = tq.imminent_event_ids();
        CHECK(ids.size() == 1);
        CHECK(ids.find(0) == end(ids));
        CHECK(ids.find(1) == end(ids));
        CHECK(ids.find(2) == end(ids));
        CHECK(ids.find(3) == end(ids));
        CHECK(ids.find(4) == end(ids));
        CHECK(ids.find(5) != end(ids));
    }

    {
        CHECK(tq.duration_until(0) == duration::inf());
        CHECK(tq.duration_until(1) == duration::inf());
        CHECK(tq.duration_until(2) == duration::inf());
        CHECK(tq.duration_until(3) == duration::inf());
        CHECK(tq.duration_until(4) == duration::inf());
        CHECK(tq.duration_until(5) == 1_Ts);
        CHECK(tq.duration_until(6) == duration::inf());
    }

    tq.advance_time();
    tq.pop_imminent_event(5);

    {
        CHECK(tq.empty() == true);
        CHECK(tq.size() == 0);
        CHECK(tq.time_count() == 0);
        CHECK(tq.current_time() == tp0 + 1_Ts);
        CHECK(tq.imminent_duration() == duration::inf());
    }

    {
        auto errmsg = std::string();
        try {
            tq.imminent_event_ids();
        }
        catch (const std::out_of_range& e) {
            errmsg = e.what();
        }
        CHECK(errmsg == "No imminent events exist");
    }

    {
        CHECK(tq.duration_until(0) == duration::inf());
        CHECK(tq.duration_until(1) == duration::inf());
        CHECK(tq.duration_until(2) == duration::inf());
        CHECK(tq.duration_until(3) == duration::inf());
        CHECK(tq.duration_until(4) == duration::inf());
        CHECK(tq.duration_until(5) == duration::inf());
        CHECK(tq.duration_until(6) == duration::inf());
    }
}

void test_time_queue_proximal_events(const time_point& tp0)
{
    auto tq = time_queue(tp0);

    tq.plan_event(0, 0_s);
    tq.plan_event(1, 0_fs);
    tq.plan_event(2, 0_ks);
    tq.plan_event(3, 0_Ts);
    tq.plan_event(4, 0_Ms);

    {
        CHECK((string_builder() << tq.duration_until(0)).str() == "0_s");
        CHECK((string_builder() << tq.duration_until(1)).str() == "0_fs");
        CHECK((string_builder() << tq.duration_until(2)).str() == "0_ks");
        CHECK((string_builder() << tq.duration_until(3)).str() == "0_Ts");
        CHECK((string_builder() << tq.duration_until(4)).str() == "0_Ms");
    }

    tq.plan_event(5, 1_s);
    tq.plan_event(6, 1_fs);
    tq.plan_event(7, 1_ks);
    tq.plan_event(8, 1_Ts);
    tq.plan_event(9, 1_Ms);

    {
        CHECK((string_builder() << tq.duration_until(0)).str() == "0_s");
        CHECK((string_builder() << tq.duration_until(1)).str() == "0_fs");
        CHECK((string_builder() << tq.duration_until(2)).str() == "0_ks");
        CHECK((string_builder() << tq.duration_until(3)).str() == "0_Ts");
        CHECK((string_builder() << tq.duration_until(4)).str() == "0_Ms");
        CHECK((string_builder() << tq.duration_until(5)).str() == "1_s");
        CHECK((string_builder() << tq.duration_until(6)).str() == "1_fs");
        CHECK((string_builder() << tq.duration_until(7)).str() == "1_ks");
        CHECK((string_builder() << tq.duration_until(8)).str() == "1_Ts");
        CHECK((string_builder() << tq.duration_until(9)).str() == "1_Ms");
    }

    tq.plan_event(4, 1_s);
    tq.plan_event(3, 1_fs);
    tq.plan_event(2, 1_ks);
    tq.plan_event(1, 1_Ts);
    tq.plan_event(0, 1_Ms);

    {
        CHECK((string_builder() << tq.duration_until(0)).str() == "1_Ms");
        CHECK((string_builder() << tq.duration_until(1)).str() == "1_Ts");
        CHECK((string_builder() << tq.duration_until(2)).str() == "1_ks");
        CHECK((string_builder() << tq.duration_until(3)).str() == "1_fs");
        CHECK((string_builder() << tq.duration_until(4)).str() == "1_s");
        CHECK((string_builder() << tq.duration_until(5)).str() == "1_s");
        CHECK((string_builder() << tq.duration_until(6)).str() == "1_fs");
        CHECK((string_builder() << tq.duration_until(7)).str() == "1_ks");
        CHECK((string_builder() << tq.duration_until(8)).str() == "1_Ts");
        CHECK((string_builder() << tq.duration_until(9)).str() == "1_Ms");
    }

    tq.pop_imminent_events();
    tq.advance_time(500_ms);
    tq.plan_event(10, 999_fs);
    tq.plan_event(11, 999999_fs);
    tq.plan_event(12, 999999999_fs);
    tq.plan_event(13, 999999999999_fs);
    tq.plan_event(14, 999999999999999_fs);

    {
        CHECK((string_builder() << tq.duration_until(0)).str() == "1_Ms");
        CHECK((string_builder() << tq.duration_until(1)).str() == "1_Ts");
        CHECK((string_builder() << tq.duration_until(2)).str() == "1_ks");
        CHECK((string_builder() << tq.duration_until(3)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(4)).str() == "1_s");
        CHECK((string_builder() << tq.duration_until(5)).str() == "1_s");
        CHECK((string_builder() << tq.duration_until(6)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(7)).str() == "1_ks");
        CHECK((string_builder() << tq.duration_until(8)).str() == "1_Ts");
        CHECK((string_builder() << tq.duration_until(9)).str() == "1_Ms");
        CHECK((string_builder() << tq.duration_until(10)).str() == "999_fs");
        CHECK((string_builder() << tq.duration_until(11)).str() == "999999_fs");
        CHECK((string_builder() << tq.duration_until(12)).str() == "999999999_fs");
        CHECK((string_builder() << tq.duration_until(13)).str() == "999999999999_fs");
        CHECK((string_builder() << tq.duration_until(14)).str() == "999999999999999_fs");
    }

    {
        auto errmsg = std::string();
        try {
            tq.plan_event(15, 999999999999999999_fs);
        }
        catch (const std::domain_error& e) {
            errmsg = e.what();
        }
        CHECK(errmsg == "Planned duration must be finite");
    }

    tq.plan_event(10, 364_day);
    tq.plan_event(11, 364_day + 23_hr);
    tq.plan_event(12, 364_day + 23_hr + 59_min);
    tq.plan_event(13, 364_day + 23_hr + 59_min + 59_s);
    tq.plan_event(17, 364_day + 23_hr + 59_min + 59_s + 999_ms);
    tq.plan_event(18, 364_day + 23_hr + 59_min + 59_s + 999_ms + 999_us);
    tq.plan_event(19, 364_day + 23_hr + 59_min + 59_s + 999_ms + 999_us + 1_us);
    tq.plan_event(20, 364_day + 23_hr + 59_min + 59_s + 999_ms + 1_ms);
    tq.plan_event(14, 364_day + 23_hr + 59_min + 59_s + 499_ms);
    tq.plan_event(15, 364_day + 23_hr + 59_min + 59_s + 499_ms + 999_us);
    tq.plan_event(16, 364_day + 23_hr + 59_min + 59_s + 499_ms + 999_us + 1_us);

    {
        CHECK((string_builder() << tq.duration_until(0)).str() == "1_Ms");
        CHECK((string_builder() << tq.duration_until(1)).str() == "1_Ts");
        CHECK((string_builder() << tq.duration_until(2)).str() == "1_ks");
        CHECK((string_builder() << tq.duration_until(3)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(4)).str() == "1_s");
        CHECK((string_builder() << tq.duration_until(5)).str() == "1_s");
        CHECK((string_builder() << tq.duration_until(6)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(7)).str() == "1_ks");
        CHECK((string_builder() << tq.duration_until(8)).str() == "1_Ts");
        CHECK((string_builder() << tq.duration_until(9)).str() == "1_Ms");        
        CHECK((string_builder() << tq.duration_until(10)).str() == "31449600_s");
        CHECK((string_builder() << tq.duration_until(11)).str() == "31532400_s");
        CHECK((string_builder() << tq.duration_until(12)).str() == "31535940_s");
        CHECK((string_builder() << tq.duration_until(13)).str() == "31535999_s");
        CHECK((string_builder() << tq.duration_until(14)).str() == "31535999499_ms");
        CHECK((string_builder() << tq.duration_until(15)).str() == "31535999499999_us");
        CHECK((string_builder() << tq.duration_until(16)).str() == "31535999500000_us");
        CHECK((string_builder() << tq.duration_until(17)).str() == "31535999999_ms");
        CHECK((string_builder() << tq.duration_until(18)).str() == "31535999999999_us");
        CHECK((string_builder() << tq.duration_until(19)).str() == "31536000000000_us");
        CHECK((string_builder() << tq.duration_until(20)).str() == "31536000000_ms");
    }

    tq.advance_time(); tq.pop_imminent_events();
    tq.advance_time(); tq.pop_imminent_events();
    tq.advance_time(); tq.pop_imminent_events();
    tq.advance_time(); tq.pop_imminent_events();
    tq.advance_time(); tq.pop_imminent_events();
    tq.advance_time(); tq.pop_imminent_events();
    tq.advance_time(); tq.pop_imminent_events();

    {
        CHECK((string_builder() << tq.duration_until(0)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(1)).str() == "1_Ts");
        CHECK((string_builder() << tq.duration_until(2)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(3)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(4)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(5)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(6)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(7)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(8)).str() == "1_Ts");
        CHECK((string_builder() << tq.duration_until(9)).str() == "duration::inf()");        
        CHECK((string_builder() << tq.duration_until(10)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(11)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(12)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(13)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(14)).str() == "999_ms");
        CHECK((string_builder() << tq.duration_until(15)).str() == "999999_us");
        CHECK((string_builder() << tq.duration_until(16)).str() == "1000000_us");
        CHECK((string_builder() << tq.duration_until(17)).str() == "1499_ms");
        CHECK((string_builder() << tq.duration_until(18)).str() == "1499999_us");
        CHECK((string_builder() << tq.duration_until(19)).str() == "1500000_us");
        CHECK((string_builder() << tq.duration_until(20)).str() == "1500_ms");
    }

    tq.advance_time();

    {
        CHECK((string_builder() << tq.duration_until(1)).str() == "1_Ts");
        CHECK((string_builder() << tq.duration_until(8)).str() == "1_Ts");
        CHECK((string_builder() << tq.duration_until(14)).str() == "0_ms");
        CHECK((string_builder() << tq.duration_until(15)).str() == "999_us");
        CHECK((string_builder() << tq.duration_until(16)).str() == "1000_us");
        CHECK((string_builder() << tq.duration_until(17)).str() == "500_ms");
        CHECK((string_builder() << tq.duration_until(18)).str() == "500999_us");
        CHECK((string_builder() << tq.duration_until(19)).str() == "501000_us");
        CHECK((string_builder() << tq.duration_until(20)).str() == "501_ms");
    }

    tq.cancel_event(18);
    tq.cancel_event(1);
    tq.cancel_event(16);
    tq.cancel_event(14);
    tq.cancel_event(19);
    tq.cancel_event(20);

    {
        CHECK((string_builder() << tq.duration_until(1)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(8)).str() == "1_Ts");
        CHECK((string_builder() << tq.duration_until(14)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(15)).str() == "999_us");
        CHECK((string_builder() << tq.duration_until(16)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(17)).str() == "500_ms");
        CHECK((string_builder() << tq.duration_until(18)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(19)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(20)).str() == "duration::inf()");
    }

    tq.advance_time(); 
    tq.pop_imminent_events();

    {
        CHECK((string_builder() << tq.duration_until(8)).str() == "1_Ts");
        CHECK((string_builder() << tq.duration_until(15)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(17)).str() == "500_ms");
    }

    tq.advance_time(); 
    tq.pop_imminent_events();

    if (tq.current_time() == time_point()) {
        CHECK((string_builder() << tq.duration_until(8)).str() == "duration::inf()");
        CHECK((string_builder() << tq.duration_until(17)).str() == "499_ms");
    }
    else {
        CHECK((string_builder() << tq.duration_until(8)).str() == "1_Ts");
        CHECK((string_builder() << tq.duration_until(17)).str() == "duration::inf()");
    }

    tq.advance_time();
    tq.pop_imminent_events();

    CHECK(tq.empty() == true);

}
    
TEST_CASE("test time queue basic operations")
{
    test_time_queue_basic_operations(time_point());
}

TEST_CASE("test time queue basic operations with large negative time points")
{
    test_time_queue_basic_operations(time_point(-1_Ts));
}

TEST_CASE("test time queue basic operations with large positive time points")
{
    test_time_queue_basic_operations(time_point(1_Ts));
}

TEST_CASE("test time queue proximal events")
{
    test_time_queue_proximal_events(time_point());
}

TEST_CASE("test time queue proximal events with negative time point")
{
    test_time_queue_proximal_events(time_point(-1_yr));
}

TEST_CASE("test time queue proximal events with positive time point")
{
    test_time_queue_proximal_events(time_point(1_yr));
}


}  // namespace
