#ifndef FNES_FILEDATA
#define FNES_FILEDATA

#include <cstdint>
#include <fstream>
#include <memory>
#include <algorithm>
#include <filesystem>

#include "Solution.h"

class FileData
{
public:
    FileData();
    FileData(std::uint32_t const& _bin_size, std::uint32_t const& _item_num);
    virtual ~FileData();

    void load_from_stream(std::istream & in);
    void load_from_path(std::filesystem::path const& path);
    
    // question 1
    std::uint32_t min_number_of_bin() const;
    // question 2
    Solution first_fit_decreasing() const;
    // question 3
    Solution solve_linear_problem() const;
    // question 4 a
    Solution solve_simple() const;
    // question 4 b
    Solution first_fit_random() const;

    inline std::uint32_t const& get_bin_size() const { return m_bin_size; }
    inline std::uint32_t const& get_item_num() const { return m_item_num; }
    inline std::unique_ptr<std::uint32_t[]> const& get_items() const { return m_items; }

private:
    std::uint32_t m_bin_size;
    std::uint32_t m_item_num;
    
    std::unique_ptr<std::uint32_t[]> m_items;
};

#endif // FNES_FILEDATA
