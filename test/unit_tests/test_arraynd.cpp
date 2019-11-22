#include <catch2/catch.hpp>
#include <sydevs/core/arraynd.h>
#include <sydevs/core/string_builder.h>

namespace sydevs {


TEST_CASE("test multidimensional array fill constructors")
{
    CHECK((string_builder() << array1d<int64>({5}, 7)).str() == "{7, 7, 7, 7, 7}");
    CHECK((string_builder() << array1d<float64>({5}, 1.4)).str() == "{1.4, 1.4, 1.4, 1.4, 1.4}");
    CHECK((string_builder() << array2d<int64>({2, 3}, 7)).str() == "{{7, 7, 7}, {7, 7, 7}}");
    CHECK((string_builder() << array2d<float64>({2, 3}, 1.4)).str() == "{{1.4, 1.4, 1.4}, {1.4, 1.4, 1.4}}");
    CHECK((string_builder() << array3d<int64>({2, 3, 4}, 7)).str() == "{{{7, 7, 7, 7}, {7, 7, 7, 7}, {7, 7, 7, 7}}, {{7, 7, 7, 7}, {7, 7, 7, 7}, {7, 7, 7, 7}}}");
    CHECK((string_builder() << array3d<float64>({2, 3, 4}, 1.4)).str() == "{{{1.4, 1.4, 1.4, 1.4}, {1.4, 1.4, 1.4, 1.4}, {1.4, 1.4, 1.4, 1.4}}, {{1.4, 1.4, 1.4, 1.4}, {1.4, 1.4, 1.4, 1.4}, {1.4, 1.4, 1.4, 1.4}}}");
    CHECK((string_builder() << array1d<int64>({0}, 7)).str() == "{}");
    CHECK((string_builder() << array2d<int64>({0, 3}, 7)).str() == "{}");
    CHECK((string_builder() << array2d<int64>({2, 0}, 7)).str() == "{{}, {}}");
    CHECK((string_builder() << array3d<int64>({0, 3, 4}, 7)).str() == "{}");
    CHECK((string_builder() << array3d<int64>({2, 0, 4}, 7)).str() == "{{}, {}}");
    CHECK((string_builder() << array3d<int64>({2, 3, 0}, 7)).str() == "{{{}, {}, {}}, {{}, {}, {}}}");
}


TEST_CASE("test multidimensional array data constructors")
{
    CHECK((string_builder() << array1d<int64>({5}, {0, 1, 2, 3, 4})).str() == "{0, 1, 2, 3, 4}");
    CHECK((string_builder() << array1d<float64>({5}, {3.1, 4.1, 5.9, 2.6, 5.3})).str() == "{3.1, 4.1, 5.9, 2.6, 5.3}");
    CHECK((string_builder() << array2d<int64>({2, 3}, {0, 1, 2, 3, 4, 5})).str() == "{{0, 1, 2}, {3, 4, 5}}");
    CHECK((string_builder() << array2d<float64>({2, 3}, {3.1, 4.1, 5.9, 2.6, 5.3, 5.8})).str() == "{{3.1, 4.1, 5.9}, {2.6, 5.3, 5.8}}");
    CHECK((string_builder() << array3d<int64>({2, 3, 4}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23})).str() == "{{{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}}, {{12, 13, 14, 15}, {16, 17, 18, 19}, {20, 21, 22, 23}}}");
    CHECK((string_builder() << array3d<float64>({2, 3, 4}, {3.1, 4.1, 5.9, 2.6, 5.3, 5.8, -3.1, -4.1, -5.9, -2.6, -5.3, -5.8, 3.1, 4.1, 5.9, 2.6, 5.3, 5.8, -3.1, -4.1, -5.9, -2.6, -5.3, -5.8})).str() == "{{{3.1, 4.1, 5.9, 2.6}, {5.3, 5.8, -3.1, -4.1}, {-5.9, -2.6, -5.3, -5.8}}, {{3.1, 4.1, 5.9, 2.6}, {5.3, 5.8, -3.1, -4.1}, {-5.9, -2.6, -5.3, -5.8}}}");
    CHECK((string_builder() << array1d<int64>({0}, {})).str() == "{}");
    CHECK((string_builder() << array2d<int64>({0, 3}, {})).str() == "{}");
    CHECK((string_builder() << array2d<int64>({2, 0}, {})).str() == "{{}, {}}");
    CHECK((string_builder() << array3d<int64>({0, 3, 4}, {})).str() == "{}");
    CHECK((string_builder() << array3d<int64>({2, 0, 4}, {})).str() == "{{}, {}}");
    CHECK((string_builder() << array3d<int64>({2, 3, 0}, {})).str() == "{{{}, {}, {}}, {{}, {}, {}}}");
}


TEST_CASE("test multidimensional array from function constructors")
{
    CHECK((string_builder() << array1d<int64>({5}, [](const std::array<int64, 1>& indices) {
        return indices[0];
    })).str() == "{0, 1, 2, 3, 4}");
    CHECK((string_builder() << array1d<float64>({5}, [](const std::array<int64, 1>& indices) {
        return indices[0]/2.0;
    })).str() == "{0, 0.5, 1, 1.5, 2}");
    CHECK((string_builder() << array2d<int64>({2, 3}, [](const std::array<int64, 2>& indices) {
        return 10*indices[0] + indices[1];
    })).str() == "{{0, 1, 2}, {10, 11, 12}}");
    CHECK((string_builder() << array2d<float64>({2, 3}, [](const std::array<int64, 2>& indices) {
        return 10*indices[0] + indices[1]/2.0;
    })).str() == "{{0, 0.5, 1}, {10, 10.5, 11}}");
    CHECK((string_builder() << array3d<int64>({2, 3, 4}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    })).str() == "{{{0, 1, 2, 3}, {10, 11, 12, 13}, {20, 21, 22, 23}}, {{100, 101, 102, 103}, {110, 111, 112, 113}, {120, 121, 122, 123}}}");
    CHECK((string_builder() << array3d<float64>({2, 3, 4}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2]/2.0;
    })).str() == "{{{0, 0.5, 1, 1.5}, {10, 10.5, 11, 11.5}, {20, 20.5, 21, 21.5}}, {{100, 100.5, 101, 101.5}, {110, 110.5, 111, 111.5}, {120, 120.5, 121, 121.5}}}");
    CHECK((string_builder() << array1d<int64>({0}, [](const std::array<int64, 1>& indices) {
        return indices[0];
    })).str() == "{}");
    CHECK((string_builder() << array2d<int64>({0, 3}, [](const std::array<int64, 2>& indices) {
        return 10*indices[0] + indices[1];
    })).str() == "{}");
    CHECK((string_builder() << array2d<int64>({2, 0}, [](const std::array<int64, 2>& indices) {
        return 10*indices[0] + indices[1];
    })).str() == "{{}, {}}");
    CHECK((string_builder() << array3d<int64>({0, 3, 4}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    })).str() == "{}");
    CHECK((string_builder() << array3d<int64>({2, 0, 4}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    })).str() == "{{}, {}}");
    CHECK((string_builder() << array3d<int64>({2, 3, 0}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    })).str() == "{{{}, {}, {}}, {{}, {}, {}}}");
}


