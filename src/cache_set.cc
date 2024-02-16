#include "cache_set.h"

#include <memory>

CacheSet::CacheSet(uint64_t cache_line_size, uint32_t ways) {
    lines = std::vector<CacheLine>();
    lines.reserve(ways);

    for (int i = 0; i < ways; i++) {
        lines[i] = CacheLine(cache_line_size);
    }
}
