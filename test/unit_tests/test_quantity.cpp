#include <catch2/catch.hpp>
#include <sydevs/core/string_builder.h>
#include <sydevs/core/quantity.h>

namespace sydevs {


TEST_CASE("test quantity with assorted operations")
{
    CHECK((string_builder() << 5_m/_s).str() == "quantity<decltype(_m/_s)>(5, unit)");
    CHECK((string_builder() << 5_um/_s).str() == "quantity<decltype(_m/_s)>(5, micro)");
    CHECK((string_builder() << 6_kg*15_m/9_s).str() == "quantity<decltype(_g*_m/_s)>(10, kilo)");
    CHECK((string_builder() << 6000_Mg*(15_nm/300_ms)).str() == "quantity<decltype(_g*_m/_s)>(300000, milli)");
    CHECK((string_builder() << -777_kg*_m/_s/_s).str() == "-quantity<decltype(_g*_m/_s/_s)>(777, kilo)");
    CHECK((string_builder() << -777_km/_s*_g/_s).str() == "-quantity<decltype(_g*_m/_s/_s)>(777, kilo)");
    CHECK((5_m/_s) == quantity<decltype(_m/_s)>(5, unit));
    CHECK((5_um/_s) == quantity<decltype(_m/_s)>(5, micro));
    CHECK((6_kg*15_m/9_s) == quantity<decltype(_g*_m/_s)>(10, kilo));
    CHECK((6000_Mg*(15_nm/300_ms)) == quantity<decltype(_g*_m/_s)>(300000, milli));
    CHECK((-777_kg*_m/_s/_s) == -quantity<decltype(_g*_m/_s/_s)>(777, kilo));
    CHECK((-777_km/_s*_g/_s) == -quantity<decltype(_g*_m/_s/_s)>(777, kilo));
    CHECK((90_km/1_hr) == quantity<decltype(_m/_s)>(25, unit));
    CHECK((200_m/8_s) == quantity<decltype(_m/_s)>(25, unit));
    CHECK((1500_mA/_A) == 1.5);
    CHECK((string_builder() << 5_g).str() == "5_g");
    CHECK((string_builder() << 5*1_g).str() == "5_g");
    CHECK((string_builder() << 5/1_g).str() == "quantity<decltype(_1/_g)>(5, unit)");
    CHECK((string_builder() << 5_m).str() == "5_m");
    CHECK((string_builder() << 5*1_m).str() == "5_m");
    CHECK((string_builder() << 5/1_m).str() == "quantity<decltype(_1/_m)>(5, unit)");
    CHECK((string_builder() << 5_s).str() == "5_s");
    CHECK((string_builder() << 5*1_s).str() == "5_s");
    CHECK((string_builder() << 5/1_s).str() == "quantity<decltype(_1/_s)>(5, unit)");
    CHECK((string_builder() << 5_A).str() == "5_A");
    CHECK((string_builder() << 5*1_A).str() == "5_A");
    CHECK((string_builder() << 5/1_A).str() == "quantity<decltype(_1/_A)>(5, unit)");
    CHECK((string_builder() << 5_K).str() == "5_K");
    CHECK((string_builder() << 5*1_K).str() == "5_K");
    CHECK((string_builder() << 5/1_K).str() == "quantity<decltype(_1/_K)>(5, unit)");
    CHECK((string_builder() << 5_mol).str() == "5_mol");
    CHECK((string_builder() << 5*1_mol).str() == "5_mol");
    CHECK((string_builder() << 5/1_mol).str() == "quantity<decltype(_1/_mol)>(5, unit)");
    CHECK((string_builder() << 5_cd).str() == "5_cd");
    CHECK((string_builder() << 5*1_cd).str() == "5_cd");
    CHECK((string_builder() << 5/1_cd).str() == "quantity<decltype(_1/_cd)>(5, unit)");
    CHECK((string_builder() << 5_ng).str() == "5_ng");
    CHECK((string_builder() << 5*1_ng).str() == "5_ng");
    CHECK((string_builder() << 5/1_Gg).str() == "quantity<decltype(_1/_g)>(5, nano)");
    CHECK((string_builder() << 5_nm).str() == "5_nm");
    CHECK((string_builder() << 5*1_nm).str() == "5_nm");
    CHECK((string_builder() << 5/1_Gm).str() == "quantity<decltype(_1/_m)>(5, nano)");
    CHECK((string_builder() << 5_ns).str() == "5_ns");
    CHECK((string_builder() << 5*1_ns).str() == "5_ns");
    CHECK((string_builder() << 5/1_Gs).str() == "quantity<decltype(_1/_s)>(5, nano)");
    CHECK((string_builder() << 5_nA).str() == "5_nA");
    CHECK((string_builder() << 5*1_nA).str() == "5_nA");
    CHECK((string_builder() << 5/1_GA).str() == "quantity<decltype(_1/_A)>(5, nano)");
    CHECK((string_builder() << 5_nK).str() == "5_nK");
    CHECK((string_builder() << 5*1_nK).str() == "5_nK");
    CHECK((string_builder() << 5/1_GK).str() == "quantity<decltype(_1/_K)>(5, nano)");
    CHECK((string_builder() << 5_nmol).str() == "5_nmol");
    CHECK((string_builder() << 5*1_nmol).str() == "5_nmol");
    CHECK((string_builder() << 5/1_Gmol).str() == "quantity<decltype(_1/_mol)>(5, nano)");
    CHECK((string_builder() << 5_ncd).str() == "5_ncd");
    CHECK((string_builder() << 5*1_ncd).str() == "5_ncd");
    CHECK((string_builder() << 5/1_Gcd).str() == "quantity<decltype(_1/_cd)>(5, nano)");
    CHECK((string_builder() << 5385_mm + 7_km).str() == "7005385_mm");
    CHECK((string_builder() << 5385_mm - 7_km).str() == "-6994615_mm");
}


}  // namespace
