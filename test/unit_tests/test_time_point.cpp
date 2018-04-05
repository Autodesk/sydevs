#include <catch2/catch.hpp>
#include <sydevs/time/time_point.h>
#include <sydevs/core/string_builder.h>

namespace sydevs {


TEST_CASE("test single time point")
{
    auto tp = time_point();
    REQUIRE(tp == time_point());

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == unit);
    CHECK(tp.nscales() == 0);
    CHECK(tp.scale_digit(milli) == 0);
    CHECK(tp.scale_phase(milli) == 0);
    CHECK(tp.epoch_phase(milli) == 0);
    CHECK(tp.scale_digit(unit ) == 0);
    CHECK(tp.scale_phase(unit ) == 0);
    CHECK(tp.epoch_phase(unit ) == 0);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 0);
    CHECK(tp.epoch_phase(kilo ) == 0);

    {
        auto errmsg = std::string();
        try {
            tp += duration();
        }
        catch (const std::invalid_argument& e) {
            errmsg = e.what();
        }
        CHECK(errmsg == "Adjustment duration must be valid");
    }

    {
        auto errmsg = std::string();
        try {
            tp += duration::inf();
        }
        catch (const std::domain_error& e) {
            errmsg = e.what();
        }
        CHECK(errmsg == "Adjustment duration must be finite");
    }

    tp += 732_us;
    REQUIRE(tp == (time_point() + 732_us));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == micro);
    CHECK(tp.nscales() == 1);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 0);
    CHECK(tp.epoch_phase(nano ) == 732000);
    CHECK(tp.scale_digit(micro) == 732);
    CHECK(tp.scale_phase(micro) == 732);
    CHECK(tp.epoch_phase(micro) == 732);
    CHECK(tp.scale_digit(milli) == 0);
    CHECK(tp.scale_phase(milli) == 0);
    CHECK(tp.epoch_phase(milli) == 0);

    tp += 403_us;
    REQUIRE(tp == (time_point() + 1_ms + 135_us));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == micro);
    CHECK(tp.nscales() == 2);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 0);
    CHECK(tp.epoch_phase(nano ) == 1135000);
    CHECK(tp.scale_digit(micro) == 135);
    CHECK(tp.scale_phase(micro) == 135);
    CHECK(tp.epoch_phase(micro) == 1135);
    CHECK(tp.scale_digit(milli) == 1);
    CHECK(tp.scale_phase(milli) == 1);
    CHECK(tp.epoch_phase(milli) == 1);
    CHECK(tp.scale_digit(unit ) == 0);
    CHECK(tp.scale_phase(unit ) == 0);
    CHECK(tp.epoch_phase(unit ) == 0);

    tp += 1_min;
    REQUIRE(tp == (time_point() + 60_s + 1_ms + 135_us));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == micro);
    CHECK(tp.nscales() == 3);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 0);
    CHECK(tp.epoch_phase(nano ) == 60001135000);
    CHECK(tp.scale_digit(micro) == 135);
    CHECK(tp.scale_phase(micro) == 135);
    CHECK(tp.epoch_phase(micro) == 60001135);
    CHECK(tp.scale_digit(milli) == 1);
    CHECK(tp.scale_phase(milli) == 1);
    CHECK(tp.epoch_phase(milli) == 60001);
    CHECK(tp.scale_digit(unit ) == 60);
    CHECK(tp.scale_phase(unit ) == 60);
    CHECK(tp.epoch_phase(unit ) == 60);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 0);
    CHECK(tp.epoch_phase(kilo ) == 0);

    tp += 123456789_as;
    REQUIRE(tp == (time_point() + 60_s + 1_ms + 135_us + 000_ns + 123_ps + 456_fs + 789_as));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == atto);
    CHECK(tp.nscales() == 7);
    CHECK(tp.scale_digit(zepto) == 0);
    CHECK(tp.scale_phase(zepto) == 0);
    CHECK(tp.epoch_phase(zepto) == 123456789000);
    CHECK(tp.scale_digit(atto ) == 789);
    CHECK(tp.scale_phase(atto ) == 789);
    CHECK(tp.epoch_phase(atto ) == 135000123456789);
    CHECK(tp.scale_digit(femto) == 456);
    CHECK(tp.scale_phase(femto) == 456);
    CHECK(tp.epoch_phase(femto) == 1135000123456);
    CHECK(tp.scale_digit(pico ) == 123);
    CHECK(tp.scale_phase(pico ) == 123);
    CHECK(tp.epoch_phase(pico ) == 60001135000123);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 0);
    CHECK(tp.epoch_phase(nano ) == 60001135000);
    CHECK(tp.scale_digit(micro) == 135);
    CHECK(tp.scale_phase(micro) == 135);
    CHECK(tp.epoch_phase(micro) == 60001135);
    CHECK(tp.scale_digit(milli) == 1);
    CHECK(tp.scale_phase(milli) == 1);
    CHECK(tp.epoch_phase(milli) == 60001);
    CHECK(tp.scale_digit(unit ) == 60);
    CHECK(tp.scale_phase(unit ) == 60);
    CHECK(tp.epoch_phase(unit ) == 60);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 0);
    CHECK(tp.epoch_phase(kilo ) == 0);

    tp -= 12345678_as;
    REQUIRE(tp == (time_point() + 60_s + 1_ms + 135_us + 000_ns + 111_ps + 111_fs + 111_as));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == atto);
    CHECK(tp.nscales() == 7);
    CHECK(tp.scale_digit(zepto) == 0);
    CHECK(tp.scale_phase(zepto) == 0);
    CHECK(tp.epoch_phase(zepto) == 111111111000);
    CHECK(tp.scale_digit(atto ) == 111);
    CHECK(tp.scale_phase(atto ) == 111);
    CHECK(tp.epoch_phase(atto ) == 135000111111111);
    CHECK(tp.scale_digit(femto) == 111);
    CHECK(tp.scale_phase(femto) == 111);
    CHECK(tp.epoch_phase(femto) == 1135000111111);
    CHECK(tp.scale_digit(pico ) == 111);
    CHECK(tp.scale_phase(pico ) == 111);
    CHECK(tp.epoch_phase(pico ) == 60001135000111);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 0);
    CHECK(tp.epoch_phase(nano ) == 60001135000);
    CHECK(tp.scale_digit(micro) == 135);
    CHECK(tp.scale_phase(micro) == 135);
    CHECK(tp.epoch_phase(micro) == 60001135);
    CHECK(tp.scale_digit(milli) == 1);
    CHECK(tp.scale_phase(milli) == 1);
    CHECK(tp.epoch_phase(milli) == 60001);
    CHECK(tp.scale_digit(unit ) == 60);
    CHECK(tp.scale_phase(unit ) == 60);
    CHECK(tp.epoch_phase(unit ) == 60);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 0);
    CHECK(tp.epoch_phase(kilo ) == 0);

    tp -= 500_as;
    REQUIRE(tp == (time_point() + 60_s + 1_ms + 135_us + 000_ns + 111_ps + 110_fs + 611_as));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == atto);
    CHECK(tp.nscales() == 7);
    CHECK(tp.scale_digit(zepto) == 0);
    CHECK(tp.scale_phase(zepto) == 0);
    CHECK(tp.epoch_phase(zepto) == 111110611000);
    CHECK(tp.scale_digit(atto ) == 611);
    CHECK(tp.scale_phase(atto ) == 611);
    CHECK(tp.epoch_phase(atto ) == 135000111110611);
    CHECK(tp.scale_digit(femto) == 110);
    CHECK(tp.scale_phase(femto) == 110);
    CHECK(tp.epoch_phase(femto) == 1135000111110);
    CHECK(tp.scale_digit(pico ) == 111);
    CHECK(tp.scale_phase(pico ) == 111);
    CHECK(tp.epoch_phase(pico ) == 60001135000111);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 0);
    CHECK(tp.epoch_phase(nano ) == 60001135000);
    CHECK(tp.scale_digit(micro) == 135);
    CHECK(tp.scale_phase(micro) == 135);
    CHECK(tp.epoch_phase(micro) == 60001135);
    CHECK(tp.scale_digit(milli) == 1);
    CHECK(tp.scale_phase(milli) == 1);
    CHECK(tp.epoch_phase(milli) == 60001);
    CHECK(tp.scale_digit(unit ) == 60);
    CHECK(tp.scale_phase(unit ) == 60);
    CHECK(tp.epoch_phase(unit ) == 60);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 0);
    CHECK(tp.epoch_phase(kilo ) == 0);

    tp -= -389_as;
    REQUIRE(tp == (time_point() + 60_s + 1_ms + 135_us + 000_ns + 111_ps + 111_fs));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == femto);
    CHECK(tp.nscales() == 6);
    CHECK(tp.scale_digit(atto ) == 0);
    CHECK(tp.scale_phase(atto ) == 0);
    CHECK(tp.epoch_phase(atto ) == 135000111111000);
    CHECK(tp.scale_digit(femto) == 111);
    CHECK(tp.scale_phase(femto) == 111);
    CHECK(tp.epoch_phase(femto) == 1135000111111);
    CHECK(tp.scale_digit(pico ) == 111);
    CHECK(tp.scale_phase(pico ) == 111);
    CHECK(tp.epoch_phase(pico ) == 60001135000111);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 0);
    CHECK(tp.epoch_phase(nano ) == 60001135000);
    CHECK(tp.scale_digit(micro) == 135);
    CHECK(tp.scale_phase(micro) == 135);
    CHECK(tp.epoch_phase(micro) == 60001135);
    CHECK(tp.scale_digit(milli) == 1);
    CHECK(tp.scale_phase(milli) == 1);
    CHECK(tp.epoch_phase(milli) == 60001);
    CHECK(tp.scale_digit(unit ) == 60);
    CHECK(tp.scale_phase(unit ) == 60);
    CHECK(tp.epoch_phase(unit ) == 60);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 0);
    CHECK(tp.epoch_phase(kilo ) == 0);

    tp += -60001_ms;
    REQUIRE(tp == (time_point() + 135_us + 000_ns + 111_ps + 111_fs));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == femto);
    CHECK(tp.nscales() == 4);
    CHECK(tp.scale_digit(atto ) == 0);
    CHECK(tp.scale_phase(atto ) == 0);
    CHECK(tp.epoch_phase(atto ) == 135000111111000);
    CHECK(tp.scale_digit(femto) == 111);
    CHECK(tp.scale_phase(femto) == 111);
    CHECK(tp.epoch_phase(femto) == 135000111111);
    CHECK(tp.scale_digit(pico ) == 111);
    CHECK(tp.scale_phase(pico ) == 111);
    CHECK(tp.epoch_phase(pico ) == 135000111);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 0);
    CHECK(tp.epoch_phase(nano ) == 135000);
    CHECK(tp.scale_digit(micro) == 135);
    CHECK(tp.scale_phase(micro) == 135);
    CHECK(tp.epoch_phase(micro) == 135);
    CHECK(tp.scale_digit(milli) == 0);
    CHECK(tp.scale_phase(milli) == 0);
    CHECK(tp.epoch_phase(milli) == 0);

    tp -= 111112_fs;
    REQUIRE(tp == (time_point() + 134_us + 999_ns + 999_ps + 999_fs));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == femto);
    CHECK(tp.nscales() == 4);
    CHECK(tp.scale_digit(atto ) == 0);
    CHECK(tp.scale_phase(atto ) == 0);
    CHECK(tp.epoch_phase(atto ) == 134999999999000);
    CHECK(tp.scale_digit(femto) == 999);
    CHECK(tp.scale_phase(femto) == 999);
    CHECK(tp.epoch_phase(femto) == 134999999999);
    CHECK(tp.scale_digit(pico ) == 999);
    CHECK(tp.scale_phase(pico ) == 999);
    CHECK(tp.epoch_phase(pico ) == 134999999);
    CHECK(tp.scale_digit(nano ) == 999);
    CHECK(tp.scale_phase(nano ) == 999);
    CHECK(tp.epoch_phase(nano ) == 134999);
    CHECK(tp.scale_digit(micro) == 134);
    CHECK(tp.scale_phase(micro) == 134);
    CHECK(tp.epoch_phase(micro) == 134);
    CHECK(tp.scale_digit(milli) == 0);
    CHECK(tp.scale_phase(milli) == 0);
    CHECK(tp.epoch_phase(milli) == 0);

    tp -= 135_us;
    REQUIRE(tp == (time_point() - 1_fs));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == -1);
    CHECK(tp.precision() == femto);
    CHECK(tp.nscales() == 1);
    CHECK(tp.scale_digit(atto ) == 0);
    CHECK(tp.scale_phase(atto ) == 0);
    CHECK(tp.epoch_phase(atto ) == 999999999999000);
    CHECK(tp.scale_digit(femto) == 1);
    CHECK(tp.scale_phase(femto) == 999);
    CHECK(tp.epoch_phase(femto) == 999999999999999);
    CHECK(tp.scale_digit(pico ) == 0);
    CHECK(tp.scale_phase(pico ) == 999);
    CHECK(tp.epoch_phase(pico ) == 999999999999999);

    tp += 2_fs;
    REQUIRE(tp == (time_point() + 1_fs));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == femto);
    CHECK(tp.nscales() == 1);
    CHECK(tp.scale_digit(atto ) == 0);
    CHECK(tp.scale_phase(atto ) == 0);
    CHECK(tp.epoch_phase(atto ) == 1000);
    CHECK(tp.scale_digit(femto) == 1);
    CHECK(tp.scale_phase(femto) == 1);
    CHECK(tp.epoch_phase(femto) == 1);
    CHECK(tp.scale_digit(pico ) == 0);
    CHECK(tp.scale_phase(pico ) == 0);
    CHECK(tp.epoch_phase(pico ) == 0);

    tp += -2_fs;
    REQUIRE(tp == (time_point() - 1_fs));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == -1);
    CHECK(tp.precision() == femto);
    CHECK(tp.nscales() == 1);
    CHECK(tp.scale_digit(atto ) == 0);
    CHECK(tp.scale_phase(atto ) == 0);
    CHECK(tp.epoch_phase(atto ) == 999999999999000);
    CHECK(tp.scale_digit(femto) == 1);
    CHECK(tp.scale_phase(femto) == 999);
    CHECK(tp.epoch_phase(femto) == 999999999999999);
    CHECK(tp.scale_digit(pico ) == 0);
    CHECK(tp.scale_phase(pico ) == 999);
    CHECK(tp.epoch_phase(pico ) == 999999999999999);

    tp += 1_fs;
    REQUIRE(tp == time_point());

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == unit);
    CHECK(tp.nscales() == 0);
    CHECK(tp.scale_digit(milli) == 0);
    CHECK(tp.scale_phase(milli) == 0);
    CHECK(tp.epoch_phase(milli) == 0);
    CHECK(tp.scale_digit(unit ) == 0);
    CHECK(tp.scale_phase(unit ) == 0);
    CHECK(tp.epoch_phase(unit ) == 0);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 0);
    CHECK(tp.epoch_phase(kilo ) == 0);

    tp += 2_s;
    REQUIRE(tp == (time_point() + 2_s));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == unit);
    CHECK(tp.nscales() == 1);
    CHECK(tp.scale_digit(milli) == 0);
    CHECK(tp.scale_phase(milli) == 0);
    CHECK(tp.epoch_phase(milli) == 2000);
    CHECK(tp.scale_digit(unit ) == 2);
    CHECK(tp.scale_phase(unit ) == 2);
    CHECK(tp.epoch_phase(unit ) == 2);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 0);
    CHECK(tp.epoch_phase(kilo ) == 0);

    tp += 7_ys;
    REQUIRE(tp == (time_point() + 2_s + 0_ms + 0_us + 0_ns + 0_ps + 0_fs + 0_as + 0_zs + 7_ys));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == yocto);
    CHECK(tp.nscales() == 9);
    CHECK(tp.scale_digit(scale(-9)) == 0);
    CHECK(tp.scale_phase(scale(-9)) == 0);
    CHECK(tp.epoch_phase(scale(-9)) == 7000);
    CHECK(tp.scale_digit(yocto) == 7);
    CHECK(tp.scale_phase(yocto) == 7);
    CHECK(tp.epoch_phase(yocto) == 7);
    CHECK(tp.scale_digit(zepto) == 0);
    CHECK(tp.scale_phase(zepto) == 0);
    CHECK(tp.epoch_phase(zepto) == 0);
    CHECK(tp.scale_digit(atto ) == 0);
    CHECK(tp.scale_phase(atto ) == 0);
    CHECK(tp.epoch_phase(atto ) == 0);
    CHECK(tp.scale_digit(femto) == 0);
    CHECK(tp.scale_phase(femto) == 0);
    CHECK(tp.epoch_phase(femto) == 0);
    CHECK(tp.scale_digit(pico ) == 0);
    CHECK(tp.scale_phase(pico ) == 0);
    CHECK(tp.epoch_phase(pico ) == 2000000000000);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 0);
    CHECK(tp.epoch_phase(nano ) == 2000000000);
    CHECK(tp.scale_digit(micro) == 0);
    CHECK(tp.scale_phase(micro) == 0);
    CHECK(tp.epoch_phase(micro) == 2000000);
    CHECK(tp.scale_digit(milli) == 0);
    CHECK(tp.scale_phase(milli) == 0);
    CHECK(tp.epoch_phase(milli) == 2000);
    CHECK(tp.scale_digit(unit ) == 2);
    CHECK(tp.scale_phase(unit ) == 2);
    CHECK(tp.epoch_phase(unit ) == 2);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 0);
    CHECK(tp.epoch_phase(kilo ) == 0);

    tp -= 5_s;
    REQUIRE(tp == (time_point() - 2_s - 999_ms - 999_us - 999_ns - 999_ps - 999_fs - 999_as - 999_zs - 993_ys));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == -1);
    CHECK(tp.precision() == yocto);
    CHECK(tp.nscales() == 9);
    CHECK(tp.scale_digit(scale(-9)) == 0);
    CHECK(tp.scale_phase(scale(-9)) == 0);
    CHECK(tp.epoch_phase(scale(-9)) == 7000);
    CHECK(tp.scale_digit(yocto) == 993);
    CHECK(tp.scale_phase(yocto) == 7);
    CHECK(tp.epoch_phase(yocto) == 7);
    CHECK(tp.scale_digit(zepto) == 999);
    CHECK(tp.scale_phase(zepto) == 0);
    CHECK(tp.epoch_phase(zepto) == 0);
    CHECK(tp.scale_digit(atto ) == 999);
    CHECK(tp.scale_phase(atto ) == 0);
    CHECK(tp.epoch_phase(atto ) == 0);
    CHECK(tp.scale_digit(femto) == 999);
    CHECK(tp.scale_phase(femto) == 0);
    CHECK(tp.epoch_phase(femto) == 0);
    CHECK(tp.scale_digit(pico ) == 999);
    CHECK(tp.scale_phase(pico ) == 0);
    CHECK(tp.epoch_phase(pico ) == 997000000000000);
    CHECK(tp.scale_digit(nano ) == 999);
    CHECK(tp.scale_phase(nano ) == 0);
    CHECK(tp.epoch_phase(nano ) == 999997000000000);
    CHECK(tp.scale_digit(micro) == 999);
    CHECK(tp.scale_phase(micro) == 0);
    CHECK(tp.epoch_phase(micro) == 999999997000000);
    CHECK(tp.scale_digit(milli) == 999);
    CHECK(tp.scale_phase(milli) == 0);
    CHECK(tp.epoch_phase(milli) == 999999999997000);
    CHECK(tp.scale_digit(unit ) == 2);
    CHECK(tp.scale_phase(unit ) == 997);
    CHECK(tp.epoch_phase(unit ) == 999999999999997);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 999);
    CHECK(tp.epoch_phase(kilo ) == 999999999999999);
}

