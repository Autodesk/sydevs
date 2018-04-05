#include <catch2/catch.hpp>
#include <sydevs/time/time_sequence.h>
#include <sydevs/core/string_builder.h>

using std::begin;
using std::end;

namespace sydevs {


TEST_CASE("test time sequence")
{
    auto ts = time_sequence();
    auto tp = time_point();

    ts.append(tp += 5_us);
    ts.append(tp += 72_fs);
    ts.append(tp += 1_hr);

    {
        auto ts_iter = begin(ts);
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 0);
            CHECK(entry.second == time_point() + 5_us);
        }
        ++ts_iter;
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 1);
            CHECK(entry.second == time_point() + 5_us + 0_ns + 0_ps + 72_fs);
        }
        ++ts_iter;
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 2 );
            CHECK(entry.second == time_point() + 3_ks + 600_s + 0_ms + 5_us + 0_ns + 0_ps + 72_fs);
        }
        ++ts_iter;
        CHECK((end(ts) - ts_iter) == 0);
    }

    {
        auto tsp_iter = begin(ts.partitions());
        {
            const auto& entry = *tsp_iter;
            CHECK(entry.first  == 0);
            CHECK(entry.second == time_point() + 5_us);
        }
        ++tsp_iter;
        {
            const auto& entry = *tsp_iter;
            CHECK(entry.first  == 2);
            CHECK(entry.second == time_point() + 3_ks + 600_s + 0_ms + 5_us + 0_ns + 0_ps + 72_fs);
        }
        ++tsp_iter;
        CHECK((end(ts.partitions()) - tsp_iter) == 0);
    }

    {
        auto errmsg = std::string();
        try {
            ts.append(tp);
        }
        catch (const std::out_of_range& e) {
            errmsg = e.what();
        }
        CHECK(errmsg == "Time points must be appended in increasing order");
    }

    {
        auto errmsg = std::string();
        try {
            ts.append(tp - 1_ms);
        }
        catch (const std::out_of_range& e) {
            errmsg = e.what();
        }
        CHECK(errmsg == "Time points must be appended in increasing order");
    }

    tp -= 1_ms;
    ts.append(tp += 5_Ms);
    ts.append(tp += 123456789_ms);
    ts.append(tp += 100000000000000_as);
    ts.append(tp += 100000000000001_as);

    {
        auto ts_iter = begin(ts);
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 0);
            CHECK(entry.second == time_point() + 5_us);
        }
        ++ts_iter;
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 1);
            CHECK(entry.second == time_point() + 5_us + 0_ns + 0_ps + 72_fs);
        }
        ++ts_iter;
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 2);
            CHECK(entry.second == time_point() + 3_ks + 600_s + 0_ms + 5_us + 0_ns + 0_ps + 72_fs);
        }
        ++ts_iter;
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 3);
            CHECK(entry.second == time_point() + 5_Ms + 3_ks + 599_s + 999_ms + 5_us + 0_ns + 0_ps + 72_fs);
        }
        ++ts_iter;
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 4);
            CHECK(entry.second == time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 5_us + 0_ns + 0_ps + 72_fs);
        }
        ++ts_iter;
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 5);
            CHECK(entry.second == time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 105_us + 0_ns + 0_ps + 72_fs);
        }
        ++ts_iter;
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 6);
            CHECK(entry.second == time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 205_us + 0_ns + 0_ps + 72_fs + 1_as);
        }
        ++ts_iter;
        CHECK((end(ts) - ts_iter) == 0);
    }

    {
        auto tsp_iter = begin(ts.partitions());
        {
            const auto& entry = *tsp_iter;
            CHECK(entry.first  == 0);
            CHECK(entry.second == time_point() + 5_us);
        }
        ++tsp_iter;
        {
            const auto& entry = *tsp_iter;
            CHECK(entry.first  == 2);
            CHECK(entry.second == time_point() + 1_hr + 5_us + 72_fs);
        }
        ++tsp_iter;
        {
            const auto& entry = *tsp_iter;
            CHECK(entry.first  == 6);
            CHECK(entry.second == time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 205_us + 0_ns + 0_ps + 72_fs + 1_as);
        }
        ++tsp_iter;
        CHECK((end(ts.partitions()) - tsp_iter) == 0);
    }

    {
        CHECK(ts.front() == (time_point() + 5_us));
        CHECK(ts.back()  == (time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 205_us + 0_ns + 0_ps + 72_fs + 1_as));
    }

    {
        auto ts_iter = end(ts) - 1;
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 6);
            CHECK(entry.second == time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 205_us + 0_ns + 0_ps + 72_fs + 1_as);
        }
        --ts_iter;
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 5);
            CHECK(entry.second == time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 105_us + 0_ns + 0_ps + 72_fs);
        }
        --ts_iter;
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 4);
            CHECK(entry.second == time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 5_us + 0_ns + 0_ps + 72_fs);
        }
        --ts_iter;
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 3);
            CHECK(entry.second == time_point() + 5_Ms + 3_ks + 599_s + 999_ms + 5_us + 0_ns + 0_ps + 72_fs);
        }
        --ts_iter;
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 2);
            CHECK(entry.second == time_point() + 3_ks + 600_s + 0_ms + 5_us + 0_ns + 0_ps + 72_fs);
        }
        --ts_iter;
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 1);
            CHECK(entry.second == time_point() + 5_us + 0_ns + 0_ps + 72_fs);
        }
        --ts_iter;
        {
            const auto& entry = *ts_iter;
            CHECK(entry.first  == 0);
            CHECK(entry.second == time_point() + 5_us);
        }
        CHECK((ts_iter - begin(ts)) == 0);
    }

    {
        CHECK(ts[0] == time_point() + 5_us);
        CHECK(ts[1] == time_point() + 5_us + 0_ns + 0_ps + 72_fs);
        CHECK(ts[2] == time_point() + 3_ks + 600_s + 0_ms + 5_us + 0_ns + 0_ps + 72_fs);
        CHECK(ts[3] == time_point() + 5_Ms + 3_ks + 599_s + 999_ms + 5_us + 0_ns + 0_ps + 72_fs);
        CHECK(ts[4] == time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 5_us + 0_ns + 0_ps + 72_fs);
        CHECK(ts[5] == time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 105_us + 0_ns + 0_ps + 72_fs);
        CHECK(ts[6] == time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 205_us + 0_ns + 0_ps + 72_fs + 1_as);
        CHECK(ts.size() == 7);
    }

    ts.append(tp += 999_as);

    {
        auto errmsg = std::string();
        try {
            ts.append(tp -= 1_as);
        }
        catch (const std::out_of_range& e) {
            errmsg = e.what();
        }
        CHECK(errmsg == "Time points must be appended in increasing order");
    }

    {
        CHECK(ts[7] == time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 205_us + 0_ns + 0_ps + 73_fs);
        CHECK(ts.size() == 8);
    }

    {
        tp = time_point(-1_Ts);
        CHECK(ts.lower_bound(tp)->first == 0);
        CHECK(ts.upper_bound(tp)->first == 0);

        tp = time_point();
        CHECK(ts.lower_bound(tp)->first == 0);
        CHECK(ts.upper_bound(tp)->first == 0);
    }

    {
        auto ts_iter = begin(ts);
        tp = ts_iter->second;
        {
            CHECK(ts.lower_bound(tp - 1_ys)->first == 0);
            CHECK(ts.lower_bound(tp)->first == 0);
            CHECK(ts.lower_bound(tp + 1_ys)->first == 1);
            CHECK(ts.upper_bound(tp - 1_ys)->first == 0);
            CHECK(ts.upper_bound(tp)->first == 1);
            CHECK(ts.upper_bound(tp + 1_ys)->first == 1);
        }
        ++ts_iter;
        tp = ts_iter->second;
        {
            CHECK(ts.lower_bound(tp - 1_ys)->first == 1);
            CHECK(ts.lower_bound(tp)->first == 1);
            CHECK(ts.lower_bound(tp + 1_ys)->first == 2);
            CHECK(ts.upper_bound(tp - 1_ys)->first == 1);
            CHECK(ts.upper_bound(tp)->first == 2);
            CHECK(ts.upper_bound(tp + 1_ys)->first == 2);
        }
        ++ts_iter;
        tp = ts_iter->second;
        {
            CHECK(ts.lower_bound(tp - 1_ys)->first == 2);
            CHECK(ts.lower_bound(tp)->first == 2);
            CHECK(ts.lower_bound(tp + 1_ys)->first == 3);
            CHECK(ts.upper_bound(tp - 1_ys)->first == 2);
            CHECK(ts.upper_bound(tp)->first == 3);
            CHECK(ts.upper_bound(tp + 1_ys)->first == 3);
        }
        ++ts_iter;
        tp = ts_iter->second;
        {
            CHECK(ts.lower_bound(tp - 1_ys)->first == 3);
            CHECK(ts.lower_bound(tp)->first == 3);
            CHECK(ts.lower_bound(tp + 1_ys)->first == 4);
            CHECK(ts.upper_bound(tp - 1_ys)->first == 3);
            CHECK(ts.upper_bound(tp)->first == 4);
            CHECK(ts.upper_bound(tp + 1_ys)->first == 4);
        }
        ++ts_iter;
        tp = ts_iter->second;
        {
            CHECK(ts.lower_bound(tp - 1_ys)->first == 4);
            CHECK(ts.lower_bound(tp)->first == 4);
            CHECK(ts.lower_bound(tp + 1_ys)->first == 5);
            CHECK(ts.upper_bound(tp - 1_ys)->first == 4);
            CHECK(ts.upper_bound(tp)->first == 5);
            CHECK(ts.upper_bound(tp + 1_ys)->first == 5);
        }
        ++ts_iter;
        tp = ts_iter->second;
        {
            CHECK(ts.lower_bound(tp - 1_ys)->first == 5);
            CHECK(ts.lower_bound(tp)->first == 5);
            CHECK(ts.lower_bound(tp + 1_ys)->first == 6);
            CHECK(ts.upper_bound(tp - 1_ys)->first == 5);
            CHECK(ts.upper_bound(tp)->first == 6);
            CHECK(ts.upper_bound(tp + 1_ys)->first == 6);
        }
        ++ts_iter;
        tp = ts_iter->second;
        {
            CHECK(ts.lower_bound(tp - 1_ys)->first == 6);
            CHECK(ts.lower_bound(tp)->first == 6);
            CHECK(ts.lower_bound(tp + 1_ys)->first == 7);
            CHECK(ts.upper_bound(tp - 1_ys)->first == 6);
            CHECK(ts.upper_bound(tp)->first == 7);
            CHECK(ts.upper_bound(tp + 1_ys)->first == 7);
        }
        ++ts_iter;
        tp = ts_iter->second;
        {
            CHECK(ts.lower_bound(tp - 1_ys)->first == 7);
            CHECK(ts.lower_bound(tp)->first == 7);
            CHECK(ts.lower_bound(tp + 1_ys) == end(ts));
            CHECK(ts.upper_bound(tp - 1_ys)->first == 7);
            CHECK(ts.upper_bound(tp) == end(ts));
            CHECK(ts.upper_bound(tp + 1_ys) == end(ts));
        }
        ++ts_iter;
    }

    {
        tp = time_point(1_Ts);
        CHECK(ts.lower_bound(tp) == end(ts));
        CHECK(ts.upper_bound(tp) == end(ts));

        CHECK((ts.lower_bound(tp) - begin(ts)) == 8);
        CHECK((ts.upper_bound(tp) - begin(ts)) == 8);
    }

    {
        auto s = std::string();
        for (const auto& entry : ts) {
            auto i = entry.first;          // current index
            const auto& t = entry.second;  // current time point
            if (i > 0) {
                s += ", ";
            }
            s += (string_builder() << "(" << i << ", " << t << ")").str();
        }
        CHECK(s == std::string() +
                   "(0, time_point() + 5_us), " +
                   "(1, time_point() + 5_us + 0_ns + 0_ps + 72_fs), " + 
                   "(2, time_point() + 3_ks + 600_s + 0_ms + 5_us + 0_ns + 0_ps + 72_fs), " +
                   "(3, time_point() + 5_Ms + 3_ks + 599_s + 999_ms + 5_us + 0_ns + 0_ps + 72_fs), " +
                   "(4, time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 5_us + 0_ns + 0_ps + 72_fs), " +
                   "(5, time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 105_us + 0_ns + 0_ps + 72_fs), " +
                   "(6, time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 205_us + 0_ns + 0_ps + 72_fs + 1_as), " +
                   "(7, time_point() + 5_Ms + 127_ks + 56_s + 788_ms + 205_us + 0_ns + 0_ps + 73_fs)");
    }

}


}  // namespace
