#include "FileData.h"

FileData::FileData(uint32_t const& _bin_size, uint32_t const& _item_num) : 
    bin_size(_bin_size),
    item_num(_item_num)
{
    items = new uint32_t[_item_num];
}

FileData::~FileData()
{
    delete[] items;
}

FileData FileData::from_stream(std::istream & in)
{
    uint32_t bin_size, item_num;
    in >> bin_size >> item_num;

    FileData data(bin_size, item_num);

    for(uint32_t i = 0; i<item_num; ++i)
    {
        in >> data.items[i];
    }

    return data;
}

uint32_t FileData::min_nb_bin() const
{
    uint32_t sum = 0;
    for(uint32_t i = 0; i<item_num; ++i)
    {
        sum += items[i];
    }
    return sum / bin_size + (sum%bin_size ? 1 : 0);
}