#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#include "data.h"
#include "replacement_policy/replacement_policy.h"
#include "set_associative_cache.h"

int main() {
    latency_t miss_latency = 10;
    latency_t hit_latency = 5;

    size_t cache_line_size = 8;
    size_t sets = 4;
    size_t ways = 2;

    auto sac = std::make_unique<SetAssociativeCache>(false, true, miss_latency,
                                                     hit_latency, cache_line_size, sets,
                                                     ways, ReplacementPolicyType::LRU);

    std::cout << "Cache size: " << sac->size() << std::endl;

    // trigger overlaping writes to cache lines and sets
    /*
    Data d = Data(14);
    d.set<uint64_t>(0x87654321deadbeef, 0);
    d.set<uint64_t>(0xaabbccddeeff, 8, false);

    sac->write(0x1000, d);
    std::cout << std::endl;

    sac->write(0x1001, d);
    std::cout << std::endl;

    sac->write(0x1002, d);
    std::cout << std::endl;

    sac->write(0x1003, d);
    std::cout << std::endl;

    sac->write(0x1004, d);
    std::cout << std::endl;

    sac->write(0x1005, d);
    std::cout << std::endl;

    sac->write(0x1006, d);
    std::cout << std::endl;

    sac->write(0x1007, d);
    std::cout << std::endl;

    sac->write(0x1008, d);
    std::cout << std::endl;

    sac->write(0x1009, d);
    std::cout << std::endl;
    */

    Data d_byte = Data(1);
    d_byte.set<uint8_t>(0xab, 0);

    // partial write to empty line
    sac->write(0x0000, d_byte);

    // partial line update
    sac->write(0x0001, d_byte);

    // partial line update
    sac->write(0x0002, d_byte);

    // partial line update
    sac->write(0x0003, d_byte);

    // partial line update
    sac->write(0x0004, d_byte);

    // partial line update
    sac->write(0x0005, d_byte);

    // partial line update
    sac->write(0x0006, d_byte);

    // partial line update
    sac->write(0x0007, d_byte);

    Data d_line = Data(8);
    d_line.set<uint64_t>(0xcdcdcdcdcdcdcdcd, 0);

    // full write to empty line
    sac->write(0x0008, d_line);
    sac->write(0x0010, d_line);
    sac->write(0x0018, d_line);
    sac->write(0x0020, d_line);
    sac->write(0x0028, d_line);
    sac->write(0x0030, d_line);
    sac->write(0x0038, d_line);

    sac->write(0x0000, d_line);

    // evict line
    sac->write(0x0040, d_line);

    return 0;
}
