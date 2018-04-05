#pragma once
#ifndef REGRESSION_TEST_H_
#define REGRESSION_TEST_H_

#include <string>
#include <functional>


int perform_regression_test(const std::string& filename, std::function<void()> test_function);


#endif
