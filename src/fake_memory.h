#ifndef FAKE_MEMORY_H
#define FAKE_MEMORY_H

#include <string>
#include <vector>

#include "memory_interface.h"

namespace kachesim {
class FakeMemory : public MemoryInterface {
public:
    FakeMemory(const std::string& name, uint64_t size, latency_t read_latency,
               latency_t write_latency);

    std::string get_name();
    size_t size();

    DataStorageTransaction write(address_t address, Data& data);
    DataStorageTransaction read(address_t address, size_t num_bytes);

    void read_hex_memory_file(const std::string& memory_file_path,
                              address_t start_address = 0, address_t end_address = 0);
    void write_hex_memory_file(const std::string& memory_file_path,
                               address_t start_address = 0, address_t end_address = 0,
                               uint8_t bytes_per_line = 4);

    void read_bin_memory_file(const std::string& memory_file_path,
                              address_t start_address = 0, address_t end_address = 0);
    void write_bin_memory_file(const std::string& memory_file_path,
                               address_t start_address = 0, address_t end_address = 0);

    void set(address_t address, uint8_t);
    uint8_t get(address_t address);

    void reset();

private:
    std::string name_;
    size_t size_;
    std::vector<uint8_t> data_;
};
}  // namespace kachesim

#endif
