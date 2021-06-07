#include "question.h"

#include <iostream>
#include <filesystem>

#include "FileData.h"
#include "Solution.h"

void question::question2(Config const& config)
{ 
    std::cout << " ------------ First Fit Decreasing ------------ " << std::endl << std::endl;

    std::filesystem::directory_iterator dir_it(config.in_dir);

    for(auto const& file : dir_it)
    {
        FileData fd;
        fd.load_from_path(file.path());
        std::cout << file.path().filename() << " " << fd.get_bin_size() << " " << fd.get_item_num() << std::endl;


        // résolution avec first fit decreasing

        Solution sol = fd.first_fit_decreasing();
        if(config.verbose)
        {
            std::cout << sol << std::endl;
        }
        else
        {
            std::cout << "nombre de bin : " << sol.get_nb_bin() << std::endl;
        }
    }
}