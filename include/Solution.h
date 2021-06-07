#ifndef FNES_SOLUTION
#define FNES_SOLUTION

#include <cstdint>
#include <vector>
#include <ostream>

class Solution
{
public:
    struct Bin
    {
        std::uint32_t total = 0;
        std::vector<std::uint32_t> items;
    };

public:
    Solution(std::uint32_t const& bin_size = 0);
    virtual ~Solution();
    Solution(Solution const& autre);
    Solution& operator=(Solution const& autre);

    inline std::size_t get_nb_bin() const { return m_solution.size(); }

    inline std::uint32_t get_bin_size() const { return m_bin_size; }
    inline void set_bin_size(std::uint32_t const& bin_size) { m_bin_size = bin_size; }
    
    void add_bin();

    bool contains(std::size_t const& bin, std::uint32_t const& item) const;
    bool remove(std::size_t const& bin, std::uint32_t const& item);
    
    bool can_insert(std::size_t const& bin, std::uint32_t const& item) const;
    bool insert(std::size_t const& bin, std::uint32_t const& item);

    void remove_empty_bins();

    inline std::vector<std::uint32_t> const& list_items_in_bin(std::size_t const& bin) const { return m_solution.at(bin).items; }

    bool move_item(std::size_t const& bin_from, std::size_t const& bin_to, std::uint32_t const& item);
    bool swap_items(std::size_t const& bin_a, std::size_t const& bin_b, std::uint32_t const& item_a, std::uint32_t const& item_b);

    friend std::ostream& operator<<(std::ostream& out, Solution const& obj);

private:
    std::vector<Bin> m_solution;

    std::uint32_t m_bin_size;
};

#endif // !FNES_SOLUTION