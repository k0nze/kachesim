#include "fake_memory.h"

#include <memory>
#include <stdexcept>

#include "common.h"

FakeMemory::FakeMemory(uint64_t size) : size_(size) { reset(); }

size_t FakeMemory::size() { return size_; }

/**
 * @brief write data to memory
 * @param address the address to write to
 * @param data the data to write
 */
DataStorageTransaction FakeMemory::write(address_t address, Data& data) {
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

    DataStorageTransaction dst = {WRITE, address, 0, 0,
                                  std::unique_ptr<Data>(new Data(data))};
    return dst;
}

/**
 * @brief read data from memory
 * @param address the address to read from
 * @param num_bytes the number of bytes to read
 * @return bytes read from memory
 */
DataStorageTransaction FakeMemory::read(address_t address, size_t num_bytes) {
    // check if address is in range
    if (address + num_bytes > size_) {
        std::string err_msg =
            std::string("read address ") + int_to_hex<uint64_t>(address) +
            std::string(" + ") + int_to_hex<uint64_t>(num_bytes) +
            std::string(" is out of range for size ") + int_to_hex<uint64_t>(size_);
        THROW_OUT_OF_RANGE(err_msg);
    }

    auto data = std::unique_ptr<Data>(new Data(num_bytes));

    for (int i = 0; i < num_bytes; i++) {
        data->operator[](i) = data_[address + i];
    }

    DataStorageTransaction dst = {READ, address, 0, 0, std::move(data)};
    return dst;
}

/**
 * @brief reset whole memory
 */
void FakeMemory::reset() { data_ = std::vector<uint8_t>(size_); }