TEST_CASE("test multidimensional array indexing")
{
    auto a1 = array1d<int64>({5}, [](const std::array<int64, 1>& indices) {
        return indices[0];
    });
    auto a2 = array2d<int64>({2, 3}, [](const std::array<int64, 2>& indices) {
        return 10*indices[0] + indices[1];
    });
    auto a3 = array3d<int64>({2, 3, 4}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    });
    CHECK(a1.dims()[0] == 5);
    CHECK(a1.strides()[0] == 1);
    CHECK(a1.size() == 5);
    CHECK(a1.offset() == 0);
    CHECK(a1[0] == 0);
    CHECK(a1[2] == 2);
    CHECK(a1[4] == 4);
    CHECK(a1({0}) == 0);
    CHECK(a1({2}) == 2);
    CHECK(a1({4}) == 4);
    CHECK(a1(0) == 0);
    CHECK(a1(2) == 2);
    CHECK(a1(4) == 4);
    CHECK(a2.dims()[0] == 2);
    CHECK(a2.dims()[1] == 3);
    CHECK(a2.strides()[0] == 3);
    CHECK(a2.strides()[1] == 1);
    CHECK(a2.size() == 6);
    CHECK(a2.offset() == 0);
    CHECK(a2[0][0] == 0);
    CHECK(a2[0][2] == 2);
    CHECK(a2[1][0] == 10);
    CHECK(a2[1][2] == 12);
    CHECK(a2({0, 0}) == 0);
    CHECK(a2({0, 2}) == 2);
    CHECK(a2({1, 0}) == 10);
    CHECK(a2({1, 2}) == 12);
    CHECK(a2(0, 0) == 0);
    CHECK(a2(0, 2) == 2);
    CHECK(a2(1, 0) == 10);
    CHECK(a2(1, 2) == 12);
    CHECK(a3.dims()[0] == 2);
    CHECK(a3.dims()[1] == 3);
    CHECK(a3.dims()[2] == 4);
    CHECK(a3.strides()[0] == 12);
    CHECK(a3.strides()[1] == 4);
    CHECK(a3.strides()[2] == 1);
    CHECK(a3.size() == 24);
    CHECK(a3.offset() == 0);
    CHECK(a3({0, 0, 0}) == 0);
    CHECK(a3({0, 0, 3}) == 3);
    CHECK(a3({0, 2, 0}) == 20);
    CHECK(a3({0, 2, 3}) == 23);
    CHECK(a3({1, 0, 0}) == 100);
    CHECK(a3({1, 0, 3}) == 103);
    CHECK(a3({1, 2, 0}) == 120);
    CHECK(a3({1, 2, 3}) == 123);
    CHECK(a3(0, 0, 0) == 0);
    CHECK(a3(0, 0, 3) == 3);
    CHECK(a3(0, 2, 0) == 20);
    CHECK(a3(0, 2, 3) == 23);
    CHECK(a3(1, 0, 0) == 100);
    CHECK(a3(1, 0, 3) == 103);
    CHECK(a3(1, 2, 0) == 120);
    CHECK(a3(1, 2, 3) == 123);
}


TEST_CASE("test multidimensional array slicing on first of one dimensions")
{
    auto a1 = array1d<int64>({6}, [](const std::array<int64, 1>& indices) {
        return indices[0];
    });
    CHECK((string_builder() << a1[range()]).str() == 
        "{0, 1, 2, 3, 4, 5}");
    CHECK((string_builder() << a1[range().start_at(1)]).str() == 
        "{1, 2, 3, 4, 5}");
    CHECK((string_builder() << a1[range().stop_before(5)]).str() == 
        "{0, 1, 2, 3, 4}");
    CHECK((string_builder() << a1[range().stride_by(2)]).str() == 
        "{0, 2, 4}");
    CHECK((string_builder() << a1[range().start_after(0).stop_at(4).stride_by(2)]).str() == 
        "{1, 3}");
    CHECK((string_builder() << a1[range().start_after(6).stop_at(0).stride_by(-1)]).str() == 
        "{5, 4, 3, 2, 1, 0}");
    CHECK((string_builder() << a1[range().start_after(6).stop_at(0).stride_by(-2)]).str() == 
        "{5, 3, 1}");
    CHECK(a1[0] == 0);
    CHECK(a1[1] == 1);
    CHECK(a1[2] == 2);
    CHECK(a1[3] == 3);
    CHECK(a1[4] == 4);
    CHECK(a1[5] == 5);
}


TEST_CASE("test multidimensional array slicing on first of two dimensions")
{
    auto a2 = array2d<int64>({4, 6}, [](const std::array<int64, 2>& indices) {
        return 10*indices[0] + indices[1];
    });
    CHECK((string_builder() << a2[range()][range()]).str() == 
        std::string("{{0, 1, 2, 3, 4, 5}, ") +
                     "{10, 11, 12, 13, 14, 15}, " +
                     "{20, 21, 22, 23, 24, 25}, " +
                     "{30, 31, 32, 33, 34, 35}}");
    CHECK((string_builder() << a2[range().start_at(1)][range()]).str() == 
        std::string("{{10, 11, 12, 13, 14, 15}, ") +
                     "{20, 21, 22, 23, 24, 25}, " +
                     "{30, 31, 32, 33, 34, 35}}");
    CHECK((string_builder() << a2[range().stop_before(3)][range()]).str() == 
        std::string("{{0, 1, 2, 3, 4, 5}, ") +
                     "{10, 11, 12, 13, 14, 15}, " +
                     "{20, 21, 22, 23, 24, 25}}");
    CHECK((string_builder() << a2[range().stride_by(2)][range()]).str() == 
        std::string("{{0, 1, 2, 3, 4, 5}, ") +
                     "{20, 21, 22, 23, 24, 25}}");
    CHECK((string_builder() << a2[range().start_after(0).stop_at(2).stride_by(2)][range()]).str() == 
        "{{10, 11, 12, 13, 14, 15}}");
    CHECK((string_builder() << a2[range().start_after(4).stop_at(0).stride_by(-1)][range()]).str() == 
        std::string("{{30, 31, 32, 33, 34, 35}, ") +
                     "{20, 21, 22, 23, 24, 25}, " +
                     "{10, 11, 12, 13, 14, 15}, " +
                     "{0, 1, 2, 3, 4, 5}}");
    CHECK((string_builder() << a2[range().start_after(4).stop_at(0).stride_by(-2)][range()]).str() == 
        std::string("{{30, 31, 32, 33, 34, 35}, ") +
                     "{10, 11, 12, 13, 14, 15}}");
    CHECK((string_builder() << a2[0][range()]).str() == 
        "{0, 1, 2, 3, 4, 5}");
    CHECK((string_builder() << a2[1][range()]).str() == 
        "{10, 11, 12, 13, 14, 15}");
    CHECK((string_builder() << a2[2][range()]).str() == 
        "{20, 21, 22, 23, 24, 25}");
    CHECK((string_builder() << a2[3][range()]).str() == 
        "{30, 31, 32, 33, 34, 35}");
}


TEST_CASE("test multidimensional array slicing on first of three dimensions")
{
    auto a3 = array3d<int64>({3, 4, 6}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    });
    CHECK((string_builder() << a3[range()][range()][range()]).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " 
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 211, 212, 213, 214, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    CHECK((string_builder() << a3[range().start_at(1)][range()][range()]).str() == 
        std::string("{{{100, 101, 102, 103, 104, 105}, ") +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                  "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 211, 212, 213, 214, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    CHECK((string_builder() << a3[range().stop_before(2)][range()][range()]).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                  "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}}");
    CHECK((string_builder() << a3[range().stride_by(2)][range()][range()]).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 211, 212, 213, 214, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    CHECK((string_builder() << a3[range().start_after(0).stop_at(1).stride_by(2)][range()][range()]).str() == 
        std::string("{{{100, 101, 102, 103, 104, 105}, ") +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}}");
    CHECK((string_builder() << a3[range().start_after(3).stop_at(0).stride_by(-1)][range()][range()]).str() == 
        std::string("{{{200, 201, 202, 203, 204, 205}, ") +
                      "{210, 211, 212, 213, 214, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}, " +
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{0, 1, 2, 3, 4, 5}, " +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}}");
    CHECK((string_builder() << a3[range().start_after(3).stop_at(0).stride_by(-2)][range()][range()]).str() == 
        std::string("{{{200, 201, 202, 203, 204, 205}, ") +
                      "{210, 211, 212, 213, 214, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}, " +
                     "{{0, 1, 2, 3, 4, 5}, " +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}}");
    CHECK((string_builder() << a3[0][range()][range()]).str() == 
        std::string("{{0, 1, 2, 3, 4, 5}, ") +
                     "{10, 11, 12, 13, 14, 15}, " +
                     "{20, 21, 22, 23, 24, 25}, " +
                     "{30, 31, 32, 33, 34, 35}}");
    CHECK((string_builder() << a3[1][range()][range()]).str() == 
        std::string("{{100, 101, 102, 103, 104, 105}, ") +
                     "{110, 111, 112, 113, 114, 115}, " +
                     "{120, 121, 122, 123, 124, 125}, " +
                     "{130, 131, 132, 133, 134, 135}}");
    CHECK((string_builder() << a3[2][range()][range()]).str() == 
        std::string("{{200, 201, 202, 203, 204, 205}, ") +
                     "{210, 211, 212, 213, 214, 215}, " +
                     "{220, 221, 222, 223, 224, 225}, " +
                     "{230, 231, 232, 233, 234, 235}}");
}