TEST_CASE("test single time point with duration signs inverted")
{
    auto tp = time_point();
    REQUIRE(tp == time_point());

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == unit);
    CHECK(tp.nscales() == 0);
    CHECK(tp.scale_digit(milli) == 0);
    CHECK(tp.scale_phase(milli) == 0);
    CHECK(tp.epoch_phase(milli) == 0);
    CHECK(tp.scale_digit(unit ) == 0);
    CHECK(tp.scale_phase(unit ) == 0);
    CHECK(tp.epoch_phase(unit ) == 0);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 0);
    CHECK(tp.epoch_phase(kilo ) == 0);

    {
        auto errmsg = std::string();
        try {
            tp -= duration();
        }
        catch (const std::invalid_argument& e) {
            errmsg = e.what();
        }
        CHECK(errmsg == "Adjustment duration must be valid");
    }

    {
        auto errmsg = std::string();
        try {
            tp -= duration::inf();
        }
        catch (const std::domain_error& e) {
            errmsg = e.what();
        }
        CHECK(errmsg == "Adjustment duration must be finite");
    }

    tp -= 732_us;
    REQUIRE(tp == (time_point() - 732_us));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == -1);
    CHECK(tp.precision() == micro);
    CHECK(tp.nscales() == 1);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 0);
    CHECK(tp.epoch_phase(nano ) == 999999999268000);
    CHECK(tp.scale_digit(micro) == 732);
    CHECK(tp.scale_phase(micro) == 268);
    CHECK(tp.epoch_phase(micro) == 999999999999268);
    CHECK(tp.scale_digit(milli) == 0);
    CHECK(tp.scale_phase(milli) == 999);
    CHECK(tp.epoch_phase(milli) == 999999999999999);

    tp -= 403_us;
    REQUIRE(tp == (time_point() - 1_ms - 135_us));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == -1);
    CHECK(tp.precision() == micro);
    CHECK(tp.nscales() == 2);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 0);
    CHECK(tp.epoch_phase(nano ) == 999999998865000);
    CHECK(tp.scale_digit(micro) == 135);
    CHECK(tp.scale_phase(micro) == 865);
    CHECK(tp.epoch_phase(micro) == 999999999998865);
    CHECK(tp.scale_digit(milli) == 1);
    CHECK(tp.scale_phase(milli) == 998);
    CHECK(tp.epoch_phase(milli) == 999999999999998);
    CHECK(tp.scale_digit(unit ) == 0);
    CHECK(tp.scale_phase(unit ) == 999);
    CHECK(tp.epoch_phase(unit ) == 999999999999999);

    tp -= 1_min;
    REQUIRE(tp == (time_point() - 60_s - 1_ms - 135_us));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == -1);
    CHECK(tp.precision() == micro);
    CHECK(tp.nscales() == 3);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 0);
    CHECK(tp.epoch_phase(nano ) == 999939998865000);
    CHECK(tp.scale_digit(micro) == 135);
    CHECK(tp.scale_phase(micro) == 865);
    CHECK(tp.epoch_phase(micro) == 999999939998865);
    CHECK(tp.scale_digit(milli) == 1);
    CHECK(tp.scale_phase(milli) == 998);
    CHECK(tp.epoch_phase(milli) == 999999999939998);
    CHECK(tp.scale_digit(unit ) == 60);
    CHECK(tp.scale_phase(unit ) == 939);
    CHECK(tp.epoch_phase(unit ) == 999999999999939);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 999);
    CHECK(tp.epoch_phase(kilo ) == 999999999999999);

    tp -= 123456789_as;
    REQUIRE(tp == (time_point() - 60_s - 1_ms - 135_us - 000_ns - 123_ps - 456_fs - 789_as));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == -1);
    CHECK(tp.precision() == atto);
    CHECK(tp.nscales() == 7);
    CHECK(tp.scale_digit(zepto) == 0);
    CHECK(tp.scale_phase(zepto) == 0);
    CHECK(tp.epoch_phase(zepto) == 999876543211000);
    CHECK(tp.scale_digit(atto ) == 789);
    CHECK(tp.scale_phase(atto ) == 211);
    CHECK(tp.epoch_phase(atto ) == 864999876543211);
    CHECK(tp.scale_digit(femto) == 456);
    CHECK(tp.scale_phase(femto) == 543);
    CHECK(tp.epoch_phase(femto) == 998864999876543);
    CHECK(tp.scale_digit(pico ) == 123);
    CHECK(tp.scale_phase(pico ) == 876);
    CHECK(tp.epoch_phase(pico ) == 939998864999876);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 999);
    CHECK(tp.epoch_phase(nano ) == 999939998864999);
    CHECK(tp.scale_digit(micro) == 135);
    CHECK(tp.scale_phase(micro) == 864);
    CHECK(tp.epoch_phase(micro) == 999999939998864);
    CHECK(tp.scale_digit(milli) == 1);
    CHECK(tp.scale_phase(milli) == 998);
    CHECK(tp.epoch_phase(milli) == 999999999939998);
    CHECK(tp.scale_digit(unit ) == 60);
    CHECK(tp.scale_phase(unit ) == 939);
    CHECK(tp.epoch_phase(unit ) == 999999999999939);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 999);
    CHECK(tp.epoch_phase(kilo ) == 999999999999999);
    
    tp += 12345678_as;
    REQUIRE(tp == (time_point() - 60_s - 1_ms - 135_us - 000_ns - 111_ps - 111_fs - 111_as));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == -1);
    CHECK(tp.precision() == atto);
    CHECK(tp.nscales() == 7);
    CHECK(tp.scale_digit(zepto) == 0);
    CHECK(tp.scale_phase(zepto) == 0);
    CHECK(tp.epoch_phase(zepto) == 999888888889000);
    CHECK(tp.scale_digit(atto ) == 111);
    CHECK(tp.scale_phase(atto ) == 889);
    CHECK(tp.epoch_phase(atto ) == 864999888888889);
    CHECK(tp.scale_digit(femto) == 111);
    CHECK(tp.scale_phase(femto) == 888);
    CHECK(tp.epoch_phase(femto) == 998864999888888);
    CHECK(tp.scale_digit(pico ) == 111);
    CHECK(tp.scale_phase(pico ) == 888);
    CHECK(tp.epoch_phase(pico ) == 939998864999888);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 999);
    CHECK(tp.epoch_phase(nano ) == 999939998864999);
    CHECK(tp.scale_digit(micro) == 135);
    CHECK(tp.scale_phase(micro) == 864);
    CHECK(tp.epoch_phase(micro) == 999999939998864);
    CHECK(tp.scale_digit(milli) == 1);
    CHECK(tp.scale_phase(milli) == 998);
    CHECK(tp.epoch_phase(milli) == 999999999939998);
    CHECK(tp.scale_digit(unit ) == 60);
    CHECK(tp.scale_phase(unit ) == 939);
    CHECK(tp.epoch_phase(unit ) == 999999999999939);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 999);
    CHECK(tp.epoch_phase(kilo ) == 999999999999999);
    
    tp += 500_as;
    REQUIRE(tp == (time_point() - 60_s - 1_ms - 135_us - 000_ns - 111_ps - 110_fs - 611_as));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == -1);
    CHECK(tp.precision() == atto);
    CHECK(tp.nscales() == 7);
    CHECK(tp.scale_digit(zepto) == 0);
    CHECK(tp.scale_phase(zepto) == 0);
    CHECK(tp.epoch_phase(zepto) == 999888889389000);
    CHECK(tp.scale_digit(atto ) == 611);
    CHECK(tp.scale_phase(atto ) == 389);
    CHECK(tp.epoch_phase(atto ) == 864999888889389);
    CHECK(tp.scale_digit(femto) == 110);
    CHECK(tp.scale_phase(femto) == 889);
    CHECK(tp.epoch_phase(femto) == 998864999888889);
    CHECK(tp.scale_digit(pico ) == 111);
    CHECK(tp.scale_phase(pico ) == 888);
    CHECK(tp.epoch_phase(pico ) == 939998864999888);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 999);
    CHECK(tp.epoch_phase(nano ) == 999939998864999);
    CHECK(tp.scale_digit(micro) == 135);
    CHECK(tp.scale_phase(micro) == 864);
    CHECK(tp.epoch_phase(micro) == 999999939998864);
    CHECK(tp.scale_digit(milli) == 1);
    CHECK(tp.scale_phase(milli) == 998);
    CHECK(tp.epoch_phase(milli) == 999999999939998);
    CHECK(tp.scale_digit(unit ) == 60);
    CHECK(tp.scale_phase(unit ) == 939);
    CHECK(tp.epoch_phase(unit ) == 999999999999939);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 999);
    CHECK(tp.epoch_phase(kilo ) == 999999999999999);
    
    tp += -389_as;
    REQUIRE(tp == (time_point() - 60_s - 1_ms - 135_us - 000_ns - 111_ps - 111_fs));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == -1);
    CHECK(tp.precision() == femto);
    CHECK(tp.nscales() == 6);
    CHECK(tp.scale_digit(atto ) == 0);
    CHECK(tp.scale_phase(atto ) == 0);
    CHECK(tp.epoch_phase(atto ) == 864999888889000);
    CHECK(tp.scale_digit(femto) == 111);
    CHECK(tp.scale_phase(femto) == 889);
    CHECK(tp.epoch_phase(femto) == 998864999888889);
    CHECK(tp.scale_digit(pico ) == 111);
    CHECK(tp.scale_phase(pico ) == 888);
    CHECK(tp.epoch_phase(pico ) == 939998864999888);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 999);
    CHECK(tp.epoch_phase(nano ) == 999939998864999);
    CHECK(tp.scale_digit(micro) == 135);
    CHECK(tp.scale_phase(micro) == 864);
    CHECK(tp.epoch_phase(micro) == 999999939998864);
    CHECK(tp.scale_digit(milli) == 1);
    CHECK(tp.scale_phase(milli) == 998);
    CHECK(tp.epoch_phase(milli) == 999999999939998);
    CHECK(tp.scale_digit(unit ) == 60);
    CHECK(tp.scale_phase(unit ) == 939);
    CHECK(tp.epoch_phase(unit ) == 999999999999939);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 999);
    CHECK(tp.epoch_phase(kilo ) == 999999999999999);
    
    tp -= -60001_ms;
    REQUIRE(tp == (time_point() - 135_us - 000_ns - 111_ps - 111_fs));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == -1);
    CHECK(tp.precision() == femto);
    CHECK(tp.nscales() == 4);
    CHECK(tp.scale_digit(atto ) == 0);
    CHECK(tp.scale_phase(atto ) == 0);
    CHECK(tp.epoch_phase(atto ) == 864999888889000);
    CHECK(tp.scale_digit(femto) == 111);
    CHECK(tp.scale_phase(femto) == 889);
    CHECK(tp.epoch_phase(femto) == 999864999888889);
    CHECK(tp.scale_digit(pico ) == 111);
    CHECK(tp.scale_phase(pico ) == 888);
    CHECK(tp.epoch_phase(pico ) == 999999864999888);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 999);
    CHECK(tp.epoch_phase(nano ) == 999999999864999);
    CHECK(tp.scale_digit(micro) == 135);
    CHECK(tp.scale_phase(micro) == 864);
    CHECK(tp.epoch_phase(micro) == 999999999999864);
    CHECK(tp.scale_digit(milli) == 0);
    CHECK(tp.scale_phase(milli) == 999);
    CHECK(tp.epoch_phase(milli) == 999999999999999);
    
    tp += 111112_fs;
    REQUIRE(tp == (time_point() - 134_us - 999_ns - 999_ps - 999_fs));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == -1);
    CHECK(tp.precision() == femto);
    CHECK(tp.nscales() == 4);
    CHECK(tp.scale_digit(atto ) == 0);
    CHECK(tp.scale_phase(atto ) == 0);
    CHECK(tp.epoch_phase(atto ) == 865000000001000);
    CHECK(tp.scale_digit(femto) == 999);
    CHECK(tp.scale_phase(femto) == 1);
    CHECK(tp.epoch_phase(femto) == 999865000000001);
    CHECK(tp.scale_digit(pico ) == 999);
    CHECK(tp.scale_phase(pico ) == 0);
    CHECK(tp.epoch_phase(pico ) == 999999865000000);
    CHECK(tp.scale_digit(nano ) == 999);
    CHECK(tp.scale_phase(nano ) == 0);
    CHECK(tp.epoch_phase(nano ) == 999999999865000);
    CHECK(tp.scale_digit(micro) == 134);
    CHECK(tp.scale_phase(micro) == 865);
    CHECK(tp.epoch_phase(micro) == 999999999999865);
    CHECK(tp.scale_digit(milli) == 0);
    CHECK(tp.scale_phase(milli) == 999);
    CHECK(tp.epoch_phase(milli) == 999999999999999);
    
    tp += 135_us;
    REQUIRE(tp == (time_point() + 1_fs));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == femto);
    CHECK(tp.nscales() == 1);
    CHECK(tp.scale_digit(atto ) == 0);
    CHECK(tp.scale_phase(atto ) == 0);
    CHECK(tp.epoch_phase(atto ) == 1000);
    CHECK(tp.scale_digit(femto) == 1);
    CHECK(tp.scale_phase(femto) == 1);
    CHECK(tp.epoch_phase(femto) == 1);
    CHECK(tp.scale_digit(pico ) == 0);
    CHECK(tp.scale_phase(pico ) == 0);
    CHECK(tp.epoch_phase(pico ) == 0);
    
    tp -= 2_fs;
    REQUIRE(tp == (time_point() - 1_fs));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == -1);
    CHECK(tp.precision() == femto);
    CHECK(tp.nscales() == 1);
    CHECK(tp.scale_digit(atto ) == 0);
    CHECK(tp.scale_phase(atto ) == 0);
    CHECK(tp.epoch_phase(atto ) == 999999999999000);
    CHECK(tp.scale_digit(femto) == 1);
    CHECK(tp.scale_phase(femto) == 999);
    CHECK(tp.epoch_phase(femto) == 999999999999999);
    CHECK(tp.scale_digit(pico ) == 0);
    CHECK(tp.scale_phase(pico ) == 999);
    CHECK(tp.epoch_phase(pico ) == 999999999999999);

    tp -= -2_fs;
    REQUIRE(tp == (time_point() + 1_fs));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == femto);
    CHECK(tp.nscales() == 1);
    CHECK(tp.scale_digit(atto ) == 0);
    CHECK(tp.scale_phase(atto ) == 0);
    CHECK(tp.epoch_phase(atto ) == 1000);
    CHECK(tp.scale_digit(femto) == 1);
    CHECK(tp.scale_phase(femto) == 1);
    CHECK(tp.epoch_phase(femto) == 1);
    CHECK(tp.scale_digit(pico ) == 0);
    CHECK(tp.scale_phase(pico ) == 0);
    CHECK(tp.epoch_phase(pico ) == 0);

    tp -= 1_fs;
    REQUIRE(tp == time_point());

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == unit);
    CHECK(tp.nscales() == 0);
    CHECK(tp.scale_digit(milli) == 0);
    CHECK(tp.scale_phase(milli) == 0);
    CHECK(tp.epoch_phase(milli) == 0);
    CHECK(tp.scale_digit(unit ) == 0);
    CHECK(tp.scale_phase(unit ) == 0);
    CHECK(tp.epoch_phase(unit ) == 0);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 0);
    CHECK(tp.epoch_phase(kilo ) == 0);
    
    tp -= 2_s;
    REQUIRE(tp == (time_point() - 2_s));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == -1);
    CHECK(tp.precision() == unit);
    CHECK(tp.nscales() == 1);
    CHECK(tp.scale_digit(milli) == 0);
    CHECK(tp.scale_phase(milli) == 0);
    CHECK(tp.epoch_phase(milli) == 999999999998000);
    CHECK(tp.scale_digit(unit ) == 2);
    CHECK(tp.scale_phase(unit ) == 998);
    CHECK(tp.epoch_phase(unit ) == 999999999999998);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 999);
    CHECK(tp.epoch_phase(kilo ) == 999999999999999);
    
    tp -= 7_ys;
    REQUIRE(tp == (time_point() - 2_s - 0_ms - 0_us - 0_ns - 0_ps - 0_fs - 0_as - 0_zs - 7_ys));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == -1);
    CHECK(tp.precision() == yocto);
    CHECK(tp.nscales() == 9);
    CHECK(tp.scale_digit(scale(-9)) == 0);
    CHECK(tp.scale_phase(scale(-9)) == 0);
    CHECK(tp.epoch_phase(scale(-9)) == 999999999993000);
    CHECK(tp.scale_digit(yocto) == 7);
    CHECK(tp.scale_phase(yocto) == 993);
    CHECK(tp.epoch_phase(yocto) == 999999999999993);
    CHECK(tp.scale_digit(zepto) == 0);
    CHECK(tp.scale_phase(zepto) == 999);
    CHECK(tp.epoch_phase(zepto) == 999999999999999);
    CHECK(tp.scale_digit(atto ) == 0);
    CHECK(tp.scale_phase(atto ) == 999);
    CHECK(tp.epoch_phase(atto ) == 999999999999999);
    CHECK(tp.scale_digit(femto) == 0);
    CHECK(tp.scale_phase(femto) == 999);
    CHECK(tp.epoch_phase(femto) == 999999999999999);
    CHECK(tp.scale_digit(pico ) == 0);
    CHECK(tp.scale_phase(pico ) == 999);
    CHECK(tp.epoch_phase(pico ) == 997999999999999);
    CHECK(tp.scale_digit(nano ) == 0);
    CHECK(tp.scale_phase(nano ) == 999);
    CHECK(tp.epoch_phase(nano ) == 999997999999999);
    CHECK(tp.scale_digit(micro) == 0);
    CHECK(tp.scale_phase(micro) == 999);
    CHECK(tp.epoch_phase(micro) == 999999997999999);
    CHECK(tp.scale_digit(milli) == 0);
    CHECK(tp.scale_phase(milli) == 999);
    CHECK(tp.epoch_phase(milli) == 999999999997999);
    CHECK(tp.scale_digit(unit ) == 2);
    CHECK(tp.scale_phase(unit ) == 997);
    CHECK(tp.epoch_phase(unit ) == 999999999999997);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 999);
    CHECK(tp.epoch_phase(kilo ) == 999999999999999);
    
    tp += 5_s;
    REQUIRE(tp == (time_point() + 2_s + 999_ms + 999_us + 999_ns + 999_ps + 999_fs + 999_as + 999_zs + 993_ys));

    CHECK(tp.epoch_phase(tp.precision() - 5) == 0);
    CHECK(tp.epoch_phase(tp.precision() - 4) == tp.scale_phase(tp.precision())*1000000000000);
    CHECK(tp.sign() == 1);
    CHECK(tp.precision() == yocto);
    CHECK(tp.nscales() == 9);
    CHECK(tp.scale_digit(scale(-9)) == 0);
    CHECK(tp.scale_phase(scale(-9)) == 0);
    CHECK(tp.epoch_phase(scale(-9)) == 999999999993000);
    CHECK(tp.scale_digit(yocto) == 993);
    CHECK(tp.scale_phase(yocto) == 993);
    CHECK(tp.epoch_phase(yocto) == 999999999999993);
    CHECK(tp.scale_digit(zepto) == 999);
    CHECK(tp.scale_phase(zepto) == 999);
    CHECK(tp.epoch_phase(zepto) == 999999999999999);
    CHECK(tp.scale_digit(atto ) == 999);
    CHECK(tp.scale_phase(atto ) == 999);
    CHECK(tp.epoch_phase(atto ) == 999999999999999);
    CHECK(tp.scale_digit(femto) == 999);
    CHECK(tp.scale_phase(femto) == 999);
    CHECK(tp.epoch_phase(femto) == 999999999999999);
    CHECK(tp.scale_digit(pico ) == 999);
    CHECK(tp.scale_phase(pico ) == 999);
    CHECK(tp.epoch_phase(pico ) == 2999999999999);
    CHECK(tp.scale_digit(nano ) == 999);
    CHECK(tp.scale_phase(nano ) == 999);
    CHECK(tp.epoch_phase(nano ) == 2999999999);
    CHECK(tp.scale_digit(micro) == 999);
    CHECK(tp.scale_phase(micro) == 999);
    CHECK(tp.epoch_phase(micro) == 2999999);
    CHECK(tp.scale_digit(milli) == 999);
    CHECK(tp.scale_phase(milli) == 999);
    CHECK(tp.epoch_phase(milli) == 2999);
    CHECK(tp.scale_digit(unit ) == 2);
    CHECK(tp.scale_phase(unit ) == 2);
    CHECK(tp.epoch_phase(unit ) == 2);
    CHECK(tp.scale_digit(kilo ) == 0);
    CHECK(tp.scale_phase(kilo ) == 0);
    CHECK(tp.epoch_phase(kilo ) == 0);
}

