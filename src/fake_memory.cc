#include "fake_memory.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "common.h"

FakeMemory::FakeMemory(const std::string& name, uint64_t size, latency_t read_latency,
                       latency_t write_latency)
    : name_(name), size_(size) {
    read_latency_ = read_latency;
    write_latency_ = write_latency;
    reset();
}

std::string FakeMemory::get_name() { return name_; }

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

    DataStorageTransaction dst = {WRITE, address, write_latency_, 0, data};

    DEBUG_PRINT("> %s w @ 0x%016llx : d=%s - write\n", name_.c_str(), address,
                data.to_string().c_str());

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

    Data data = Data(num_bytes);

    for (int i = 0; i < num_bytes; i++) {
        data[i] = data_[address + i];
    }

    DataStorageTransaction dst = {READ, address, read_latency_, 0, data};

    DEBUG_PRINT("> %s r @ 0x%016llx : d=%s - read\n", name_.c_str(), address,
                data.to_string().c_str());

    return dst;
}

/**
 * @brief read memory from hex file
 * @param memory_file_path the path to the memory file
 * @param start_address the start address to read to
 * @param end_address the end address to read to (if the end_address is not set or is 0
 * it wont be considered)
 */
void FakeMemory::read_hex_memory_file(const std::string& memory_file_path,
                                      address_t start_address, address_t end_address) {
    // check if file exists
    std::filesystem::path p(memory_file_path);
    if (!std::filesystem::exists(p)) {
        std::string err_msg =
            std::string("file ") + memory_file_path + std::string(" does not exist");
        THROW_RUNTIME_ERROR(err_msg);
    }

    // check if start_address is in range
    if (start_address > size_) {
        std::string err_msg =
            std::string("start address ") + int_to_hex<uint64_t>(start_address) +
            std::string(" is out of range for size ") + int_to_hex<uint64_t>(size_);
        THROW_OUT_OF_RANGE(err_msg);
    }

    std::ifstream memory_file;

    memory_file.open(memory_file_path, std::ios::in);

    if (memory_file.good()) {
        char c;
        char buffer[1024];
        uint32_t buffer_index = 0;
        uint64_t memory_address = start_address;

        while (memory_file.get(c)) {
            // read into buffer until newline is found
            if (c != ' ' && c != '\n') {
                buffer[buffer_index] = c;
                buffer_index++;
            }
            // if newline is found, convert buffer to data and write to memory
            else if (c == '\n') {
                buffer[buffer_index] = '\0';

                size_t num_bytes;

                // if buffer size is odd add one to make it even (because each byte are
                // 2 chars)
                if (buffer_index % 2 != 0) {
                    num_bytes = buffer_index / 2 + 1;
                } else {
                    num_bytes = buffer_index / 2;
                }

                Data data(num_bytes);
                uint32_t data_byte_index = 0;

                char byte_chars[3];
                byte_chars[2] = '\0';

                // read buffer from right to left
                for (int i = buffer_index - 1; i >= 0; i -= 2) {
                    if (i - 1 == -1) {
                        byte_chars[1] = buffer[i];
                        byte_chars[0] = '0';
                    } else {
                        byte_chars[1] = buffer[i];
                        byte_chars[0] = buffer[i - 1];
                    }
                    // convert byte_chars to integer
                    uint32_t byte;
                    std::stringstream ss;
                    ss << byte_chars;
                    ss >> std::hex >> byte;
                    data[data_byte_index] = byte;
                    data_byte_index++;
                }
                buffer_index = 0;

                // write data to memory
                write(memory_address, data);
                memory_address += data.size();

                if (end_address != 0 && memory_address >= end_address) {
                    break;
                }
            }
        }
        memory_file.close();
    }
}

/**
 * @brief write hex file from memory
 * @param memory_file_path to write to
 * @param start_address the start address read from
 * @param end_address the end address to read from
 */
void FakeMemory::write_hex_memory_file(const std::string& memory_file_path,
                                       address_t start_address, address_t end_address,
                                       uint8_t bytes_per_line) {
    // check if start_address is in range
    if (start_address > size_) {
        std::string err_msg =
            std::string("start address ") + int_to_hex<uint64_t>(start_address) +
            std::string(" is out of range for size ") + int_to_hex<uint64_t>(size_);
        THROW_OUT_OF_RANGE(err_msg);
    }

    if (end_address == 0) {
        end_address = size_ - 1;
    }

    std::ofstream memory_file;

    memory_file.open(memory_file_path, std::ios::out);

    for (uint64_t i = start_address; i <= end_address; i++) {
        memory_file << std::hex << std::setfill('0') << std::setw(2) << (int)data_[i];
        if ((i + 1) % bytes_per_line == 0) {
            memory_file << std::endl;
        }
    }
    memory_file.close();
}

/**
 * @brief read memory from bin file
 * @param memory_file_path the path to the memory file
 * @param start_address the start address to read to
 * @param end_address the end address to read to
 */
void FakeMemory::read_bin_memory_file(const std::string& memory_file_path,
                                      address_t start_address, address_t end_address) {
    if (end_address == 0) {
        end_address = size_ - 1;
    }

    // check if file exists
    std::filesystem::path p(memory_file_path);
    if (!std::filesystem::exists(p)) {
        std::string err_msg =
            std::string("file ") + memory_file_path + std::string(" does not exist");
        THROW_RUNTIME_ERROR(err_msg);
    }

    // check if start_address is in range
    if (start_address > size_) {
        std::string err_msg =
            std::string("start address ") + int_to_hex<uint64_t>(start_address) +
            std::string(" is out of range for size ") + int_to_hex<uint64_t>(size_);
        THROW_OUT_OF_RANGE(err_msg);
    }

    std::ifstream memory_file;

    memory_file.open(memory_file_path, std::ios::binary);

    uint64_t memory_address = start_address;

    if (memory_file.good()) {
        char byte;
        while (!memory_file.eof() && memory_address <= end_address) {
            memory_file.get(byte);
            Data data(1);
            data.set<uint8_t>(byte);
            write(memory_address, data);
            memory_address++;
        }
    }

    memory_file.close();
}

/**
 * @brief write bin file from memory
 * @param memory_file_path to write to
 * @param start_address the start address read from
 * @param end_address the end address to read from
 */
void FakeMemory::write_bin_memory_file(const std::string& memory_file_path,
                                       uint64_t start_address, uint64_t end_address) {
    if (end_address == 0) {
        end_address = size_ - 1;
    }

    std::ofstream memory_file;

    memory_file.open(memory_file_path, std::ios::binary);

    for (uint64_t i = start_address; i <= end_address; i++) {
        memory_file << data_[i];
    }

    memory_file.close();
}

/**
 * @brief set memory address to value. CAUTION: this method is intended for
 * debuggin purposes only and should not be used in a simulation
 * @param address the address to set
 * @param value the value to set
 */
void FakeMemory::set(uint64_t address, uint8_t value) { data_[address] = value; }

/**
 * @brief get memory address value. CAUTION: this method is intended for
 * debuggin purposes only and should not be used in a simulation
 * @param address the address to get
 * @return the value at the address
 */
uint8_t FakeMemory::get(uint64_t address) { return data_[address]; }

/**
 * @brief reset whole memory
 */
void FakeMemory::reset() { data_ = std::vector<uint8_t>(size_); }