TEST_CASE("test multidimensional array slicing on all dimensions")
{
    auto a3 = array3d<int64>({3, 4, 6}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    });
    CHECK((string_builder() << a3[range()][range().start_at(0).stop_at(3)][range().stride_by(1)]).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 211, 212, 213, 214, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    CHECK((string_builder() << a3[range().stride_by(2)][1][range().start_at(4).stride_by(-3)]).str() == 
        std::string("{{14, 11}, ") +
                     "{214, 211}}");
    CHECK((string_builder() << a3[range().start_after(3).stop_before(0).stride_by(-1)][range()][5]).str() == 
        std::string("{{205, ") +
                      "215, " +
                      "225, " +
                      "235}, " +
                     "{105, " +
                      "115, " +
                      "125, " +
                      "135}}");
    CHECK((string_builder() << a3[range().start_after(2).stop_before(2).stride_by(-1)][range()][5]).str() == "{}");
    CHECK((string_builder() << a3[range().start_after(3).stop_before(0).stride_by(-1)][range().start_at(2).stop_at(2)][5]).str() == 
        std::string("{{225}, ") +
                     "{125}}");
    CHECK((string_builder() << a3[range().start_after(3).stop_before(0).stride_by(-1)][2][5]).str() == 
        std::string("{225, ") +
                     "125}");
    CHECK((string_builder() << a3[range().start_after(3).stride_by(-3)][2][5]).str() == "{225}");
    CHECK(a3[2][2][5] == 225);
}


TEST_CASE("test multidimensional array slicing and fill")
{
    auto a3 = array3d<int64>({3, 4, 6}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    });
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 211, 212, 213, 214, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    a3[range().stride_by(2)][1][range().start_at(4).stride_by(-3)].fill(7);
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 7, 12, 13, 7, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 7, 212, 213, 7, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    a3[range().start_after(3).stop_before(0).stride_by(-1)][range()][5].fill(-1000);
    a3[range().stride_by(2)][1][range().start_at(4).stride_by(-3)].fill(7);
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 7, 12, 13, 7, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, -1000}, " +
                      "{110, 111, 112, 113, 114, -1000}, " +
                      "{120, 121, 122, 123, 124, -1000}, " +
                      "{130, 131, 132, 133, 134, -1000}}, " +
                     "{{200, 201, 202, 203, 204, -1000}, " +
                      "{210, 7, 212, 213, 7, -1000}, " +
                      "{220, 221, 222, 223, 224, -1000}, " +
                      "{230, 231, 232, 233, 234, -1000}}}");
}


TEST_CASE("test multidimensional array slicing and assign")
{
    auto a3 = array3d<int64>({3, 4, 6}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    });
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 211, 212, 213, 214, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    a3[range().stride_by(2)][1][range().start_at(4).stride_by(-3)].assign(a3[range().stride_by(2)][1][range().start_at(3).stride_by(-3)]);
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 10, 12, 13, 13, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 210, 212, 213, 213, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    a3[range().start_after(3).stop_before(0).stride_by(-1)][range()][5].assign(a3[range().start_after(3).stop_before(0).stride_by(-1)][range()][1]);
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 10, 12, 13, 13, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 101}, " +
                      "{110, 111, 112, 113, 114, 111}, " +
                      "{120, 121, 122, 123, 124, 121}, " +
                      "{130, 131, 132, 133, 134, 131}}, " +
                     "{{200, 201, 202, 203, 204, 201}, " +
                      "{210, 210, 212, 213, 213, 210}, " +
                      "{220, 221, 222, 223, 224, 221}, " +
                      "{230, 231, 232, 233, 234, 231}}}");
}


TEST_CASE("test multidimensional array slicing and assign from function")
{
    auto a3 = array3d<int64>({3, 4, 6}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    });
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 211, 212, 213, 214, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    a3[range().stride_by(2)][1][range().start_at(4).stride_by(-3)].assign_from_function([](const std::array<int64, 2>& indices) {
        return 10000*(indices[0] + 1) + 1000*(indices[1] + 1);
    });
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 12000, 12, 13, 11000, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 22000, 212, 213, 21000, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    a3[range().start_after(3).stop_before(0).stride_by(-1)][range()][5].assign_from_function([](const std::array<int64, 2>& indices) {
        return -(9999 - 100*indices[0] - indices[1]);
    });
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 12000, 12, 13, 11000, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, -9899}, " +
                      "{110, 111, 112, 113, 114, -9898}, " +
                      "{120, 121, 122, 123, 124, -9897}, " +
                      "{130, 131, 132, 133, 134, -9896}}, " +
                     "{{200, 201, 202, 203, 204, -9999}, " +
                      "{210, 22000, 212, 213, 21000, -9998}, " +
                      "{220, 221, 222, 223, 224, -9997}, " +
                      "{230, 231, 232, 233, 234, -9996}}}");
}


TEST_CASE("test multidimensional array view and copy in one dimension")
{
    auto a1 = array1d<int64>({6}, {0, 1, 2, 3, 4, 5});
    CHECK((string_builder() << a1).str() == "{0, 1, 2, 3, 4, 5}");
    auto a1_view = a1[range().start_after(6).stop_at(0).stride_by(-2)].view();
    CHECK((string_builder() << a1_view).str() == "{5, 3, 1}");
    auto a1_view_view = a1_view[range().start_after(0)].view();
    CHECK((string_builder() << a1_view_view).str() == "{3, 1}");
    auto a1_view_copy = a1_view[range().start_after(0)].copy();
    CHECK((string_builder() << a1_view_copy).str() == "{3, 1}");
    a1_view.fill(7);
    CHECK((string_builder() << a1).str() == "{0, 7, 2, 7, 4, 7}");
    CHECK((string_builder() << a1_view).str() == "{7, 7, 7}");
    CHECK((string_builder() << a1_view_view).str() == "{7, 7}");
    CHECK((string_builder() << a1_view_copy).str() == "{3, 1}");
    a1 = array1d<int64>({6}, {0, 1, 2, 3, 4, 5});
    auto a1_copy = a1[range().start_after(6).stop_at(0).stride_by(-2)].copy();
    CHECK((string_builder() << a1_copy).str() == "{5, 3, 1}");
    auto a1_copy_view = a1_copy[range().start_after(0)].view();
    CHECK((string_builder() << a1_copy_view).str() == "{3, 1}");
    auto a1_copy_copy = a1_copy[range().start_after(0)].copy();
    CHECK((string_builder() << a1_copy_copy).str() == "{3, 1}");
    a1_copy.fill(7);
    CHECK((string_builder() << a1).str() == "{0, 1, 2, 3, 4, 5}");
    CHECK((string_builder() << a1_copy).str() == "{7, 7, 7}");
    CHECK((string_builder() << a1_copy_view).str() == "{7, 7}");
    CHECK((string_builder() << a1_copy_copy).str() == "{3, 1}");
}


