#ifndef FAKE_MEMORY_H
#define FAKE_MEMORY_H

#include <vector>

#include "data_storage.h"

class FakeMemory : public DataStorage {
public:
    FakeMemory(uint64_t size);

    uint64_t size() const { return size_; }

    void write(uint64_t address, const std::vector<uint8_t>& data);
    std::vector<uint8_t> read(uint64_t address, uint32_t num_bytes);
    void reset();

private:
    uint64_t size_;
    std::vector<uint8_t> data_;
};

#endif
