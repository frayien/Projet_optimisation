#include "FileData.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>

FileData::FileData() : 
    m_bin_size(0),
    m_item_num(0),
    m_items(nullptr)
{}

FileData::FileData(uint32_t const& bin_size, uint32_t const& item_num) : 
    m_bin_size(bin_size),
    m_item_num(item_num)
{
    m_items = std::make_unique<uint32_t[]>(item_num);
}

FileData::~FileData()
{ }

void FileData::load_from_stream(std::istream & in)
{
    in >> m_bin_size >> m_item_num;
    
    m_items = std::make_unique<uint32_t[]>(m_item_num);

    for(uint32_t i = 0; i<m_item_num; ++i)
    {
        in >> m_items[i];
    }
}

uint32_t FileData::min_number_of_bin() const
{
    uint32_t sum = 0;
    for(uint32_t i = 0; i<m_item_num; ++i)
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
    std::unique_ptr<uint32_t[]> items = std::make_unique<uint32_t[]>(m_item_num);
    for(size_t i = 0; i<m_item_num; ++i)
    {
        items[i] = m_items[i];
    }

    // tous les objets dans un ordre croissant
    std::sort(items.get(), items.get() + m_item_num, std::greater<uint32_t>());

    Solution solution(m_bin_size);

    // inserer les objets
    for(size_t i = 0; i<m_item_num; ++i)
    {
        bool inserted = false;
        size_t j = 0;
        while(!inserted && solution.get_nb_bin() > j)
        {
            inserted = solution.insert(j, items[i]);
            j++;
        }
        // si pas de bin dispo on en crée une nouvelle
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
    std::ofstream file;

    file.open("bpp_tmp.mod", std::ios::trunc);

    file << "param m, integer, > 0;"                                      << std::endl;
    file << "set I := 1..m;"                                              << std::endl;
    file << "param w{i in 1..m}, > 0;"                                    << std::endl;
    file << "param c, > 0;"                                               << std::endl;
    file << "param z{i in I, j in 1..m} :="                               << std::endl;
    file << "if i = 1 and j = 1 then 1"                                   << std::endl;
    file << "else if exists{jj in 1..j-1} z[i,jj] then 0"                 << std::endl;
    file << "else if sum{ii in 1..i-1} w[ii] * z[ii,j] + w[i] > c then 0" << std::endl;
    file << "else 1;"                                                     << std::endl;
    file << "check{i in I}: sum{j in 1..m} z[i,j] = 1;"                   << std::endl;
    file << "check{j in 1..m}: sum{i in I} w[i] * z[i,j] <= c;"           << std::endl;
    file << "param n := sum{j in 1..m} if exists{i in I} z[i,j] then 1;"  << std::endl;
    file << "display n;"                                                  << std::endl;
    file << "set J := 1..n;"                                              << std::endl;
    file << "var x{i in I, j in J}, binary;"                              << std::endl;
    file << "var used{j in J}, binary;"                                   << std::endl;
    file << "s.t. one{i in I}: sum{j in J} x[i,j] = 1;"                   << std::endl;
    file << "s.t. lim{j in J}: sum{i in I} w[i] * x[i,j] <= c * used[j];" << std::endl;
    file << "minimize obj: sum{j in J} used[j];"                          << std::endl;
    file << "data;"                                                       << std::endl;

    file << "param m := " << m_item_num << ";"                            << std::endl;

    file << "param w := ";

    for(size_t i = 0; i<m_item_num; ++i)
    {
        file << i+1 << " " << m_items[i];
        if(i < m_item_num-1) file << ",";
    }
    file << ";" << std::endl;

    file << "param c := " << m_bin_size << ";"                            << std::endl;
    file << "end;"                                                        << std::endl;

    file.close();

    std::system("glpsol -m bpp_tmp.mod --output solution.txt");

    Solution solution(m_bin_size);

    std::regex result_regex("x\\[[0-9]+\\,[0-9]+\\]\\s*\\*\\s*1", std::regex_constants::ECMAScript);
    std::map<int,int> bin_map;

    std::ifstream file_sol;
    file_sol.open("solution.txt");

    std::string line;
    while (std::getline(file_sol, line)) 
    {
        if(std::regex_search(line, result_regex))
        {
            int i = 0;
            std::string buf = "";
            while(line[i] != '[') i++;
            i++;
            while(line[i] != ',') { buf += line[i]; i++; }
            int item_id = std::stoi(buf);
            buf.clear();
            i++;
            while(line[i] != ']') { buf += line[i]; i++; }

            int bin_id = std::stoi(buf);

            if(!bin_map.contains(bin_id))
            {
                bin_map[bin_id] = bin_map.size();
                solution.add_bin();
            }

            solution.insert(bin_map[bin_id], m_items[item_id-1]);
        }
    }

    file_sol.close();

    return solution;
}