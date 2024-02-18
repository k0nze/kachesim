#include "fake_memory.h"

FakeMemory::FakeMemory(uint64_t size) : size_(size) {
    data_ = std::vector<uint8_t>(size_);
}

void FakeMemory::write(uint64_t address, const std::vector<uint8_t>& data) {}

std::vector<uint8_t> FakeMemory::read(uint64_t address, uint32_t num_bytes) {
    std::vector<uint8_t> bytes;
    return bytes;
}
