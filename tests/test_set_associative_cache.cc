#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#include "data.h"
#include "data_storage.h"
#include "fake_memory.h"
#include "replacement_policy/replacement_policy.h"
#include "set_associative_cache.h"

int main() {
    auto fm = std::make_shared<FakeMemory>(1024, 10, 5);

    // write 0xff to each byte in memory
    for (int i = 0; i < 1024; i++) {
        Data d = Data(1);
        d.set<uint8_t>(0xff, 0);
        fm->write(i, d);
    }

    latency_t miss_latency = 10;
    latency_t hit_latency = 5;

    size_t cache_line_size = 8;
    size_t sets = 4;
    size_t ways = 2;

    auto sac = std::make_shared<SetAssociativeCache>(fm, false, true, miss_latency,
                                                     hit_latency, cache_line_size, sets,
                                                     ways, ReplacementPolicyType::LRU);

    std::cout << "Cache size: " << sac->size() << std::endl;

    Data d_byte = Data(1);
    d_byte.set<uint8_t>(0x11, 0);

    // partial write to empty line
    sac->write(0x0000, d_byte);
    assert(sac->is_address_cached(0x0000));
    assert(sac->is_address_valid(0x0000));
    assert(sac->get(0x0000) == 0x11);

    for (uint64_t i = 0x0001; i <= 0x0007; i++) {
        assert(sac->is_address_cached(i));
        assert(sac->is_address_valid(i));
        assert(sac->get(i) == fm->get(i));
    }

    // partial line update
    d_byte.set<uint8_t>(0x22, 0);
    sac->write(0x0001, d_byte);

    assert(sac->is_address_cached(0x0000));
    assert(sac->is_address_valid(0x0000));
    assert(sac->get(0x0000) == 0x11);
    assert(sac->is_address_cached(0x0001));
    assert(sac->is_address_valid(0x0001));
    assert(sac->get(0x0001) == 0x22);

    for (uint64_t i = 0x0002; i <= 0x0007; i++) {
        assert(sac->is_address_cached(i));
        assert(sac->is_address_valid(i));
        assert(sac->get(i) == fm->get(i));
    }

    // partial line update
    d_byte.set<uint8_t>(0x33, 0);
    sac->write(0x0002, d_byte);

    assert(sac->is_address_cached(0x0000));
    assert(sac->is_address_valid(0x0000));
    assert(sac->get(0x0000) == 0x11);
    assert(sac->is_address_cached(0x0001));
    assert(sac->is_address_valid(0x0001));
    assert(sac->get(0x0001) == 0x22);
    assert(sac->is_address_cached(0x0002));
    assert(sac->is_address_valid(0x0002));
    assert(sac->get(0x0002) == 0x33);

    for (uint64_t i = 0x0003; i <= 0x0007; i++) {
        assert(sac->is_address_cached(i));
        assert(sac->is_address_valid(i));
        assert(sac->get(i) == fm->get(i));
    }

    // partial line update
    d_byte.set<uint8_t>(0x44, 0);
    sac->write(0x0003, d_byte);

    assert(sac->is_address_cached(0x0000));
    assert(sac->is_address_valid(0x0000));
    assert(sac->get(0x0000) == 0x11);
    assert(sac->is_address_cached(0x0001));
    assert(sac->is_address_valid(0x0001));
    assert(sac->get(0x0001) == 0x22);
    assert(sac->is_address_cached(0x0002));
    assert(sac->is_address_valid(0x0002));
    assert(sac->get(0x0002) == 0x33);
    assert(sac->is_address_cached(0x0003));
    assert(sac->is_address_valid(0x0003));
    assert(sac->get(0x0003) == 0x44);

    for (uint64_t i = 0x0004; i <= 0x0007; i++) {
        assert(sac->is_address_cached(i));
        assert(sac->is_address_valid(i));
        assert(sac->get(i) == fm->get(i));
    }

    // partial line update
    d_byte.set<uint8_t>(0x55, 0);
    sac->write(0x0004, d_byte);

    assert(sac->is_address_cached(0x0000));
    assert(sac->is_address_valid(0x0000));
    assert(sac->get(0x0000) == 0x11);
    assert(sac->is_address_cached(0x0001));
    assert(sac->is_address_valid(0x0001));
    assert(sac->get(0x0001) == 0x22);
    assert(sac->is_address_cached(0x0002));
    assert(sac->is_address_valid(0x0002));
    assert(sac->get(0x0002) == 0x33);
    assert(sac->is_address_cached(0x0003));
    assert(sac->is_address_valid(0x0003));
    assert(sac->get(0x0003) == 0x44);
    assert(sac->is_address_cached(0x0004));
    assert(sac->is_address_valid(0x0004));
    assert(sac->get(0x0004) == 0x55);

    for (uint64_t i = 0x0005; i <= 0x0007; i++) {
        assert(sac->is_address_cached(i));
        assert(sac->is_address_valid(i));
        assert(sac->get(i) == fm->get(i));
    }

    // partial line update
    d_byte.set<uint8_t>(0x66, 0);
    sac->write(0x0005, d_byte);

    assert(sac->is_address_cached(0x0000));
    assert(sac->is_address_valid(0x0000));
    assert(sac->get(0x0000) == 0x11);
    assert(sac->is_address_cached(0x0001));
    assert(sac->is_address_valid(0x0001));
    assert(sac->get(0x0001) == 0x22);
    assert(sac->is_address_cached(0x0002));
    assert(sac->is_address_valid(0x0002));
    assert(sac->get(0x0002) == 0x33);
    assert(sac->is_address_cached(0x0003));
    assert(sac->is_address_valid(0x0003));
    assert(sac->get(0x0003) == 0x44);
    assert(sac->is_address_cached(0x0004));
    assert(sac->is_address_valid(0x0004));
    assert(sac->get(0x0004) == 0x55);
    assert(sac->is_address_cached(0x0005));
    assert(sac->is_address_valid(0x0005));
    assert(sac->get(0x0005) == 0x66);

    for (uint64_t i = 0x0006; i <= 0x0007; i++) {
        assert(sac->is_address_cached(i));
        assert(sac->is_address_valid(i));
        assert(sac->get(i) == fm->get(i));
    }

    // partial line update
    d_byte.set<uint8_t>(0x77, 0);
    sac->write(0x0006, d_byte);

    assert(sac->is_address_cached(0x0000));
    assert(sac->is_address_valid(0x0000));
    assert(sac->get(0x0000) == 0x11);
    assert(sac->is_address_cached(0x0001));
    assert(sac->is_address_valid(0x0001));
    assert(sac->get(0x0001) == 0x22);
    assert(sac->is_address_cached(0x0002));
    assert(sac->is_address_valid(0x0002));
    assert(sac->get(0x0002) == 0x33);
    assert(sac->is_address_cached(0x0003));
    assert(sac->is_address_valid(0x0003));
    assert(sac->get(0x0003) == 0x44);
    assert(sac->is_address_cached(0x0004));
    assert(sac->is_address_valid(0x0004));
    assert(sac->get(0x0004) == 0x55);
    assert(sac->is_address_cached(0x0005));
    assert(sac->is_address_valid(0x0005));
    assert(sac->get(0x0005) == 0x66);
    assert(sac->is_address_cached(0x0006));
    assert(sac->is_address_valid(0x0006));
    assert(sac->get(0x0006) == 0x77);

    assert(sac->is_address_cached(0x0007));
    assert(sac->get(0x0007) == fm->get(0x0007));

    // partial line update
    d_byte.set<uint8_t>(0x88, 0);
    sac->write(0x0007, d_byte);

    assert(sac->is_address_cached(0x0000));
    assert(sac->is_address_valid(0x0000));
    assert(sac->get(0x0000) == 0x11);
    assert(sac->is_address_cached(0x0001));
    assert(sac->is_address_valid(0x0001));
    assert(sac->get(0x0001) == 0x22);
    assert(sac->is_address_cached(0x0002));
    assert(sac->is_address_valid(0x0002));
    assert(sac->get(0x0002) == 0x33);
    assert(sac->is_address_cached(0x0003));
    assert(sac->is_address_valid(0x0003));
    assert(sac->get(0x0003) == 0x44);
    assert(sac->is_address_cached(0x0004));
    assert(sac->is_address_valid(0x0004));
    assert(sac->get(0x0004) == 0x55);
    assert(sac->is_address_cached(0x0005));
    assert(sac->is_address_valid(0x0005));
    assert(sac->get(0x0005) == 0x66);
    assert(sac->is_address_cached(0x0006));
    assert(sac->is_address_valid(0x0006));
    assert(sac->get(0x0006) == 0x77);
    assert(sac->is_address_cached(0x0007));
    assert(sac->is_address_valid(0x0007));
    assert(sac->get(0x0007) == 0x88);

    // full write to empty lines
    Data d_line = Data(8);
    d_line.set<uint64_t>(0x0101010101010101, 0);
    sac->write(0x0008, d_line);

    for (uint64_t i = 0x0008; i <= 0x000a; i++) {
        assert(sac->is_address_cached(i));
        assert(sac->is_address_valid(i));
        assert(sac->get(i) == 0x01);
    }

    d_line.set<uint64_t>(0x0202020202020202, 0);
    sac->write(0x0010, d_line);

    for (uint64_t i = 0x0010; i <= 0x0017; i++) {
        assert(sac->is_address_cached(i));
        assert(sac->is_address_valid(i));
        assert(sac->get(i) == 0x02);
    }

    d_line.set<uint64_t>(0x0303030303030303, 0);
    sac->write(0x0018, d_line);

    for (uint64_t i = 0x0018; i <= 0x001a; i++) {
        assert(sac->is_address_cached(i));
        assert(sac->is_address_valid(i));
        assert(sac->get(i) == 0x03);
    }

    d_line.set<uint64_t>(0x0404040404040404, 0);
    sac->write(0x0020, d_line);

    for (uint64_t i = 0x0020; i <= 0x0027; i++) {
        assert(sac->is_address_cached(i));
        assert(sac->is_address_valid(i));
        assert(sac->get(i) == 0x04);
    }

    d_line.set<uint64_t>(0x0505050505050505, 0);
    sac->write(0x0028, d_line);

    for (uint64_t i = 0x0028; i <= 0x002a; i++) {
        assert(sac->is_address_cached(i));
        assert(sac->is_address_valid(i));
        assert(sac->get(i) == 0x05);
    }

    d_line.set<uint64_t>(0x0606060606060606, 0);
    sac->write(0x0030, d_line);

    for (uint64_t i = 0x0030; i <= 0x0037; i++) {
        assert(sac->is_address_cached(i));
        assert(sac->is_address_valid(i));
        assert(sac->get(i) == 0x06);
    }

    d_line.set<uint64_t>(0x0707070707070707, 0);
    sac->write(0x0038, d_line);

    for (uint64_t i = 0x0038; i <= 0x003a; i++) {
        assert(sac->is_address_cached(i));
        assert(sac->is_address_valid(i));
        assert(sac->get(i) == 0x07);
    }

    // update address 0x0000
    // this places the cache line which contains address 0x0000 in front of all other
    // lines in the LRU
    d_line.set<uint64_t>(0x0808080808080808, 0);
    sac->write(0x0000, d_line);

    for (uint64_t i = 0x0000; i <= 0x0007; i++) {
        assert(sac->is_address_cached(i));
        assert(sac->is_address_valid(i));
        assert(sac->get(i) == 0x08);
    }

    // offset (length) = 3 [2:0]
    // index (length) = 2  [4:3]
    // cache_set 0
    // 0x0000 = 0b00|00|000
    // 0x0020 = 0b01|00|000 <- LRU
    // cache_set 1
    // 0x0008 = 0b00|01|000 <- LRU
    // 0x0028 = 0b01|01|000
    // cache_set 2
    // 0x0010 = 0b00|10|000 <- LRU
    // 0x0030 = 0b01|10|000
    // cache_set 3
    // 0x0018 = 0b00|11|000 <- LRU
    // 0x0038 = 0b01|11|000

    // all cache lines are now occupied -> this will trigger the eviction of the line
    // containing address 0x0020
    // because 0x0020 and 0x0040 have the same index (0) and there for they are in the
    // same cache set. in this cache set 0x0020 was accesses last.
    d_line.set<uint64_t>(0x0909090909090909, 0);
    sac->write(0x0040, d_line);

    assert(sac->is_address_cached(0x0000));
    assert(sac->is_address_cached(0x0008));
    assert(sac->is_address_cached(0x0010));
    assert(sac->is_address_cached(0x0018));

    // was evicted
    assert(!sac->is_address_cached(0x0020));

    assert(sac->is_address_cached(0x0028));
    assert(sac->is_address_cached(0x0030));
    assert(sac->is_address_cached(0x0038));

    assert(sac->is_address_cached(0x0040));
    assert(sac->is_address_valid(0x0040));

    for (uint64_t i = 0x0040; i <= 0x0047; i++) {
        assert(sac->is_address_cached(i));
        assert(sac->is_address_valid(i));
        assert(sac->get(i) == 0x09);
    }

    // issue unalinged write
    Data d_multi_line = Data(11);

    for (int i = 0; i < 11; i++) {
        d_multi_line.set<uint8_t>(i + 1, i);
    }

    d_byte.set<uint8_t>(0xab);

    // sac->write(0x0051, d_multi_line);
    sac->write(0x0041, d_byte);
    sac->write(0x0042, d_byte);
    sac->write(0x0043, d_byte);
    sac->write(0x0044, d_byte);
    sac->write(0x0045, d_byte);
    sac->write(0x0046, d_byte);
    sac->write(0x0047, d_byte);

    // sac->write(0x0048, d_byte);

    return 0;
}