TEST_CASE("test single time point with complex durations")
{
    auto tp = time_point();

    tp += (732_us + 403_us + 1_min);
    REQUIRE(tp == (time_point() + 60_s + 1_ms + 135_us));

    tp += (123456789_as - 12345678_as - 500_as - (-389_as));
    REQUIRE(tp == (time_point() + 60_s + 1_ms + 135_us + 000_ns + 111_ps + 111_fs));

    tp += -60001_ms;
    REQUIRE(tp == (time_point() + 135_us + 000_ns + 111_ps + 111_fs));

    tp -= (111112_fs + 135_us - 2_fs);
    REQUIRE(tp == (time_point() + 1_fs));
}

TEST_CASE("test single time point with complex durations and signs inverted")
{
    auto tp = time_point();

    tp += (-732_us - 403_us - 1_min);
    REQUIRE(tp == (time_point() - 60_s - 1_ms - 135_us));

    tp += (-123456789_as + 12345678_as + 500_as - (389_as));
    REQUIRE(tp == (time_point() - 60_s - 1_ms - 135_us - 000_ns - 111_ps - 111_fs));

    tp += 60001_ms;
    REQUIRE(tp == (time_point() - 135_us - 000_ns - 111_ps - 111_fs));

    tp -= (-111112_fs - 135_us + 2_fs);
    REQUIRE(tp == (time_point() - 1_fs));
}

