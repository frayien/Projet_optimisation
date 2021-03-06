#include "FileData.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <algorithm>
#include <random>

FileData::FileData() : 
    m_bin_size(0),
    m_item_num(0),
    m_items(nullptr)
{}

FileData::FileData(std::uint32_t const& bin_size, std::uint32_t const& item_num) : 
    m_bin_size(bin_size),
    m_item_num(item_num)
{
    m_items = std::make_unique<std::uint32_t[]>(item_num);
}

FileData::~FileData()
{ }

void FileData::load_from_stream(std::istream & in)
{
    in >> m_bin_size >> m_item_num;
    
    m_items = std::make_unique<std::uint32_t[]>(m_item_num);

    for(std::uint32_t i = 0; i<m_item_num; ++i)
    {
        in >> m_items[i];
    }
}

void FileData::load_from_path(std::filesystem::path const& path)
{
    std::ifstream input(path);
    load_from_stream(input);
    input.close();
}

std::uint32_t FileData::min_number_of_bin() const
{
    std::uint32_t sum = 0;
    for(std::uint32_t i = 0; i<m_item_num; ++i)
    {
        sum += m_items[i];
    }
    return sum / m_bin_size + (sum%m_bin_size ? 1 : 0);
}

/**
 * trier les objets dans un ordre croissant et
 * essayer de les inserer dans chanque bin existant
 * avant d'en créer un nouveau
 */
Solution FileData::first_fit_decreasing() const
{
    // copie des items
    std::unique_ptr<std::uint32_t[]> items = std::make_unique<std::uint32_t[]>(m_item_num);
    for(std::size_t i = 0; i<m_item_num; ++i)
    {
        items[i] = m_items[i];
    }

    // tous les objets dans un ordre croissant
    std::sort(items.get(), items.get() + m_item_num, std::greater<std::uint32_t>());

    Solution solution(m_bin_size);

    // inserer les objets
    for(std::size_t i = 0; i<m_item_num; ++i)
    {
        bool inserted = false;
        std::size_t j = 0;
        while(!inserted && solution.get_nb_bin() > j)
        {
            inserted = solution.insert(j, items[i]);
            j++;
        }
        // si pas de bin dispo on en crée un nouveau
        if(!inserted)
        {
            solution.add_bin();
            solution.insert(solution.get_nb_bin() - 1, items[i]);
        }
    }

    return solution;
}

Solution FileData::solve_linear_problem() const
{
    // On détermine la borne suppérieure du nombre de bin avec FirstFitDecreasing
    int nb_bin_upper_bound = first_fit_decreasing().get_nb_bin();

    // write problem to module file
    std::ofstream file;

    file.open("glpk/bpp.mod", std::ios::trunc);

    file << "param m, integer, > 0;"                                          << std::endl; // m is the number of items
    file << "set I := 1..m;"                                                  << std::endl; // I is the item id set
    file << "param w{i in 1..m}, > 0;"                                        << std::endl; // w is the map of item weights
    file << "param c, > 0;"                                                   << std::endl; // c is the size of the bins 
    file << "param n, integer, > 0;"                                          << std::endl; // n is an upper bound of the optimal solution
    file << "set J := 1..n;"                                                  << std::endl; // J is the bin id set
    file << "var x{i in I, j in J}, binary;"                                  << std::endl; // x[i,j] = 1 if item i is in bin j
    file << "var used{j in J}, binary;"                                       << std::endl; // used[j] = 1 if bin j is used
    file << "s.t. one{i in I}: sum{j in J} x[i,j] = 1;"                       << std::endl; // each item must be in exactly one bin
    file << "s.t. lim{j in J}: sum{i in I} w[i] * x[i,j] <= c * used[j];"     << std::endl; // each bin cannot be filled more than its size
    file << "minimize obj: sum{j in J} used[j];"                              << std::endl; // we want to minimize the number of bin used
    file << "data;"                                                           << std::endl;

    file << "param n := " << nb_bin_upper_bound << ";"                        << std::endl;
    file << "param m := " << m_item_num << ";"                                << std::endl;

    file << "param w := ";

    for(std::size_t i = 0; i<m_item_num; ++i)
    {
        file << i+1 << " " << m_items[i];
        if(i < m_item_num-1) file << ",";
    }
    file << ";" << std::endl;

    file << "param c := " << m_bin_size << ";"                                << std::endl;
    file << "end;"                                                            << std::endl;

    file.close();

    // call glpk to solve the problem
    #if defined(_WIN32)

    std::system(".\\glpk\\glpsol.exe -m glpk\\bpp.mod --output glpk\\solution.txt > glpk\\log.txt");

    #elif defined(__linux__)

    std::system("glpsol -m glpk/bpp.mod --output glpk/solution.txt > glpk/log.txt");

    #else
    #error unsupported OS
    #endif

    // read output to get the solution

    Solution solution(m_bin_size);

    // regex to find relevent lines
    // looking for : x[2,3] * 1
    // meaning that the item 2 is in the bin 3
    std::regex result_regex("x\\[[0-9]+\\,[0-9]+\\]\\s*\\*\\s*1", std::regex_constants::ECMAScript);

    // maping each bin to it's new id (we start with 'number of items' bins and end up with 'minimized' bins, so a mapping is needed)
    std::map<int,int> bin_map;

    std::ifstream file_sol;
    file_sol.open("glpk/solution.txt");

    std::string line;
    while (std::getline(file_sol, line)) 
    {
        if(std::regex_search(line, result_regex))
        {
            int i = 0;
            std::string buf = "";
            while(line[i] != '[') i++;
            i++;

            // read item id
            while(line[i] != ',') { buf += line[i]; i++; }
            int item_id = std::stoi(buf);
            buf.clear();
            i++;

            // read bin id
            while(line[i] != ']') { buf += line[i]; i++; }
            int bin_id = std::stoi(buf);

            // update mapping if needed
            if(!bin_map.contains(bin_id))
            {
                bin_map[bin_id] = bin_map.size();
                solution.add_bin();
            }

            // construct solution
            solution.insert(bin_map[bin_id], m_items[item_id-1]);
        }
    }

    file_sol.close();

    return solution;
}

// question 4 a
Solution FileData::solve_simple() const
{
    Solution solution(m_bin_size);

    for(std::size_t i = 0; i<m_item_num; ++i)
    {
        solution.add_bin();
        solution.insert(solution.get_nb_bin() - 1, m_items[i]);
    }

    return solution;
}

Solution FileData::first_fit_random() const
{
    // copie des items
    std::unique_ptr<std::uint32_t[]> items = std::make_unique<std::uint32_t[]>(m_item_num);
    for(std::size_t i = 0; i<m_item_num; ++i)
    {
        items[i] = m_items[i];
    }

    // tous les objets dans un ordre aléatoire
    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(items.get(), items.get() + m_item_num, gen);

    Solution solution(m_bin_size);

    // inserer les objets
    for(std::size_t i = 0; i<m_item_num; ++i)
    {
        bool inserted = false;
        std::size_t j = 0;
        while(!inserted && solution.get_nb_bin() > j)
        {
            inserted = solution.insert(j, items[i]);
            j++;
        }
        // si pas de bin dispo on en crée un nouveau
        if(!inserted)
        {
            solution.add_bin();
            solution.insert(solution.get_nb_bin() - 1, items[i]);
        }
    }

    return solution;
}