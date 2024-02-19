#include "cache_set.h"

#include <memory>

CacheSet::CacheSet(uint64_t cache_line_size, uint32_t ways) {
    lines = std::vector<CacheLine>();
    lines.reserve(ways);

    for (int i = 0; i < ways; i++) {
        lines[i] = CacheLine(cache_line_size);
    }
}

/**
 * @brief returns the index of a free line in the cache set
 * @return The index of a free line in the cache set, -1 if no line was found
 */
int32_t CacheSet::get_free_line_index() {
    for (int i = 0; i < lines.size(); i++) {
        CacheLine line = lines[i];
        if (line.valid() == false) {
            return i;
        }
    }

    return -1;
}
