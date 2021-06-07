#include "question.h"

#include <iostream>
#include <filesystem>
#include <random>
#include <vector>
#include <numeric>

#include "FileData.h"
#include "Solution.h"
#include "algo.h"

namespace
{    
    algo::energy_t score(Solution const& solution)
    {
        algo::energy_t score = 0;
        for(std::size_t i = 0; i<solution.get_nb_bin(); ++i)
        {
            algo::energy_t lscore = 0;
            for(algo::energy_t it : solution.list_items_in_bin(i))
            {
                lscore += it;
            }
            score += lscore*lscore;
        }
        return -score;
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
        std::uint32_t item = bin_from_items[item_id];

        // choix de l'ordre des candidats
        std::vector<std::size_t> bin_to_candidates(voisin.get_nb_bin()-1);
        std::iota(bin_to_candidates.begin(), bin_to_candidates.end(), 0);
        if(bin_from < voisin.get_nb_bin()-1) bin_to_candidates[bin_from] = voisin.get_nb_bin()-1;

        std::shuffle(bin_to_candidates.begin(), bin_to_candidates.end(), generator);

        // essayer de faire le mouvement avec les candidats dans l'ordre
        for(std::size_t bin_to : bin_to_candidates)
        {
            if(voisin.move_item(bin_from, bin_to, item))
            {
                break;
            }
        }

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

    Solution sol = fd.solve_simple();

    std::cout << sol << std::endl;
    std::cout << "score : " << score(sol) << std::endl;
    
    algo::recuit(sol, config.max_step_n, -10'000'000, config.temperature_initiale, config.lambda, ::score, ::voisin);

    std::cout << "Recuit : " << std::endl;

    std::cout << sol << std::endl;
    std::cout << "score : " << score(sol) << std::endl;
}