#include <catch2/catch.hpp>
#include <sydevs/core/scale.h>

TEST_CASE("test scale constants")
{
    CHECK(sydevs::yotta.level() == 8);
    CHECK(sydevs::zetta.level() == 7);
    CHECK(sydevs::exa  .level() == 6);
    CHECK(sydevs::peta .level() == 5);
    CHECK(sydevs::tera .level() == 4);
    CHECK(sydevs::giga .level() == 3);
    CHECK(sydevs::mega .level() == 2);
    CHECK(sydevs::kilo .level() == 1);

    CHECK(sydevs::unit.level() == 0);

    CHECK(sydevs::milli.level() == -1);
    CHECK(sydevs::micro.level() == -2);
    CHECK(sydevs::nano .level() == -3);
    CHECK(sydevs::pico .level() == -4);
    CHECK(sydevs::femto.level() == -5);
    CHECK(sydevs::atto .level() == -6);
    CHECK(sydevs::zepto.level() == -7);
    CHECK(sydevs::yocto.level() == -8);

    CHECK(sydevs::yotta.approx() == Approx(1000000000000000000000000.0));
    CHECK(sydevs::zetta.approx() == Approx(1000000000000000000000.0));
    CHECK(sydevs::exa  .approx() == Approx(1000000000000000000.0));
    CHECK(sydevs::peta .approx() == Approx(1000000000000000.0));
    CHECK(sydevs::tera .approx() == Approx(1000000000000.0));
    CHECK(sydevs::giga .approx() == Approx(1000000000.0));
    CHECK(sydevs::mega .approx() == Approx(1000000.0));
    CHECK(sydevs::kilo .approx() == Approx(1000.0));

    CHECK(sydevs::unit.approx() == Approx(1.0));

    CHECK(sydevs::milli.approx() == Approx(0.001));
    CHECK(sydevs::micro.approx() == Approx(0.000001));
    CHECK(sydevs::nano .approx() == Approx(0.000000001));
    CHECK(sydevs::pico .approx() == Approx(0.000000000001));
    CHECK(sydevs::femto.approx() == Approx(0.000000000000001));
    CHECK(sydevs::atto .approx() == Approx(0.000000000000000001));
    CHECK(sydevs::zepto.approx() == Approx(0.000000000000000000001));
    CHECK(sydevs::yocto.approx() == Approx(0.000000000000000000000001));
}

TEST_CASE("test scale constructors")
{
    CHECK(sydevs::scale().level() == 0);
    CHECK(sydevs::scale().approx() == Approx(1.0));

    sydevs::int64 limit = 16;

    for (sydevs::int64 level = -limit; level <= limit; level++)
    {
        CHECK(sydevs::scale(level).level() == level);

        sydevs::scale aScale(level);
        sydevs::scale bScale(aScale);
        sydevs::scale cScale = aScale;

        CHECK(bScale.level() == aScale.level());
        CHECK(cScale.level() == aScale.level());
    }

    sydevs::float64 approx = 1.0;
    CHECK(sydevs::scale(0).approx() == Approx(approx));

    SECTION("negative multipliers")
    {
        for (sydevs::int64 level = -1; level >= -limit; level--)
        {
            approx *= 0.001;
            CHECK(sydevs::scale(level).approx() == Approx(approx));
        }
    }

    SECTION("positive multipliers")
    {
        for (sydevs::int64 level = 1; level <= limit; level++)
        {
            approx *= 1000;
            CHECK(sydevs::scale(level).approx() == Approx(approx));
        }
    }
}

TEST_CASE("test scale operators")
{
    sydevs::int64 limit = 16;

    //  subtraction operator for two scales.
    //
    {
        for (sydevs::int64 level = -limit; level <= limit; level++)
        {
            CHECK((sydevs::scale(level) - sydevs::unit) == level);
        }
    }

    //  addition operator for scale and level.
    //
    {
        for (sydevs::int64 level = -limit; level <= limit; level++)
        {
            CHECK((sydevs::unit + level) == sydevs::scale(level));
        }
    }

    //  subtraction operator for scale and level.
    //
    {
        for (sydevs::int64 level = -limit; level <= limit; level++)
        {
            CHECK((sydevs::unit - level) == sydevs::scale(-level));
        }
    }

    //  division operator for two scales.
    //
    for (sydevs::int64 level = -limit; level <= limit; level++)
    {
        CHECK((sydevs::scale(level) / sydevs::scale(level)) == Approx(1.0));
        CHECK((sydevs::scale(level) / sydevs::unit) == Approx(sydevs::scale(level).approx()));
    }
}

TEST_CASE("test scale comparison operators")
{
    sydevs::int64 limit = 16;

    for (sydevs::int64 level = -limit; level <= limit; level++)
    {
        CHECK(sydevs::scale(level) == sydevs::scale(level));

        CHECK(sydevs::scale(level) != sydevs::scale( limit + 1));
        CHECK(sydevs::scale(level) != sydevs::scale(-limit - 1));

        CHECK(sydevs::scale(level) > sydevs::scale(-limit - 1));
        CHECK(sydevs::scale(level) < sydevs::scale( limit + 1));

        CHECK(sydevs::scale(level) >= sydevs::scale(-limit));
        CHECK(sydevs::scale(level) <= sydevs::scale( limit));
    }
}


namespace sydevs {


TEST_CASE("test scale with assorted operations")
{
    CHECK((nano + 4) == kilo);
    CHECK((tera - mega) == 2);
    CHECK((micro - 3) == femto);
    CHECK((1 + milli) == unit);
    CHECK((milli/pico) == 1000000000);

    CHECK((nano + 4 == kilo));
    CHECK((tera - mega == 2));
    CHECK((micro - 3 == femto));
    CHECK((1 + milli == unit));
    CHECK((milli/pico == 1000000000));
}


}  // namespace
