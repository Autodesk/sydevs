#pragma once
#ifndef SYDEVS_EXAMPLES_PREDICTION_H_
#define SYDEVS_EXAMPLES_PREDICTION_H_

#include <examples/research/prediction/prediction_closed_system.h>
#include <ctime>
#include <chrono>
#include <iostream>
#include <time.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


void prediction_systems(const std::string& task, int64 upper_scale, int64 lower_scale, int64 repetitions)
{
    try {
        if (task == "detail") {
            sydevs::int64 seed = 0;
            if (repetitions != 1) throw std::invalid_argument("Only 1 repetition allowed for task=\"detail\"");
            simulate_prediction_system(upper_scale, lower_scale, seed, true);
        }
        else if (task == "matrix") {
            auto chrono_tp_0 = std::chrono::system_clock::now();
            {
                auto now = std::chrono::system_clock::to_time_t(chrono_tp_0);
                char buffer[26];
#if _MSC_VER
                struct tm local_now;
                localtime_s(&local_now, &now);
                strftime(buffer, sizeof(buffer), "%c", &local_now);
#else
                auto local_now = localtime(&now);
                strftime(buffer, sizeof(buffer), "%c", local_now);
#endif
                std::cout << buffer << std::endl;
                std::cout << std::endl;
            }
            int64 configurations = upper_scale*lower_scale;
            std::vector<float64> noise_grid(configurations, 0.0);
            std::vector<float64> correct_grid(configurations, 0.0);
            std::vector<float64> stderr_grid(configurations, 0.0);
            sydevs::int64 seed = 0;
            for (int64 uscale = 0; uscale < upper_scale; ++uscale) {
                for (int64 lscale = 0; lscale < lower_scale; ++lscale) {
                    int64 index = upper_scale*lscale + uscale;
                    for (int64 rep = 0; rep < repetitions; ++rep) {
                        auto stats = simulate_prediction_system(upper_scale, lower_scale, seed, false);
                        int64 noise = stats.second;
                        int64 correct = stats.first;
                        std::cout << "Simulation Run: " << uscale << " " << lscale << " " << rep << std::endl;
                        std::cout << "Noise Count:    " << noise << std::endl;
                        std::cout << "Correct Count:  " << correct << std::endl;
                        std::cout << std::endl;
                        noise_grid[index] += noise;
                        correct_grid[index] += correct;
                        stderr_grid[index] += correct*correct;
                        ++seed;
                    }
                    noise_grid[index] /= repetitions;
                    correct_grid[index] /= repetitions;
                    stderr_grid[index] /= repetitions;
                    stderr_grid[index] = stderr_grid[index] - correct_grid[index]*correct_grid[index];
                    stderr_grid[index] *= (repetitions/(repetitions - 1.0));
                    stderr_grid[index] = std::sqrt(stderr_grid[index]/repetitions);
                }
            }
            auto chrono_tp_1 = std::chrono::system_clock::now();
            {
                auto now = std::chrono::system_clock::to_time_t(chrono_tp_1);
                char buffer[26];
#if _MSC_VER
                struct tm local_now;
                localtime_s(&local_now, &now);
                strftime(buffer, sizeof(buffer), "%c", &local_now);
#else
                auto local_now = localtime(&now);
                strftime(buffer, sizeof(buffer), "%c", local_now);
#endif
                std::cout << buffer << std::endl;
                std::cout << std::endl;
                auto chono_dt = std::chrono::duration_cast<std::chrono::microseconds>(chrono_tp_1 - chrono_tp_0);
                std::cout << "Total duration: " << chono_dt.count() << std::endl;
                std::cout << std::endl;
            }
            std::cout << "array([";
            for (int64 i = 0; i < configurations; ++i) {
                if (i > 0) {
                    std::cout << ",";
                }
                std::cout << "0.";
            }
            std::cout << "])" << std::endl;
            std::cout << "array([";
            for (int64 i = 0; i < configurations; ++i) {
                if (i > 0) {
                    std::cout << ",";
                }
                std::cout << noise_grid[i];
            }
            std::cout << "])" << std::endl;
            std::cout << "array([";
            for (int64 i = 0; i < configurations; ++i) {
                if (i > 0) {
                    std::cout << ",";
                }
                std::cout << correct_grid[i];
            }
            std::cout << "])" << std::endl;
            std::cout << "array([";
            for (int64 i = 0; i < configurations; ++i) {
                if (i > 0) {
                    std::cout << ",";
                }
                std::cout << stderr_grid[i];
            }
            std::cout << "])" << std::endl;
        }
        else {
            std::cout << "Error: unrecognized task (" << task << ")" << std::endl;
        }
    }
    catch (const std::range_error& e) {
        std::cerr << e.what() << std::endl;
    }
}


void prediction_system()
{
    prediction_systems("detail", 0, 0, 1);
}


}  // namespace

#endif
