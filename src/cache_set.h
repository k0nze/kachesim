#ifndef CACHE_SET_H
#define CACHE_SET_H

#include <cstdint>
#include <memory>
#include <vector>

#include "cache_line.h"

class CacheSet {
public:
    CacheSet(uint64_t cache_line_size, uint32_t ways);

    std::vector<CacheLine> lines;
};

#endif
