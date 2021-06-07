#include "question.h"

#include <iostream>

#include "Solution.h"

void question::question5([[maybe_unused]] Config const& config)
{
    std::cout << " ------------ Opérateurs de voisinage ------------ " << std::endl << std::endl;

    Solution sol(10);

    sol.add_bin();
    sol.insert(0, 5);
    sol.add_bin();
    sol.insert(1, 3);
    sol.insert(1, 4);
    sol.add_bin();
    sol.insert(2, 7);

    std::cout << sol << std::endl;

    std::cout << "Déplacer un item d'un bin à l'autre" << std::endl;

    sol.move_item(1,0,3);
    std::cout << sol << std::endl;

    std::cout << "Échanger deux items de deux bins différents" << std::endl;

    sol.swap_items(0,1,5,4);
    std::cout << sol << std::endl;
}