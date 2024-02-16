#include "set_associative_cache.h"

#include <memory>

SetAssociativeCache::SetAssociativeCache(bool write_allocate, bool write_back,
                                         uint32_t miss_latency, uint32_t hit_latency,
                                         uint32_t cache_line_size, uint32_t sets,
                                         uint32_t ways)
    : write_allocate_(write_allocate),
      write_back_(write_back),
      miss_latency_(miss_latency),
      hit_latency_(hit_latency),
      cache_line_size_(cache_line_size),
      sets_(sets),
      ways_(ways) {}

/**
 * @brief Returns the size of the cache in bytes
 * @return The size of the cache in bytes
 */
uint64_t SetAssociativeCache::size() { return sets_ * ways_ * cache_line_size_; }
