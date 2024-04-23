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
    auto fm = std::make_shared<FakeMemory>("mem0", 1024, 10, 5);

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

    auto sac = std::make_shared<SetAssociativeCache>(
        "sac0", fm, false, true, miss_latency, hit_latency, cache_line_size, sets, ways,
        ReplacementPolicyType::LRU);

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

    // cache_set 0
    assert(sac->get_cache_line_data(0, 0).get<uint64_t>() == 0x0808080808080808);
    assert(sac->get_cache_line_tag(0, 0) == 0x00);
    assert(sac->is_cache_line_valid(0, 0));
    assert(sac->is_cache_line_dirty(0, 0));

    assert(sac->get_cache_line_data(0, 1).get<uint64_t>() == 0x0404040404040404);
    assert(sac->get_cache_line_tag(0, 1) == 0x01);
    assert(sac->is_cache_line_valid(0, 1));
    assert(sac->is_cache_line_dirty(0, 1));

    // cache_set 1
    assert(sac->get_cache_line_data(1, 0).get<uint64_t>() == 0x0101010101010101);
    assert(sac->get_cache_line_tag(1, 0) == 0x00);
    assert(sac->is_cache_line_valid(1, 0));
    assert(sac->is_cache_line_dirty(1, 0));

    assert(sac->get_cache_line_data(1, 1).get<uint64_t>() == 0x0505050505050505);
    assert(sac->get_cache_line_tag(1, 1) == 0x01);
    assert(sac->is_cache_line_valid(1, 1));
    assert(sac->is_cache_line_dirty(1, 1));

    // cache_set 2
    assert(sac->get_cache_line_data(2, 0).get<uint64_t>() == 0x0202020202020202);
    assert(sac->get_cache_line_tag(2, 0) == 0x00);
    assert(sac->is_cache_line_valid(2, 0));
    assert(sac->is_cache_line_dirty(2, 0));

    assert(sac->get_cache_line_data(2, 1).get<uint64_t>() == 0x0606060606060606);
    assert(sac->get_cache_line_tag(2, 1) == 0x01);
    assert(sac->is_cache_line_valid(2, 1));
    assert(sac->is_cache_line_dirty(2, 1));

    // cache_set 3
    assert(sac->get_cache_line_data(3, 0).get<uint64_t>() == 0x0303030303030303);
    assert(sac->get_cache_line_tag(3, 0) == 0x00);
    assert(sac->is_cache_line_valid(3, 0));
    assert(sac->is_cache_line_dirty(3, 0));

    assert(sac->get_cache_line_data(3, 1).get<uint64_t>() == 0x0707070707070707);
    assert(sac->get_cache_line_tag(3, 1) == 0x01);
    assert(sac->is_cache_line_valid(3, 1));
    assert(sac->is_cache_line_dirty(3, 1));

    // all cache lines are now occupied -> this will trigger the eviction of the line
    // containing address 0x0020
    // because 0x0020 and 0x0040 have the same index (0) and there for they are in the
    // same cache set. in this cache set 0x0020 was accesses last.
    d_line.set<uint64_t>(0x0909090909090909, 0);
    sac->write(0x0040, d_line);

    // check if data has been written
    for (int i = 0; i < 8; i++) {
        assert(fm->get(0x0020 + i) == 0x04);
    }

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
        d_multi_line.set<uint8_t>(i + 1, i, false);
    }

    fm->set(0x0050, 0xab);
    fm->set(0x005f, 0xcd);

    // this will evict two lines both with line_index 0
    // both writes are unaligned
    sac->write(0x0051, d_multi_line);

    // 0x0051 = 0b10|10|001
    assert(sac->get_cache_line_data(2, 0).get<uint64_t>() == 0x7060504030201ab);

    // 0x0058 = 0b10|11|000
    assert(sac->get_cache_line_data(3, 0).get<uint64_t>() == 0xcdffffff0b0a0908);

    Data d_multi_line2 = Data(26);

    for (int i = 0; i < 26; i++) {
        d_multi_line2.set<uint8_t>(i + 1, i, false);
    }

    // issue aligned write for first second, and thrid line and an unaligned write for
    // the fourth line
    sac->write(0x0080, d_multi_line2);

    // 0x0080 = 0b100|00|000
    assert(sac->get_cache_line_data(0, 0) == 0x0807060504030201);
    // 0x0088 = 0b100|01|000
    assert(sac->get_cache_line_data(1, 0) == 0x100f0e0d0c0b0a09);
    // 0x0090 = 0b100|10|000
    assert(sac->get_cache_line_data(2, 1) == 0x1817161514131211);
    // 0x0090 = 0b100|11|000
    assert(sac->get_cache_line_data(3, 1) == 0xffffffffffff1a19);

    for (int i = 0x0200; i <= 0x220; i++) {
        fm->set(i, 0xed);
    }
    fm->set(0x200, 0x89);

    // issue aligned write for the second and thrid line and an unaligned write for the
    // first and fourth line
    sac->write(0x0203, d_multi_line2);

    // 0x0200 = 0b10000|00|000
    assert(sac->get_cache_line_data(0, 1) == 0x0504030201eded89);
    // 0x0208 = 0b10000|01|000
    assert(sac->get_cache_line_data(1, 1) == 0x0d0c0b0a09080706);
    // 0x0210 = 0b10000|10|000
    assert(sac->get_cache_line_data(2, 0) == 0x1514131211100f0e);
    // 0x0218 = 0b10000|11|000
    assert(sac->get_cache_line_data(3, 0) == 0xededed1a19181716);

    fm->set(0x0001, 0xef);

    // issue unaligned for the first line and an aligned write for the second, thrid,
    // and fourth line
    sac->write(0x0006, d_multi_line2);

    // 0x0000 = 0b00000|00|000
    // data from memory is not 0xff because there was a write back with
    // 0x0808080808080808
    assert(sac->get_cache_line_data(0, 0) == 0x020108080808ef08);
    // 0x0008 = 0b00000|01|000
    assert(sac->get_cache_line_data(1, 0) == 0x0a09080706050403);
    // 0x0010 = 0b00000|10|000
    assert(sac->get_cache_line_data(2, 1) == 0x1211100f0e0d0c0b);
    // 0x0018 = 0b00000|11|000
    assert(sac->get_cache_line_data(3, 1) == 0x1a19181716151413);

    // read data

    fm->set(0x0300, 0xef);
    fm->set(0x0301, 0xbe);
    fm->set(0x0302, 0xad);
    fm->set(0x0303, 0xde);
    fm->set(0x0304, 0xab);
    fm->set(0x0305, 0xcd);
    fm->set(0x0306, 0xef);
    fm->set(0x0307, 0x89);

    // read full aligned line that is not cached
    auto dst = sac->read(0x0020, 8);
    std::cout << dst.type << std::endl;
    std::cout << dst.data.size() << std::endl;
    std::cout << std::hex << dst.address << std::endl;
    std::cout << std::hex << dst.data.get<uint32_t>() << std::endl;
    std::cout << dst.data << std::endl;

    return 0;
}