TEST_CASE("test time-point-time-point comparisons and differences")
{
    auto tpA = time_point();
    auto tpB = time_point();

    CHECK((tpA == tpB) == true);
    CHECK((tpA != tpB) == false);
    CHECK((tpA < tpB) == false);
    CHECK((tpA > tpB) == false);
    CHECK((tpA <= tpB) == true);
    CHECK((tpA >= tpB) == true);
    CHECK((tpA - tpB) == 0_s);
    CHECK((tpB - tpA) == 0_s);
    CHECK(tpA.gap(tpB) == 0_s);
    CHECK(tpB.gap(tpA) == 0_s);
    CHECK((time_point(tpA).advance(tpB.gap(tpA))) <= time_point(tpB));
    CHECK((time_point(tpB).advance(tpA.gap(tpB))) <= time_point(tpA));

    tpA += 1_s;

    CHECK((tpA == tpB) == false);
    CHECK((tpA != tpB) == true);
    CHECK((tpA < tpB) == false);
    CHECK((tpA > tpB) == true);
    CHECK((tpA <= tpB) == false);
    CHECK((tpA >= tpB) == true);
    CHECK((tpA - tpB) == 1_s);
    CHECK((tpB - tpA) == -1_s);
    CHECK(tpA.gap(tpB) == 1_s);
    CHECK(tpB.gap(tpA) == -1_s);
    CHECK((time_point(tpB).advance(tpA.gap(tpB))) <= time_point(tpA));

    tpB += 2_s;
    tpB += 7_ys;

    CHECK((tpA == tpB) == false);
    CHECK((tpA != tpB) == true);
    CHECK((tpA < tpB) == true);
    CHECK((tpA > tpB) == false);
    CHECK((tpA <= tpB) == true);
    CHECK((tpA >= tpB) == false);
    CHECK((tpA - tpB) == -duration::inf());
    CHECK((tpB - tpA) == duration::inf());
    CHECK(tpA.gap(tpB) == -1_s);
    CHECK(tpB.gap(tpA) == 1_s);
    CHECK((time_point(tpA).advance(tpB.gap(tpA))) <= time_point(tpB));

    tpA += 1_s;
    tpA += 7_ys;

    CHECK((tpA == tpB) == true);
    CHECK((tpA != tpB) == false);
    CHECK((tpA < tpB) == false);
    CHECK((tpA > tpB) == false);
    CHECK((tpA <= tpB) == true);
    CHECK((tpA >= tpB) == true);
    CHECK((tpA - tpB) == 0_s);
    CHECK((tpB - tpA) == 0_s);
    CHECK(tpA.gap(tpB) == 0_s);
    CHECK(tpB.gap(tpA) == 0_s);
    CHECK((time_point(tpA).advance(tpB.gap(tpA))) <= time_point(tpB));
    CHECK((time_point(tpB).advance(tpA.gap(tpB))) <= time_point(tpA));
    
    tpA -= 1_ys;

    CHECK((tpA == tpB) == false);
    CHECK((tpA != tpB) == true);
    CHECK((tpA < tpB) == true);
    CHECK((tpA > tpB) == false);
    CHECK((tpA <= tpB) == true);
    CHECK((tpA >= tpB) == false);
    CHECK((tpA - tpB) == -1_ys);
    CHECK((tpB - tpA) == 1_ys);
    CHECK(tpA.gap(tpB) == -1_ys);
    CHECK(tpB.gap(tpA) == 1_ys);
    CHECK((time_point(tpA).advance(tpB.gap(tpA))) <= time_point(tpB));

    tpB -= 5_s;

    CHECK((tpA == tpB) == false);
    CHECK((tpA != tpB) == true);
    CHECK((tpA < tpB) == false);
    CHECK((tpA > tpB) == true);
    CHECK((tpA <= tpB) == false);
    CHECK((tpA >= tpB) == true);
    CHECK((tpA - tpB) == duration::inf());
    CHECK((tpB - tpA) == -duration::inf());
    CHECK(tpA.gap(tpB) == 5000000000000_ps);
    CHECK(tpB.gap(tpA) == -5000000000000_ps);
    CHECK((time_point(tpB).advance(tpA.gap(tpB))) <= time_point(tpA));

    tpA -= 7_s;

    CHECK((tpA == tpB) == false);
    CHECK((tpA != tpB) == true);
    CHECK((tpA < tpB) == true);
    CHECK((tpA > tpB) == false);
    CHECK((tpA <= tpB) == true);
    CHECK((tpA >= tpB) == false);
    CHECK((tpA - tpB) == -duration::inf());
    CHECK((tpB - tpA) == duration::inf());
    CHECK(tpA.gap(tpB) == -2000000000000_ps);
    CHECK(tpB.gap(tpA) == 2000000000000_ps);
    CHECK((time_point(tpA).advance(tpB.gap(tpA))) <= time_point(tpB));

    tpA += 2_s;
    tpA += 374_fs;

    CHECK((tpA == tpB) == false);
    CHECK((tpA != tpB) == true);
    CHECK((tpA < tpB) == false);
    CHECK((tpA > tpB) == true);
    CHECK((tpA <= tpB) == false);
    CHECK((tpA >= tpB) == true);
    CHECK((tpA - tpB) == 373999999999_ys);
    CHECK((tpB - tpA) == -373999999999_ys);
    CHECK(tpA.gap(tpB) == 373999999999_ys);
    CHECK(tpB.gap(tpA) == -373999999999_ys);
    CHECK((time_point(tpB).advance(tpA.gap(tpB))) <= time_point(tpA));
}

