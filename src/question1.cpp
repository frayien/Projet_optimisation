#include "question.h"

#include <iostream>
#include <filesystem>

#include "FileData.h"

void question::question1(Config const& config)
{
    std::filesystem::directory_iterator dir_it(config.in_dir);

    for(auto const& file : dir_it)
    {
        FileData fd;
        fd.load_from_path(file.path());

        std::cout << file.path().filename() << " " << fd.get_bin_size() << " " << fd.get_item_num() << std::endl;
        std::cout << "nombre de bin min : " << fd.min_number_of_bin() << std::endl;
    }
}