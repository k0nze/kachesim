#ifndef SET_ASSOCIATIVE_CACHE_H
#define SET_ASSOCIATIVE_CACHE_H

#include <cstdint>

/**
 * represents a set-associative cache
 *
 * 1-way set associative
 * 8 sets, 1 line per set
 *   +--------+
 * 0 |        |
 *   +--------+
 * 1 |        |
 *   +--------+
 * 2 |        |
 *   +--------+
 * 3 |        |
 *   +--------+
 * 4 |        |
 *   +--------+
 * 5 |        |
 *   +--------+
 * 6 |        |
 *   +--------+
 * 7 |        |
 *   +--------+
 *
 * 2-way set associative
 * 4 sets, 2 lines per set
 *   +--------+
 * 0 |        |
 *   |        |
 *   +--------+
 * 1 |        |
 *   |        |
 *   +--------+
 * 2 |        |
 *   |        |
 *   +--------+
 * 3 |        |
 *   |        |
 *   +--------+
 *
 * 4-way set associative
 * 2 sets, 4 lines per set
 *   +--------+
 * 0 |        |
 *   |        |
 *   |        |
 *   |        |
 *   +--------+
 * 1 |        |
 *   |        |
 *   |        |
 *   |        |
 *   +--------+
 *
 *   ways: is the number of lines per set
 */
class SetAssociativeCache {
public:
    SetAssociativeCache(bool write_allocate, bool write_back, uint32_t miss_latency,
                        uint32_t hit_latency, uint32_t cache_line_size, uint32_t sets,
                        uint32_t ways);

    bool write_allocate_;
    bool write_back_;
    uint32_t miss_latency_;
    uint32_t hit_latency_;
    uint32_t cache_line_size_;
    uint32_t sets_;
    uint32_t ways_;

    uint64_t size();

private:
};

#endif
