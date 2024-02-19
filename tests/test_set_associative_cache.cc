#include <cassert>
#include <memory>
#include <vector>

#include "data.h"
#include "set_associative_cache.h"

int main() {
    latency_t miss_latency = 10;
    latency_t hit_latency = 5;

    size_t cache_line_size = 8;
    size_t sets = 4;
    size_t ways = 2;

    auto sac = std::make_unique<SetAssociativeCache>(
        false, true, miss_latency, hit_latency, cache_line_size, sets, ways);

    Data d = Data(14);
    d.set<uint64_t>(0x87654321deadbeef, 0);
    d.set<uint64_t>(0xaabbccddeeff, 8, false);

    sac->write(0x1000, d);
    sac->write(0x1001, d);
    sac->write(0x1002, d);
    sac->write(0x1003, d);
    sac->write(0x1004, d);
    sac->write(0x1005, d);
    sac->write(0x1006, d);
    sac->write(0x1007, d);

    sac->write(0x1008, d);
    sac->write(0x1009, d);

    return 0;
}
