#include "fake_memory.h"

#include <stdexcept>

#include "common.h"

FakeMemory::FakeMemory(uint64_t size) : size_(size) { reset(); }

/**
 * @brief write data to memory
 * @param address the address to write to
 * @param data the data to write
 */
void FakeMemory::write(uint64_t address, const std::vector<uint8_t>& data) {
    // check if address is in range
    if (address + data.size() > size_) {
        std::string err_msg =
            std::string("write address ") + int_to_hex<uint64_t>(address) +
            std::string(" + ") + int_to_hex<uint64_t>(data.size()) +
            std::string(" is out of range for size ") + int_to_hex<uint64_t>(size_);
        THROW_OUT_OF_RANGE(err_msg);
    }

    for (int i = 0; i < data.size(); i++) {
        data_[address + i] = data[i];
    }
}

/**
 * @brief read data from memory
 * @param address the address to read from
 * @param num_bytes the number of bytes to read
 * @return bytes read from memory
 */
std::vector<uint8_t> FakeMemory::read(uint64_t address, uint32_t num_bytes) {
    // check if address is in range
    if (address + num_bytes > size_) {
        std::string err_msg =
            std::string("read address ") + int_to_hex<uint64_t>(address) +
            std::string(" + ") + int_to_hex<uint64_t>(num_bytes) +
            std::string(" is out of range for size ") + int_to_hex<uint64_t>(size_);
        THROW_OUT_OF_RANGE(err_msg);
    }

    std::vector<uint8_t> bytes;

    for (int i = 0; i < num_bytes; i++) {
        bytes.push_back(data_[address + i]);
    }

    return bytes;
}

/**
 * @brief reset whole memory
 */
void FakeMemory::reset() { data_ = std::vector<uint8_t>(size_); }
