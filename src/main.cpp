#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>


struct
{
    std::string in_file = "resources/binpack1d_00.txt";
} config;

struct FileData
{
    const uint32_t bin_size;
    const uint32_t item_num;
    uint32_t *items;

    FileData(uint32_t const& _bin_size, uint32_t const& _item_num) : 
        bin_size(_bin_size),
        item_num(_item_num)
    {
        items = new uint32_t[_item_num];
    }

    virtual ~FileData()
    {
        delete[] items;
    }
};

FileData read_file(std::istream & in)
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

int main(int argc, char *args[]) 
{
    std::ifstream input(config.in_file);

    FileData fd = read_file(input);

    std::cout << config.in_file << " " << fd.bin_size << " " << fd.item_num << std::endl;
    for(uint32_t i = 0; i<fd.item_num; ++i)
    {
        std::cout << fd.items[i] << std::endl;
    }

    return 0;
}