TEST_CASE("test multidimensional array view and copy")
{
    auto a3 = array3d<int64>({3, 4, 6}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    });
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 211, 212, 213, 214, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    auto a3_view = a3[range().stride_by(2)][1][range().start_at(4).stride_by(-3)].view();
    CHECK((string_builder() << a3_view).str() == 
        std::string("{{14, 11}, ") +
                     "{214, 211}}");
    auto a3_view_view = a3_view[range()][range().start_at(1)].view();
    CHECK((string_builder() << a3_view_view).str() == 
        std::string("{{11}, ") +
                     "{211}}");
    auto a3_view_copy = a3_view[range()][range().start_at(1)].copy();
    CHECK((string_builder() << a3_view_copy).str() == 
        std::string("{{11}, ") +
                     "{211}}");
    a3_view.fill(7);
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 7, 12, 13, 7, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 7, 212, 213, 7, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    CHECK((string_builder() << a3_view).str() == 
        std::string("{{7, 7}, ") +
                     "{7, 7}}");
    CHECK((string_builder() << a3_view_view).str() == 
        std::string("{{7}, ") +
                     "{7}}");
    CHECK((string_builder() << a3_view_copy).str() == 
        std::string("{{11}, ") +
                     "{211}}");
    a3 = array3d<int64>({3, 4, 6}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    });
    auto a3_copy = a3[range().stride_by(2)][1][range().start_at(4).stride_by(-3)].copy();
    CHECK((string_builder() << a3_copy).str() == 
        std::string("{{14, 11}, ") +
                     "{214, 211}}");
    auto a3_copy_view = a3_copy[range()][range().start_at(1)].view();
    CHECK((string_builder() << a3_copy_view).str() == 
        std::string("{{11}, ") +
                     "{211}}");
    auto a3_copy_copy = a3_copy[range()][range().start_at(1)].copy();
    CHECK((string_builder() << a3_copy_copy).str() == 
        std::string("{{11}, ") +
                     "{211}}");
    a3_copy.fill(7);
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 211, 212, 213, 214, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    CHECK((string_builder() << a3_copy).str() == 
        std::string("{{7, 7}, ") +
                     "{7, 7}}");
    CHECK((string_builder() << a3_copy_view).str() == 
        std::string("{{7}, ") +
                     "{7}}");
    CHECK((string_builder() << a3_copy_copy).str() == 
        std::string("{{11}, ") +
                     "{211}}");
}


TEST_CASE("test multidimensional array transpose")
{
    auto a3 = array3d<int64>({3, 4, 6}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    });
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 211, 212, 213, 214, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    CHECK(a3.is_contiguous() == true);
    auto a3_transposed_view = a3.view_transposed();
    CHECK((string_builder() << a3_transposed_view).str() == 
        std::string("{{{0, 100, 200}, ") +
                      "{10, 110, 210}, " +
                      "{20, 120, 220}, " +
                      "{30, 130, 230}}, " +
                     "{{1, 101, 201}, " +
                      "{11, 111, 211}, " +
                      "{21, 121, 221}, " +
                      "{31, 131, 231}}, " +
                     "{{2, 102, 202}, " +
                      "{12, 112, 212}, " +
                      "{22, 122, 222}, " +
                      "{32, 132, 232}}, " +
                     "{{3, 103, 203}, " +
                      "{13, 113, 213}, " +
                      "{23, 123, 223}, " +
                      "{33, 133, 233}}, " +
                     "{{4, 104, 204}, " +
                      "{14, 114, 214}, " +
                      "{24, 124, 224}, " +
                      "{34, 134, 234}}, " +
                     "{{5, 105, 205}, " +
                      "{15, 115, 215}, " +
                      "{25, 125, 225}, " +
                      "{35, 135, 235}}}");
    CHECK(a3_transposed_view.is_contiguous() == false);
    auto a3_transposed_copy = a3.copy_transposed();
    CHECK((string_builder() << a3_transposed_copy).str() == 
        std::string("{{{0, 100, 200}, ") +
                      "{10, 110, 210}, " +
                      "{20, 120, 220}, " +
                      "{30, 130, 230}}, " +
                     "{{1, 101, 201}, " +
                      "{11, 111, 211}, " +
                      "{21, 121, 221}, " +
                      "{31, 131, 231}}, " +
                     "{{2, 102, 202}, " +
                      "{12, 112, 212}, " +
                      "{22, 122, 222}, " +
                      "{32, 132, 232}}, " +
                     "{{3, 103, 203}, " +
                      "{13, 113, 213}, " +
                      "{23, 123, 223}, " +
                      "{33, 133, 233}}, " +
                     "{{4, 104, 204}, " +
                      "{14, 114, 214}, " +
                      "{24, 124, 224}, " +
                      "{34, 134, 234}}, " +
                     "{{5, 105, 205}, " +
                      "{15, 115, 215}, " +
                      "{25, 125, 225}, " +
                      "{35, 135, 235}}}");
    CHECK(a3_transposed_copy.is_contiguous() == true);
    a3({1, 2, 3}) = -999999;
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, -999999, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 211, 212, 213, 214, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    CHECK((string_builder() << a3_transposed_view).str() == 
        std::string("{{{0, 100, 200}, ") +
                      "{10, 110, 210}, " +
                      "{20, 120, 220}, " +
                      "{30, 130, 230}}, " +
                     "{{1, 101, 201}, " +
                      "{11, 111, 211}, " +
                      "{21, 121, 221}, " +
                      "{31, 131, 231}}, " +
                     "{{2, 102, 202}, " +
                      "{12, 112, 212}, " +
                      "{22, 122, 222}, " +
                      "{32, 132, 232}}, " +
                     "{{3, 103, 203}, " +
                      "{13, 113, 213}, " +
                      "{23, -999999, 223}, " +
                      "{33, 133, 233}}, " +
                     "{{4, 104, 204}, " +
                      "{14, 114, 214}, " +
                      "{24, 124, 224}, " +
                      "{34, 134, 234}}, " +
                     "{{5, 105, 205}, " +
                      "{15, 115, 215}, " +
                      "{25, 125, 225}, " +
                      "{35, 135, 235}}}");
    CHECK((string_builder() << a3_transposed_copy).str() == 
        std::string("{{{0, 100, 200}, ") +
                      "{10, 110, 210}, " +
                      "{20, 120, 220}, " +
                      "{30, 130, 230}}, " +
                     "{{1, 101, 201}, " +
                      "{11, 111, 211}, " +
                      "{21, 121, 221}, " +
                      "{31, 131, 231}}, " +
                     "{{2, 102, 202}, " +
                      "{12, 112, 212}, " +
                      "{22, 122, 222}, " +
                      "{32, 132, 232}}, " +
                     "{{3, 103, 203}, " +
                      "{13, 113, 213}, " +
                      "{23, 123, 223}, " +
                      "{33, 133, 233}}, " +
                     "{{4, 104, 204}, " +
                      "{14, 114, 214}, " +
                      "{24, 124, 224}, " +
                      "{34, 134, 234}}, " +
                     "{{5, 105, 205}, " +
                      "{15, 115, 215}, " +
                      "{25, 125, 225}, " +
                      "{35, 135, 235}}}");
    auto a3_sliced = a3[range().start_after(3).stop_before(0).stride_by(-1)][range()][5];
    CHECK((string_builder() << a3_sliced).str() ==
        std::string("{{205, ") +
                      "215, " +
                      "225, " +
                      "235}, " +
                     "{105, " +
                      "115, " +
                      "125, " +
                      "135}}");
    CHECK(a3_sliced.is_contiguous() == false);
    auto a3_sliced_transposed_view = a3_sliced.view_transposed();
    CHECK((string_builder() << a3_sliced_transposed_view).str() == 
        std::string("{{205, 105}, ") +
                     "{215, 115}, " +
                     "{225, 125}, " +
                     "{235, 135}}");
    CHECK(a3_sliced_transposed_view.is_contiguous() == false);
    auto a3_sliced_transposed_copy = a3_sliced.copy_transposed();
    CHECK((string_builder() << a3_sliced_transposed_copy).str() == 
        std::string("{{205, 105}, ") +
                     "{215, 115}, " +
                     "{225, 125}, " +
                     "{235, 135}}");
    CHECK(a3_sliced_transposed_copy.is_contiguous() == true);
    a3_sliced_transposed_view.assign_from_function([](const std::array<int64, 2>& indices) {
        return 100*(10*(indices[0] + 1) + (indices[1] + 1));
    });
    CHECK((string_builder() << a3_sliced_transposed_view).str() == 
        std::string("{{1100, 1200}, ") +
                     "{2100, 2200}, " +
                     "{3100, 3200}, " +
                     "{4100, 4200}}");
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 1200}, " +
                      "{110, 111, 112, 113, 114, 2200}, " +
                      "{120, 121, 122, -999999, 124, 3200}, " +
                      "{130, 131, 132, 133, 134, 4200}}, " +
                     "{{200, 201, 202, 203, 204, 1100}, " +
                      "{210, 211, 212, 213, 214, 2100}, " +
                      "{220, 221, 222, 223, 224, 3100}, " +
                      "{230, 231, 232, 233, 234, 4100}}}");
}


