#include <regression_test.h>
#include <example/test_systems/hello/hello_systems.h>
#include <example/test_systems/basic/basic_systems.h>
#include <example/test_systems/sorting/sorting_systems.h>
#include <example/test_systems/data/data_systems.h>
#include <example/test_systems/agentbased/agentbased_systems.h>
#include <example/test_systems/cellular/cellular_systems.h>
#include <example/research/customs/customs.h>
#include <example/research/prediction/prediction.h>
#include <example/demo/building7m/building7m.h>
#include <example/demo/queueing/queueing.h>
#include <vector>
#include <algorithm>
#include <iostream>


int main(int argc, const char* argv[])
{
    auto error_codes = std::vector<int>();

    error_codes.push_back(perform_regression_test("output_of_hello_systems.txt", &hello_systems));
    error_codes.push_back(perform_regression_test("output_of_basic_systems.txt", &basic_systems));
    error_codes.push_back(perform_regression_test("output_of_sorting_systems.txt", &sorting_systems));
    error_codes.push_back(perform_regression_test("output_of_data_systems.txt", &data_systems));
    error_codes.push_back(perform_regression_test("output_of_cellular_systems.txt", &cellular_systems));
#ifdef _WIN32
    error_codes.push_back(perform_regression_test("output_of_agentbased_systems.txt", &agentbased_systems));
    error_codes.push_back(perform_regression_test("output_of_prediction.txt", &prediction_system));
    error_codes.push_back(perform_regression_test("output_of_customs.txt", &customs));
#endif
    error_codes.push_back(perform_regression_test("output_of_building7m.txt", &building7m));
    error_codes.push_back(perform_regression_test("output_of_queueing.txt", &queueing));

    int error_count = std::count_if(begin(error_codes), end(error_codes), [](int error_code) { return error_code != 0; } );

    if (error_count == 0) {
        std::cout << "===============================================================================" << std::endl;
        std::cout << "All regression tests passed" << std::endl;
        std::cout << std::endl;
    }
    else {
        std::cout << "===============================================================================" << std::endl;
        std::cout << error_count << " regression test(s) failed" << std::endl;
        std::cout << std::endl;
    }
    
    return (error_count == 0 ? 0 : 1);
}
