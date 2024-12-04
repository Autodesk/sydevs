#include <sydevs/core/arraynd.h>
#include <chrono>
#include <memory>
#include <random>
#include <iostream>
#include <stdexcept>

using namespace sydevs;


using pointer = std::shared_ptr<void>;

template< typename T >
struct raw_array_deleter
{
    void operator()(const T* p)
    { 
        delete[] p; 
    }
};


using array_op = std::function<pointer(int64 m, int64 n, int64 p)>;
using array_assert_op = std::function<void(const pointer& obj_arr_ptr, const pointer& raw_arr_ptr)>;


void arrayperf_test_scale_factor(array_op obj_array_op, array_op raw_array_op, array_assert_op assert_op, int64 scale_factor)
{
    int64 m = 3*scale_factor;
    int64 n = 5*scale_factor;
    int64 p = 8*scale_factor;

    auto chrono_tp_0 = std::chrono::system_clock::now();

    auto obj_arr_ptr = obj_array_op(m, n, p);
    
    auto chrono_tp_1 = std::chrono::system_clock::now();

    auto raw_arr_ptr = raw_array_op(m, n, p);

    auto chrono_tp_2 = std::chrono::system_clock::now();

    assert_op(obj_arr_ptr, raw_arr_ptr);

    auto obj_array_dt = double(std::chrono::duration_cast<std::chrono::microseconds>(chrono_tp_1 - chrono_tp_0).count());
    auto raw_array_dt = double(std::chrono::duration_cast<std::chrono::microseconds>(chrono_tp_2 - chrono_tp_1).count());

    std::cout << "Obj Array: " << obj_array_dt << ";   Raw Array: " << raw_array_dt << ";   Ratio: " << obj_array_dt/raw_array_dt << std::endl;
}


void arrayperf_test(array_op obj_array_op, array_op raw_array_op, array_assert_op assert_op)
{
    arrayperf_test_scale_factor(obj_array_op, raw_array_op, assert_op, 25);
    arrayperf_test_scale_factor(obj_array_op, raw_array_op, assert_op, 50);
    arrayperf_test_scale_factor(obj_array_op, raw_array_op, assert_op, 100);
}


