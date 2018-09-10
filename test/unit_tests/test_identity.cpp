#include <catch2/catch.hpp>
#include <sydevs/core/string_builder.h>
#include <sydevs/core/identity.h>

namespace sydevs {

class unit_test_identity_unit;
using ut_id = identity<unit_test_identity_unit>;


TEST_CASE("test identity constructors")
{
    CHECK(!ut_id().valid());
    CHECK(ut_id(0).valid());
    CHECK(ut_id(0).index() == 0);
    CHECK(ut_id(100).valid());
    CHECK(ut_id(100).index() == 100);
    CHECK(ut_id(-100).valid());
    CHECK(ut_id(-100).index() == -100);
}

TEST_CASE("test identity increment/decrement")
{
    auto id = ut_id(0);

    ++id;
    CHECK(id.index() == 1);
    CHECK((++id).index() == 2);
    CHECK((id++).index() == 2);
    CHECK(id.index() == 3);

    --id;
    CHECK(id.index() == 2);
    CHECK((--id).index() == 1);
    CHECK((id--).index() == 1);
    CHECK(id.index() == 0);
}

TEST_CASE("test identity add/subtract assignment operators")
{
    auto id = ut_id(0);

    id += 10;
    CHECK(id.index() == 10);

    id -= 100;
    CHECK(id.index() == -90);

    id = ut_id();

    id += 10;
    CHECK(!id.valid());

    id -= 100;
    CHECK(!id.valid());
}

TEST_CASE("test identity add/subtract infix operators")
{
    auto id = ut_id(555);

    CHECK((id + 10).index() == 565);
    CHECK((id - 1000).index() == -445);
    CHECK((215 + id).index() == 770);

    id = ut_id();

    CHECK(!(id + 10).valid());
    CHECK(!(id - 1000).valid());
    CHECK(!(215 + id).valid());
}


TEST_CASE("test identity output to stream")
{
    CHECK((string_builder() << ut_id(1240)).str() == "{1240}");
    CHECK((string_builder() << ut_id(1240) - 2000).str() == "{-760}");
    CHECK((string_builder() << ut_id()).str() == "{}");
}


}  // namespace