TEST_CASE("test time-point-time-point comparisons and differences with duration signs inverted")
{
    auto tpA = time_point();
    auto tpB = time_point();

    CHECK((tpA == tpB) == true);
    CHECK((tpA != tpB) == false);
    CHECK((tpA < tpB) == false);
    CHECK((tpA > tpB) == false);
    CHECK((tpA <= tpB) == true);
    CHECK((tpA >= tpB) == true);
    CHECK((tpA - tpB) == 0_s);
    CHECK((tpB - tpA) == 0_s);
    CHECK(tpA.gap(tpB) == 0_s);
    CHECK(tpB.gap(tpA) == 0_s);
    CHECK((time_point(tpA).advance(tpB.gap(tpA))) <= time_point(tpB));
    CHECK((time_point(tpB).advance(tpA.gap(tpB))) <= time_point(tpA));

    tpA -= 1_s;

    CHECK((tpA == tpB) == false);
    CHECK((tpA != tpB) == true);
    CHECK((tpA < tpB) == true);
    CHECK((tpA > tpB) == false);
    CHECK((tpA <= tpB) == true);
    CHECK((tpA >= tpB) == false);
    CHECK((tpA - tpB) == -1_s);
    CHECK((tpB - tpA) == 1_s);
    CHECK(tpA.gap(tpB) == -1_s);
    CHECK(tpB.gap(tpA) == 1_s);
    CHECK((time_point(tpA).advance(tpB.gap(tpA))) <= time_point(tpB));

    tpB -= 2_s;
    tpB -= 7_ys;

    CHECK((tpA == tpB) == false);
    CHECK((tpA != tpB) == true);
    CHECK((tpA < tpB) == false);
    CHECK((tpA > tpB) == true);
    CHECK((tpA <= tpB) == false);
    CHECK((tpA >= tpB) == true);
    CHECK((tpA - tpB) == duration::inf());
    CHECK((tpB - tpA) == -duration::inf());
    CHECK(tpA.gap(tpB) == 1_s);
    CHECK(tpB.gap(tpA) == -1_s);
    CHECK((time_point(tpB).advance(tpA.gap(tpB))) <= time_point(tpA));

    tpA -= 1_s;
    tpA -= 7_ys;

    CHECK((tpA == tpB) == true);
    CHECK((tpA != tpB) == false);
    CHECK((tpA < tpB) == false);
    CHECK((tpA > tpB) == false);
    CHECK((tpA <= tpB) == true);
    CHECK((tpA >= tpB) == true);
    CHECK((tpA - tpB) == 0_s);
    CHECK((tpB - tpA) == 0_s);
    CHECK(tpA.gap(tpB) == 0_s);
    CHECK(tpB.gap(tpA) == 0_s);
    CHECK((time_point(tpA).advance(tpB.gap(tpA))) <= time_point(tpB));
    CHECK((time_point(tpB).advance(tpA.gap(tpB))) <= time_point(tpA));
    
    tpA += 1_ys;

    CHECK((tpA == tpB) == false);
    CHECK((tpA != tpB) == true);
    CHECK((tpA < tpB) == false);
    CHECK((tpA > tpB) == true);
    CHECK((tpA <= tpB) == false);
    CHECK((tpA >= tpB) == true);
    CHECK((tpA - tpB) == 1_ys);
    CHECK((tpB - tpA) == -1_ys);
    CHECK(tpA.gap(tpB) == 1_ys);
    CHECK(tpB.gap(tpA) == -1_ys);
    CHECK((time_point(tpB).advance(tpA.gap(tpB))) <= time_point(tpA));

    tpB += 5_s;

    CHECK((tpA == tpB) == false);
    CHECK((tpA != tpB) == true);
    CHECK((tpA < tpB) == true);
    CHECK((tpA > tpB) == false);
    CHECK((tpA <= tpB) == true);
    CHECK((tpA >= tpB) == false);
    CHECK((tpA - tpB) == -duration::inf());
    CHECK((tpB - tpA) == duration::inf());
    CHECK(tpA.gap(tpB) == -5000000000000_ps);
    CHECK(tpB.gap(tpA) == 5000000000000_ps);
    CHECK((time_point(tpA).advance(tpB.gap(tpA))) <= time_point(tpB));

    tpA += 7_s;

    CHECK((tpA == tpB) == false);
    CHECK((tpA != tpB) == true);
    CHECK((tpA < tpB) == false);
    CHECK((tpA > tpB) == true);
    CHECK((tpA <= tpB) == false);
    CHECK((tpA >= tpB) == true);
    CHECK((tpA - tpB) == duration::inf());
    CHECK((tpB - tpA) == -duration::inf());
    CHECK(tpA.gap(tpB) == 2000000000000_ps);
    CHECK(tpB.gap(tpA) == -2000000000000_ps);
    CHECK((time_point(tpB).advance(tpA.gap(tpB))) <= time_point(tpA));

    tpA -= 2_s;
    tpA -= 374_fs;

    CHECK((tpA == tpB) == false);
    CHECK((tpA != tpB) == true);
    CHECK((tpA < tpB) == true);
    CHECK((tpA > tpB) == false);
    CHECK((tpA <= tpB) == true);
    CHECK((tpA >= tpB) == false);
    CHECK((tpA - tpB) == -373999999999_ys);
    CHECK((tpB - tpA) == 373999999999_ys);
    CHECK(tpA.gap(tpB) == -373999999999_ys);
    CHECK(tpB.gap(tpA) == 373999999999_ys);
    CHECK((time_point(tpA).advance(tpB.gap(tpA))) <= time_point(tpB));
}

