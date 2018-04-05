#include <catch2/catch.hpp>
#include <sydevs/core/string_builder.h>
#include <sydevs/core/quantity.h>

namespace sydevs {


TEST_CASE("test distance constructors")
{
    CHECK(!distance().valid());
    CHECK(distance(0).multiplier() == 0);
    CHECK(distance(0).precision() == unit);
    CHECK(distance(100).multiplier() == 100);
    CHECK(distance(100).precision() == unit);
    CHECK(distance(-100).multiplier() == -100);
    CHECK(distance(-100).precision() == unit);
    CHECK(distance(100, nano).multiplier() == 100);
    CHECK(distance(100, nano).precision() == nano);
    CHECK(distance(-100, nano).multiplier() == -100);
    CHECK(distance(-100, nano).precision() == nano);

    CHECK(distance(quantity_limit - 1).multiplier() == quantity_limit - 1);
    CHECK(distance(quantity_limit - 1).finite());
    CHECK(!distance(quantity_limit).finite());
    CHECK(!distance(quantity_limit + 1).finite());
    CHECK(distance(1 - quantity_limit).multiplier() == 1 - quantity_limit);
    CHECK(distance(1 - quantity_limit).finite());
    CHECK(!distance(quantity_limit).finite());
    CHECK(!distance(-1 - quantity_limit).finite());
}

TEST_CASE("test distance literals")
{
    CHECK(0_m == distance(0));
    CHECK((0_m).multiplier() == 0);
    CHECK((0_m).precision() == unit);
    CHECK(0_um == distance(0));
    CHECK((0_um).multiplier() == 0);
    CHECK((0_um).precision() == micro);
    CHECK(0_Mm == distance(0));
    CHECK((0_Mm).multiplier() == 0);
    CHECK((0_Mm).precision() == mega);

    CHECK(1_ym == distance(1, yocto));
    CHECK(1_zm == distance(1, zepto));
    CHECK(1_am == distance(1, atto));
    CHECK(1_fm == distance(1, femto));
    CHECK(1_pm == distance(1, pico));
    CHECK(1_nm == distance(1, nano));
    CHECK(1_um == distance(1, micro));
    CHECK(1_mm == distance(1, milli));
    CHECK(1_m == distance(1, unit));
    CHECK(1_km == distance(1, kilo));
    CHECK(1_Mm == distance(1, mega));
    CHECK(1_Gm == distance(1, giga));
    CHECK(1_Tm == distance(1, tera));
    CHECK(1_Pm == distance(1, peta));
    CHECK(1_Em == distance(1, exa));
    CHECK(1_Zm == distance(1, zetta));
    CHECK(1_Ym == distance(1, yotta));

    CHECK(999999999999999_m == distance(999999999999999, unit));
    CHECK(999999999999999_um == distance(999999999999999, micro));
    CHECK(1000000000000000_m == distance::inf());
    CHECK(1000000000000000_um == distance::inf());
    CHECK(-999999999999999_m == distance(-999999999999999, unit));
    CHECK(-999999999999999_um == distance(-999999999999999, micro));
    CHECK(-1000000000000000_m == -distance::inf());
    CHECK(-1000000000000000_um == -distance::inf());
}

TEST_CASE("test non-operator distance methods with distance return values")
{
    CHECK((5_m).refined().multiplier() == 5000000000000);
    CHECK((5_m).refined().precision() == pico);
    CHECK((5_m).refined().fixed() == false);
    CHECK((5_m).coarsened().multiplier() == 5);
    CHECK((5_m).coarsened().precision() == unit);
    CHECK((5_m).coarsened().fixed() == false);
    CHECK((5_m).refined().coarsened().multiplier() == 5);
    CHECK((5_m).refined().coarsened().precision() == unit);
    CHECK((5_m).refined().coarsened().fixed() == false);
    CHECK((5_m).fixed_at(nano).multiplier() == 5000000000);
    CHECK((5_m).fixed_at(nano).precision() == nano);
    CHECK((5_m).fixed_at(nano).fixed() == true);
    CHECK((83000_um).fixed_at(milli).multiplier() == 83);
    CHECK((83000_um).fixed_at(milli).precision() == milli);
    CHECK((83000_um).fixed_at(milli).fixed() == true);
    CHECK((83123_um).fixed_at(milli).multiplier() == 83);
    CHECK((83123_um).fixed_at(milli).precision() == milli);
    CHECK((83123_um).fixed_at(milli).fixed() == true);
    CHECK((5_m).rescaled(nano).multiplier() == 5000000000);
    CHECK((5_m).rescaled(nano).precision() == nano);
    CHECK((5_m).rescaled(nano).fixed() == false);
    CHECK((83000_um).rescaled(milli).multiplier() == 83);
    CHECK((83000_um).rescaled(milli).precision() == milli);
    CHECK((83000_um).rescaled(milli).fixed() == false);
    CHECK((83123_um).rescaled(milli).multiplier() == 83);
    CHECK((83123_um).rescaled(milli).precision() == milli);
    CHECK((83123_um).rescaled(milli).fixed() == false);
    CHECK((83000_um).fixed_at(milli).unfixed().multiplier() == 83);
    CHECK((83000_um).fixed_at(milli).unfixed().precision() == milli);
    CHECK((83000_um).fixed_at(milli).unfixed().fixed() == false);
    CHECK((83123_um).fixed_at(milli).unfixed().multiplier() == 83);
    CHECK((83123_um).fixed_at(milli).unfixed().precision() == milli);
    CHECK((83123_um).fixed_at(milli).unfixed().fixed() == false);
}

TEST_CASE("test distance base-1000 floating-point addition")
{
    CHECK((10800_m + 45_m) == 10845_m);
    CHECK((10800_m + 45_m).precision() == unit);
    CHECK((10800_m + 45_m).refined() == 10845000000000_nm);
    CHECK((10800_m + 45_m).refined().precision() == nano);
    CHECK((10800_m + 45_m).coarsened() == 10845_m);
    CHECK((10800_m + 45_m).coarsened().precision() == unit);
    CHECK((10800_m + 45_m).refined().coarsened() == 10845_m);
    CHECK((10800_m + 45_m).refined().coarsened().precision() == unit);
    CHECK(((1_m).rescaled(milli) + (1_m).rescaled(micro)) == 2000000_um);
    CHECK(((1_m).rescaled(milli) + (1_m).rescaled(micro)).precision() == micro);
    CHECK(!((1_m).fixed_at(milli) + (1_m).fixed_at(micro)).valid());
    CHECK(((47_mm).rescaled(pico) + 31536000_m) == 31536000047000_um);
    CHECK(((47_mm).rescaled(pico) + 31536000_m).precision() == micro);
    CHECK(((47_mm).rescaled(femto) + 31536000_m) == 31536000047000_um);
    CHECK(((47_mm).rescaled(femto) + 31536000_m).precision() == micro);
    CHECK((47_mm).rescaled(atto) == +distance::inf());
    CHECK(((47_mm).rescaled(atto) + 31536000_m) == +distance::inf());
    CHECK((31536000_m + (47_mm).rescaled(atto)) == +distance::inf());
    CHECK(((47_um).rescaled(atto) + 31536000_m) == 31536000000047_um);
    CHECK(((47_um).rescaled(atto) + 31536000_m).precision() == micro);
    CHECK((31536000_m + (47_um).rescaled(atto)) == 31536000000047_um);
    CHECK((31536000_m + (47_um).rescaled(atto)).precision() == micro);
    CHECK((500000000000000_Mm + 500000000000000_Mm) == 1000000000000_Gm);
    CHECK((500000000000000_Mm + 500000000000000_Mm).precision() == giga);
    CHECK(((500000000000000_Mm).fixed_at(mega) + (500000000000000_Mm).fixed_at(mega)) == +distance::inf());
    CHECK(((500000000000000_Mm).fixed_at(mega) + (500000000000000_Mm).fixed_at(mega)).precision() == unit);
}

TEST_CASE("test distance negation base-1000 floating-point subtraction")
{
    CHECK(-60_m == -60_m);
    CHECK((-60_m).precision() == unit);
    CHECK((300_m - 3600_m) == -3300_m);
    CHECK((300_m - 3600_m).precision() == unit);
    CHECK(((47_mm).rescaled(atto) - 31536000_m) == +distance::inf());
    CHECK((31536000_m - (47_mm).rescaled(atto)) == -distance::inf());
    CHECK(((47_um).rescaled(atto) - 31536000_m) == -31535999999953_um);
    CHECK(((47_um).rescaled(atto) - 31536000_m).precision() == micro);
    CHECK((31536000_m - (47_um).rescaled(atto)) == +31535999999953_um);
    CHECK((31536000_m - (47_um).rescaled(atto)).precision() == micro);
    CHECK((-distance::inf()) == -distance::inf());
    CHECK(!(distance::inf() - distance::inf()).valid());
    CHECK((1_m - 1_m).precision() == unit);
    CHECK((1_mm - 1_mm).precision() == milli);
    CHECK((1_um - 1_um).precision() == micro);
    CHECK((1_km - 1_km).precision() == kilo);
}

TEST_CASE("test distance multiplication by number")
{
    CHECK((3600_m*3) == 10800_m);
    CHECK((3600_m*3).precision() == unit);
    CHECK((3*3600_m) == 10800_m);
    CHECK((3*3600_m).precision() == unit);
    CHECK((3600_m*1000000000000000.0) == 3600000000000_Mm);
    CHECK((3600_m*1000000000000000.0).precision() == mega);
    CHECK((3600_m*0.000000000000001) == 3600000000000_ym);
    CHECK((3600_m*0.000000000000001).precision() == yocto);
    CHECK(((3600_m).fixed_at(unit)*3) == 10800_m);
    CHECK(((3600_m).fixed_at(unit)*3).precision() == unit);
    CHECK(((3600_m).fixed_at(unit)*1000000000000000.0) == +distance::inf());
    CHECK(((3600_m).fixed_at(unit)*0.000000000000001) == 0_m);
    CHECK(((3600_m).fixed_at(unit)*0.000000000000001).precision() == unit);
}

TEST_CASE("test distance division by number")
{
    CHECK((3600_m/3) == 1200_m);
    CHECK((3600_m/3).precision() == unit);
    CHECK((3600_m/1000000000000000.0) == 3600_fm);
    CHECK((3600_m/1000000000000000.0).precision() == femto);
    CHECK((3600_m/0.000000000000001) == 3600000000000_Mm);
    CHECK((3600_m/0.000000000000001).precision() == mega);
    CHECK(((3600_m).fixed_at(unit)/3) == 1200_m);
    CHECK(((3600_m).fixed_at(unit)/3).precision() == unit);
    CHECK(((3600_m).fixed_at(unit)/1000000000000000.0) == 0_m);
    CHECK(((3600_m).fixed_at(unit)/1000000000000000.0).precision() == unit);
    CHECK(((3600_m).fixed_at(unit)/0.000000000000001) == +distance::inf());
}

TEST_CASE("test distance-distance division")
{
    CHECK((3600_m/60_m) == 60);
    CHECK((3600_m/2_m) == 1800);
    CHECK((60_m/3600_m) == Approx(0.0166667));
    CHECK((5_m/1_nm) == Approx(5e+09));
    CHECK((1_nm/1_m) == Approx(1e-09));
}

TEST_CASE("test distance-distance comparisons")
{
    CHECK((1_nm == 1_m) == false);
    CHECK((1_m == 1_nm) == false);
    CHECK((1_nm == 1_nm) == true);
    CHECK((1_nm != 1_m) == true);
    CHECK((1_m != 1_nm) == true);
    CHECK((1_nm != 1_nm) == false);
    CHECK((1_nm < 1_m) == true);
    CHECK((1_m < 1_nm) == false);
    CHECK((1_nm < 1_nm) == false);
    CHECK((1_nm > 1_m) == false);
    CHECK((1_m > 1_nm) == true);
    CHECK((1_nm > 1_nm) == false);
    CHECK((1_nm <= 1_m) == true);
    CHECK((1_m <= 1_nm) == false);
    CHECK((1_nm <= 1_nm) == true);
    CHECK((1_nm >= 1_m) == false);
    CHECK((1_m >= 1_nm) == true);
    CHECK((1_nm >= 1_nm) == true);
}

TEST_CASE("test distance add/subtract/multiply/divide assignment operations")
{
    auto dx = 3600_m;

    SECTION("add assignment") {
        CHECK((dx += 60_m) == 3660_m);
        CHECK(dx == 3660_m);
        CHECK(dx.precision() == unit);
    }

    SECTION("subtract assignment") {
        CHECK((dx -= 60_m) == 3540_m);
        CHECK(dx == 3540_m);
        CHECK(dx.precision() == unit);
    }

    SECTION("multiply assignment") {
        CHECK((dx *= 3.14159) == 11309724_mm);
        CHECK(dx == 11309724_mm);
        CHECK(dx.precision() == milli);
    }

    SECTION("divide assignment") {
        CHECK((dx /= 4) == 900_m);
        CHECK(dx == 900_m);
        CHECK(dx.precision() == unit);
    }
}

TEST_CASE("test max_distance")
{
    CHECK(distance::max(unit) == 999999999999999_m);
    CHECK(distance::max(yocto) == 999999999999999_ym);
}

TEST_CASE("test distance addition and subtraction with both fixed and unfixed operands")
{
    CHECK(((1_m).fixed_at(micro) + (1_m).rescaled(nano)) == 2000000_um);
    CHECK(((1_m).fixed_at(micro) + (1_m).rescaled(nano)).precision() == micro);
    CHECK(((1_m).fixed_at(nano) + (1_m).rescaled(micro)) == 2000000000_nm);
    CHECK(((1_m).fixed_at(nano) + (1_m).rescaled(micro)).precision() == nano);
    CHECK(((1_m).rescaled(micro) + (1_m).fixed_at(nano)) == 2000000000_nm);
    CHECK(((1_m).rescaled(micro) + (1_m).fixed_at(nano)).precision() == nano);
    CHECK(((1_m).rescaled(nano) + (1_m).fixed_at(micro)) == 2000000_um);
    CHECK(((1_m).rescaled(nano) + (1_m).fixed_at(micro)).precision() == micro);
    CHECK(((2_m).fixed_at(micro) - (1_m).rescaled(nano)) == 1000000_um);
    CHECK(((2_m).fixed_at(micro) - (1_m).rescaled(nano)).precision() == micro);
    CHECK(((2_m).fixed_at(nano) - (1_m).rescaled(micro)) == 1000000000_nm);
    CHECK(((2_m).fixed_at(nano) - (1_m).rescaled(micro)).precision() == nano);
    CHECK(((2_m).rescaled(micro) - (1_m).fixed_at(nano)) == 1000000000_nm);
    CHECK(((2_m).rescaled(micro) - (1_m).fixed_at(nano)).precision() == nano);
    CHECK(((2_m).rescaled(nano) - (1_m).fixed_at(micro)) == 1000000_um);
    CHECK(((2_m).rescaled(nano) - (1_m).fixed_at(micro)).precision() == micro);
}

TEST_CASE("test distance with assorted operations")
{
    CHECK((string_builder() << (2_m).fixed_at(unit) + (3_m).fixed_at(unit)).str() == "5_m");
    CHECK((string_builder() << (2_m).fixed_at(unit) - (3_m).fixed_at(unit)).str() == "-1_m");
    CHECK((string_builder() << 5*(100_m).fixed_at(unit)).str() == "500_m");
    CHECK((string_builder() << (1.0/5.0)*(100_m).fixed_at(unit)).str() == "20_m");
    CHECK((string_builder() << (100_m).fixed_at(unit)*(1.0/5.0)).str() == "20_m");
    CHECK((string_builder() << (100_m).fixed_at(unit)/5.0).str() == "20_m");
    CHECK((string_builder() << (100_m).fixed_at(unit)/8.0).str() == "13_m");
    CHECK(2_m > 1000_mm);
    CHECK(2_m < 3000_mm);
    CHECK(-8_pm < -7_pm);
    CHECK(1_m == 1000_mm);
    CHECK(1_m == 1000000_um);
    CHECK(1000_mm == 1000000_um);
    CHECK(((500_mm).fixed_at(milli) + (500_mm).fixed_at(milli)) == 1_m);
    CHECK((string_builder() << (500_mm).fixed_at(milli) + (500_mm).fixed_at(milli)).str() == "1000_mm");
    CHECK((string_builder() << (999999999999999_Mm).fixed_at(mega) + (1_Mm).fixed_at(mega)).str() == "distance::inf()");
    CHECK((string_builder() << -1000000*(1000000000_fm).fixed_at(femto)).str() == "-distance::inf()");
    CHECK((string_builder() << 3_m + 475_mm).str() == "3475_mm");
    CHECK((string_builder() << 1_km + 1_um).str() == "1000000001_um");
    CHECK((string_builder() << 500_pm - 1_nm).str() == "-500_pm");
    CHECK((string_builder() << (1.0/3.0)*(1_m)).str() == "333333333333333_fm");
    CHECK((string_builder() << (1.0/3.0)*(1000_mm)).str() == "333333333333333_fm");
    CHECK((string_builder() << 1000_mm/3.0).str() == "333333333333333_fm");
    CHECK((string_builder() << (1.0/3.0)*(1000000_um)).str() == "333333333333333_fm");
    CHECK((string_builder() << (999999999999999_Mm) + (1_Mm)).str() == "1000000000000_Gm");
    CHECK((string_builder() << -1000000*(1000000000_fm)).str() == "-1000000000000_pm");
    CHECK((10_mm/250_um) == Approx(40.0));
    CHECK(((-100_m)/900_m) == Approx(-1.0/9.0));
    CHECK(((123_mm)/1_m) == Approx(0.123));
    CHECK(((123_mm)/1_mm) == 123);
    CHECK(((123_mm)/0_m) == std::numeric_limits<float64>::infinity());
    CHECK(distance::inf().precision() == unit);
    CHECK(-distance::inf().precision() == unit);
    CHECK(distance(0, unit).precision() == unit);
    CHECK(distance(0, milli).precision() == milli);
    CHECK(distance(0, micro).precision() == micro);
    CHECK((string_builder() << (60_m + 40_m).fixed_at(micro)/8.0).str() == "12500000_um");
    CHECK((string_builder() << (60_m + 40_m).fixed_at(milli)/8.0).str() == "12500_mm");
    CHECK((string_builder() << (60_m + 40_m).fixed_at(unit)/8.0).str() == "13_m");
    CHECK((string_builder() << (60_m + 40_m).rescaled(micro)/8.0).str() == "12500000_um");
    CHECK((string_builder() << (60_m + 40_m).rescaled(milli)/8.0).str() == "12500_mm");
    CHECK((string_builder() << (60_m + 40_m).rescaled(unit)/8.0).str() == "12500_mm");
    CHECK((string_builder() << (4_m + 10_mm)).str() == "4010_mm");
    CHECK((string_builder() << (4_m + 10_mm)/4).str() == "1002500_um");
    CHECK((string_builder() << (4_m + 10_mm).fixed_at(milli)/4).str() == "1003_mm");
    CHECK((string_builder() << (4_m + 10_mm).fixed_at(micro)/4).str() == "1002500_um");
}


}  // namespace
