#ifndef SET_ASSOCIATIVE_CACHE_H
#define SET_ASSOCIATIVE_CACHE_H

#include "cache_interface.h"
#include "cache_set.h"

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
class SetAssociativeCache : public CacheInterface {
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

    void write(uint64_t address, const std::vector<uint8_t>& data);
    std::vector<uint8_t> read(uint64_t address, uint32_t num_bytes);

private:
    uint64_t offset_mask_;
    uint64_t index_mask_;
    uint64_t tag_mask_;

    std::vector<CacheSet> cache_sets_;

    uint64_t get_address_offset(uint64_t address);
    uint64_t get_address_index(uint64_t address);
    uint64_t get_address_tag(uint64_t address);
};

#endif
