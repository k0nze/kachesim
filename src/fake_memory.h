#ifndef FAKE_MEMORY_H
#define FAKE_MEMORY_H

#include <vector>

#include "memory_interface.h"

class FakeMemory : public MemoryInterface {
public:
    FakeMemory(uint64_t size);

    size_t size();

    DataStorageTransaction write(address_t address, Data& data);
    DataStorageTransaction read(address_t address, size_t num_bytes);

    void reset();

private:
    size_t size_;
    std::vector<uint8_t> data_;
};

#endif