TEST_CASE("test multidimensional array swap axes")
{
    auto a3 = array3d<int64>({3, 4, 6}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    });
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, 123, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 211, 212, 213, 214, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    CHECK(a3.is_contiguous() == true);
    auto a3_transposed_view = a3.view_swapped_axes(0, 2);
    CHECK((string_builder() << a3_transposed_view).str() == 
        std::string("{{{0, 100, 200}, ") +
                      "{10, 110, 210}, " +
                      "{20, 120, 220}, " +
                      "{30, 130, 230}}, " +
                     "{{1, 101, 201}, " +
                      "{11, 111, 211}, " +
                      "{21, 121, 221}, " +
                      "{31, 131, 231}}, " +
                     "{{2, 102, 202}, " +
                      "{12, 112, 212}, " +
                      "{22, 122, 222}, " +
                      "{32, 132, 232}}, " +
                     "{{3, 103, 203}, " +
                      "{13, 113, 213}, " +
                      "{23, 123, 223}, " +
                      "{33, 133, 233}}, " +
                     "{{4, 104, 204}, " +
                      "{14, 114, 214}, " +
                      "{24, 124, 224}, " +
                      "{34, 134, 234}}, " +
                     "{{5, 105, 205}, " +
                      "{15, 115, 215}, " +
                      "{25, 125, 225}, " +
                      "{35, 135, 235}}}");
    CHECK(a3_transposed_view.is_contiguous() == false);
    auto a3_transposed_copy = a3.copy_swapped_axes(0, 2);
    CHECK((string_builder() << a3_transposed_copy).str() == 
        std::string("{{{0, 100, 200}, ") +
                      "{10, 110, 210}, " +
                      "{20, 120, 220}, " +
                      "{30, 130, 230}}, " +
                     "{{1, 101, 201}, " +
                      "{11, 111, 211}, " +
                      "{21, 121, 221}, " +
                      "{31, 131, 231}}, " +
                     "{{2, 102, 202}, " +
                      "{12, 112, 212}, " +
                      "{22, 122, 222}, " +
                      "{32, 132, 232}}, " +
                     "{{3, 103, 203}, " +
                      "{13, 113, 213}, " +
                      "{23, 123, 223}, " +
                      "{33, 133, 233}}, " +
                     "{{4, 104, 204}, " +
                      "{14, 114, 214}, " +
                      "{24, 124, 224}, " +
                      "{34, 134, 234}}, " +
                     "{{5, 105, 205}, " +
                      "{15, 115, 215}, " +
                      "{25, 125, 225}, " +
                      "{35, 135, 235}}}");
    CHECK(a3_transposed_copy.is_contiguous() == true);
    a3({1, 2, 3}) = -999999;
    CHECK((string_builder() << a3).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{10, 11, 12, 13, 14, 15}, " +
                      "{20, 21, 22, 23, 24, 25}, " +
                      "{30, 31, 32, 33, 34, 35}}, " +
                     "{{100, 101, 102, 103, 104, 105}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{120, 121, 122, -999999, 124, 125}, " +
                      "{130, 131, 132, 133, 134, 135}}, " +
                     "{{200, 201, 202, 203, 204, 205}, " +
                      "{210, 211, 212, 213, 214, 215}, " +
                      "{220, 221, 222, 223, 224, 225}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    CHECK((string_builder() << a3_transposed_view).str() == 
        std::string("{{{0, 100, 200}, ") +
                      "{10, 110, 210}, " +
                      "{20, 120, 220}, " +
                      "{30, 130, 230}}, " +
                     "{{1, 101, 201}, " +
                      "{11, 111, 211}, " +
                      "{21, 121, 221}, " +
                      "{31, 131, 231}}, " +
                     "{{2, 102, 202}, " +
                      "{12, 112, 212}, " +
                      "{22, 122, 222}, " +
                      "{32, 132, 232}}, " +
                     "{{3, 103, 203}, " +
                      "{13, 113, 213}, " +
                      "{23, -999999, 223}, " +
                      "{33, 133, 233}}, " +
                     "{{4, 104, 204}, " +
                      "{14, 114, 214}, " +
                      "{24, 124, 224}, " +
                      "{34, 134, 234}}, " +
                     "{{5, 105, 205}, " +
                      "{15, 115, 215}, " +
                      "{25, 125, 225}, " +
                      "{35, 135, 235}}}");
    CHECK((string_builder() << a3_transposed_copy).str() == 
        std::string("{{{0, 100, 200}, ") +
                      "{10, 110, 210}, " +
                      "{20, 120, 220}, " +
                      "{30, 130, 230}}, " +
                     "{{1, 101, 201}, " +
                      "{11, 111, 211}, " +
                      "{21, 121, 221}, " +
                      "{31, 131, 231}}, " +
                     "{{2, 102, 202}, " +
                      "{12, 112, 212}, " +
                      "{22, 122, 222}, " +
                      "{32, 132, 232}}, " +
                     "{{3, 103, 203}, " +
                      "{13, 113, 213}, " +
                      "{23, 123, 223}, " +
                      "{33, 133, 233}}, " +
                     "{{4, 104, 204}, " +
                      "{14, 114, 214}, " +
                      "{24, 124, 224}, " +
                      "{34, 134, 234}}, " +
                     "{{5, 105, 205}, " +
                      "{15, 115, 215}, " +
                      "{25, 125, 225}, " +
                      "{35, 135, 235}}}");
    auto a3_swapped_21_view = a3.view_swapped_axes(2, 1);
    CHECK((string_builder() << a3_swapped_21_view).str() == 
        std::string("{{{0, 10, 20, 30}, ") +
                      "{1, 11, 21, 31}, " + 
                      "{2, 12, 22, 32}, " +
                      "{3, 13, 23, 33}, " +
                      "{4, 14, 24, 34}, " +
                      "{5, 15, 25, 35}}, " +
                     "{{100, 110, 120, 130}, " +
                      "{101, 111, 121, 131}, " +
                      "{102, 112, 122, 132}, " +
                      "{103, 113, -999999, 133}, " +
                      "{104, 114, 124, 134}, " +
                      "{105, 115, 125, 135}}, " +
                     "{{200, 210, 220, 230}, " +
                      "{201, 211, 221, 231}, " +
                      "{202, 212, 222, 232}, " +
                      "{203, 213, 223, 233}, " + 
                      "{204, 214, 224, 234}, " +
                      "{205, 215, 225, 235}}}");
    CHECK(a3_swapped_21_view.is_contiguous() == false);
    auto a3_swapped_21_copy = a3.copy_swapped_axes(2, 1);
    CHECK((string_builder() << a3_swapped_21_copy).str() == 
        std::string("{{{0, 10, 20, 30}, ") +
                      "{1, 11, 21, 31}, " + 
                      "{2, 12, 22, 32}, " +
                      "{3, 13, 23, 33}, " +
                      "{4, 14, 24, 34}, " +
                      "{5, 15, 25, 35}}, " +
                     "{{100, 110, 120, 130}, " +
                      "{101, 111, 121, 131}, " +
                      "{102, 112, 122, 132}, " +
                      "{103, 113, -999999, 133}, " +
                      "{104, 114, 124, 134}, " +
                      "{105, 115, 125, 135}}, " +
                     "{{200, 210, 220, 230}, " +
                      "{201, 211, 221, 231}, " +
                      "{202, 212, 222, 232}, " +
                      "{203, 213, 223, 233}, " + 
                      "{204, 214, 224, 234}, " +
                      "{205, 215, 225, 235}}}");
    CHECK(a3_swapped_21_copy.is_contiguous() == true);
    auto a3_swapped_10_view = a3.view_swapped_axes(1, 0);
    CHECK((string_builder() << a3_swapped_10_view).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{100, 101, 102, 103, 104, 105}, "
                      "{200, 201, 202, 203, 204, 205}}, " +
                     "{{10, 11, 12, 13, 14, 15}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{210, 211, 212, 213, 214, 215}}, " + 
                     "{{20, 21, 22, 23, 24, 25}, " + 
                      "{120, 121, 122, -999999, 124, 125}, " +
                      "{220, 221, 222, 223, 224, 225}}, " +
                     "{{30, 31, 32, 33, 34, 35}, " +
                      "{130, 131, 132, 133, 134, 135}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    CHECK(a3_swapped_10_view.is_contiguous() == false);
    auto a3_swapped_10_copy = a3.copy_swapped_axes(1, 0);
    CHECK((string_builder() << a3_swapped_10_copy).str() == 
        std::string("{{{0, 1, 2, 3, 4, 5}, ") +
                      "{100, 101, 102, 103, 104, 105}, "
                      "{200, 201, 202, 203, 204, 205}}, " +
                     "{{10, 11, 12, 13, 14, 15}, " +
                      "{110, 111, 112, 113, 114, 115}, " +
                      "{210, 211, 212, 213, 214, 215}}, " + 
                     "{{20, 21, 22, 23, 24, 25}, " + 
                      "{120, 121, 122, -999999, 124, 125}, " +
                      "{220, 221, 222, 223, 224, 225}}, " +
                     "{{30, 31, 32, 33, 34, 35}, " +
                      "{130, 131, 132, 133, 134, 135}, " +
                      "{230, 231, 232, 233, 234, 235}}}");
    CHECK(a3_swapped_10_copy.is_contiguous() == true);
}


