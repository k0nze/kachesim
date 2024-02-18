#include "fake_memory.h"

#include <stdexcept>

#include "common.h"

FakeMemory::FakeMemory(uint64_t size) : size_(size) {
    data_ = std::vector<uint8_t>(size_);
}

void FakeMemory::write(uint64_t address, const std::vector<uint8_t>& data) {
    // check if address is in range
    if (address + data.size() > size_) {
        std::string err_msg = std::string("Address ") +
                              std::to_string(address + data.size()) +
                              std::string(" is out of range");
        THROW_OUT_OF_RANGE(err_msg);
    }

    for (int i = 0; i < data.size(); i++) {
        data_[address + i] = data[i];
    }
}

std::vector<uint8_t> FakeMemory::read(uint64_t address, uint32_t num_bytes) {
    std::vector<uint8_t> bytes;
    return bytes;
}
