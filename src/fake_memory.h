#ifndef FAKE_MEMORY_H
#define FAKE_MEMORY_H

#include <string>
#include <vector>

#include "memory_interface.h"

class FakeMemory : public MemoryInterface {
public:
    FakeMemory();
    FakeMemory(uint64_t size, latency_t read_latency, latency_t write_latency);

    size_t size();

    DataStorageTransaction write(address_t address, Data& data);
    DataStorageTransaction read(address_t address, size_t num_bytes);

    void read_hex_memory_file(const std::string& memory_file_path,
                              uint64_t start_address = 0, uint64_t end_address = 0);
    void write_hex_memory_file(const std::string& memory_file_path,
                               uint64_t start_address = 0, uint64_t end_address = 0,
                               uint8_t bytes_per_line = 4);

    void read_bin_memory_file(const std::string& memory_file_path,
                              uint64_t start_address = 0, uint64_t end_address = 0);
    void write_bin_memory_file(const std::string& memory_file_path,
                               uint64_t start_address = 0, uint64_t end_address = 0);

    void set(uint64_t address, uint8_t);
    uint8_t get(uint64_t address);

    void reset();

private:
    size_t size_;
    std::vector<uint8_t> data_;
};

#endif
