#include "FileData.h"

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
    std::unique_ptr<uint32_t[]> items = std::make_unique<uint32_t[]>(m_item_num);
    for(size_t i = 0; i<m_item_num; ++i)
    {
        items[i] = m_items[i];
    }

    std::sort(std::begin(items), std::end(items));
    Solution sol(m_bin_size);
}