int main(int argc, const char* argv[])
{
    std::cout << std::endl;

    auto equal_assert_op = [](const pointer& obj_arr_ptr, const pointer& raw_arr_ptr) -> void {
        auto& obj_arr = *static_cast<array3d<float64>*>(obj_arr_ptr.get());
        auto* raw_arr = static_cast<float64*>(raw_arr_ptr.get());
        int m = obj_arr.dims()[0];
        int n = obj_arr.dims()[1];
        int p = obj_arr.dims()[2];
        for (int64 i = 0; i < m; ++i) {
            for (int64 j = 0; j < n; ++j) {
                for (int64 k = 0; k < p; ++k) {
                    if (obj_arr({i, j, k}) != raw_arr[(i*n + j)*p + k]) {
                        throw std::range_error("assert_op failed");
                    }
                }
            }
        }
    };

    auto empty_assert_op = [](const pointer& obj_arr_ptr, const pointer& raw_arr_ptr) -> void {
    };

    {
        std::cout << "Testing performance of fill constructor..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            auto* obj_arr_ptr = new array3d<float64>({m, n, p}, 7.0);
            return pointer(obj_arr_ptr);
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = 7.0;
                    }
                }
            }
            return pointer(raw_arr, raw_array_deleter<float64>());
        };

        arrayperf_test(obj_array_op, raw_array_op, equal_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of fill constructor and direct data assigment..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            auto* obj_arr_ptr = new array3d<float64>({m, n, p}, 0);
            auto& obj_arr = *obj_arr_ptr;
            auto* obj_arr_data = obj_arr.data();
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        obj_arr_data[(i*n + j)*p + k] = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            return pointer(obj_arr_ptr);
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            return pointer(raw_arr, raw_array_deleter<float64>());
        };

        arrayperf_test(obj_array_op, raw_array_op, equal_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of fill constructor and parenthesis iteration and assigment..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            auto* obj_arr_ptr = new array3d<float64>({m, n, p}, 0);
            auto& obj_arr = *obj_arr_ptr;
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        obj_arr({i, j, k}) = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            return pointer(obj_arr_ptr);
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            return pointer(raw_arr, raw_array_deleter<float64>());
        };

        arrayperf_test(obj_array_op, raw_array_op, equal_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of fill constructor and parenthesis column-major iteration and assigment..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            auto* obj_arr_ptr = new array3d<float64>({m, n, p}, 0);
            auto& obj_arr = *obj_arr_ptr;
            for (int64 k = 0; k < p; ++k) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 i = 0; i < m; ++i) {
                        obj_arr({i, j, k}) = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            return pointer(obj_arr_ptr);
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 k = 0; k < p; ++k) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 i = 0; i < m; ++i) {
                        raw_arr[(i*n + j)*p + k] = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            return pointer(raw_arr, raw_array_deleter<float64>());
        };

        arrayperf_test(obj_array_op, raw_array_op, equal_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of fill constructor and parenthesis random iteration and assigment..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            auto* obj_arr_ptr = new array3d<float64>({m, n, p}, 0);
            auto& obj_arr = *obj_arr_ptr;
            std::minstd_rand rng(10);
            std::uniform_int_distribution<int64> idist(0, m - 1);
            std::uniform_int_distribution<int64> jdist(0, n - 1);
            std::uniform_int_distribution<int64> kdist(0, p - 1);
            for (int64 a = 0; a < 100; ++a) {
                int64 i = idist(rng);
                int64 j = jdist(rng);
                int64 k = kdist(rng);
                obj_arr({i, j, k}) = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
            }
            return pointer(obj_arr_ptr);
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = 0.0;
                    }
                }
            }
            std::minstd_rand rng(10);
            std::uniform_int_distribution<int64> idist(0, m - 1);
            std::uniform_int_distribution<int64> jdist(0, n - 1);
            std::uniform_int_distribution<int64> kdist(0, p - 1);
            for (int64 a = 0; a < 100; ++a) {
                int64 i = idist(rng);
                int64 j = jdist(rng);
                int64 k = kdist(rng);
                raw_arr[(i*n + j)*p + k] = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
            }
            return pointer(raw_arr, raw_array_deleter<float64>());
        };

        arrayperf_test(obj_array_op, raw_array_op, equal_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of fill constructor and variadic function iteration and assigment..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            auto* obj_arr_ptr = new array3d<float64>({m, n, p}, 0);
            auto& obj_arr = *obj_arr_ptr;
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        obj_arr(i, j, k) = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            return pointer(obj_arr_ptr);
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            return pointer(raw_arr, raw_array_deleter<float64>());
        };

        arrayperf_test(obj_array_op, raw_array_op, equal_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of fill constructor and square bracket iteration and assigment..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            auto* obj_arr_ptr = new array3d<float64>({m, n, p}, 0);
            auto& obj_arr = *obj_arr_ptr;
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        obj_arr[i][j][k] = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            return pointer(obj_arr_ptr);
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            return pointer(raw_arr, raw_array_deleter<float64>());
        };

        arrayperf_test(obj_array_op, raw_array_op, equal_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of fill constructor and factored square bracket iteration and assigment..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            auto* obj_arr_ptr = new array3d<float64>({m, n, p}, 0);
            auto& obj_arr = *obj_arr_ptr;
            for (int64 i = 0; i < m; ++i) {
                auto obj_arr_i = obj_arr[i];
                for (int64 j = 0; j < n; ++j) {
                    auto obj_arr_ij = obj_arr_i[j];
                    for (int64 k = 0; k < p; ++k) {
                        obj_arr_ij[k] = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            return pointer(obj_arr_ptr);
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            return pointer(raw_arr, raw_array_deleter<float64>());
        };

        arrayperf_test(obj_array_op, raw_array_op, equal_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of function application constructor..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            auto* obj_arr_ptr = new array3d<float64>({m, n, p}, [m, n, p](const std::array<int64, 3>& indices) {
                return (n*indices[2] - p*indices[1])/float64((m - indices[0])*(n + 1)*(p + 1));
            });
            return pointer(obj_arr_ptr);
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            return pointer(raw_arr, raw_array_deleter<float64>());
        };

        arrayperf_test(obj_array_op, raw_array_op, equal_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of parenthesis iteration..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            const auto* obj_arr_ptr = new array3d<float64>({m, n, p}, 7.0);
            const auto& obj_arr = *obj_arr_ptr;
            double x = 0.0;
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        x += obj_arr({i, j, k});
                    }
                }
            }
            return pointer();
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = 7.0;
                    }
                }
            }
            double x = 0.0;
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        x += raw_arr[(i*n + j)*p + k];
                    }
                }
            }
            return pointer();
        };

        arrayperf_test(obj_array_op, raw_array_op, empty_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of parenthesis column-major iteration..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            const auto* obj_arr_ptr = new array3d<float64>({m, n, p}, 7.0);
            const auto& obj_arr = *obj_arr_ptr;
            double x = 0.0;
            for (int64 k = 0; k < p; ++k) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 i = 0; i < m; ++i) {
                        x += obj_arr({i, j, k});
                    }
                }
            }
            return pointer();
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = 7.0;
                    }
                }
            }
            double x = 0.0;
            for (int64 k = 0; k < p; ++k) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 i = 0; i < m; ++i) {
                        x += raw_arr[(i*n + j)*p + k];
                    }
                }
            }
            return pointer();
        };

        arrayperf_test(obj_array_op, raw_array_op, empty_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of parenthesis random iteration..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            const auto* obj_arr_ptr = new array3d<float64>({m, n, p}, 7.0);
            const auto& obj_arr = *obj_arr_ptr;
            double x = 0.0;
            std::minstd_rand rng(10);
            std::uniform_int_distribution<int64> idist(0, m - 1);
            std::uniform_int_distribution<int64> jdist(0, n - 1);
            std::uniform_int_distribution<int64> kdist(0, p - 1);
            for (int64 a = 0; a < 100; ++a) {
                int64 i = idist(rng);
                int64 j = jdist(rng);
                int64 k = kdist(rng);
                x += obj_arr({i, j, k});
            }
            return pointer();
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = 7.0;
                    }
                }
            }
            double x = 0.0;
            std::minstd_rand rng(10);
            std::uniform_int_distribution<int64> idist(0, m - 1);
            std::uniform_int_distribution<int64> jdist(0, n - 1);
            std::uniform_int_distribution<int64> kdist(0, p - 1);
            for (int64 a = 0; a < 100; ++a) {
                int64 i = idist(rng);
                int64 j = jdist(rng);
                int64 k = kdist(rng);
                x += raw_arr[(i*n + j)*p + k];
            }
            return pointer();
        };

        arrayperf_test(obj_array_op, raw_array_op, empty_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of variadic function iteration..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            const auto* obj_arr_ptr = new array3d<float64>({m, n, p}, 7.0);
            const auto& obj_arr = *obj_arr_ptr;
            double x = 0.0;
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        x += obj_arr(i, j, k);
                    }
                }
            }
            return pointer();
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = 7.0;
                    }
                }
            }
            double x = 0.0;
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        x += raw_arr[(i*n + j)*p + k];
                    }
                }
            }
            return pointer();
        };

        arrayperf_test(obj_array_op, raw_array_op, empty_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of square bracket iteration..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            const auto* obj_arr_ptr = new array3d<float64>({m, n, p}, 7.0);
            const auto& obj_arr = *obj_arr_ptr;
            double x = 0.0;
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        x += obj_arr[i][j][k];
                    }
                }
            }
            return pointer();
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = 7.0;
                    }
                }
            }
            double x = 0.0;
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        x += raw_arr[(i*n + j)*p + k];
                    }
                }
            }
            return pointer();
        };

        arrayperf_test(obj_array_op, raw_array_op, empty_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of factored square bracket iteration..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            const auto* obj_arr_ptr = new array3d<float64>({m, n, p}, 7.0);
            const auto& obj_arr = *obj_arr_ptr;
            double x = 0.0;
            for (int64 i = 0; i < m; ++i) {
                const auto obj_arr_i = obj_arr[i];
                for (int64 j = 0; j < n; ++j) {
                    const auto obj_arr_ij = obj_arr_i[j];
                    for (int64 k = 0; k < p; ++k) {
                        x += obj_arr_ij[k];
                    }
                }
            }
            return pointer();
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = 7.0;
                    }
                }
            }
            double x = 0.0;
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        x += raw_arr[(i*n + j)*p + k];
                    }
                }
            }
            return pointer();
        };

        arrayperf_test(obj_array_op, raw_array_op, empty_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of slicing and parenthesis iteration..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            auto obj_arr = array3d<float64>({m, n, p}, 0);
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        obj_arr({i, j, k}) = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            auto* obj_slice_ptr = new array3d<float64>({0, 0, 0}, 0);
            *obj_slice_ptr = obj_arr[range().start_at(1).stop_before(m - 1)][range().stride_by(2)][range().start_at(p/2).stride_by(-3)];
            return pointer(obj_slice_ptr);
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            float64* slice_arr = new float64[m*n*p];
            int64 offset = 0;
            for (int64 i = 1; i < m - 1; ++i) {
                for (int64 j = 0; j < n; j += 2) {
                    for (int64 k = p/2; k >= 0; k -= 3) {
                        slice_arr[offset] = raw_arr[(i*n + j)*p + k];
                        ++offset;
                    }
                }
            }
            delete[] raw_arr;
            return pointer(slice_arr, raw_array_deleter<float64>());
        };

        arrayperf_test(obj_array_op, raw_array_op, equal_assert_op);
    }
    std::cout << std::endl;
    {
        std::cout << "Testing performance of slicing and assignment..." << std::endl;

        auto obj_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            auto* obj_arr_ptr = new array3d<float64>({m, n, p}, 0);
            auto& obj_arr = *obj_arr_ptr;
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        obj_arr({i, j, k}) = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            obj_arr[range().start_at(1).stop_before(m - 1)]
                   [range().stop_before(n - 1).stride_by(2)]
                   [range().start_at(p/2).stride_by(-3)].assign(obj_arr[range().start_at(1).stop_before(m - 1)]
                                                                       [range().start_after(0).stride_by(2)]
                                                                       [range().start_at(p/2).stride_by(-3)]);
            return pointer(obj_arr_ptr);
        };

        auto raw_array_op = [](int64 m, int64 n, int64 p) -> pointer {
            float64* raw_arr = new float64[m*n*p];
            for (int64 i = 0; i < m; ++i) {
                for (int64 j = 0; j < n; ++j) {
                    for (int64 k = 0; k < p; ++k) {
                        raw_arr[(i*n + j)*p + k] = (n*k - p*j)/float64((m - i)*(n + 1)*(p + 1));
                    }
                }
            }
            for (int64 i = 1; i < m - 1; ++i) {
                for (int64 j = 0; j < n - 1; j += 2) {
                    for (int64 k = p/2; k >= 0; k -= 3) {
                        raw_arr[(i*n + j)*p + k] = raw_arr[(i*n + j + 1)*p + k];
                    }
                }
            }
            return pointer(raw_arr, raw_array_deleter<float64>());
        };

        arrayperf_test(obj_array_op, raw_array_op, equal_assert_op);
    }

    return 0;
}
