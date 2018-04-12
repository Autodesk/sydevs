// This is the multiscale implementation of the example from the following publication:
// Goldstein, Khan, Dalle, and Wainer (2017). Multiscale Representation of Simulated Time
//
// https://www.autodeskresearch.com/publications/multiscale-representation-simulated-time

#include <examples/research/prediction/prediction.h>

int main(int argc, const char* argv[])
{
    if (argc == 1) {
        sydevs_examples::prediction_system();
    }
    else if (argc == 5) {
        std::string task = argv[1];
        sydevs::int64 upper_scale = std::stoi(argv[2]);
        sydevs::int64 lower_scale = std::stoi(argv[3]);
        sydevs::int64 repetitions = std::stoi(argv[4]);
        sydevs_examples::prediction_systems(task, upper_scale, lower_scale, repetitions);
    }
    else {
        std::cout << "Command-line options: task upper_scale lower_scale repetitions" << std::endl;
    }
    return 0;
}
