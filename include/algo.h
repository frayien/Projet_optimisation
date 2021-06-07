#ifndef FNES_ALGO
#define FNES_ALGO

#include <cstdint>
#include <functional>
#include <random>

#include "Solution.h"

namespace algo
{
    typedef std::int32_t energy_t;
    typedef std::function<energy_t (const Solution &)> energy_function_t;
    typedef std::function<Solution (const Solution &, std::mt19937 &)> neighbor_function_t;

    std::size_t recuit(Solution & solution, std::size_t kmax, energy_t einf, double temperature_initiale, double lambda, energy_function_t const& energy, neighbor_function_t const& voisin);
    void tabou(Solution & solution, std::size_t kmax, std::size_t tabu_length, energy_function_t const& energy);
}

#endif // FNES_ALGO
