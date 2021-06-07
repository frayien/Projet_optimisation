#include "algo.h"
#include <cmath>
#include <iostream>
#include <list>
#include <algorithm>
#include <compare>
#include <limits>

namespace algo
{
    std::size_t recuit(Solution & solution, std::size_t kmax, std::size_t bin_n_inf, double temperature_initiale, double lambda, energy_function_t const& energy, neighbor_function_t const& voisin)
    {
        std::random_device random_device;
        std::mt19937 generateur(random_device());
        std::uniform_real_distribution<double> distribution(0,1);

        Solution solution_courante = solution;
        energy_t energy_courante = energy(solution_courante);
        energy_t energy_min = energy_courante;

        double temperature = temperature_initiale;

        std::size_t k = 0;
        while (k < kmax && solution_courante.get_nb_bin() > bin_n_inf)
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

        return k;
    }

    struct Direction
    {
        std::size_t bin_from;
        std::size_t bin_to;
        std::uint32_t item_from;
        std::uint32_t item_to = 0;

        auto operator<=>(const Direction&) const = default;
    };

    struct tabu_find_voisin_return_t
    {
        Solution solution;
        energy_t energie;
        Direction direction;
    };

    tabu_find_voisin_return_t tabu_find_voisin(Solution const& solution_n, std::list<Direction> & tabu_list, energy_function_t const& energy) 
    {
        Solution solution_n1;
        Direction direction_n1;
        energy_t energie_n1 = std::numeric_limits<energy_t>::max();

        // explorer tous les voisins pour trouver le meilleur

        for(std::size_t bin_from = 0; bin_from<solution_n.get_nb_bin(); ++bin_from)
        {
            for(std::uint32_t item_from : solution_n.list_items_in_bin(bin_from))
            {
                for(std::size_t bin_to = 0; bin_to<solution_n.get_nb_bin(); ++bin_to)
                {
                    if(bin_from == bin_to) continue;

                    Direction direction;
                    direction.bin_from = bin_from;
                    direction.bin_to = bin_to;
                    direction.item_from = item_from;

                    // mouvement d'insertion simple
                    
                    // si le mouvement n'est pas tabou, on l'essaye
                    if(tabu_list.end() == std::find(tabu_list.begin(), tabu_list.end(), direction))
                    {           
                        Solution solution_candidate = solution_n;

                        if(solution_candidate.move_item(bin_from, bin_to, item_from))
                        {
                            solution_candidate.remove_empty_bins();
                            energy_t energie_candidate = energy(solution_candidate);

                            if(energie_candidate < energie_n1)
                            {
                                energie_n1 = energie_candidate;
                                solution_n1 = solution_candidate;
                                direction_n1 = direction;
                            }
                        }
                    }

                    //  mouvements d'échange
                    for(std::uint32_t item_to : solution_n.list_items_in_bin(bin_to))
                    {
                        if(item_from == item_to) continue;

                        direction.item_to = item_to;

                        // si non tabou
                        if(tabu_list.end() == std::find(tabu_list.begin(), tabu_list.end(), direction))
                        {
                            Solution solution_candidate = solution_n;

                            if(solution_candidate.swap_items(bin_from, bin_to, item_from, item_to))
                            {
                                energy_t energie_candidate = energy(solution_candidate);
                                if(energie_candidate < energie_n1)
                                {
                                    energie_n1 = energie_candidate;
                                    solution_n1 = solution_candidate;
                                    direction_n1 = direction;
                                }
                            }
                        }
                    }
                }
            }
        }

        return { .solution = solution_n1, .energie = energie_n1, .direction = direction_n1 };
    }

    void tabou(Solution & solution, std::size_t kmax, std::size_t tabu_length, energy_function_t const& energy)
    {
        std::list<Direction> tabu_list;

        Solution solution_courante = solution;
        energy_t energie_courante = energy(solution_courante);
        energy_t energie_min = energie_courante;

        for(std::size_t k = 0; k<kmax; ++k)
        {
            auto search_result = tabu_find_voisin(solution_courante, tabu_list, energy);

            if(search_result.energie > energie_courante)
            {
                Direction direction_inverse;
                direction_inverse.bin_from = search_result.direction.bin_to;
                direction_inverse.bin_to = search_result.direction.bin_from;
                direction_inverse.item_from = search_result.direction.item_to;
                direction_inverse.item_to = search_result.direction.item_from;

                tabu_list.push_back(direction_inverse);
                if(tabu_list.size() > tabu_length)
                {
                    tabu_list.pop_front();
                }
            }

            if(search_result.energie < energie_min)
            {
                solution = search_result.solution;
                energie_min = search_result.energie;
            }

            solution_courante = search_result.solution;
            energie_courante = search_result.energie;
        }
    }
}