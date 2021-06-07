#include "Solution.h"

#include <algorithm>

Solution::Solution(std::uint32_t const& bin_size) :
    m_bin_size(bin_size)
{}

Solution::~Solution() {}

Solution::Solution(Solution const& autre) :
    m_bin_size(autre.m_bin_size)
{
    for(Bin const& autre_bin : autre.m_solution)
    {
        Bin& bin = m_solution.emplace_back();
        bin.total = autre_bin.total;

        for(std::uint32_t it : autre_bin.items)
        {
            bin.items.push_back(it);
        }
    }
}

Solution& Solution::operator=(Solution const& autre)
{
    m_bin_size = autre.m_bin_size;
    m_solution.clear();

    for(Bin const& autre_bin : autre.m_solution)
    {
        Bin& bin = m_solution.emplace_back();
        bin.total = autre_bin.total;

        for(std::uint32_t it : autre_bin.items)
        {
            bin.items.push_back(it);
        }
    }
    return *this;
}

void Solution::add_bin()
{
    m_solution.emplace_back();
}

bool Solution::contains(std::size_t const& bin, std::uint32_t const& item) const
{
    try
    {
        std::vector<std::uint32_t> const& tab = m_solution.at(bin).items;
        return tab.end() != std::find(tab.begin(), tab.end(), item);
    }
    catch(std::out_of_range const& e)
    {
        return false;
    }
}

bool Solution::remove(std::size_t const& bin, std::uint32_t const& item)
{
    try
    {
        std::vector<std::uint32_t> const& tab = m_solution.at(bin).items;
        auto it = std::find(tab.begin(), tab.end(), item);

        if(it == tab.end())
        {
            return false;
        }

        m_solution[bin].total -= item;
        m_solution[bin].items.erase(it);

        return true;
    }
    catch(std::out_of_range const& e)
    {
        return false;
    }
}

bool Solution::can_insert(std::size_t const& bin, std::uint32_t const& item) const
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

bool Solution::insert(std::size_t const& bin, std::uint32_t const& item)
{
    if(can_insert(bin, item))
    {
        m_solution[bin].total += item;
        m_solution[bin].items.push_back(item);
        return true;
    }
    return false;
}

void Solution::remove_empty_bins()
{
    std::erase_if(m_solution, [](Bin const& bin){ return bin.total == 0; });
}

bool Solution::move_item(std::size_t const& bin_from, std::size_t const& bin_to, std::uint32_t const& item)
{
    if(!remove(bin_from, item))
    {
        return false;
    }

    if(!insert(bin_to, item))
    {
        insert(bin_from, item);
        return false;
    }

    return true;
}

bool Solution::swap_items(std::size_t const& bin_a, std::size_t const& bin_b, std::uint32_t const& item_a, std::uint32_t const& item_b)
{
    if(!remove(bin_a, item_a))
    {
        return false;
    }

    if(!remove(bin_b, item_b))
    {
        insert(bin_a, item_a);
        return false;
    }

    if(!insert(bin_a, item_b))
    {
        insert(bin_a, item_a);
        insert(bin_b, item_b);
        return false;   
    }

    if(!insert(bin_b, item_a))
    {
        remove(bin_a, item_b);
        insert(bin_a, item_a);
        insert(bin_b, item_b);
        return false;   
    }

    return true;
}

std::ostream& operator<<(std::ostream& out, Solution const& obj)
{
    out << "bin size : " << obj.get_bin_size() << "\n";
    out << "nb de bin : " << obj.get_nb_bin() << "\n";
    out << "[\n";

    for(Solution::Bin const& bin : obj.m_solution)
    {
        out << "  [";
        for(std::uint32_t const& it : bin.items)
        {
            out << it << ", ";
        }
        out << "] total : " << bin.total << "\n";
    }
    return out << "]\n";
}
