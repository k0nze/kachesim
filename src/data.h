#ifndef DATA_H
#define DATA_H

#include <vector>

class Data {
public:
    Data(uint32_t size);

    uint32_t size();

private:
    uint32_t size_;
    std::vector<uint8_t> data_;
};

#endif
