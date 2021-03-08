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
        uint32_t total = 0;
        std::vector<uint32_t> items;
    };

public:
    Solution(uint32_t const& bin_size = 0);
    virtual ~Solution();

    inline size_t get_nb_bin() const { return m_solution.size(); }

    inline uint32_t get_bin_size() const { return m_bin_size; }
    inline void set_bin_size(uint32_t const& bin_size) { m_bin_size = bin_size; }
    
    void add_bin();

    bool can_insert(size_t const& bin, uint32_t const& item) const;
    bool insert(size_t const& bin, uint32_t const& item);

    friend std::ostream& operator<<(std::ostream& out, Solution const& obj);

private:
    std::vector<Bin> m_solution;

    uint32_t m_bin_size;
};

#endif // !FNES_SOLUTION