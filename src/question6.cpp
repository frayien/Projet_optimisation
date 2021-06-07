#include "question.h"

#include <iostream>
#include <filesystem>
#include <random>
#include <vector>
#include <numeric>
#include <chrono>

#include "FileData.h"
#include "Solution.h"
#include "algo.h"

namespace
{    
    algo::energy_t energie(Solution const& solution)
    {
        algo::energy_t energie = 0;
        for(std::size_t i = 0; i<solution.get_nb_bin(); ++i)
        {
            algo::energy_t lenergie = 0;
            for(algo::energy_t it : solution.list_items_in_bin(i))
            {
                lenergie += it;
            }
            energie += lenergie*lenergie;
        }
        return -energie;
    } 

    Solution voisin(Solution const& solution, std::mt19937 & generator)
    {
        Solution voisin(solution);
        typedef std::uniform_int_distribution<std::size_t> rnd_uniform;

        // choix du bin de départ
        std::size_t bin_from = rnd_uniform(0, voisin.get_nb_bin()-1)(generator);
        const std::vector<std::uint32_t> & bin_from_items = voisin.list_items_in_bin(bin_from);

        // choix de l'item à deplacer
        std::size_t item_id = rnd_uniform(0, bin_from_items.size()-1)(generator);
        std::uint32_t item_from = bin_from_items[item_id];

        // choix de l'ordre des candidats
        std::vector<std::size_t> bin_to_candidates(voisin.get_nb_bin()-1);
        std::iota(bin_to_candidates.begin(), bin_to_candidates.end(), 0);
        if(bin_from < voisin.get_nb_bin()-1) bin_to_candidates[bin_from] = voisin.get_nb_bin()-1;

        std::shuffle(bin_to_candidates.begin(), bin_to_candidates.end(), generator);

        // essayer de faire le mouvement avec les candidats dans l'ordre aléatoire précédent
        for(std::size_t bin_to : bin_to_candidates)
        {
            // essayer de deplacer l'item d'un bin dans l'autre
            if(voisin.move_item(bin_from, bin_to, item_from))
            {
                break;
            }

            // sinon (bin cible trop plein)
            // chercher un item cible pour faire un échange
            std::vector<std::uint32_t> bin_to_items_copy (voisin.list_items_in_bin(bin_to));
            std::shuffle(bin_to_items_copy.begin(), bin_to_items_copy.end(), generator);

            for(std::uint32_t item_to : bin_to_items_copy)
            {
                if(voisin.swap_items(bin_from, bin_to, item_from, item_to))
                {
                    goto endofloop; // break break
                }
            }
        }
    endofloop:

        voisin.remove_empty_bins();
        return voisin;
    }
}

void question::question6(Config const& config)
{
    std::cout << " ------------ Recuit simulé ------------ " << std::endl << std::endl;

    FileData fd;
    fd.load_from_path(std::filesystem::path(config.in_file));
    std::cout << config.in_file << " " << fd.get_bin_size() << " " << fd.get_item_num() << std::endl;

    // init
    algo::energy_function_t energie_fonction;
    algo::energy_t energie_min;

    if(config.objectif == 1) // objectif = nombre de bin à minimiser
    {
        energie_fonction = Solution::get_nb_bin;
        energie_min = fd.min_number_of_bin();
    }
    else // else if 2 (ou autre, failsafe) : objectif = score de remplissage
    {
        energie_fonction = ::energie;
        energie_min = -static_cast<algo::energy_t>(fd.get_bin_size() * fd.get_bin_size() * fd.min_number_of_bin());
    }

    Solution sol;

    // timer avant l'initialisation de la solution car cette étape fait partie du temps de calcul de la solution
    std::chrono::time_point start = std::chrono::high_resolution_clock::now();

    if(config.initial == 1) // initial = solution simple
    {
        sol = fd.solve_simple();
    }
    else // else if 2 (ou autre, failsafe) : initial = first fit random
    {
        sol = fd.first_fit_random();
    }

    if(config.verbose)
    {
        std::cout << sol << std::endl;
    }
    else
    {
        std::cout << "nombre de bin : " << sol.get_nb_bin() << std::endl;
    }
    std::cout << "énergie : " << energie_fonction(sol) << std::endl;
    
    std::cout << "Recuit : kmax=" << config.max_step_n << " Emin=" << energie_min << " T0=" << config.temperature_initiale << " lambda=" << config.lambda << std::endl;
    
    // résolution

    std::size_t step_nb = algo::recuit(sol, config.max_step_n, energie_min, config.temperature_initiale, config.lambda, energie_fonction, ::voisin);
    
    std::chrono::time_point stop = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    if(config.verbose)
    {
        std::cout << sol << std::endl;
    }
    else
    {
        std::cout << "nombre de bin : " << sol.get_nb_bin() << std::endl;
    }
    std::cout << "énergie : " << energie_fonction(sol) << std::endl;
    std::cout << "nombre de pas avant optimal : " << step_nb << std::endl;
    std::cout << "durée : " << duration.count() << " microsecondes" << std::endl;
}