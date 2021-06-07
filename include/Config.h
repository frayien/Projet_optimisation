#ifndef FNES_CONFIG
#define FNES_CONFIG

#include <string>
#include <cstddef>

struct Config
{
    std::string in_dir = "resources/";
    std::string in_file = "resources/binpack1d_00.txt";
    int question = 1;
    bool verbose = false;

    std::size_t max_step_n = 1000;
    double temperature_initiale = 0.1;
    double lambda = 0.99;

    void load_args(int argc, char *args[]);
};

#endif // FNES_CONFIG