TEST_CASE("test time point borderline cases")
{
    CHECK(( time_point() + 700_ms).gap(time_point() + 700_ms)  == 0_s);
    CHECK(((time_point() + 700_ms) -  (time_point() + 700_ms)) == 0_s);

    CHECK(( time_point() + 700_ms -   1_as).gap(time_point() + 700_ms)  == -1_as);
    CHECK(((time_point() + 700_ms -   1_as) -  (time_point() + 700_ms)) == -1_as);
    CHECK(( time_point() + 700_ms - 499_as).gap(time_point() + 700_ms)  == -499_as);
    CHECK(((time_point() + 700_ms - 499_as) -  (time_point() + 700_ms)) == -499_as);
    CHECK(( time_point() + 700_ms - 500_as).gap(time_point() + 700_ms)  == -500_as);
    CHECK(((time_point() + 700_ms - 500_as) -  (time_point() + 700_ms)) == -500_as);
    CHECK(( time_point() + 700_ms - 501_as).gap(time_point() + 700_ms)  == -501_as);
    CHECK(((time_point() + 700_ms - 501_as) -  (time_point() + 700_ms)) == -501_as);
    CHECK(( time_point() + 700_ms - 999_as).gap(time_point() + 700_ms)  == -999_as);
    CHECK(((time_point() + 700_ms - 999_as) -  (time_point() + 700_ms)) == -999_as);

    CHECK(( time_point() + 700_ms).gap(time_point() + 700_ms +   1_as)  == -1_as);
    CHECK(((time_point() + 700_ms) -  (time_point() + 700_ms +   1_as)) == -1_as);
    CHECK(( time_point() + 700_ms).gap(time_point() + 700_ms + 499_as)  == -499_as);
    CHECK(((time_point() + 700_ms) -  (time_point() + 700_ms + 499_as)) == -499_as);
    CHECK(( time_point() + 700_ms).gap(time_point() + 700_ms + 500_as)  == -500_as);
    CHECK(((time_point() + 700_ms) -  (time_point() + 700_ms + 500_as)) == -500_as);
    CHECK(( time_point() + 700_ms).gap(time_point() + 700_ms + 501_as)  == -501_as);
    CHECK(((time_point() + 700_ms) -  (time_point() + 700_ms + 501_as)) == -501_as);
    CHECK(( time_point() + 700_ms).gap(time_point() + 700_ms + 999_as)  == -999_as);
    CHECK(((time_point() + 700_ms) -  (time_point() + 700_ms + 999_as)) == -999_as);

    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() + (700_ms + 200_fs))  == 0_s);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() + (700_ms + 200_fs))) == 0_s);

    CHECK(( time_point() + (700_ms + 200_fs) -   1_as).gap(time_point() + (700_ms + 200_fs))  == -1_as);
    CHECK(((time_point() + (700_ms + 200_fs) -   1_as) -  (time_point() + (700_ms + 200_fs))) == -1_as);
    CHECK(( time_point() + (700_ms + 200_fs) - 499_as).gap(time_point() + (700_ms + 200_fs))  == -499_as);
    CHECK(((time_point() + (700_ms + 200_fs) - 499_as) -  (time_point() + (700_ms + 200_fs))) == -499_as);
    CHECK(( time_point() + (700_ms + 200_fs) - 500_as).gap(time_point() + (700_ms + 200_fs))  == -500_as);
    CHECK(((time_point() + (700_ms + 200_fs) - 500_as) -  (time_point() + (700_ms + 200_fs))) == -500_as);
    CHECK(( time_point() + (700_ms + 200_fs) - 501_as).gap(time_point() + (700_ms + 200_fs))  == -501_as);
    CHECK(((time_point() + (700_ms + 200_fs) - 501_as) -  (time_point() + (700_ms + 200_fs))) == -501_as);
    CHECK(( time_point() + (700_ms + 200_fs) - 999_as).gap(time_point() + (700_ms + 200_fs))  == -999_as);
    CHECK(((time_point() + (700_ms + 200_fs) - 999_as) -  (time_point() + (700_ms + 200_fs))) == -999_as);

    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() + (700_ms + 200_fs) +   1_as)  == -1_as);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() + (700_ms + 200_fs) +   1_as)) == -1_as);
    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() + (700_ms + 200_fs) + 499_as)  == -499_as);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() + (700_ms + 200_fs) + 499_as)) == -499_as);
    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() + (700_ms + 200_fs) + 500_as)  == -500_as);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() + (700_ms + 200_fs) + 500_as)) == -500_as);
    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() + (700_ms + 200_fs) + 501_as)  == -501_as);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() + (700_ms + 200_fs) + 501_as)) == -501_as);
    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() + (700_ms + 200_fs) + 999_as)  == -999_as);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() + (700_ms + 200_fs) + 999_as)) == -999_as);

    CHECK(( time_point() + 700_ms).gap(time_point() - 300_ms)  == 1_s);
    CHECK(((time_point() + 700_ms) -  (time_point() - 300_ms)) == 1_s);

    CHECK(( time_point() + 700_ms -   1_as).gap(time_point() - 300_ms)  == 1000000000000_ps);
    CHECK(((time_point() + 700_ms -   1_as) -  (time_point() - 300_ms)) == duration::inf());
    CHECK(( time_point() + 700_ms - 499_as).gap(time_point() - 300_ms)  == 1000000000000_ps);
    CHECK(((time_point() + 700_ms - 499_as) -  (time_point() - 300_ms)) == duration::inf());
    CHECK(( time_point() + 700_ms - 500_as).gap(time_point() - 300_ms)  == 1000000000000_ps);
    CHECK(((time_point() + 700_ms - 500_as) -  (time_point() - 300_ms)) == duration::inf());
    CHECK(( time_point() + 700_ms - 501_as).gap(time_point() - 300_ms)  == 999999999999999_fs);
    CHECK(((time_point() + 700_ms - 501_as) -  (time_point() - 300_ms)) == duration::inf());
    CHECK(( time_point() + 700_ms - 999_as).gap(time_point() - 300_ms)  == 999999999999999_fs);
    CHECK(((time_point() + 700_ms - 999_as) -  (time_point() - 300_ms)) == duration::inf());

    CHECK(( time_point() + 700_ms).gap(time_point() - 300_ms +   1_as)  == 1000000000000_ps);
    CHECK(((time_point() + 700_ms) -  (time_point() - 300_ms +   1_as)) == duration::inf());
    CHECK(( time_point() + 700_ms).gap(time_point() - 300_ms + 499_as)  == 1000000000000_ps);
    CHECK(((time_point() + 700_ms) -  (time_point() - 300_ms + 499_as)) == duration::inf());
    CHECK(( time_point() + 700_ms).gap(time_point() - 300_ms + 500_as)  == 1000000000000_ps);
    CHECK(((time_point() + 700_ms) -  (time_point() - 300_ms + 500_as)) == duration::inf());
    CHECK(( time_point() + 700_ms).gap(time_point() - 300_ms + 501_as)  == 999999999999999_fs);
    CHECK(((time_point() + 700_ms) -  (time_point() - 300_ms + 501_as)) == duration::inf());
    CHECK(( time_point() + 700_ms).gap(time_point() - 300_ms + 999_as)  == 999999999999999_fs);
    CHECK(((time_point() + 700_ms) -  (time_point() - 300_ms + 999_as)) == duration::inf());

    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() - (300_ms - 200_fs))  == 1000000000000_ps);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() - (300_ms - 200_fs))) == 1000000000000_ps);

    CHECK(( time_point() + (700_ms + 200_fs) -   1_as).gap(time_point() - (300_ms - 200_fs))  == 1000000000000_ps);
    CHECK(((time_point() + (700_ms + 200_fs) -   1_as) -  (time_point() - (300_ms - 200_fs))) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs) - 499_as).gap(time_point() - (300_ms - 200_fs))  == 1000000000000_ps);
    CHECK(((time_point() + (700_ms + 200_fs) - 499_as) -  (time_point() - (300_ms - 200_fs))) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs) - 500_as).gap(time_point() - (300_ms - 200_fs))  == 1000000000000_ps);
    CHECK(((time_point() + (700_ms + 200_fs) - 500_as) -  (time_point() - (300_ms - 200_fs))) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs) - 501_as).gap(time_point() - (300_ms - 200_fs))  == 999999999999999_fs);
    CHECK(((time_point() + (700_ms + 200_fs) - 501_as) -  (time_point() - (300_ms - 200_fs))) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs) - 999_as).gap(time_point() - (300_ms - 200_fs))  == 999999999999999_fs);
    CHECK(((time_point() + (700_ms + 200_fs) - 999_as) -  (time_point() - (300_ms - 200_fs))) == duration::inf());

    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() - (300_ms - 200_fs) +   1_as)  == 1000000000000_ps);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() - (300_ms - 200_fs) +   1_as)) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() - (300_ms - 200_fs) + 499_as)  == 1000000000000_ps);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() - (300_ms - 200_fs) + 499_as)) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() - (300_ms - 200_fs) + 500_as)  == 1000000000000_ps);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() - (300_ms - 200_fs) + 500_as)) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() - (300_ms - 200_fs) + 501_as)  == 999999999999999_fs);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() - (300_ms - 200_fs) + 501_as)) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() - (300_ms - 200_fs) + 999_as)  == 999999999999999_fs);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() - (300_ms - 200_fs) + 999_as)) == duration::inf());

    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() - (300_ms - 201_fs))  == 999999999999999_fs);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() - (300_ms - 201_fs))) == 999999999999999_fs);

    CHECK(( time_point() + (700_ms + 200_fs) +   1_as).gap(time_point() - (300_ms - 201_fs))  == 999999999999999_fs);
    CHECK(((time_point() + (700_ms + 200_fs) +   1_as) -  (time_point() - (300_ms - 201_fs))) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs) + 499_as).gap(time_point() - (300_ms - 201_fs))  == 999999999999999_fs);
    CHECK(((time_point() + (700_ms + 200_fs) + 499_as) -  (time_point() - (300_ms - 201_fs))) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs) + 500_as).gap(time_point() - (300_ms - 201_fs))  == 1000000000000_ps);
    CHECK(((time_point() + (700_ms + 200_fs) + 500_as) -  (time_point() - (300_ms - 201_fs))) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs) + 501_as).gap(time_point() - (300_ms - 201_fs))  == 1000000000000_ps);
    CHECK(((time_point() + (700_ms + 200_fs) + 501_as) -  (time_point() - (300_ms - 201_fs))) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs) + 999_as).gap(time_point() - (300_ms - 201_fs))  == 1000000000000_ps);
    CHECK(((time_point() + (700_ms + 200_fs) + 999_as) -  (time_point() - (300_ms - 201_fs))) == duration::inf());

    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() - (300_ms - 201_fs) -   1_as)  == 999999999999999_fs);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() - (300_ms - 201_fs) -   1_as)) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() - (300_ms - 201_fs) - 499_as)  == 999999999999999_fs);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() - (300_ms - 201_fs) - 499_as)) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() - (300_ms - 201_fs) - 500_as)  == 1000000000000_ps);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() - (300_ms - 201_fs) - 500_as)) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() - (300_ms - 201_fs) - 501_as)  == 1000000000000_ps);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() - (300_ms - 201_fs) - 501_as)) == duration::inf());
    CHECK(( time_point() + (700_ms + 200_fs)).gap(time_point() - (300_ms - 201_fs) - 999_as)  == 1000000000000_ps);
    CHECK(((time_point() + (700_ms + 200_fs)) -  (time_point() - (300_ms - 201_fs) - 999_as)) == duration::inf());
}

