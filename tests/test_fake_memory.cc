#include <cassert>
#include <climits>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "fake_memory.h"

int main() {
    latency_t read_latency = 3;
    latency_t write_latency = 3;

    auto fm = std::make_unique<FakeMemory>(65536, read_latency, write_latency);

    // write one byte to each memory address
    for (uint64_t i = 0; i < fm->size(); i++) {
        Data d = Data(1);
        d.set<uint8_t>(i);
        auto dst = fm->write(i, d);
        assert(dst.latency == write_latency);
    }

    // read bytes from memory and check if they are correct
    for (uint64_t i = 0; i < fm->size(); i++) {
        auto dst = fm->read(i, 1);
        assert(dst.data->get<uint8_t>() == (i % (UCHAR_MAX + 1)));
        assert(dst.latency == read_latency);
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
        assert(dst.data->get<uint32_t>() == (i % 4294967295));
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
        assert(dst.data->get<uint64_t>() == (i % 18446744073709551615ul));
    }

    // check if reset is correct
    fm->reset();

    // check if all bytes in memory are 0
    for (int i = 0; i < fm->size(); i++) {
        auto dst = fm->read(i, 1);
        assert(dst.data->get<uint8_t>() == 0);
    }

    // test reading from files
    fm = std::make_unique<FakeMemory>(32, read_latency, write_latency);

    // read whole hex file to memory starting from address 0
    fm->read_hex_memory_file("../data/hex_data0.mem", 0);

    assert(fm->read(0, 1).data->get<uint8_t>() == 0xad);
    assert(fm->read(1, 1).data->get<uint8_t>() == 0xde);
    assert(fm->read(2, 1).data->get<uint8_t>() == 0xef);
    assert(fm->read(3, 1).data->get<uint8_t>() == 0xbe);
    assert(fm->read(4, 1).data->get<uint8_t>() == 0x0a);
    assert(fm->read(5, 1).data->get<uint8_t>() == 0x0a);
    assert(fm->read(6, 1).data->get<uint8_t>() == 0x34);
    assert(fm->read(7, 1).data->get<uint8_t>() == 0x12);
    assert(fm->read(8, 1).data->get<uint8_t>() == 0x45);
    assert(fm->read(9, 1).data->get<uint8_t>() == 0x23);
    assert(fm->read(10, 1).data->get<uint8_t>() == 0x01);
    assert(fm->read(11, 1).data->get<uint8_t>() == 0xff);
    assert(fm->read(12, 1).data->get<uint8_t>() == 0xee);
    assert(fm->read(13, 1).data->get<uint8_t>() == 0x00);
    assert(fm->read(14, 1).data->get<uint8_t>() == 0x00);
    assert(fm->read(15, 1).data->get<uint8_t>() == 0x01);

    assert(fm->read(0, 8).data->get<uint64_t>() == 0x12340a0abeefdead);
    assert(fm->read(8, 8).data->get<uint64_t>() == 0x010000eeff012345);

    fm->write_hex_memory_file("../data/hex_data1.mem", 0);
    fm->write_bin_memory_file("../data/bin_data0.mem", 0);

    fm->reset();

    for (int i = 0; i < 32; i++) {
        assert(fm->read(i, 1).data->get<uint8_t>() == 0);
    }

    // read whole bin file to memory starting from address 0
    fm->read_bin_memory_file("../data/bin_data0.mem", 0);

    assert(fm->read(0, 1).data->get<uint8_t>() == 0xad);
    assert(fm->read(1, 1).data->get<uint8_t>() == 0xde);
    assert(fm->read(2, 1).data->get<uint8_t>() == 0xef);
    assert(fm->read(3, 1).data->get<uint8_t>() == 0xbe);
    assert(fm->read(4, 1).data->get<uint8_t>() == 0x0a);
    assert(fm->read(5, 1).data->get<uint8_t>() == 0x0a);
    assert(fm->read(6, 1).data->get<uint8_t>() == 0x34);
    assert(fm->read(7, 1).data->get<uint8_t>() == 0x12);
    assert(fm->read(8, 1).data->get<uint8_t>() == 0x45);
    assert(fm->read(9, 1).data->get<uint8_t>() == 0x23);
    assert(fm->read(10, 1).data->get<uint8_t>() == 0x01);
    assert(fm->read(11, 1).data->get<uint8_t>() == 0xff);
    assert(fm->read(12, 1).data->get<uint8_t>() == 0xee);
    assert(fm->read(13, 1).data->get<uint8_t>() == 0x00);
    assert(fm->read(14, 1).data->get<uint8_t>() == 0x00);
    assert(fm->read(15, 1).data->get<uint8_t>() == 0x01);

    assert(fm->read(0, 8).data->get<uint64_t>() == 0x12340a0abeefdead);
    assert(fm->read(8, 8).data->get<uint64_t>() == 0x010000eeff012345);

    fm->reset();

    // read 4 bytes from hex to memory starting from address 0
    fm->read_hex_memory_file("../data/hex_data0.mem", 0, 3);

    assert(fm->read(0, 1).data->get<uint8_t>() == 0xad);
    assert(fm->read(1, 1).data->get<uint8_t>() == 0xde);
    assert(fm->read(2, 1).data->get<uint8_t>() == 0xef);
    assert(fm->read(3, 1).data->get<uint8_t>() == 0xbe);
    assert(fm->read(4, 1).data->get<uint8_t>() == 0x00);

    assert(fm->read(0, 8).data->get<uint64_t>() == 0xbeefdead);

    fm->reset();

    // read whole hex file to memory starting from address 16
    fm->read_hex_memory_file("../data/hex_data0.mem", 16);

    assert(fm->read(16, 1).data->get<uint8_t>() == 0xad);
    assert(fm->read(17, 1).data->get<uint8_t>() == 0xde);
    assert(fm->read(18, 1).data->get<uint8_t>() == 0xef);
    assert(fm->read(19, 1).data->get<uint8_t>() == 0xbe);
    assert(fm->read(20, 1).data->get<uint8_t>() == 0x0a);
    assert(fm->read(21, 1).data->get<uint8_t>() == 0x0a);
    assert(fm->read(22, 1).data->get<uint8_t>() == 0x34);
    assert(fm->read(23, 1).data->get<uint8_t>() == 0x12);
    assert(fm->read(24, 1).data->get<uint8_t>() == 0x45);
    assert(fm->read(25, 1).data->get<uint8_t>() == 0x23);
    assert(fm->read(26, 1).data->get<uint8_t>() == 0x01);
    assert(fm->read(27, 1).data->get<uint8_t>() == 0xff);
    assert(fm->read(28, 1).data->get<uint8_t>() == 0xee);
    assert(fm->read(29, 1).data->get<uint8_t>() == 0x00);
    assert(fm->read(30, 1).data->get<uint8_t>() == 0x00);
    assert(fm->read(31, 1).data->get<uint8_t>() == 0x01);

    assert(fm->read(16, 8).data->get<uint64_t>() == 0x12340a0abeefdead);
    assert(fm->read(24, 8).data->get<uint64_t>() == 0x010000eeff012345);

    fm->reset();

    // read 5 bytes from hex to memory starting from address 16
    // however data in mem file is not aligned and there for the fifth byte is discarded
    fm->read_hex_memory_file("../data/hex_data0.mem", 16, 20);

    assert(fm->read(16, 1).data->get<uint8_t>() == 0xad);
    assert(fm->read(17, 1).data->get<uint8_t>() == 0xde);
    assert(fm->read(18, 1).data->get<uint8_t>() == 0xef);
    assert(fm->read(19, 1).data->get<uint8_t>() == 0xbe);
    assert(fm->read(20, 1).data->get<uint8_t>() == 0x00);

    assert(fm->read(16, 8).data->get<uint64_t>() == 0xbeefdead);

    fm->write_hex_memory_file("../data/hex_data2.mem", 16, 19);

    return 0;
}
