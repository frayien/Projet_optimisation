#include <iostream>
#include <string>
#include <filesystem>
#include <chrono>

#include "FileData.h"

namespace
{
    struct
    {
        std::string in_dir = "resources/";
        std::string in_file = "resources/binpack1d_01.txt";
        unsigned int question = 1;
        bool verbose = false;
    } config;

    void load_args(int argc, char *args[])
    {
        for(int i = 1; i<argc; ++i)
        {
            std::string msg(args[i]);
            if(msg == "question")
            {
                i++;
                if(i < argc)
                {
                    config.question = std::stoi(args[i]);
                }
                continue;
            }

            if(msg == "dir")
            {
                i++;
                if(i < argc)
                {
                    config.in_dir = std::string(args[i]);
                }
                continue;
            }

            if(msg == "file")
            {
                i++;
                if(i < argc)
                {
                    config.in_file = std::string(args[i]);
                }
                continue;
            }

            if(msg == "verbose")
            {
                config.verbose = true;
                continue;
            }
        }
    }

    void question1()
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

    void question2()
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

    void question3()
    {
        std::wcout << L" ------------ Résolution linéaire ------------ " << std::endl << std::endl;

        FileData fd;
        fd.load_from_path(std::filesystem::path(config.in_file));
        std::cout << config.in_file << " " << fd.get_bin_size() << " " << fd.get_item_num() << std::endl;

        // résolution avec le solveur linéaire

        std::chrono::time_point start = std::chrono::high_resolution_clock::now();

        Solution sol = fd.solve_linear_problem();

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

        std::wcout << L"durée : " << duration.count() << " microsecondes" << std::endl;
    }
}

int main(int argc, char *args[])
{
    load_args(argc,args);

    switch(config.question)
    {
    case 1:
        question1();
        break;
    case 2:
        question2();
        break;
    case 3:
        question3();
        break;
    default:
        break;
    }

    return 0;
}