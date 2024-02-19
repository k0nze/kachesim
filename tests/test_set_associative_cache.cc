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

    return 0;
}
