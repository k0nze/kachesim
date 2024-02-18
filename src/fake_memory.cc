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
 * @brief write a byte to memory
 * @param address the address to write to
 * @param byte the byte to write
 */
void FakeMemory::write_byte(uint64_t address, uint8_t byte) {
    std::vector<uint8_t> data;
    data.resize(1);
    data[0] = byte;

    write(address, data);
}

/**
 * @brief write a half word to memory
 * @param address the address to write to
 * @param half_word the half word to write
 */
void FakeMemory::write_half_word(uint64_t address, uint16_t half_word) {
    uint8_t byte0 = half_word & 0xff;
    uint8_t byte1 = (half_word >> 8) & 0xff;

    std::vector<uint8_t> data;
    data.resize(2);
    data[0] = byte0;
    data[1] = byte1;

    write(address, data);
}

/**
 * @brief write a word to memory
 * @param address the address to write to
 * @param word the word to write
 */
void FakeMemory::write_word(uint64_t address, uint32_t word) {
    uint8_t byte0 = word & 0xff;
    uint8_t byte1 = (word >> 8) & 0xff;
    uint8_t byte2 = (word >> 16) & 0xff;
    uint8_t byte3 = (word >> 24) & 0xff;

    std::vector<uint8_t> data;
    data.resize(4);
    data[0] = byte0;
    data[1] = byte1;
    data[2] = byte2;
    data[3] = byte3;

    write(address, data);
}

/**
 * @brief write a double word to memory
 * @param address the address to write to
 * @param double_word the double word to write
 */
void FakeMemory::write_double_word(uint64_t address, uint64_t double_word) {
    uint8_t byte0 = double_word & 0xff;
    uint8_t byte1 = (double_word >> 8) & 0xff;
    uint8_t byte2 = (double_word >> 16) & 0xff;
    uint8_t byte3 = (double_word >> 24) & 0xff;
    uint8_t byte4 = (double_word >> 32) & 0xff;
    uint8_t byte5 = (double_word >> 40) & 0xff;
    uint8_t byte6 = (double_word >> 48) & 0xff;
    uint8_t byte7 = (double_word >> 56) & 0xff;

    std::vector<uint8_t> data;
    data.resize(8);
    data[0] = byte0;
    data[1] = byte1;
    data[2] = byte2;
    data[3] = byte3;
    data[4] = byte4;
    data[5] = byte5;
    data[6] = byte6;
    data[7] = byte7;

    write(address, data);
}

/**
 * @brief read a byte from memory
 * @param address the address to read from
 * @return byte read from memory
 */
uint8_t FakeMemory::read_byte(uint64_t address) { return read(address, 1)[0]; }

/**
 * @brief read a half word from memory
 * @param address the address to read from
 * @return half_word read from memory
 */
uint16_t FakeMemory::read_half_word(uint64_t address) {
    std::vector<uint8_t> data = read(address, 2);
    uint16_t half_word = (data[1] << 8) | data[0];
    return half_word;
}

/**
 * @brief read a word from memory
 * @param address the address to read from
 * @return word read from memory
 */
uint32_t FakeMemory::read_word(uint64_t address) {
    std::vector<uint8_t> data = read(address, 4);
    uint32_t word = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
    return word;
}

/**
 * @brief read a double word from memory
 * @param address the address to read from
 * @return double_word read from memory
 */
uint64_t FakeMemory::read_double_word(uint64_t address) {
    std::vector<uint8_t> data = read(address, 4);
    uint64_t double_word = (data[7] << 56) | (data[6] << 48) | (data[5] << 40) |
                           (data[4] << 32) | (data[3] << 24) | (data[2] << 16) |
                           (data[1] << 8) | data[0];
    return double_word;
}

/**
 * @brief reset whole memory
 */
void FakeMemory::reset() { data_ = std::vector<uint8_t>(size_); }
