#include <catch2/catch.hpp>
#include <sydevs/core/string_builder.h>
#include <sydevs/core/quantity.h>

namespace sydevs {


TEST_CASE("test duration constructors")
{
    CHECK(!duration().valid());
    CHECK(duration(0).multiplier() == 0);
    CHECK(duration(0).precision() == unit);
    CHECK(duration(100).multiplier() == 100);
    CHECK(duration(100).precision() == unit);
    CHECK(duration(-100).multiplier() == -100);
    CHECK(duration(-100).precision() == unit);
    CHECK(duration(100, nano).multiplier() == 100);
    CHECK(duration(100, nano).precision() == nano);
    CHECK(duration(-100, nano).multiplier() == -100);
    CHECK(duration(-100, nano).precision() == nano);

    CHECK(duration(quantity_limit - 1).multiplier() == quantity_limit - 1);
    CHECK(duration(quantity_limit - 1).finite());
    CHECK(!duration(quantity_limit).finite());
    CHECK(!duration(quantity_limit + 1).finite());
    CHECK(duration(1 - quantity_limit).multiplier() == 1 - quantity_limit);
    CHECK(duration(1 - quantity_limit).finite());
    CHECK(!duration(quantity_limit).finite());
    CHECK(!duration(-1 - quantity_limit).finite());
}

TEST_CASE("test duration literals")
{
    CHECK(0_s == duration(0));
    CHECK((0_s).multiplier() == 0);
    CHECK((0_s).precision() == unit);
    CHECK(0_us == duration(0));
    CHECK((0_us).multiplier() == 0);
    CHECK((0_us).precision() == micro);
    CHECK(0_Ms == duration(0));
    CHECK((0_Ms).multiplier() == 0);
    CHECK((0_Ms).precision() == mega);

    CHECK(1_s == duration(1, unit));
    CHECK(1_min == duration(60, unit));
    CHECK(1_hr == duration(3600, unit));
    CHECK(1_day == duration(86400, unit));
    CHECK(1_yr == duration(31536000, unit));
    CHECK(1_ys == duration(1, yocto));
    CHECK(1_zs == duration(1, zepto));
    CHECK(1_as == duration(1, atto));
    CHECK(1_fs == duration(1, femto));
    CHECK(1_ps == duration(1, pico));
    CHECK(1_ns == duration(1, nano));
    CHECK(1_us == duration(1, micro));
    CHECK(1_ms == duration(1, milli));
    CHECK(1_s == duration(1, unit));
    CHECK(1_ks == duration(1, kilo));
    CHECK(1_Ms == duration(1, mega));
    CHECK(1_Gs == duration(1, giga));
    CHECK(1_Ts == duration(1, tera));
    CHECK(1_Ps == duration(1, peta));
    CHECK(1_Es == duration(1, exa));
    CHECK(1_Zs == duration(1, zetta));
    CHECK(1_Ys == duration(1, yotta));

    CHECK(999999999999999_s == duration(999999999999999, unit));
    CHECK(999999999999999_us == duration(999999999999999, micro));
    CHECK(1000000000000000_s == duration::inf());
    CHECK(1000000000000000_us == duration::inf());
    CHECK(-999999999999999_s == duration(-999999999999999, unit));
    CHECK(-999999999999999_us == duration(-999999999999999, micro));
    CHECK(-1000000000000000_s == -duration::inf());
    CHECK(-1000000000000000_us == -duration::inf());
}

TEST_CASE("test non-operator duration methods with duration return values")
{
    CHECK((5_s).refined().multiplier() == 5000000000000);
    CHECK((5_s).refined().precision() == pico);
    CHECK((5_s).refined().fixed() == false);
    CHECK((5_s).coarsened().multiplier() == 5);
    CHECK((5_s).coarsened().precision() == unit);
    CHECK((5_s).coarsened().fixed() == false);
    CHECK((5_s).refined().coarsened().multiplier() == 5);
    CHECK((5_s).refined().coarsened().precision() == unit);
    CHECK((5_s).refined().coarsened().fixed() == false);
    CHECK((5_s).fixed_at(nano).multiplier() == 5000000000);
    CHECK((5_s).fixed_at(nano).precision() == nano);
    CHECK((5_s).fixed_at(nano).fixed() == true);
    CHECK((83000_us).fixed_at(milli).multiplier() == 83);
    CHECK((83000_us).fixed_at(milli).precision() == milli);
    CHECK((83000_us).fixed_at(milli).fixed() == true);
    CHECK((83123_us).fixed_at(milli).multiplier() == 83);
    CHECK((83123_us).fixed_at(milli).precision() == milli);
    CHECK((83123_us).fixed_at(milli).fixed() == true);
    CHECK((5_s).rescaled(nano).multiplier() == 5000000000);
    CHECK((5_s).rescaled(nano).precision() == nano);
    CHECK((5_s).rescaled(nano).fixed() == false);
    CHECK((83000_us).rescaled(milli).multiplier() == 83);
    CHECK((83000_us).rescaled(milli).precision() == milli);
    CHECK((83000_us).rescaled(milli).fixed() == false);
    CHECK((83123_us).rescaled(milli).multiplier() == 83);
    CHECK((83123_us).rescaled(milli).precision() == milli);
    CHECK((83123_us).rescaled(milli).fixed() == false);
    CHECK((83000_us).fixed_at(milli).unfixed().multiplier() == 83);
    CHECK((83000_us).fixed_at(milli).unfixed().precision() == milli);
    CHECK((83000_us).fixed_at(milli).unfixed().fixed() == false);
    CHECK((83123_us).fixed_at(milli).unfixed().multiplier() == 83);
    CHECK((83123_us).fixed_at(milli).unfixed().precision() == milli);
    CHECK((83123_us).fixed_at(milli).unfixed().fixed() == false);
}

TEST_CASE("test duration base-1000 floating-point addition")
{
    CHECK((3_hr + 45_s) == 10845_s);
    CHECK((3_hr + 45_s).precision() == unit);
    CHECK((3_hr + 45_s).refined() == 10845000000000_ns);
    CHECK((3_hr + 45_s).refined().precision() == nano);
    CHECK((3_hr + 45_s).coarsened() == 10845_s);
    CHECK((3_hr + 45_s).coarsened().precision() == unit);
    CHECK((3_hr + 45_s).refined().coarsened() == 10845_s);
    CHECK((3_hr + 45_s).refined().coarsened().precision() == unit);
    CHECK(((1_s).rescaled(milli) + (1_s).rescaled(micro)) == 2000000_us);
    CHECK(((1_s).rescaled(milli) + (1_s).rescaled(micro)).precision() == micro);
    CHECK(!((1_s).fixed_at(milli) + (1_s).fixed_at(micro)).valid());
    CHECK(((47_ms).rescaled(pico) + 1_yr) == 31536000047000_us);
    CHECK(((47_ms).rescaled(pico) + 1_yr).precision() == micro);
    CHECK(((47_ms).rescaled(femto) + 1_yr) == 31536000047000_us);
    CHECK(((47_ms).rescaled(femto) + 1_yr).precision() == micro);
    CHECK((47_ms).rescaled(atto) == +duration::inf());
    CHECK(((47_ms).rescaled(atto) + 1_yr) == +duration::inf());
    CHECK((1_yr + (47_ms).rescaled(atto)) == +duration::inf());
    CHECK(((47_us).rescaled(atto) + 1_yr) == 31536000000047_us);
    CHECK(((47_us).rescaled(atto) + 1_yr).precision() == micro);
    CHECK((1_yr + (47_us).rescaled(atto)) == 31536000000047_us);
    CHECK((1_yr + (47_us).rescaled(atto)).precision() == micro);
    CHECK((500000000000000_Ms + 500000000000000_Ms) == 1000000000000_Gs);
    CHECK((500000000000000_Ms + 500000000000000_Ms).precision() == giga);
    CHECK(((500000000000000_Ms).fixed_at(mega) + (500000000000000_Ms).fixed_at(mega)) == +duration::inf());
    CHECK(((500000000000000_Ms).fixed_at(mega) + (500000000000000_Ms).fixed_at(mega)).precision() == unit);
}

TEST_CASE("test duration negation base-1000 floating-point subtraction")
{
    CHECK(-1_min == -60_s);
    CHECK((-1_min).precision() == unit);
    CHECK((5_min - 1_hr) == -3300_s);
    CHECK((5_min - 1_hr).precision() == unit);
    CHECK(((47_ms).rescaled(atto) - 1_yr) == +duration::inf());
    CHECK((1_yr - (47_ms).rescaled(atto)) == -duration::inf());
    CHECK(((47_us).rescaled(atto) - 1_yr) == -31535999999953_us);
    CHECK(((47_us).rescaled(atto) - 1_yr).precision() == micro);
    CHECK((1_yr - (47_us).rescaled(atto)) == +31535999999953_us);
    CHECK((1_yr - (47_us).rescaled(atto)).precision() == micro);
    CHECK((-duration::inf()) == -duration::inf());
    CHECK(!(duration::inf() - duration::inf()).valid());
    CHECK((1_s - 1_s).precision() == unit);
    CHECK((1_ms - 1_ms).precision() == milli);
    CHECK((1_us - 1_us).precision() == micro);
    CHECK((1_ks - 1_ks).precision() == kilo);
}

TEST_CASE("test duration multiplication by number")
{
    CHECK((1_hr*3) == 10800_s);
    CHECK((1_hr*3).precision() == unit);
    CHECK((3*1_hr) == 10800_s);
    CHECK((3*1_hr).precision() == unit);
    CHECK((1_hr*1000000000000000.0) == 3600000000000_Ms);
    CHECK((1_hr*1000000000000000.0).precision() == mega);
    CHECK((1_hr*0.000000000000001) == 3600000000000_ys);
    CHECK((1_hr*0.000000000000001).precision() == yocto);
    CHECK(((1_hr).fixed_at(unit)*3) == 10800_s);
    CHECK(((1_hr).fixed_at(unit)*3).precision() == unit);
    CHECK(((1_hr).fixed_at(unit)*1000000000000000.0) == +duration::inf());
    CHECK(((1_hr).fixed_at(unit)*0.000000000000001) == 0_s);
    CHECK(((1_hr).fixed_at(unit)*0.000000000000001).precision() == unit);
}

TEST_CASE("test duration division by number")
{
    CHECK((1_hr/3) == 1200_s);
    CHECK((1_hr/3).precision() == unit);
    CHECK((1_hr/1000000000000000.0) == 3600_fs);
    CHECK((1_hr/1000000000000000.0).precision() == femto);
    CHECK((1_hr/0.000000000000001) == 3600000000000_Ms);
    CHECK((1_hr/0.000000000000001).precision() == mega);
    CHECK(((1_hr).fixed_at(unit)/3) == 1200_s);
    CHECK(((1_hr).fixed_at(unit)/3).precision() == unit);
    CHECK(((1_hr).fixed_at(unit)/1000000000000000.0) == 0_s);
    CHECK(((1_hr).fixed_at(unit)/1000000000000000.0).precision() == unit);
    CHECK(((1_hr).fixed_at(unit)/0.000000000000001) == +duration::inf());
}

TEST_CASE("test duration-duration division")
{
    CHECK((1_hr/1_min) == 60);
    CHECK((1_hr/2_s) == 1800);
    CHECK((1_min/1_hr) == Approx(0.0166667));
    CHECK((5_s/1_ns) == Approx(5e+09));
    CHECK((1_ns/1_s) == Approx(1e-09));
}

TEST_CASE("test duration-duration comparisons")
{
    CHECK((1_ns == 1_s) == false);
    CHECK((1_s == 1_ns) == false);
    CHECK((1_ns == 1_ns) == true);
    CHECK((1_ns != 1_s) == true);
    CHECK((1_s != 1_ns) == true);
    CHECK((1_ns != 1_ns) == false);
    CHECK((1_ns < 1_s) == true);
    CHECK((1_s < 1_ns) == false);
    CHECK((1_ns < 1_ns) == false);
    CHECK((1_ns > 1_s) == false);
    CHECK((1_s > 1_ns) == true);
    CHECK((1_ns > 1_ns) == false);
    CHECK((1_ns <= 1_s) == true);
    CHECK((1_s <= 1_ns) == false);
    CHECK((1_ns <= 1_ns) == true);
    CHECK((1_ns >= 1_s) == false);
    CHECK((1_s >= 1_ns) == true);
    CHECK((1_ns >= 1_ns) == true);
}

TEST_CASE("test duration add/subtract/multiply/divide assignment operations")
{
    auto dt = 1_hr;

    SECTION("add assignment") {
        CHECK((dt += 1_min) == 3660_s);
        CHECK(dt == 3660_s);
        CHECK(dt.precision() == unit);
    }

    SECTION("subtract assignment") {
        CHECK((dt -= 1_min) == 3540_s);
        CHECK(dt == 3540_s);
        CHECK(dt.precision() == unit);
    }

    SECTION("multiply assignment") {
        CHECK((dt *= 3.14159) == 11309724_ms);
        CHECK(dt == 11309724_ms);
        CHECK(dt.precision() == milli);
    }

    SECTION("divide assignment") {
        CHECK((dt /= 4) == 900_s);
        CHECK(dt == 900_s);
        CHECK(dt.precision() == unit);
    }
}

TEST_CASE("test max_duration")
{
    CHECK(duration::max(unit) == 999999999999999_s);
    CHECK(duration::max(yocto) == 999999999999999_ys);
}

TEST_CASE("test duration addition and subtraction with both fixed and unfixed operands")
{
    CHECK(((1_s).fixed_at(micro) + (1_s).rescaled(nano)) == 2000000_us);
    CHECK(((1_s).fixed_at(micro) + (1_s).rescaled(nano)).precision() == micro);
    CHECK(((1_s).fixed_at(nano) + (1_s).rescaled(micro)) == 2000000000_ns);
    CHECK(((1_s).fixed_at(nano) + (1_s).rescaled(micro)).precision() == nano);
    CHECK(((1_s).rescaled(micro) + (1_s).fixed_at(nano)) == 2000000000_ns);
    CHECK(((1_s).rescaled(micro) + (1_s).fixed_at(nano)).precision() == nano);
    CHECK(((1_s).rescaled(nano) + (1_s).fixed_at(micro)) == 2000000_us);
    CHECK(((1_s).rescaled(nano) + (1_s).fixed_at(micro)).precision() == micro);
    CHECK(((2_s).fixed_at(micro) - (1_s).rescaled(nano)) == 1000000_us);
    CHECK(((2_s).fixed_at(micro) - (1_s).rescaled(nano)).precision() == micro);
    CHECK(((2_s).fixed_at(nano) - (1_s).rescaled(micro)) == 1000000000_ns);
    CHECK(((2_s).fixed_at(nano) - (1_s).rescaled(micro)).precision() == nano);
    CHECK(((2_s).rescaled(micro) - (1_s).fixed_at(nano)) == 1000000000_ns);
    CHECK(((2_s).rescaled(micro) - (1_s).fixed_at(nano)).precision() == nano);
    CHECK(((2_s).rescaled(nano) - (1_s).fixed_at(micro)) == 1000000_us);
    CHECK(((2_s).rescaled(nano) - (1_s).fixed_at(micro)).precision() == micro);
}

TEST_CASE("test duration with assorted operations")
{
    CHECK((string_builder() << (2_s).fixed_at(unit) + (3_s).fixed_at(unit)).str() == "5_s");
    CHECK((string_builder() << (2_s).fixed_at(unit) - (3_s).fixed_at(unit)).str() == "-1_s");
    CHECK((string_builder() << 5*(100_s).fixed_at(unit)).str() == "500_s");
    CHECK((string_builder() << (1.0/5.0)*(100_s).fixed_at(unit)).str() == "20_s");
    CHECK((string_builder() << (100_s).fixed_at(unit)*(1.0/5.0)).str() == "20_s");
    CHECK((string_builder() << (100_s).fixed_at(unit)/5.0).str() == "20_s");
    CHECK((string_builder() << (100_s).fixed_at(unit)/8.0).str() == "13_s");
    CHECK(2_s > 1000_ms);
    CHECK(2_s < 3000_ms);
    CHECK(-8_ps < -7_ps);
    CHECK(1_s == 1000_ms);
    CHECK(1_s == 1000000_us);
    CHECK(1000_ms == 1000000_us);
    CHECK(((500_ms).fixed_at(milli) + (500_ms).fixed_at(milli)) == 1_s);
    CHECK((string_builder() << (500_ms).fixed_at(milli) + (500_ms).fixed_at(milli)).str() == "1000_ms");
    CHECK((string_builder() << (999999999999999_Ms).fixed_at(mega) + (1_Ms).fixed_at(mega)).str() == "duration::inf()");
    CHECK((string_builder() << -1000000*(1000000000_fs).fixed_at(femto)).str() == "-duration::inf()");
    CHECK((string_builder() << 3_s + 475_ms).str() == "3475_ms");
    CHECK((string_builder() << 1_ks + 1_us).str() == "1000000001_us");
    CHECK((string_builder() << 500_ps - 1_ns).str() == "-500_ps");
    CHECK((string_builder() << (1.0/3.0)*(1_s)).str() == "333333333333333_fs");
    CHECK((string_builder() << (1.0/3.0)*(1000_ms)).str() == "333333333333333_fs");
    CHECK((string_builder() << 1000_ms/3.0).str() == "333333333333333_fs");
    CHECK((string_builder() << (1.0/3.0)*(1000000_us)).str() == "333333333333333_fs");
    CHECK((string_builder() << (999999999999999_Ms) + (1_Ms)).str() == "1000000000000_Gs");
    CHECK((string_builder() << -1000000*(1000000000_fs)).str() == "-1000000000000_ps");
    CHECK((10_ms/250_us) == Approx(40.0));
    CHECK(((-100_s)/900_s) == Approx(-1.0/9.0));
    CHECK(((123_ms)/1_s) == Approx(0.123));
    CHECK(((123_ms)/1_ms) == 123);
    CHECK(((123_ms)/0_s) == std::numeric_limits<float64>::infinity());
    CHECK(duration::inf().precision() == unit);
    CHECK(-duration::inf().precision() == unit);
    CHECK(duration(0, unit).precision() == unit);
    CHECK(duration(0, milli).precision() == milli);
    CHECK(duration(0, micro).precision() == micro);
    CHECK((string_builder() << (1_min + 40_s).fixed_at(micro)/8.0).str() == "12500000_us");
    CHECK((string_builder() << (1_min + 40_s).fixed_at(milli)/8.0).str() == "12500_ms");
    CHECK((string_builder() << (1_min + 40_s).fixed_at(unit)/8.0).str() == "13_s");
    CHECK((string_builder() << (1_min + 40_s).rescaled(micro)/8.0).str() == "12500000_us");
    CHECK((string_builder() << (1_min + 40_s).rescaled(milli)/8.0).str() == "12500_ms");
    CHECK((string_builder() << (1_min + 40_s).rescaled(unit)/8.0).str() == "12500_ms");
    CHECK((string_builder() << (4_s + 10_ms)).str() == "4010_ms");
    CHECK((string_builder() << (4_s + 10_ms)/4).str() == "1002500_us");
    CHECK((string_builder() << (4_s + 10_ms).fixed_at(milli)/4).str() == "1003_ms");
    CHECK((string_builder() << (4_s + 10_ms).fixed_at(micro)/4).str() == "1002500_us");
}


}  // namespace
