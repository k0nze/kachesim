#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "fake_memory.h"

int main() {
    auto fm = std::make_unique<FakeMemory>(65536);

    // write a single byte to each address
    for (uint64_t i = 0; i < fm->size(); i++) {
        std::vector<uint8_t> data;
        data.push_back(i);
        fm->write(i, data);
    }

    // read a single byte from each address
    // and check if the data is correct
    for (uint64_t i = 0; i < fm->size(); i++) {
        std::vector<uint8_t> data = fm->read(i, 1);
        assert(data[0] == (i % 256));
    }

    // write a single byte to each address
    for (uint64_t i = 0; i < fm->size(); i++) {
        uint8_t byte = i;
        fm->write_byte(i, byte);
    }

    // read a single byte from each address
    // and check if the data is correct
    for (uint64_t i = 0; i < fm->size(); i++) {
        uint8_t byte = fm->read_byte(i);
        assert(byte == (i % 256));
    }

    // write half word to each 2nd address
    for (uint64_t i = 0; i < fm->size(); i += 2) {
        uint16_t half_word = i / 2;
        fm->write_half_word(i, half_word);
    }

    // read half word from each 2nd address
    // and check if the data is correct
    for (uint64_t i = 0; i < fm->size(); i += 2) {
        uint16_t half_word = fm->read_half_word(i);
        assert(half_word == ((i / 2) % 65536));
    }

    // write word to each 4th address
    for (uint64_t i = 0; i < fm->size(); i += 4) {
        uint32_t word = i / 4;
        fm->write_word(i, word);
    }

    // read word from each 4th address
    // and check if the data is correct
    for (uint64_t i = 0; i < fm->size(); i += 4) {
        uint32_t word = fm->read_word(i);
        assert(word == ((i / 4) % 4294967296));
    }

    // write double word to each 8th address
    for (uint64_t i = 0; i < fm->size(); i += 8) {
        uint64_t double_word = i / 8;
        fm->write_double_word(i, double_word);
    }

    // read double word from each 8th address
    // and check if the data is correct
    for (uint64_t i = 0; i < fm->size(); i += 8) {
        uint64_t double_word = fm->read_double_word(i);
        assert(double_word == (i / 8));
    }

    return 0;
}
