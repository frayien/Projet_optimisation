#include "Solution.h"

Solution::Solution(uint32_t const& bin_size) :
    m_bin_size(bin_size)
{}

Solution::~Solution() {}

void Solution::add_bin()
{
    m_solution.emplace_back();
}

bool Solution::can_insert(size_t const& bin, uint32_t const& item) const
{
    try
    {
        return m_solution.at(bin).total + item <= m_bin_size;
    }
    catch(std::out_of_range const& e)
    {
        return false;
    }
}

bool Solution::insert(size_t const& bin, uint32_t const& item)
{
    if(can_insert(bin, item))
    {
        m_solution[bin].total += item;
        m_solution[bin].items.push_back(item);
        return true;
    }
    return false;
}

void Solution::print(std::ostream & out) const
{
    out << "bin size : " << get_bin_size() << "\n";
    out << "nb de bin : " << get_nb_bin() << "\n";
    out << "[\n";

    for(Bin const& bin : m_solution)
    {
        out << "  [";
        for(uint32_t const& it : bin.items)
        {
            out << it << ", ";
        }
        out << "] total : " << bin.total << "\n";
    }
    out << "]\n";
}