TEST_CASE("test multidimensional array subdivide and absorb (view only)")
{
    auto a1 = array1d<int64>({60}, [](const std::array<int64, 1>& indices) {
        return indices[0];
    });
    CHECK((string_builder() << a1).str() == 
        std::string("{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, ") +
                     "10, 11, 12, 13, 14, 15, 16, 17, 18, 19, " +
                     "20, 21, 22, 23, 24, 25, 26, 27, 28, 29, " +
                     "30, 31, 32, 33, 34, 35, 36, 37, 38, 39, " +
                     "40, 41, 42, 43, 44, 45, 46, 47, 48, 49, " +
                     "50, 51, 52, 53, 54, 55, 56, 57, 58, 59}");
    CHECK(a1.is_contiguous() == true);
    auto a12 = a1.view_subdivided_axis(0, {6, 10});
    CHECK((string_builder() << a12).str() == 
        std::string("{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, ") +
                     "{10, 11, 12, 13, 14, 15, 16, 17, 18, 19}, " +
                     "{20, 21, 22, 23, 24, 25, 26, 27, 28, 29}, " +
                     "{30, 31, 32, 33, 34, 35, 36, 37, 38, 39}, " +
                     "{40, 41, 42, 43, 44, 45, 46, 47, 48, 49}, " +
                     "{50, 51, 52, 53, 54, 55, 56, 57, 58, 59}}");
    CHECK(a12.is_contiguous() == true);
    auto a123 = a12.view_subdivided_axis(1, {2, 5});
    CHECK((string_builder() << a123).str() == 
        std::string("{{{0, 1, 2, 3, 4}, ") +
                      "{5, 6, 7, 8, 9}}, " +
                     "{{10, 11, 12, 13, 14}, " +
                      "{15, 16, 17, 18, 19}}, " +
                     "{{20, 21, 22, 23, 24}, " +
                      "{25, 26, 27, 28, 29}}, " +
                     "{{30, 31, 32, 33, 34}, " +
                      "{35, 36, 37, 38, 39}}, " +
                     "{{40, 41, 42, 43, 44}, " +
                      "{45, 46, 47, 48, 49}}, " +
                     "{{50, 51, 52, 53, 54}, " +
                      "{55, 56, 57, 58, 59}}}");
    CHECK(a123.is_contiguous() == true);
    auto a1234 = a123.view_subdivided_axis(0, {2, 3});
    CHECK((string_builder() << a1234).str() == 
        std::string("{{{{0, 1, 2, 3, 4}, ") +
                       "{5, 6, 7, 8, 9}}, " +
                      "{{10, 11, 12, 13, 14}, " +
                       "{15, 16, 17, 18, 19}}, " +
                      "{{20, 21, 22, 23, 24}, " +
                       "{25, 26, 27, 28, 29}}}, " +
                     "{{{30, 31, 32, 33, 34}, " +
                       "{35, 36, 37, 38, 39}}, " +
                      "{{40, 41, 42, 43, 44}, " +
                       "{45, 46, 47, 48, 49}}, " +
                      "{{50, 51, 52, 53, 54}, " +
                       "{55, 56, 57, 58, 59}}}}");
    CHECK(a1234.is_contiguous() == true);
    auto a12343 = a1234.view_absorbed_axis(2);
    CHECK((string_builder() << a12343).str() == 
        std::string("{{{0, 1, 2, 3, 4}, ") +
                      "{5, 6, 7, 8, 9}, " +
                      "{10, 11, 12, 13, 14}, " +
                      "{15, 16, 17, 18, 19}, " +
                      "{20, 21, 22, 23, 24}, " +
                      "{25, 26, 27, 28, 29}}, " +
                     "{{30, 31, 32, 33, 34}, " +
                      "{35, 36, 37, 38, 39}, " +
                      "{40, 41, 42, 43, 44}, " +
                      "{45, 46, 47, 48, 49}, " +
                      "{50, 51, 52, 53, 54}, " +
                      "{55, 56, 57, 58, 59}}}");
    CHECK(a12343.is_contiguous() == true);
    auto a123433 = a12343[1][range()][range()].view_subdivided_axis(0, {2, 3});
    CHECK((string_builder() << a123433).str() == 
        std::string("{{{30, 31, 32, 33, 34}, ") +
                      "{35, 36, 37, 38, 39}, " +
                      "{40, 41, 42, 43, 44}}, " +
                     "{{45, 46, 47, 48, 49}, " +
                      "{50, 51, 52, 53, 54}, " +
                      "{55, 56, 57, 58, 59}}}");
    CHECK(a123433.is_contiguous() == true);
    auto a1234332 = a123433.view_absorbed_axis(2);
    CHECK((string_builder() << a1234332).str() == 
        std::string("{{30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44}, ") +
                     "{45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59}}");
    CHECK(a1234332.is_contiguous() == true);
    auto a12343321 = a1234332.view_absorbed_axis(1);
    CHECK((string_builder() << a12343321).str() == 
        std::string("{30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, ") +
                     "45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59}");
    CHECK(a12343321.is_contiguous() == true);
    a1({59}) = 1059;
    a12({5, 8}) = 2005008;
    a123({5, 1, 2}) = 3005001002;
    a1234({1, 2, 1, 1}) = 4001002001001;
    a12343({1, 5, 0}) = 3001005000;
    a123433({1, 1, 4}) = 3001001004;
    a1234332({1, 8}) = 2001008;
    a12343321({22}) = 1022;
    CHECK((string_builder() << a1).str() == 
        std::string("{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, ") +
                     "10, 11, 12, 13, 14, 15, 16, 17, 18, 19, " +
                     "20, 21, 22, 23, 24, 25, 26, 27, 28, 29, " +
                     "30, 31, 32, 33, 34, 35, 36, 37, 38, 39, " +
                     "40, 41, 42, 43, 44, 45, 46, 47, 48, 49, " +
                     "50, 51, 1022, 2001008, 3001001004, 3001005000, 4001002001001, 3005001002, 2005008, 1059}");
    CHECK((string_builder() << a12).str() == 
        std::string("{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, ") +
                     "{10, 11, 12, 13, 14, 15, 16, 17, 18, 19}, " +
                     "{20, 21, 22, 23, 24, 25, 26, 27, 28, 29}, " +
                     "{30, 31, 32, 33, 34, 35, 36, 37, 38, 39}, " +
                     "{40, 41, 42, 43, 44, 45, 46, 47, 48, 49}, " +
                     "{50, 51, 1022, 2001008, 3001001004, 3001005000, 4001002001001, 3005001002, 2005008, 1059}}");
    CHECK((string_builder() << a123).str() == 
        std::string("{{{0, 1, 2, 3, 4}, ") +
                      "{5, 6, 7, 8, 9}}, " +
                     "{{10, 11, 12, 13, 14}, " +
                      "{15, 16, 17, 18, 19}}, " +
                     "{{20, 21, 22, 23, 24}, " +
                      "{25, 26, 27, 28, 29}}, " +
                     "{{30, 31, 32, 33, 34}, " +
                      "{35, 36, 37, 38, 39}}, " +
                     "{{40, 41, 42, 43, 44}, " +
                      "{45, 46, 47, 48, 49}}, " +
                     "{{50, 51, 1022, 2001008, 3001001004}, " +
                      "{3001005000, 4001002001001, 3005001002, 2005008, 1059}}}");
    CHECK((string_builder() << a1234).str() == 
        std::string("{{{{0, 1, 2, 3, 4}, ") +
                       "{5, 6, 7, 8, 9}}, " +
                      "{{10, 11, 12, 13, 14}, " +
                       "{15, 16, 17, 18, 19}}, " +
                      "{{20, 21, 22, 23, 24}, " +
                       "{25, 26, 27, 28, 29}}}, " +
                     "{{{30, 31, 32, 33, 34}, " +
                       "{35, 36, 37, 38, 39}}, " +
                      "{{40, 41, 42, 43, 44}, " +
                       "{45, 46, 47, 48, 49}}, " +
                      "{{50, 51, 1022, 2001008, 3001001004}, " +
                       "{3001005000, 4001002001001, 3005001002, 2005008, 1059}}}}");
    CHECK((string_builder() << a12343).str() == 
        std::string("{{{0, 1, 2, 3, 4}, ") +
                      "{5, 6, 7, 8, 9}, " +
                      "{10, 11, 12, 13, 14}, " +
                      "{15, 16, 17, 18, 19}, " +
                      "{20, 21, 22, 23, 24}, " +
                      "{25, 26, 27, 28, 29}}, " +
                     "{{30, 31, 32, 33, 34}, " +
                      "{35, 36, 37, 38, 39}, " +
                      "{40, 41, 42, 43, 44}, " +
                      "{45, 46, 47, 48, 49}, " +
                      "{50, 51, 1022, 2001008, 3001001004}, " +
                      "{3001005000, 4001002001001, 3005001002, 2005008, 1059}}}");
    CHECK((string_builder() << a123433).str() == 
        std::string("{{{30, 31, 32, 33, 34}, ") +
                      "{35, 36, 37, 38, 39}, " +
                      "{40, 41, 42, 43, 44}}, " +
                     "{{45, 46, 47, 48, 49}, " +
                      "{50, 51, 1022, 2001008, 3001001004}, " +
                      "{3001005000, 4001002001001, 3005001002, 2005008, 1059}}}");
    CHECK((string_builder() << a1234332).str() == 
        std::string("{{30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44}, ") +
                     "{45, 46, 47, 48, 49, 50, 51, 1022, 2001008, 3001001004, 3001005000, 4001002001001, 3005001002, 2005008, 1059}}");
    CHECK((string_builder() << a12343321).str() == 
        std::string("{30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, ") +
                     "45, 46, 47, 48, 49, 50, 51, 1022, 2001008, 3001001004, 3001005000, 4001002001001, 3005001002, 2005008, 1059}");
}


