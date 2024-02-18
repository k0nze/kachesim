#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include <cstdint>
#include <vector>

class DataStorage {
public:
    DataStorage() {}
    virtual ~DataStorage() {}

    virtual void write(uint64_t address, const std::vector<uint8_t>& data) = 0;
    virtual std::vector<uint8_t> read(uint64_t address, uint32_t num_bytes) = 0;

    virtual void write_byte(uint64_t address, uint8_t byte) = 0;
    virtual void write_half_word(uint64_t address, uint16_t hald_word) = 0;
    virtual void write_word(uint64_t address, uint32_t word) = 0;
    virtual void write_double_word(uint64_t address, uint64_t double_word) = 0;

    virtual uint8_t read_byte(uint64_t address) = 0;
    virtual uint16_t read_half_word(uint64_t address) = 0;
    virtual uint32_t read_word(uint64_t address) = 0;
    virtual uint64_t read_double_word(uint64_t address) = 0;

    virtual void reset() = 0;
};

#endif
