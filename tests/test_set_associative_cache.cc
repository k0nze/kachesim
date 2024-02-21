#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#include "data.h"
#include "replacement_policy.h"
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

    /*
    sac->write(0x1008, d);
    sac->write(0x1009, d);

    Data e = Data(4);
    e.set<uint32_t>(0xdeadbeef, 0);
    sac->write(0x2000, e);
    */

    return 0;
}
