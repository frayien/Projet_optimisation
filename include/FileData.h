#ifndef FNES_FILEDATA
#define FNES_FILEDATA

#include <cstdint>
#include <istream>

class FileData
{
public:
    const uint32_t bin_size;
    const uint32_t item_num;
    uint32_t *items;

    FileData(uint32_t const& _bin_size, uint32_t const& _item_num);
    virtual ~FileData();

    static FileData from_stream(std::istream & in);

    uint32_t min_nb_bin() const;
};

#endif // FNES_FILEDATA
