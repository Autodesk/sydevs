#include <catch2/catch.hpp>
#include <sydevs/core/arraynd.h>
#include <sydevs/core/string_builder.h>

namespace sydevs {


TEST_CASE("test multidimensional array arithmetic")
{
    auto a3 = array3d<int64>({2, 3, 5}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    });
    CHECK((string_builder() << a3[range()][range()][range()]).str() == 
        std::string("{{{0, 1, 2, 3, 4}, ") +
                      "{10, 11, 12, 13, 14}, " +
                      "{20, 21, 22, 23, 24}}, " +
                     "{{100, 101, 102, 103, 104}, " +
                      "{110, 111, 112, 113, 114}, " +
                      "{120, 121, 122, 123, 124}}}");

    SECTION("check addition") {
        CHECK((string_builder() << (a3 + a3)).str() == 
            std::string("{{{0, 2, 4, 6, 8}, ") +
                          "{20, 22, 24, 26, 28}, " +
                          "{40, 42, 44, 46, 48}}, " +
                         "{{200, 202, 204, 206, 208}, " +
                          "{220, 222, 224, 226, 228}, " +
                          "{240, 242, 244, 246, 248}}}");
        CHECK((string_builder() << (a3 + 5)).str() == 
            std::string("{{{5, 6, 7, 8, 9}, ") +
                          "{15, 16, 17, 18, 19}, " +
                          "{25, 26, 27, 28, 29}}, " +
                         "{{105, 106, 107, 108, 109}, " +
                          "{115, 116, 117, 118, 119}, " +
                          "{125, 126, 127, 128, 129}}}");
        CHECK((string_builder() << (5 + a3)).str() == 
            std::string("{{{5, 6, 7, 8, 9}, ") +
                          "{15, 16, 17, 18, 19}, " +
                          "{25, 26, 27, 28, 29}}, " +
                         "{{105, 106, 107, 108, 109}, " +
                          "{115, 116, 117, 118, 119}, " +
                          "{125, 126, 127, 128, 129}}}");
    }

    SECTION("check subtraction") {
        CHECK((string_builder() << (a3 - a3)).str() == 
            std::string("{{{0, 0, 0, 0, 0}, ") +
                          "{0, 0, 0, 0, 0}, " +
                          "{0, 0, 0, 0, 0}}, " +
                         "{{0, 0, 0, 0, 0}, " +
                          "{0, 0, 0, 0, 0}, " +
                          "{0, 0, 0, 0, 0}}}");
        CHECK((string_builder() << (a3 - (-5))).str() == 
            std::string("{{{5, 6, 7, 8, 9}, ") +
                          "{15, 16, 17, 18, 19}, " +
                          "{25, 26, 27, 28, 29}}, " +
                         "{{105, 106, 107, 108, 109}, " +
                          "{115, 116, 117, 118, 119}, " +
                          "{125, 126, 127, 128, 129}}}");
        CHECK((string_builder() << (-5 - a3)).str() == 
            std::string("{{{-5, -6, -7, -8, -9}, ") +
                          "{-15, -16, -17, -18, -19}, " +
                          "{-25, -26, -27, -28, -29}}, " +
                         "{{-105, -106, -107, -108, -109}, " +
                          "{-115, -116, -117, -118, -119}, " +
                          "{-125, -126, -127, -128, -129}}}");
    }

    SECTION("check multiplication") {
        CHECK((string_builder() << (a3*a3)).str() == 
            std::string("{{{0, 1, 4, 9, 16}, ") +
                          "{100, 121, 144, 169, 196}, " +
                          "{400, 441, 484, 529, 576}}, " + 
                         "{{10000, 10201, 10404, 10609, 10816}, " +
                          "{12100, 12321, 12544, 12769, 12996}, " +
                          "{14400, 14641, 14884, 15129, 15376}}}");
        CHECK((string_builder() << (a3*5)).str() == 
            std::string("{{{0, 5, 10, 15, 20}, ") +
                          "{50, 55, 60, 65, 70}, " +
                          "{100, 105, 110, 115, 120}}, " 
                         "{{500, 505, 510, 515, 520}, " +
                          "{550, 555, 560, 565, 570}, " +
                          "{600, 605, 610, 615, 620}}}");
        CHECK((string_builder() << (5*a3)).str() == 
            std::string("{{{0, 5, 10, 15, 20}, ") +
                          "{50, 55, 60, 65, 70}, " +
                          "{100, 105, 110, 115, 120}}, " 
                         "{{500, 505, 510, 515, 520}, " +
                          "{550, 555, 560, 565, 570}, " +
                          "{600, 605, 610, 615, 620}}}");
    }