TEST_CASE("test time point borderline cases with time point signs inverted")
{
    CHECK(( time_point() - 700_ms).gap(time_point() - 700_ms)  == 0_s);
    CHECK(((time_point() - 700_ms) -  (time_point() - 700_ms)) == 0_s);

    CHECK(( time_point() - 700_ms +   1_as).gap(time_point() - 700_ms)  == 1_as);
    CHECK(((time_point() - 700_ms +   1_as) -  (time_point() - 700_ms)) == 1_as);
    CHECK(( time_point() - 700_ms + 499_as).gap(time_point() - 700_ms)  == 499_as);
    CHECK(((time_point() - 700_ms + 499_as) -  (time_point() - 700_ms)) == 499_as);
    CHECK(( time_point() - 700_ms + 500_as).gap(time_point() - 700_ms)  == 500_as);
    CHECK(((time_point() - 700_ms + 500_as) -  (time_point() - 700_ms)) == 500_as);
    CHECK(( time_point() - 700_ms + 501_as).gap(time_point() - 700_ms)  == 501_as);
    CHECK(((time_point() - 700_ms + 501_as) -  (time_point() - 700_ms)) == 501_as);
    CHECK(( time_point() - 700_ms + 999_as).gap(time_point() - 700_ms)  == 999_as);
    CHECK(((time_point() - 700_ms + 999_as) -  (time_point() - 700_ms)) == 999_as);

    CHECK(( time_point() - 700_ms).gap(time_point() - 700_ms -   1_as)  == 1_as);
    CHECK(((time_point() - 700_ms) -  (time_point() - 700_ms -   1_as)) == 1_as);
    CHECK(( time_point() - 700_ms).gap(time_point() - 700_ms - 499_as)  == 499_as);
    CHECK(((time_point() - 700_ms) -  (time_point() - 700_ms - 499_as)) == 499_as);
    CHECK(( time_point() - 700_ms).gap(time_point() - 700_ms - 500_as)  == 500_as);
    CHECK(((time_point() - 700_ms) -  (time_point() - 700_ms - 500_as)) == 500_as);
    CHECK(( time_point() - 700_ms).gap(time_point() - 700_ms - 501_as)  == 501_as);
    CHECK(((time_point() - 700_ms) -  (time_point() - 700_ms - 501_as)) == 501_as);
    CHECK(( time_point() - 700_ms).gap(time_point() - 700_ms - 999_as)  == 999_as);
    CHECK(((time_point() - 700_ms) -  (time_point() - 700_ms - 999_as)) == 999_as);

    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() - (700_ms - 200_fs))  == 0_s);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() - (700_ms - 200_fs))) == 0_s);

    CHECK(( time_point() - (700_ms - 200_fs) +   1_as).gap(time_point() - (700_ms - 200_fs))  == 1_as);
    CHECK(((time_point() - (700_ms - 200_fs) +   1_as) -  (time_point() - (700_ms - 200_fs))) == 1_as);
    CHECK(( time_point() - (700_ms - 200_fs) + 499_as).gap(time_point() - (700_ms - 200_fs))  == 499_as);
    CHECK(((time_point() - (700_ms - 200_fs) + 499_as) -  (time_point() - (700_ms - 200_fs))) == 499_as);
    CHECK(( time_point() - (700_ms - 200_fs) + 500_as).gap(time_point() - (700_ms - 200_fs))  == 500_as);
    CHECK(((time_point() - (700_ms - 200_fs) + 500_as) -  (time_point() - (700_ms - 200_fs))) == 500_as);
    CHECK(( time_point() - (700_ms - 200_fs) + 501_as).gap(time_point() - (700_ms - 200_fs))  == 501_as);
    CHECK(((time_point() - (700_ms - 200_fs) + 501_as) -  (time_point() - (700_ms - 200_fs))) == 501_as);
    CHECK(( time_point() - (700_ms - 200_fs) + 999_as).gap(time_point() - (700_ms - 200_fs))  == 999_as);
    CHECK(((time_point() - (700_ms - 200_fs) + 999_as) -  (time_point() - (700_ms - 200_fs))) == 999_as);

    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() - (700_ms - 200_fs) -   1_as)  == 1_as);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() - (700_ms - 200_fs) -   1_as)) == 1_as);
    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() - (700_ms - 200_fs) - 499_as)  == 499_as);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() - (700_ms - 200_fs) - 499_as)) == 499_as);
    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() - (700_ms - 200_fs) - 500_as)  == 500_as);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() - (700_ms - 200_fs) - 500_as)) == 500_as);
    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() - (700_ms - 200_fs) - 501_as)  == 501_as);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() - (700_ms - 200_fs) - 501_as)) == 501_as);
    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() - (700_ms - 200_fs) - 999_as)  == 999_as);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() - (700_ms - 200_fs) - 999_as)) == 999_as);

    CHECK(( time_point() - 700_ms).gap(time_point() + 300_ms)  == -1_s);
    CHECK(((time_point() - 700_ms) -  (time_point() + 300_ms)) == -1_s);

    CHECK(( time_point() - 700_ms +   1_as).gap(time_point() + 300_ms)  == -1000000000000_ps);
    CHECK(((time_point() - 700_ms +   1_as) -  (time_point() + 300_ms)) == -duration::inf());
    CHECK(( time_point() - 700_ms + 499_as).gap(time_point() + 300_ms)  == -1000000000000_ps);
    CHECK(((time_point() - 700_ms + 499_as) -  (time_point() + 300_ms)) == -duration::inf());
    CHECK(( time_point() - 700_ms + 500_as).gap(time_point() + 300_ms)  == -1000000000000_ps);
    CHECK(((time_point() - 700_ms + 500_as) -  (time_point() + 300_ms)) == -duration::inf());
    CHECK(( time_point() - 700_ms + 501_as).gap(time_point() + 300_ms)  == -999999999999999_fs);
    CHECK(((time_point() - 700_ms + 501_as) -  (time_point() + 300_ms)) == -duration::inf());
    CHECK(( time_point() - 700_ms + 999_as).gap(time_point() + 300_ms)  == -999999999999999_fs);
    CHECK(((time_point() - 700_ms + 999_as) -  (time_point() + 300_ms)) == -duration::inf());

    CHECK(( time_point() - 700_ms).gap(time_point() + 300_ms -   1_as)  == -1000000000000_ps);
    CHECK(((time_point() - 700_ms) -  (time_point() + 300_ms -   1_as)) == -duration::inf());
    CHECK(( time_point() - 700_ms).gap(time_point() + 300_ms - 499_as)  == -1000000000000_ps);
    CHECK(((time_point() - 700_ms) -  (time_point() + 300_ms - 499_as)) == -duration::inf());
    CHECK(( time_point() - 700_ms).gap(time_point() + 300_ms - 500_as)  == -1000000000000_ps);
    CHECK(((time_point() - 700_ms) -  (time_point() + 300_ms - 500_as)) == -duration::inf());
    CHECK(( time_point() - 700_ms).gap(time_point() + 300_ms - 501_as)  == -999999999999999_fs);
    CHECK(((time_point() - 700_ms) -  (time_point() + 300_ms - 501_as)) == -duration::inf());
    CHECK(( time_point() - 700_ms).gap(time_point() + 300_ms - 999_as)  == -999999999999999_fs);
    CHECK(((time_point() - 700_ms) -  (time_point() + 300_ms - 999_as)) == -duration::inf());

    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() + (300_ms + 200_fs))  == -1000000000000_ps);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() + (300_ms + 200_fs))) == -1000000000000_ps);

    CHECK(( time_point() - (700_ms - 200_fs) +   1_as).gap(time_point() + (300_ms + 200_fs))  == -1000000000000_ps);
    CHECK(((time_point() - (700_ms - 200_fs) +   1_as) -  (time_point() + (300_ms + 200_fs))) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs) + 499_as).gap(time_point() + (300_ms + 200_fs))  == -1000000000000_ps);
    CHECK(((time_point() - (700_ms - 200_fs) + 499_as) -  (time_point() + (300_ms + 200_fs))) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs) + 500_as).gap(time_point() + (300_ms + 200_fs))  == -1000000000000_ps);
    CHECK(((time_point() - (700_ms - 200_fs) + 500_as) -  (time_point() + (300_ms + 200_fs))) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs) + 501_as).gap(time_point() + (300_ms + 200_fs))  == -999999999999999_fs);
    CHECK(((time_point() - (700_ms - 200_fs) + 501_as) -  (time_point() + (300_ms + 200_fs))) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs) + 999_as).gap(time_point() + (300_ms + 200_fs))  == -999999999999999_fs);
    CHECK(((time_point() - (700_ms - 200_fs) + 999_as) -  (time_point() + (300_ms + 200_fs))) == -duration::inf());

    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() + (300_ms + 200_fs) -   1_as)  == -1000000000000_ps);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() + (300_ms + 200_fs) -   1_as)) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() + (300_ms + 200_fs) - 499_as)  == -1000000000000_ps);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() + (300_ms + 200_fs) - 499_as)) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() + (300_ms + 200_fs) - 500_as)  == -1000000000000_ps);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() + (300_ms + 200_fs) - 500_as)) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() + (300_ms + 200_fs) - 501_as)  == -999999999999999_fs);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() + (300_ms + 200_fs) - 501_as)) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() + (300_ms + 200_fs) - 999_as)  == -999999999999999_fs);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() + (300_ms + 200_fs) - 999_as)) == -duration::inf());

    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() + (300_ms + 199_fs))  == -999999999999999_fs);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() + (300_ms + 199_fs))) == -999999999999999_fs);

    CHECK(( time_point() - (700_ms - 200_fs) -   1_as).gap(time_point() + (300_ms + 199_fs))  == -999999999999999_fs);
    CHECK(((time_point() - (700_ms - 200_fs) -   1_as) -  (time_point() + (300_ms + 199_fs))) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs) - 499_as).gap(time_point() + (300_ms + 199_fs))  == -999999999999999_fs);
    CHECK(((time_point() - (700_ms - 200_fs) - 499_as) -  (time_point() + (300_ms + 199_fs))) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs) - 500_as).gap(time_point() + (300_ms + 199_fs))  == -1000000000000_ps);
    CHECK(((time_point() - (700_ms - 200_fs) - 500_as) -  (time_point() + (300_ms + 199_fs))) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs) - 501_as).gap(time_point() + (300_ms + 199_fs))  == -1000000000000_ps);
    CHECK(((time_point() - (700_ms - 200_fs) - 501_as) -  (time_point() + (300_ms + 199_fs))) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs) - 999_as).gap(time_point() + (300_ms + 199_fs))  == -1000000000000_ps);
    CHECK(((time_point() - (700_ms - 200_fs) - 999_as) -  (time_point() + (300_ms + 199_fs))) == -duration::inf());

    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() + (300_ms + 199_fs) +   1_as)  == -999999999999999_fs);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() + (300_ms + 199_fs) +   1_as)) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() + (300_ms + 199_fs) + 499_as)  == -999999999999999_fs);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() + (300_ms + 199_fs) + 499_as)) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() + (300_ms + 199_fs) + 500_as)  == -1000000000000_ps);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() + (300_ms + 199_fs) + 500_as)) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() + (300_ms + 199_fs) + 501_as)  == -1000000000000_ps);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() + (300_ms + 199_fs) + 501_as)) == -duration::inf());
    CHECK(( time_point() - (700_ms - 200_fs)).gap(time_point() + (300_ms + 199_fs) + 999_as)  == -1000000000000_ps);
    CHECK(((time_point() - (700_ms - 200_fs)) -  (time_point() + (300_ms + 199_fs) + 999_as)) == -duration::inf());
}