TEST_CASE("test multidimensional array subdivide and absorb (copy only)")
{
    auto a1 = array1d<int64>({60}, [](const std::array<int64, 1>& indices) {
        return indices[0];
    });
    CHECK((string_builder() << a1).str() == 
        std::string("{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, ") +
                     "10, 11, 12, 13, 14, 15, 16, 17, 18, 19, " +
                     "20, 21, 22, 23, 24, 25, 26, 27, 28, 29, " +
                     "30, 31, 32, 33, 34, 35, 36, 37, 38, 39, " +
                     "40, 41, 42, 43, 44, 45, 46, 47, 48, 49, " +
                     "50, 51, 52, 53, 54, 55, 56, 57, 58, 59}");
    CHECK(a1.is_contiguous() == true);
    auto a12 = a1.copy_subdivided_axis(0, {6, 10});
    CHECK((string_builder() << a12).str() == 
        std::string("{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, ") +
                     "{10, 11, 12, 13, 14, 15, 16, 17, 18, 19}, " +
                     "{20, 21, 22, 23, 24, 25, 26, 27, 28, 29}, " +
                     "{30, 31, 32, 33, 34, 35, 36, 37, 38, 39}, " +
                     "{40, 41, 42, 43, 44, 45, 46, 47, 48, 49}, " +
                     "{50, 51, 52, 53, 54, 55, 56, 57, 58, 59}}");
    CHECK(a12.is_contiguous() == true);
    auto a123 = a12.copy_subdivided_axis(1, {2, 5});
    CHECK((string_builder() << a123).str() == 
        std::string("{{{0, 1, 2, 3, 4}, ") +
                      "{5, 6, 7, 8, 9}}, " +
                     "{{10, 11, 12, 13, 14}, " +
                      "{15, 16, 17, 18, 19}}, " +
                     "{{20, 21, 22, 23, 24}, " +
                      "{25, 26, 27, 28, 29}}, " +
                     "{{30, 31, 32, 33, 34}, " +
                      "{35, 36, 37, 38, 39}}, " +
                     "{{40, 41, 42, 43, 44}, " +
                      "{45, 46, 47, 48, 49}}, " +
                     "{{50, 51, 52, 53, 54}, " +
                      "{55, 56, 57, 58, 59}}}");
    CHECK(a123.is_contiguous() == true);
    auto a1234 = a123.copy_subdivided_axis(0, {2, 3});
    CHECK((string_builder() << a1234).str() == 
        std::string("{{{{0, 1, 2, 3, 4}, ") +
                       "{5, 6, 7, 8, 9}}, " +
                      "{{10, 11, 12, 13, 14}, " +
                       "{15, 16, 17, 18, 19}}, " +
                      "{{20, 21, 22, 23, 24}, " +
                       "{25, 26, 27, 28, 29}}}, " +
                     "{{{30, 31, 32, 33, 34}, " +
                       "{35, 36, 37, 38, 39}}, " +
                      "{{40, 41, 42, 43, 44}, " +
                       "{45, 46, 47, 48, 49}}, " +
                      "{{50, 51, 52, 53, 54}, " +
                       "{55, 56, 57, 58, 59}}}}");
    CHECK(a1234.is_contiguous() == true);
    auto a12343 = a1234.copy_absorbed_axis(2);
    CHECK((string_builder() << a12343).str() == 
        std::string("{{{0, 1, 2, 3, 4}, ") +
                      "{5, 6, 7, 8, 9}, " +
                      "{10, 11, 12, 13, 14}, " +
                      "{15, 16, 17, 18, 19}, " +
                      "{20, 21, 22, 23, 24}, " +
                      "{25, 26, 27, 28, 29}}, " +
                     "{{30, 31, 32, 33, 34}, " +
                      "{35, 36, 37, 38, 39}, " +
                      "{40, 41, 42, 43, 44}, " +
                      "{45, 46, 47, 48, 49}, " +
                      "{50, 51, 52, 53, 54}, " +
                      "{55, 56, 57, 58, 59}}}");
    CHECK(a12343.is_contiguous() == true);
    auto a123433 = a12343[1][range()][range()].copy_subdivided_axis(0, {2, 3});
    CHECK((string_builder() << a123433).str() == 
        std::string("{{{30, 31, 32, 33, 34}, ") +
                      "{35, 36, 37, 38, 39}, " +
                      "{40, 41, 42, 43, 44}}, " +
                     "{{45, 46, 47, 48, 49}, " +
                      "{50, 51, 52, 53, 54}, " +
                      "{55, 56, 57, 58, 59}}}");
    CHECK(a123433.is_contiguous() == true);
    auto a1234332 = a123433.copy_absorbed_axis(2);
    CHECK((string_builder() << a1234332).str() == 
        std::string("{{30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44}, ") +
                     "{45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59}}");
    CHECK(a1234332.is_contiguous() == true);
    auto a12343321 = a1234332.copy_absorbed_axis(1);
    CHECK((string_builder() << a12343321).str() == 
        std::string("{30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, ") +
                     "45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59}");
    CHECK(a12343321.is_contiguous() == true);
    a1({59}) = 1059;
    a12({5, 8}) = 2005008;
    a123({5, 1, 2}) = 3005001002;
    a1234({1, 2, 1, 1}) = 4001002001001;
    a12343({1, 5, 0}) = 3001005000;
    a123433({1, 1, 4}) = 3001001004;
    a1234332({1, 8}) = 2001008;
    a12343321({22}) = 1022;
    CHECK((string_builder() << a1).str() == 
        std::string("{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, ") +
                     "10, 11, 12, 13, 14, 15, 16, 17, 18, 19, " +
                     "20, 21, 22, 23, 24, 25, 26, 27, 28, 29, " +
                     "30, 31, 32, 33, 34, 35, 36, 37, 38, 39, " +
                     "40, 41, 42, 43, 44, 45, 46, 47, 48, 49, " +
                     "50, 51, 52, 53, 54, 55, 56, 57, 58, 1059}");
    CHECK((string_builder() << a12).str() == 
        std::string("{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, ") +
                     "{10, 11, 12, 13, 14, 15, 16, 17, 18, 19}, " +
                     "{20, 21, 22, 23, 24, 25, 26, 27, 28, 29}, " +
                     "{30, 31, 32, 33, 34, 35, 36, 37, 38, 39}, " +
                     "{40, 41, 42, 43, 44, 45, 46, 47, 48, 49}, " +
                     "{50, 51, 52, 53, 54, 55, 56, 57, 2005008, 59}}");
    CHECK((string_builder() << a123).str() == 
        std::string("{{{0, 1, 2, 3, 4}, ") +
                      "{5, 6, 7, 8, 9}}, " +
                     "{{10, 11, 12, 13, 14}, " +
                      "{15, 16, 17, 18, 19}}, " +
                     "{{20, 21, 22, 23, 24}, " +
                      "{25, 26, 27, 28, 29}}, " +
                     "{{30, 31, 32, 33, 34}, " +
                      "{35, 36, 37, 38, 39}}, " +
                     "{{40, 41, 42, 43, 44}, " +
                      "{45, 46, 47, 48, 49}}, " +
                     "{{50, 51, 52, 53, 54}, " +
                      "{55, 56, 3005001002, 58, 59}}}");
    CHECK((string_builder() << a1234).str() == 
        std::string("{{{{0, 1, 2, 3, 4}, ") +
                       "{5, 6, 7, 8, 9}}, " +
                      "{{10, 11, 12, 13, 14}, " +
                       "{15, 16, 17, 18, 19}}, " +
                      "{{20, 21, 22, 23, 24}, " +
                       "{25, 26, 27, 28, 29}}}, " +
                     "{{{30, 31, 32, 33, 34}, " +
                       "{35, 36, 37, 38, 39}}, " +
                      "{{40, 41, 42, 43, 44}, " +
                       "{45, 46, 47, 48, 49}}, " +
                      "{{50, 51, 52, 53, 54}, " +
                       "{55, 4001002001001, 57, 58, 59}}}}");
    CHECK((string_builder() << a12343).str() == 
        std::string("{{{0, 1, 2, 3, 4}, ") +
                      "{5, 6, 7, 8, 9}, " +
                      "{10, 11, 12, 13, 14}, " +
                      "{15, 16, 17, 18, 19}, " +
                      "{20, 21, 22, 23, 24}, " +
                      "{25, 26, 27, 28, 29}}, " +
                     "{{30, 31, 32, 33, 34}, " +
                      "{35, 36, 37, 38, 39}, " +
                      "{40, 41, 42, 43, 44}, " +
                      "{45, 46, 47, 48, 49}, " +
                      "{50, 51, 52, 53, 54}, " +
                      "{3001005000, 56, 57, 58, 59}}}");
    CHECK((string_builder() << a123433).str() == 
        std::string("{{{30, 31, 32, 33, 34}, ") +
                      "{35, 36, 37, 38, 39}, " +
                      "{40, 41, 42, 43, 44}}, " +
                     "{{45, 46, 47, 48, 49}, " +
                      "{50, 51, 52, 53, 3001001004}, " +
                      "{55, 56, 57, 58, 59}}}");
    CHECK((string_builder() << a1234332).str() == 
        std::string("{{30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44}, ") +
                     "{45, 46, 47, 48, 49, 50, 51, 52, 2001008, 54, 55, 56, 57, 58, 59}}");
    CHECK((string_builder() << a12343321).str() == 
        std::string("{30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, ") +
                     "45, 46, 47, 48, 49, 50, 51, 1022, 53, 54, 55, 56, 57, 58, 59}");
}


