#include <cassert>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "doubly_linked_list/doubly_linked_list.h"
#include "kachesim.h"

using namespace kachesim;

int main() {
    std::srand(42);

    auto addresses = DoublyLinkedList<uint64_t>();
    std::map<address_t, uint8_t> address_data_map;

    auto fm = std::make_shared<FakeMemory>("mem0", 1024, 10, 5);

    // write 0xff to each byte in memory
    for (int i = 0; i < 1024; i++) {
        Data d = Data(1);
        d.set<uint8_t>(0xff, 0);
        fm->write(i, d);
    }

    latency_t miss_latency = 10;
    latency_t hit_latency = 5;

    bool write_allocate = true;
    bool write_through = false;

    size_t cache_block_size = 8;
    size_t sets = 4;
    size_t ways = 2;

    auto sac0 = std::make_shared<SetAssociativeCache>(
        "sac0", fm, write_allocate, write_through, miss_latency, hit_latency,
        cache_block_size, sets, ways, ReplacementPolicyType::LRU);

    std::cout << "Cache size: " << sac0->size() << std::endl;

    Data d_byte = Data(1);
    d_byte.set<uint8_t>(0x11, 0);

    // partial write to empty block
    sac0->write(0x0000, d_byte);
    assert(sac0->is_address_cached(0x0000));
    assert(sac0->is_address_valid(0x0000));
    assert(sac0->get(0x0000) == 0x11);

    for (uint64_t i = 0x0001; i <= 0x0007; i++) {
        assert(sac0->is_address_cached(i));
        assert(sac0->is_address_valid(i));
        assert(sac0->get(i) == fm->get(i));
    }

    // partial block update
    d_byte.set<uint8_t>(0x22, 0);
    sac0->write(0x0001, d_byte);

    assert(sac0->is_address_cached(0x0000));
    assert(sac0->is_address_valid(0x0000));
    assert(sac0->get(0x0000) == 0x11);
    assert(sac0->is_address_cached(0x0001));
    assert(sac0->is_address_valid(0x0001));
    assert(sac0->get(0x0001) == 0x22);

    for (uint64_t i = 0x0002; i <= 0x0007; i++) {
        assert(sac0->is_address_cached(i));
        assert(sac0->is_address_valid(i));
        assert(sac0->get(i) == fm->get(i));
    }

    // partial block update
    d_byte.set<uint8_t>(0x33, 0);
    sac0->write(0x0002, d_byte);

    assert(sac0->is_address_cached(0x0000));
    assert(sac0->is_address_valid(0x0000));
    assert(sac0->get(0x0000) == 0x11);
    assert(sac0->is_address_cached(0x0001));
    assert(sac0->is_address_valid(0x0001));
    assert(sac0->get(0x0001) == 0x22);
    assert(sac0->is_address_cached(0x0002));
    assert(sac0->is_address_valid(0x0002));
    assert(sac0->get(0x0002) == 0x33);

    for (uint64_t i = 0x0003; i <= 0x0007; i++) {
        assert(sac0->is_address_cached(i));
        assert(sac0->is_address_valid(i));
        assert(sac0->get(i) == fm->get(i));
    }

    // partial block update
    d_byte.set<uint8_t>(0x44, 0);
    sac0->write(0x0003, d_byte);

    assert(sac0->is_address_cached(0x0000));
    assert(sac0->is_address_valid(0x0000));
    assert(sac0->get(0x0000) == 0x11);
    assert(sac0->is_address_cached(0x0001));
    assert(sac0->is_address_valid(0x0001));
    assert(sac0->get(0x0001) == 0x22);
    assert(sac0->is_address_cached(0x0002));
    assert(sac0->is_address_valid(0x0002));
    assert(sac0->get(0x0002) == 0x33);
    assert(sac0->is_address_cached(0x0003));
    assert(sac0->is_address_valid(0x0003));
    assert(sac0->get(0x0003) == 0x44);

    for (uint64_t i = 0x0004; i <= 0x0007; i++) {
        assert(sac0->is_address_cached(i));
        assert(sac0->is_address_valid(i));
        assert(sac0->get(i) == fm->get(i));
    }

    // partial block update
    d_byte.set<uint8_t>(0x55, 0);
    sac0->write(0x0004, d_byte);

    assert(sac0->is_address_cached(0x0000));
    assert(sac0->is_address_valid(0x0000));
    assert(sac0->get(0x0000) == 0x11);
    assert(sac0->is_address_cached(0x0001));
    assert(sac0->is_address_valid(0x0001));
    assert(sac0->get(0x0001) == 0x22);
    assert(sac0->is_address_cached(0x0002));
    assert(sac0->is_address_valid(0x0002));
    assert(sac0->get(0x0002) == 0x33);
    assert(sac0->is_address_cached(0x0003));
    assert(sac0->is_address_valid(0x0003));
    assert(sac0->get(0x0003) == 0x44);
    assert(sac0->is_address_cached(0x0004));
    assert(sac0->is_address_valid(0x0004));
    assert(sac0->get(0x0004) == 0x55);

    for (uint64_t i = 0x0005; i <= 0x0007; i++) {
        assert(sac0->is_address_cached(i));
        assert(sac0->is_address_valid(i));
        assert(sac0->get(i) == fm->get(i));
    }

    // partial block update
    d_byte.set<uint8_t>(0x66, 0);
    sac0->write(0x0005, d_byte);

    assert(sac0->is_address_cached(0x0000));
    assert(sac0->is_address_valid(0x0000));
    assert(sac0->get(0x0000) == 0x11);
    assert(sac0->is_address_cached(0x0001));
    assert(sac0->is_address_valid(0x0001));
    assert(sac0->get(0x0001) == 0x22);
    assert(sac0->is_address_cached(0x0002));
    assert(sac0->is_address_valid(0x0002));
    assert(sac0->get(0x0002) == 0x33);
    assert(sac0->is_address_cached(0x0003));
    assert(sac0->is_address_valid(0x0003));
    assert(sac0->get(0x0003) == 0x44);
    assert(sac0->is_address_cached(0x0004));
    assert(sac0->is_address_valid(0x0004));
    assert(sac0->get(0x0004) == 0x55);
    assert(sac0->is_address_cached(0x0005));
    assert(sac0->is_address_valid(0x0005));
    assert(sac0->get(0x0005) == 0x66);

    for (uint64_t i = 0x0006; i <= 0x0007; i++) {
        assert(sac0->is_address_cached(i));
        assert(sac0->is_address_valid(i));
        assert(sac0->get(i) == fm->get(i));
    }

    // partial block update
    d_byte.set<uint8_t>(0x77, 0);
    sac0->write(0x0006, d_byte);

    assert(sac0->is_address_cached(0x0000));
    assert(sac0->is_address_valid(0x0000));
    assert(sac0->get(0x0000) == 0x11);
    assert(sac0->is_address_cached(0x0001));
    assert(sac0->is_address_valid(0x0001));
    assert(sac0->get(0x0001) == 0x22);
    assert(sac0->is_address_cached(0x0002));
    assert(sac0->is_address_valid(0x0002));
    assert(sac0->get(0x0002) == 0x33);
    assert(sac0->is_address_cached(0x0003));
    assert(sac0->is_address_valid(0x0003));
    assert(sac0->get(0x0003) == 0x44);
    assert(sac0->is_address_cached(0x0004));
    assert(sac0->is_address_valid(0x0004));
    assert(sac0->get(0x0004) == 0x55);
    assert(sac0->is_address_cached(0x0005));
    assert(sac0->is_address_valid(0x0005));
    assert(sac0->get(0x0005) == 0x66);
    assert(sac0->is_address_cached(0x0006));
    assert(sac0->is_address_valid(0x0006));
    assert(sac0->get(0x0006) == 0x77);

    assert(sac0->is_address_cached(0x0007));
    assert(sac0->get(0x0007) == fm->get(0x0007));

    // partial block update
    d_byte.set<uint8_t>(0x88, 0);
    sac0->write(0x0007, d_byte);

    assert(sac0->is_address_cached(0x0000));
    assert(sac0->is_address_valid(0x0000));
    assert(sac0->get(0x0000) == 0x11);
    assert(sac0->is_address_cached(0x0001));
    assert(sac0->is_address_valid(0x0001));
    assert(sac0->get(0x0001) == 0x22);
    assert(sac0->is_address_cached(0x0002));
    assert(sac0->is_address_valid(0x0002));
    assert(sac0->get(0x0002) == 0x33);
    assert(sac0->is_address_cached(0x0003));
    assert(sac0->is_address_valid(0x0003));
    assert(sac0->get(0x0003) == 0x44);
    assert(sac0->is_address_cached(0x0004));
    assert(sac0->is_address_valid(0x0004));
    assert(sac0->get(0x0004) == 0x55);
    assert(sac0->is_address_cached(0x0005));
    assert(sac0->is_address_valid(0x0005));
    assert(sac0->get(0x0005) == 0x66);
    assert(sac0->is_address_cached(0x0006));
    assert(sac0->is_address_valid(0x0006));
    assert(sac0->get(0x0006) == 0x77);
    assert(sac0->is_address_cached(0x0007));
    assert(sac0->is_address_valid(0x0007));
    assert(sac0->get(0x0007) == 0x88);

    // full write to empty blocks
    Data d_block = Data(8);
    d_block.set<uint64_t>(0x0101010101010101, 0);
    sac0->write(0x0008, d_block);

    for (uint64_t i = 0x0008; i <= 0x000a; i++) {
        assert(sac0->is_address_cached(i));
        assert(sac0->is_address_valid(i));
        assert(sac0->get(i) == 0x01);
    }

    d_block.set<uint64_t>(0x0202020202020202, 0);
    sac0->write(0x0010, d_block);

    for (uint64_t i = 0x0010; i <= 0x0017; i++) {
        assert(sac0->is_address_cached(i));
        assert(sac0->is_address_valid(i));
        assert(sac0->get(i) == 0x02);
    }

    d_block.set<uint64_t>(0x0303030303030303, 0);
    sac0->write(0x0018, d_block);

    for (uint64_t i = 0x0018; i <= 0x001a; i++) {
        assert(sac0->is_address_cached(i));
        assert(sac0->is_address_valid(i));
        assert(sac0->get(i) == 0x03);
    }

    d_block.set<uint64_t>(0x0404040404040404, 0);
    sac0->write(0x0020, d_block);

    for (uint64_t i = 0x0020; i <= 0x0027; i++) {
        assert(sac0->is_address_cached(i));
        assert(sac0->is_address_valid(i));
        assert(sac0->get(i) == 0x04);
    }

    d_block.set<uint64_t>(0x0505050505050505, 0);
    sac0->write(0x0028, d_block);

    for (uint64_t i = 0x0028; i <= 0x002a; i++) {
        assert(sac0->is_address_cached(i));
        assert(sac0->is_address_valid(i));
        assert(sac0->get(i) == 0x05);
    }

    d_block.set<uint64_t>(0x0606060606060606, 0);
    sac0->write(0x0030, d_block);

    for (uint64_t i = 0x0030; i <= 0x0037; i++) {
        assert(sac0->is_address_cached(i));
        assert(sac0->is_address_valid(i));
        assert(sac0->get(i) == 0x06);
    }

    d_block.set<uint64_t>(0x0707070707070707, 0);
    sac0->write(0x0038, d_block);

    for (uint64_t i = 0x0038; i <= 0x003a; i++) {
        assert(sac0->is_address_cached(i));
        assert(sac0->is_address_valid(i));
        assert(sac0->get(i) == 0x07);
    }

    // update address 0x0000
    // this places the cache block which contains address 0x0000 in front of all other
    // blocks in the LRU
    d_block.set<uint64_t>(0x0808080808080808, 0);
    sac0->write(0x0000, d_block);

    for (uint64_t i = 0x0000; i <= 0x0007; i++) {
        assert(sac0->is_address_cached(i));
        assert(sac0->is_address_valid(i));
        assert(sac0->get(i) == 0x08);
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
    assert(sac0->get_cache_block_data(0, 0).get<uint64_t>() == 0x0808080808080808);
    assert(sac0->get_cache_block_tag(0, 0) == 0x00);
    assert(sac0->is_cache_block_valid(0, 0));
    assert(sac0->is_cache_block_dirty(0, 0));

    assert(sac0->get_cache_block_data(0, 1).get<uint64_t>() == 0x0404040404040404);
    assert(sac0->get_cache_block_tag(0, 1) == 0x01);
    assert(sac0->is_cache_block_valid(0, 1));
    assert(sac0->is_cache_block_dirty(0, 1));

    // cache_set 1
    assert(sac0->get_cache_block_data(1, 0).get<uint64_t>() == 0x0101010101010101);
    assert(sac0->get_cache_block_tag(1, 0) == 0x00);
    assert(sac0->is_cache_block_valid(1, 0));
    assert(sac0->is_cache_block_dirty(1, 0));

    assert(sac0->get_cache_block_data(1, 1).get<uint64_t>() == 0x0505050505050505);
    assert(sac0->get_cache_block_tag(1, 1) == 0x01);
    assert(sac0->is_cache_block_valid(1, 1));
    assert(sac0->is_cache_block_dirty(1, 1));

    // cache_set 2
    assert(sac0->get_cache_block_data(2, 0).get<uint64_t>() == 0x0202020202020202);
    assert(sac0->get_cache_block_tag(2, 0) == 0x00);
    assert(sac0->is_cache_block_valid(2, 0));
    assert(sac0->is_cache_block_dirty(2, 0));

    assert(sac0->get_cache_block_data(2, 1).get<uint64_t>() == 0x0606060606060606);
    assert(sac0->get_cache_block_tag(2, 1) == 0x01);
    assert(sac0->is_cache_block_valid(2, 1));
    assert(sac0->is_cache_block_dirty(2, 1));

    // cache_set 3
    assert(sac0->get_cache_block_data(3, 0).get<uint64_t>() == 0x0303030303030303);
    assert(sac0->get_cache_block_tag(3, 0) == 0x00);
    assert(sac0->is_cache_block_valid(3, 0));
    assert(sac0->is_cache_block_dirty(3, 0));

    assert(sac0->get_cache_block_data(3, 1).get<uint64_t>() == 0x0707070707070707);
    assert(sac0->get_cache_block_tag(3, 1) == 0x01);
    assert(sac0->is_cache_block_valid(3, 1));
    assert(sac0->is_cache_block_dirty(3, 1));

    // all cache blocks are now occupied -> this will trigger the eviction of the block
    // containing address 0x0020
    // because 0x0020 and 0x0040 have the same index (0) and there for they are in the
    // same cache set. in this cache set 0x0020 was accesses last.
    d_block.set<uint64_t>(0x0909090909090909, 0);
    sac0->write(0x0040, d_block);

    // check if data has been written
    for (int i = 0; i < 8; i++) {
        assert(fm->get(0x0020 + i) == 0x04);
    }

    assert(sac0->is_address_cached(0x0000));
    assert(sac0->is_address_cached(0x0008));
    assert(sac0->is_address_cached(0x0010));
    assert(sac0->is_address_cached(0x0018));

    // was evicted
    assert(!sac0->is_address_cached(0x0020));

    assert(sac0->is_address_cached(0x0028));
    assert(sac0->is_address_cached(0x0030));
    assert(sac0->is_address_cached(0x0038));

    assert(sac0->is_address_cached(0x0040));
    assert(sac0->is_address_valid(0x0040));

    for (uint64_t i = 0x0040; i <= 0x0047; i++) {
        assert(sac0->is_address_cached(i));
        assert(sac0->is_address_valid(i));
        assert(sac0->get(i) == 0x09);
    }

    // issue unalinged write
    Data d_multi_block = Data(11);

    for (int i = 0; i < 11; i++) {
        d_multi_block.set<uint8_t>(i + 1, i, false);
    }

    fm->set(0x0050, 0xab);
    fm->set(0x005f, 0xcd);

    // this will evict two blocks both with block_index 0
    // both writes are unaligned
    sac0->write(0x0051, d_multi_block);

    // 0x0051 = 0b10|10|001
    assert(sac0->get_cache_block_data(2, 0).get<uint64_t>() == 0x7060504030201ab);

    // 0x0058 = 0b10|11|000
    assert(sac0->get_cache_block_data(3, 0).get<uint64_t>() == 0xcdffffff0b0a0908);

    Data d_multi_block2 = Data(26);

    for (int i = 0; i < 26; i++) {
        d_multi_block2.set<uint8_t>(i + 1, i, false);
    }

    // issue aligned write for first second, and thrid block and an unaligned write for
    // the fourth block
    sac0->write(0x0080, d_multi_block2);

    // 0x0080 = 0b100|00|000
    assert(sac0->get_cache_block_data(0, 0) == 0x0807060504030201);
    // 0x0088 = 0b100|01|000
    assert(sac0->get_cache_block_data(1, 0) == 0x100f0e0d0c0b0a09);
    // 0x0090 = 0b100|10|000
    assert(sac0->get_cache_block_data(2, 1) == 0x1817161514131211);
    // 0x0090 = 0b100|11|000
    assert(sac0->get_cache_block_data(3, 1) == 0xffffffffffff1a19);

    for (int i = 0x0200; i <= 0x220; i++) {
        fm->set(i, 0xed);
    }
    fm->set(0x200, 0x89);

    // issue aligned write for the second and thrid block and an unaligned write for the
    // first and fourth block
    sac0->write(0x0203, d_multi_block2);

    // 0x0200 = 0b10000|00|000
    assert(sac0->get_cache_block_data(0, 1) == 0x0504030201eded89);
    // 0x0208 = 0b10000|01|000
    assert(sac0->get_cache_block_data(1, 1) == 0x0d0c0b0a09080706);
    // 0x0210 = 0b10000|10|000
    assert(sac0->get_cache_block_data(2, 0) == 0x1514131211100f0e);
    // 0x0218 = 0b10000|11|000
    assert(sac0->get_cache_block_data(3, 0) == 0xededed1a19181716);

    fm->set(0x0001, 0xef);

    // issue unaligned for the first block and an aligned write for the second, thrid,
    // and fourth block
    sac0->write(0x0006, d_multi_block2);

    // 0x0000 = 0b00000|00|000
    // data from memory is not 0xff because there was a write back with
    // 0x0808080808080808
    assert(sac0->get_cache_block_data(0, 0) == 0x020108080808ef08);
    // 0x0008 = 0b00000|01|000
    assert(sac0->get_cache_block_data(1, 0) == 0x0a09080706050403);
    // 0x0010 = 0b00000|10|000
    assert(sac0->get_cache_block_data(2, 1) == 0x1211100f0e0d0c0b);
    // 0x0018 = 0b00000|11|000
    assert(sac0->get_cache_block_data(3, 1) == 0x1a19181716151413);

    // read data

    fm->set(0x0300, 0xef);
    fm->set(0x0301, 0xbe);
    fm->set(0x0302, 0xad);
    fm->set(0x0303, 0xde);
    fm->set(0x0304, 0xab);
    fm->set(0x0305, 0xcd);
    fm->set(0x0306, 0xef);
    fm->set(0x0307, 0x89);

    // read full aligned block that is not cached, since the cache is full a block needs
    // to be evicted, the block to be evicted is in set 0, block index 1 is least
    // recently used -> address 0x0200 is written back

    // 0x0020 = 0b0000001|00|000
    // 0x0200 = 0b0b10000|00|000
    Data write_back_data = sac0->get_cache_block_data(0, 1);

    auto read_dst0 = sac0->read(0x0020, 8);
    assert(read_dst0.data.get<uint64_t>() == 0x0404040404040404);

    assert(fm->get(0x0200) == 0x89);
    assert(fm->get(0x0201) == 0xed);
    assert(fm->get(0x0202) == 0xed);
    assert(fm->get(0x0203) == 0x01);
    assert(fm->get(0x0204) == 0x02);
    assert(fm->get(0x0205) == 0x03);
    assert(fm->get(0x0206) == 0x04);
    assert(fm->get(0x0207) == 0x05);

    // unaligned read within single block that is not cached, since the cache is full a
    // block needs to be evicted, the block to be evicted is in set 2, block index 0 is
    // least recently used -> address 0x0210 is written back
    fm->set(0x0310, 0xbb);
    fm->set(0x0311, 0xcc);
    auto read_dst1 = sac0->read(0x0310, 2);
    assert(read_dst1.data.get<uint64_t>() == 0xccbb);

    assert(fm->get(0x0210) == 0x0e);
    assert(fm->get(0x0211) == 0x0f);
    assert(fm->get(0x0212) == 0x10);
    assert(fm->get(0x0213) == 0x11);
    assert(fm->get(0x0214) == 0x12);
    assert(fm->get(0x0215) == 0x13);
    assert(fm->get(0x0216) == 0x14);
    assert(fm->get(0x0217) == 0x15);

    // unaligned read spanning two blocks
    fm->set(0x0308, 0xcc);
    fm->set(0x0309, 0xdd);
    fm->set(0x030a, 0xee);

    auto read_dst2 = sac0->read(0x030a, 9);
    assert(read_dst2.data.get<uint64_t>() == 0xccbb'ffff'ffff'ffee);
    assert(read_dst2.data.get<uint8_t>(8) == 0xff);

    // unaligned read spanning three blocks
    for (int i = 0; i < 24; i++) {
        fm->set(0x100 + i, i);
    }

    auto read_dst3 = sac0->read(0x0103, 19);
    assert(read_dst3.data.size() == 19);
    assert(read_dst3.data.get<uint64_t>() == 0x0a09'0807'0605'0403);
    assert(read_dst3.data.get<uint64_t>(8) == 0x1211'100f'0e0d'0c0b);
    assert(read_dst3.data.get<uint64_t>(16) == 0x151413);

    // aligned read from first block and unaligned read from second block
    auto read_dst4 = sac0->read(0x0100, 13);
    assert(read_dst4.data.size() == 13);
    assert(read_dst4.data.get<uint64_t>() == 0x0706'0504'0302'0100);
    assert(read_dst4.data.get<uint64_t>(8) == 0x0c'0b0a'0908);

    // unaligned read of first block aligned read of second block
    fm->set(0x0250, 0x11);
    fm->set(0x0251, 0x22);
    fm->set(0x0252, 0x33);
    fm->set(0x0253, 0x44);
    fm->set(0x0254, 0x55);
    fm->set(0x0255, 0x66);
    fm->set(0x0256, 0x77);
    fm->set(0x0257, 0x88);

    fm->set(0x0258, 0x99);
    fm->set(0x0259, 0xaa);
    fm->set(0x025a, 0xbb);
    fm->set(0x025b, 0xcc);
    fm->set(0x025c, 0xdd);
    fm->set(0x025d, 0xee);
    fm->set(0x025e, 0xff);
    fm->set(0x025f, 0x00);

    auto read_dst5 = sac0->read(0x0253, 13);
    assert(read_dst5.data.size() == 13);
    assert(read_dst5.data.get<uint64_t>() == 0xbbaa'9988'7766'5544);
    assert(read_dst5.data.get<uint64_t>(8) == 0x00'ffee'ddcc);

    // test full read of fake memory through set associative cache
    // reset fake memroy and fill with random data
    fm->reset();
    for (int i = 0; i < fm->size(); i++) {
        uint8_t random_int = std::rand() % 256;
        fm->set(i, random_int);
    }

    sac0->reset();

    // generate ddl with all fake memory addresses
    for (int i = 0; i < fm->size(); i++) {
        addresses.insert_tail(i);
    }

    std::cout << addresses.size() << std::endl;

    while (!addresses.empty()) {
        auto address_nodes = addresses.get_nodes();
        size_t node_index = std::rand() % address_nodes.size();
        auto node = address_nodes[node_index];
        uint64_t address = node->value;
        addresses.remove(node);

        auto read_dst = sac0->read(address, 1);
        assert(read_dst.data.get<uint8_t>() == fm->get(address));
    }

    // test full write to fake memory through set associative cache
    // generate data to write to fake memory
    for (int i = 0; i < fm->size(); i++) {
        uint8_t random_int = std::rand() % 256;
        address_data_map.insert({i, random_int});
    }

    // generate ddl with all fake memory addresses
    addresses = DoublyLinkedList<uint64_t>();
    for (int i = 0; i < fm->size(); i++) {
        addresses.insert_tail(i);
    }

    while (!addresses.empty()) {
        auto address_nodes = addresses.get_nodes();
        size_t node_index = std::rand() % address_nodes.size();
        auto node = address_nodes[node_index];
        uint64_t address = node->value;
        addresses.remove(node);
        Data write_data = Data(1);
        write_data.set<uint8_t>(address_data_map.at(address));
        sac0->write(address, write_data);
    }

    sac0->flush();

    // check fake memory values
    for (int i = 0; i < fm->size(); i++) {
        assert(fm->get(i) == address_data_map.at(i));
    }

    // test hit_level calculation
    // reset fake memory and fill
    fm->reset();
    for (int i = 0; i < fm->size(); i++) {
        fm->set(i, 0xff);
    }

    sac0->reset();

    // miss without eviction and without write
    auto read_dst6 = sac0->read(0x0000, 8);
    assert(read_dst6.hit_level == 1);

    auto read_dst7 = sac0->read(0x0008, 8);
    assert(read_dst7.hit_level == 1);

    auto read_dst8 = sac0->read(0x0010, 8);
    assert(read_dst8.hit_level == 1);

    auto read_dst9 = sac0->read(0x0018, 8);
    assert(read_dst9.hit_level == 1);

    auto read_dst10 = sac0->read(0x0020, 8);
    assert(read_dst10.hit_level == 1);

    auto read_dst11 = sac0->read(0x0028, 8);
    assert(read_dst11.hit_level == 1);

    auto read_dst12 = sac0->read(0x0030, 8);
    assert(read_dst12.hit_level == 1);

    auto read_dst13 = sac0->read(0x0038, 8);
    assert(read_dst13.hit_level == 1);

    // miss with eviction and without write
    auto read_dst14 = sac0->read(0x0040, 8);
    assert(read_dst14.hit_level == 1);

    // hit
    auto read_dst15 = sac0->read(0x0010, 8);
    assert(read_dst15.hit_level == 0);

    Data d_block2 = Data(8);
    d_block2.set<uint64_t>(0xaaaa'aaaa'aaaa'aaaa);
    auto write_dst0 = sac0->write(0x0010, d_block2);
    assert(write_dst0.hit_level == 0);

    auto write_dst1 = sac0->write(0x0110, d_block2);
    assert(write_dst1.hit_level == -1);

    Data d_block3 = Data(4);
    auto write_dst2 = sac0->write(0x0230, d_block3);
    assert(write_dst2.hit_level == 1);

    auto write_dst3 = sac0->write(0x0234, d_block3);
    assert(write_dst3.hit_level == 0);

    // test write_allocate = false
    fm->reset();

    write_allocate = false;
    write_through = false;

    auto sac1 = std::make_shared<SetAssociativeCache>(
        "sac1", fm, write_allocate, write_through, miss_latency, hit_latency,
        cache_block_size, sets, ways, ReplacementPolicyType::LRU);

    // full aligned write to empty block
    Data d_block4 = Data(8);
    d_block4.set<uint64_t>(0x2211'ffee'ddcc'bbaa);

    auto write_dst4 = sac1->write(0x0000, d_block4);
    assert(write_dst4.hit_level == 1);

    assert(fm->get(0x0000) == 0xaa);
    assert(fm->get(0x0001) == 0xbb);
    assert(fm->get(0x0002) == 0xcc);
    assert(fm->get(0x0003) == 0xdd);
    assert(fm->get(0x0004) == 0xee);
    assert(fm->get(0x0005) == 0xff);
    assert(fm->get(0x0006) == 0x11);
    assert(fm->get(0x0007) == 0x22);

    fm->set(0x0100, 0xef);
    fm->set(0x0101, 0xbe);
    fm->set(0x0102, 0xad);
    fm->set(0x0103, 0xde);
    fm->set(0x0104, 0x01);
    fm->set(0x0105, 0x02);
    fm->set(0x0106, 0x03);
    fm->set(0x0107, 0x04);

    auto read_dst16 = sac1->read(0x0100, 8);
    assert(read_dst16.hit_level == 1);
    assert(read_dst16.data.get<uint64_t>() == 0x04030201'deadbeef);

    auto write_dst5 = sac1->write(0x0100, d_block4);
    assert(write_dst5.hit_level == 0);

    // no write back has taken place
    assert(fm->get(0x0100) == 0xef);
    assert(fm->get(0x0101) == 0xbe);
    assert(fm->get(0x0102) == 0xad);
    assert(fm->get(0x0103) == 0xde);
    assert(fm->get(0x0104) == 0x01);
    assert(fm->get(0x0105) == 0x02);
    assert(fm->get(0x0106) == 0x03);
    assert(fm->get(0x0107) == 0x04);

    fm->reset();
    sac1->reset();

    // write to every fakje memory through set asosciative cache
    // generate data to write to fake memory
    for (int i = 0; i < fm->size(); i++) {
        uint8_t random_int = std::rand() % 256;
        address_data_map.insert({i, random_int});
    }

    // generate ddl with all fake memory addresses
    addresses = DoublyLinkedList<uint64_t>();
    for (int i = 0; i < fm->size(); i++) {
        addresses.insert_tail(i);
    }

    while (!addresses.empty()) {
        auto address_nodes = addresses.get_nodes();
        size_t node_index = std::rand() % address_nodes.size();
        auto node = address_nodes[node_index];
        uint64_t address = node->value;
        addresses.remove(node);
        Data write_data = Data(1);
        write_data.set<uint8_t>(address_data_map.at(address));
        sac1->write(address, write_data);
    }

    // since there was no read no block was ever cached and no write allocation appeared
    // therefore no cache flush needs to be issued

    // check fake memory values
    for (int i = 0; i < fm->size(); i++) {
        assert(fm->get(i) == address_data_map.at(i));
    }

    // test write_through = true
    fm->reset();

    write_allocate = false;
    write_through = true;

    auto sac2 = std::make_shared<SetAssociativeCache>(
        "sac2", fm, write_allocate, write_through, miss_latency, hit_latency,
        cache_block_size, sets, ways, ReplacementPolicyType::LRU);

    return 0;
}
