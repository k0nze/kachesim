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

    void write_byte(uint64_t address, uint8_t byte);
    void write_half_word(uint64_t address, uint16_t half_word);
    void write_word(uint64_t address, uint32_t word);
    void write_double_word(uint64_t address, uint64_t double_word);

    uint8_t read_byte(uint64_t address);
    uint16_t read_half_word(uint64_t address);
    uint32_t read_word(uint64_t address);
    uint64_t read_double_word(uint64_t address);

    void reset();

private:
    uint64_t size_;
    std::vector<uint8_t> data_;
};

#endif