    SECTION("check division") {
        CHECK((string_builder() << ((a3 + 5)/(a3 + 1))).str() == 
            std::string("{{{5, 3, 2, 2, 1}, ") +
                          "{1, 1, 1, 1, 1}, " +
                          "{1, 1, 1, 1, 1}}, " +
                         "{{1, 1, 1, 1, 1}, " +
                          "{1, 1, 1, 1, 1}, " +
                          "{1, 1, 1, 1, 1}}}");
        CHECK((string_builder() << (a3/5)).str() == 
            std::string("{{{0, 0, 0, 0, 0}, ") +
                          "{2, 2, 2, 2, 2}, " +
                          "{4, 4, 4, 4, 4}}, " +
                         "{{20, 20, 20, 20, 20}, " +
                          "{22, 22, 22, 22, 22}, " +
                          "{24, 24, 24, 24, 24}}}");
        CHECK((string_builder() << (5/(a3 + 1))).str() == 
            std::string("{{{5, 2, 1, 1, 1}, ") +
                          "{0, 0, 0, 0, 0}, " +
                          "{0, 0, 0, 0, 0}}, " +
                         "{{0, 0, 0, 0, 0}, " +
                          "{0, 0, 0, 0, 0}, " +
                          "{0, 0, 0, 0, 0}}}");
    }