TEST_CASE("check documentation examples")
{
    auto a1 = array3d<int64>({2, 3, 4}, [](const std::array<int64, 3>& indices) {
        return 100*indices[0] + 10*indices[1] + indices[2];
    });
    auto a2 = a1.copy();
    auto a3 = a1.copy();
    auto a4 = a1.copy();
    auto a5 = a1.copy();
    CHECK((string_builder() << a1).str() ==
        std::string("{{{0, 1, 2, 3}, ") +
                      "{10, 11, 12, 13}, " +
                      "{20, 21, 22, 23}}, " +
                     "{{100, 101, 102, 103}, " +
                      "{110, 111, 112, 113}, " +
                      "{120, 121, 122, 123}}}");
    CHECK(a1(1, 0, 3) == 103);
    a1(1, 0, 3) = 7;
    CHECK((string_builder() << a1).str() ==
        std::string("{{{0, 1, 2, 3}, ") +
                      "{10, 11, 12, 13}, " +
                      "{20, 21, 22, 23}}, " +
                      "{{100, 101, 102, 7}, " +
                      "{110, 111, 112, 113}, " +
                      "{120, 121, 122, 123}}}");
    CHECK(a2[1][0][3] == 103);
    a2(1, 0, 3) = 7;
    CHECK((string_builder() << a2).str() ==
        std::string("{{{0, 1, 2, 3}, ") +
                      "{10, 11, 12, 13}, " +
                      "{20, 21, 22, 23}}, " +
                     "{{100, 101, 102, 7}, " +
                      "{110, 111, 112, 113}, " +
                      "{120, 121, 122, 123}}}");
    CHECK((string_builder() << a3[range()][1][range().start_at(1).stride_by(2)]).str() ==
        std::string("{{11, 13}, ") +
                     "{111, 113}}");
    a3[range()][1][range().start_at(1).stride_by(2)].fill(7);
    CHECK((string_builder() << a3).str() ==
        std::string("{{{0, 1, 2, 3}, ") +
                      "{10, 7, 12, 7}, " +
                      "{20, 21, 22, 23}}, " +
                     "{{100, 101, 102, 103}, " +
                      "{110, 7, 112, 7}, " +
                      "{120, 121, 122, 123}}}");
    auto a4_view = a4[range()][1][range().start_at(1).stride_by(2)];
    a4_view.fill(7);
    CHECK((string_builder() << a4).str() ==
        std::string("{{{0, 1, 2, 3}, ") +
                      "{10, 7, 12, 7}, " +
                      "{20, 21, 22, 23}}, " +
                     "{{100, 101, 102, 103}, " +
                      "{110, 7, 112, 7}, " +
                      "{120, 121, 122, 123}}}");
    auto a5_copy = a5[range()][1][range().start_at(1).stride_by(2)].copy();
    a5_copy.fill(7);
    CHECK((string_builder() << a5).str() ==
        std::string("{{{0, 1, 2, 3}, ") +
                      "{10, 11, 12, 13}, " +
                      "{20, 21, 22, 23}}, " +
                     "{{100, 101, 102, 103}, " +
                      "{110, 111, 112, 113}, " +
                      "{120, 121, 122, 123}}}");
    const auto a6 = array2d<std::string>({2, 2}, {"A", "B", "C", "D"});
    auto a6_view = a6.view();
    {
        auto errmsg = std::string();
        try {
            a6_view(0, 0) = "E";
        }
        catch (const std::logic_error& e) {
            errmsg = e.what();
        }
        CHECK(errmsg == "Attempt to obtain a non-const reference to readonly multidimensional array data");
    }
    const auto a7 = array2d<std::string>({2, 2}, {"A", "B", "C", "D"});
    auto a7_view = a7[range()];
    {
        auto errmsg = std::string();
        try {
            a7_view(0, 0) = "E";
        }
        catch (const std::logic_error& e) {
            errmsg = e.what();
        }
        CHECK(errmsg == "Attempt to obtain a non-const reference to readonly multidimensional array data");
    }
}


}  // namespace
