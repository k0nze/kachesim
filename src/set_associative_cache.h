#ifndef SET_ASSOCIATIVE_CACHE_H
#define SET_ASSOCIATIVE_CACHE_H

#include <cstdint>

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

    uint32_t size();
};

#endif
