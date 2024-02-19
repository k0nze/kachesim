#include <cassert>
#include <climits>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "fake_memory.h"

int main() {
    auto fm = std::make_unique<FakeMemory>(65536);

    // write one byte to each memory address
    for (uint64_t i = 0; i < fm->size(); i++) {
        Data d = Data(1);
        d.set<uint8_t>(i);
        fm->write(i, d);
    }

    // read bytes from memory and check if they are correct
    for (uint64_t i = 0; i < fm->size(); i++) {
        auto dst = fm->read(i, 1);
        assert(dst.data->get<uint8_t>() == (i % (UCHAR_MAX + 1)));
    }

    // write 2 bytes to each 2ne memory address
    for (uint64_t i = 0; i < fm->size(); i += 2) {
        Data d = Data(2);
        d.set<uint16_t>(i);
        fm->write(i, d);
    }

    // read 2 bytes from memory and check if they are correct
    for (uint64_t i = 0; i < fm->size(); i += 2) {
        auto dst = fm->read(i, 2);
        assert(dst.data->get<uint16_t>() == (i % (USHRT_MAX + 1)));
    }

    // write 4 bytes to each 4th memory address
    for (uint64_t i = 0; i < fm->size(); i += 4) {
        Data d = Data(4);
        d.set<uint32_t>(i);
        fm->write(i, d);
    }

    // read 4 bytes from memory and check if they are correct
    for (uint64_t i = 0; i < fm->size(); i += 4) {
        auto dst = fm->read(i, 4);
        assert(dst.data->get<uint32_t>() == (i % (UINT_MAX + 1)));
    }

    // write 8 bytes to each 8th memory address
    for (uint64_t i = 0; i < fm->size(); i += 8) {
        Data d = Data(8);
        d.set<uint64_t>(i);
        fm->write(i, d);
    }

    // read 8 bytes from memory and check if they are correct
    for (uint64_t i = 0; i < fm->size(); i += 8) {
        auto dst = fm->read(i, 8);
        assert(dst.data->get<uint64_t>() == (i % (ULONG_MAX + 1)));
    }

    // check if reset is correct
    fm->reset();

    // check if all bytes in memory are 0
    for (int i = 0; i < fm->size(); i++) {
        auto dst = fm->read(i, 1);
        assert(dst.data->get<uint8_t>() == 0);
    }

    return 0;
}
