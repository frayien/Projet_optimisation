#include "algo.h"
#include <cmath>
#include <iostream>

namespace algo
{
    void recuit(Solution & solution, std::size_t kmax, energy_t einf, double temperature_initiale, double lambda, energy_function_t const& energy, neighbor_function_t const& voisin)
    {
        std::random_device random_device;
        std::mt19937 generateur(random_device());
        std::uniform_real_distribution<double> distribution(0,1);

        Solution solution_courante = solution;
        energy_t energy_courante = energy(solution_courante);
        energy_t energy_min = energy_courante;

        double temperature = temperature_initiale;

        std::size_t k = 0;
        while (k < kmax && energy_courante > einf)
        {
            Solution solution_n = voisin(solution_courante, generateur);
            energy_t energy_n = energy(solution_n);
            
            double delta_energy = std::abs(energy_n - energy_courante);
            if(energy_n < energy_courante || distribution(generateur) < std::exp(-delta_energy / temperature ))
            {
                solution_courante = solution_n;
                energy_courante = energy_n;

                if(energy_n < energy_min)
                {
                    solution = solution_n;
                    energy_min = energy_n;
                }
            }
            temperature = temperature*lambda;
            k++;
        }
    }
}