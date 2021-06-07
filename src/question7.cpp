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
}

void question::question7(Config const& config)
{
    std::cout << " ------------ Tabu Search ------------ " << std::endl << std::endl;

    FileData fd;
    fd.load_from_path(std::filesystem::path(config.in_file));
    std::cout << config.in_file << " " << fd.get_bin_size() << " " << fd.get_item_num() << std::endl;

    // init
    algo::energy_function_t energie_fonction;

    if(config.objectif == 1) // objectif = nombre de bin à minimiser
    {
        energie_fonction = Solution::get_nb_bin;
    }
    else // else if 2 (ou autre, failsafe) : objectif = score de remplissage
    {
        energie_fonction = ::energie;
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
    
    std::cout << "Tabou : N=" << config.max_step_n << " buffer=" << config.buffer << std::endl;
    
    // résolution

    algo::tabou(sol, config.max_step_n, config.buffer, energie_fonction);
    
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
    std::cout << "durée : " << duration.count() << " microsecondes" << std::endl;
}