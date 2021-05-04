#include "experiments/experiments.h"

int main(int argc, char *argv[]) {
    int size = std::stoi(argv[1]);
    std::string instance_type = argv[2];
    int num_of_queries = std::stod(argv[3]);
    std::string data_structure = argv[4];

    experiments::run_synthetic(size, instance_type, num_of_queries, data_structure);
}