TEST_CASE("test time point advancement")
{
    auto tp = time_point();
    tp += 123456789_s;
    tp += 123456789_ns;

    REQUIRE((string_builder() << tp).str() == "time_point() + 123_Ms + 456_ks + 789_s + 123_ms + 456_us + 789_ns");

    tp.advance(100543211_s);

    REQUIRE((string_builder() << tp).str() == "time_point() + 224_Ms");

    tp.advance(3333_fs);

    REQUIRE((string_builder() << tp).str() == "time_point() + 224_Ms + 0_ks + 0_s + 0_ms + 0_us + 0_ns + 3_ps + 333_fs");

    tp.advance(1_Gs);
    
    REQUIRE((string_builder() << tp).str() == "time_point() + 1_Gs");
}

TEST_CASE("test time point advancement with negative time points")
{
    auto tp = time_point();
    tp -= 123456789_s;
    tp -= 123456789_ns;

    REQUIRE((string_builder() << tp).str() == "time_point() - 123_Ms - 456_ks - 789_s - 123_ms - 456_us - 789_ns");

    tp.advance(100456789_s);

    REQUIRE((string_builder() << tp).str() == "time_point() - 23_Ms - 0_ks - 1_s");
    
    tp.advance(1_ns);

    REQUIRE((string_builder() << tp).str() == "time_point() - 23_Ms - 0_ks - 0_s - 999_ms - 999_us - 999_ns");

    tp.advance(1_ms);

    REQUIRE((string_builder() << tp).str() == "time_point() - 23_Ms - 0_ks - 0_s - 999_ms");

    tp.advance(1_s);

    REQUIRE((string_builder() << tp).str() == "time_point() - 23_Ms");

    tp.advance(1_Gs);

    REQUIRE((string_builder() << tp).str() == "time_point()");

    tp.advance(123456789_ns);

    REQUIRE((string_builder() << tp).str() == "time_point() + 123_ms + 456_us + 789_ns");
}

TEST_CASE("test time point with assorted operations")
{
    auto tp = time_point();

    REQUIRE((string_builder() << (tp += 5000388_ns)).str() == "time_point() + 5_ms + 0_us + 388_ns");
    REQUIRE((string_builder() << (tp += 1777_ps)).str()    == "time_point() + 5_ms + 0_us + 389_ns + 777_ps");
    REQUIRE((string_builder() << (tp += 640_us)).str()     == "time_point() + 5_ms + 640_us + 389_ns + 777_ps");
    REQUIRE((string_builder() << (tp -= 40109_ns)).str()   == "time_point() + 5_ms + 600_us + 280_ns + 777_ps");
    REQUIRE((string_builder() << (tp -= 5_ms)).str()       == "time_point() + 600_us + 280_ns + 777_ps");
    REQUIRE((string_builder() << (tp += 223_ps)).str()     == "time_point() + 600_us + 281_ns");

    CHECK((string_builder() << ((time_point() + 31_s + 775_ms + 100_us) -  (time_point() + 1_s + 170_ms))).str()        == "30605100_us");
    CHECK((string_builder() << ((time_point() + 31_s + 775_ms + 100_us).gap(time_point() + 1_s + 170_ms))).str()        == "30605100_us");
    CHECK((string_builder() << ((time_point() + 7_s + 3_ms + 5_us + 6_ns + 2_ps + 9_fs) -  (time_point() + 7_s))).str() == "3005006002009_fs");
    CHECK((string_builder() << ((time_point() + 7_s + 3_ms + 5_us + 6_ns + 2_ps + 9_fs).gap(time_point() + 7_s))).str() == "3005006002009_fs");

    CHECK((string_builder() << ((time_point() + 7_s + 3_ms + 5_us + 6_ns + 2_ps + 9_fs) -  (time_point() + 6_s))).str() == "duration::inf()");
    CHECK((string_builder() << ((time_point() + 7_s + 3_ms + 5_us + 6_ns + 2_ps + 9_fs).gap(time_point() + 6_s))).str() == "1003005006002_ps");

    CHECK((string_builder() << ((time_point() + 6_s) -  (time_point() + 7_s + 3_ms + 5_us + 6_ns + 2_ps + 9_fs))).str() == "-duration::inf()");
    CHECK((string_builder() << ((time_point() + 6_s).gap(time_point() + 7_s + 3_ms + 5_us + 6_ns + 2_ps + 9_fs))).str() == "-1003005006002_ps");

    CHECK((string_builder() << time_point(1_s).gap(time_point(2800_as))).str() == "999999999999997_fs");

    tp = time_point(72800444321_ns);

    SECTION("advance by nanoseconds") {
        tp.advance(1150000000_ns);
        CHECK((string_builder() << tp).str() == "time_point() + 73_s + 950_ms + 444_us + 321_ns");
    }

    SECTION("advance by microseconds") {
        tp.advance(1150000_us);
        CHECK((string_builder() << tp).str() == "time_point() + 73_s + 950_ms + 444_us");
    }

    SECTION("advance by milliseconds") {
        tp.advance(1150_ms);
        CHECK((string_builder() << tp).str() == "time_point() + 73_s + 950_ms");
    }
}


}  // namespace