    SECTION("check comparisons") {
        auto a2 = a3[0][range()][range()];
        auto b2 = a3[0][range()][range().start_after(5).stop_at(0).stride_by(-1)];
        CHECK((string_builder() << (a2 < b2)).str() == 
            std::string("{{1, 1, 0, 0, 0}, ") +
                         "{1, 1, 0, 0, 0}, " +
                         "{1, 1, 0, 0, 0}}");
        CHECK((string_builder() << (a2 > b2)).str() == 
            std::string("{{0, 0, 0, 1, 1}, ") +
                         "{0, 0, 0, 1, 1}, " +
                         "{0, 0, 0, 1, 1}}");
        CHECK((string_builder() << (a2 <= b2)).str() == 
            std::string("{{1, 1, 1, 0, 0}, ") +
                         "{1, 1, 1, 0, 0}, " +
                         "{1, 1, 1, 0, 0}}");
        CHECK((string_builder() << (a2 >= b2)).str() == 
            std::string("{{0, 0, 1, 1, 1}, ") +
                         "{0, 0, 1, 1, 1}, " +
                         "{0, 0, 1, 1, 1}}");
        CHECK((string_builder() << (a2 == b2)).str() == 
            std::string("{{0, 0, 1, 0, 0}, ") +
                         "{0, 0, 1, 0, 0}, " +
                         "{0, 0, 1, 0, 0}}");
        CHECK((string_builder() << (a2 != b2)).str() == 
            std::string("{{1, 1, 0, 1, 1}, ") +
                         "{1, 1, 0, 1, 1}, " +
                         "{1, 1, 0, 1, 1}}");
        CHECK((string_builder() << (a2 < 12)).str() == 
            std::string("{{1, 1, 1, 1, 1}, ") +
                         "{1, 1, 0, 0, 0}, " +
                         "{0, 0, 0, 0, 0}}");
        CHECK((string_builder() << (a2 > 12)).str() == 
            std::string("{{0, 0, 0, 0, 0}, ") +
                         "{0, 0, 0, 1, 1}, " +
                         "{1, 1, 1, 1, 1}}");
        CHECK((string_builder() << (a2 <= 12)).str() == 
            std::string("{{1, 1, 1, 1, 1}, ") +
                         "{1, 1, 1, 0, 0}, " +
                         "{0, 0, 0, 0, 0}}");
        CHECK((string_builder() << (a2 >= 12)).str() == 
            std::string("{{0, 0, 0, 0, 0}, ") +
                         "{0, 0, 1, 1, 1}, " +
                         "{1, 1, 1, 1, 1}}");
        CHECK((string_builder() << (a2 == 12)).str() == 
            std::string("{{0, 0, 0, 0, 0}, ") +
                         "{0, 0, 1, 0, 0}, " +
                         "{0, 0, 0, 0, 0}}");
        CHECK((string_builder() << (a2 != 12)).str() == 
            std::string("{{1, 1, 1, 1, 1}, ") +
                         "{1, 1, 0, 1, 1}, " +
                         "{1, 1, 1, 1, 1}}");
        CHECK((string_builder() << (12 < a2)).str() == 
            std::string("{{0, 0, 0, 0, 0}, ") +
                         "{0, 0, 0, 1, 1}, " +
                         "{1, 1, 1, 1, 1}}");
        CHECK((string_builder() << (12 > a2)).str() == 
            std::string("{{1, 1, 1, 1, 1}, ") +
                         "{1, 1, 0, 0, 0}, " +
                         "{0, 0, 0, 0, 0}}");
        CHECK((string_builder() << (12 <= a2)).str() == 
            std::string("{{0, 0, 0, 0, 0}, ") +
                         "{0, 0, 1, 1, 1}, " +
                         "{1, 1, 1, 1, 1}}");
        CHECK((string_builder() << (12 >= a2)).str() == 
            std::string("{{1, 1, 1, 1, 1}, ") +
                         "{1, 1, 1, 0, 0}, " +
                         "{0, 0, 0, 0, 0}}");
        CHECK((string_builder() << (12 == a2)).str() == 
            std::string("{{0, 0, 0, 0, 0}, ") +
                         "{0, 0, 1, 0, 0}, " +
                         "{0, 0, 0, 0, 0}}");
        CHECK((string_builder() << (12 != a2)).str() == 
            std::string("{{1, 1, 1, 1, 1}, ") +
                         "{1, 1, 0, 1, 1}, " +
                         "{1, 1, 1, 1, 1}}");
        CHECK((string_builder() << !(a2 < b2)).str() == 
            std::string("{{0, 0, 1, 1, 1}, ") +
                         "{0, 0, 1, 1, 1}, " +
                         "{0, 0, 1, 1, 1}}");
        CHECK((string_builder() << (2*a2 >= (b2 + 12))).str() == 
            std::string("{{0, 0, 0, 0, 0}, ") +
                         "{0, 0, 1, 1, 1}, " +
                         "{1, 1, 1, 1, 1}}");
        CHECK(all(a2 < b2) == false);
        CHECK(all(a2 >= 0) == true);
        CHECK(any(a2 < 0) == false);
        CHECK(any(a2 < b2) == true);
        CHECK((string_builder() << ((a2 < 12) || (a2 > 12))).str() == 
            std::string("{{1, 1, 1, 1, 1}, ") +
                         "{1, 1, 0, 1, 1}, " +
                         "{1, 1, 1, 1, 1}}");
        CHECK((string_builder() << ((a2 >= 2) && (a2 <= 22))).str() == 
            std::string("{{0, 0, 1, 1, 1}, ") +
                         "{1, 1, 1, 1, 1}, " +
                         "{1, 1, 1, 0, 0}}");
    }

    SECTION("check documentation examples") {
        auto a = array2d<int64>({2, 3}, {0, 1, 2, 3, 4, 5});
        auto b = array2d<int64>({2, 3}, {2, 1, 0, 2, 4, 6});
        CHECK((string_builder() << a + b).str() == "{{2, 2, 2}, {5, 8, 11}}");
        CHECK((string_builder() << a + 5).str() == "{{5, 6, 7}, {8, 9, 10}}");
        CHECK((string_builder() << 5 - a).str() == "{{5, 4, 3}, {2, 1, 0}}");
        CHECK((string_builder() << a/2).str() == "{{0, 0, 1}, {1, 2, 2}}");
        CHECK((string_builder() << (a < b)).str() == "{{1, 0, 0}, {0, 0, 1}}");
        CHECK((string_builder() << (a == b)).str() == "{{0, 1, 0}, {0, 1, 0}}");
        CHECK((string_builder() << (a >= 1 && a <= 4)).str() == "{{0, 1, 1}, {1, 1, 0}}");
        CHECK((string_builder() << all(a >= 0)).str() == "1");
        CHECK((string_builder() << any(a >= 7)).str() == "0");
    }
}


}  // namespace
