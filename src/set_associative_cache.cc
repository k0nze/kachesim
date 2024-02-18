#include "set_associative_cache.h"

#include <memory>

#include "common.h"

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
      ways_(ways) {
    offset_mask_ = bitmask<uint64_t>(clog2(cache_line_size_));
    index_mask_ = bitmask<uint64_t>(clog2(sets_)) << clog2(cache_line_size_);
    tag_mask_ = ~offset_mask_ & ~index_mask_;

    reset();
}

/**
 * @brief Returns the size of the cache in bytes
 * @return The size of the cache in bytes
 */
uint64_t SetAssociativeCache::size() { return sets_ * ways_ * cache_line_size_; }

/**
 * @brief calculates the offset of the address (n LSBs, while 2^n is the cache line
 * size)
 * @param address the address to calculate the offset from
 * @return the offset of the address
 */
inline uint64_t SetAssociativeCache::get_address_offset(uint64_t address) {
    return address & offset_mask_;
}

/**
 * @brief calculates the index of the address
 * @param address the address to calculate the index from
 * @return the offset of the address
 */
inline uint64_t SetAssociativeCache::get_address_index(uint64_t address) {
    return (address & index_mask_) >> clog2(cache_line_size_);
}

/**
 * @brief calculates the tag of the address
 * @param address the address to calculate the tag from
 * @return the tag of the address
 */
inline uint64_t SetAssociativeCache::get_address_tag(uint64_t address) {
    return (address & tag_mask_) >> (clog2(cache_line_size_) + clog2(sets_));
}

/**
 * @brief writes data to the cache
 * @param address the address to write to
 * @param data the data to write to the cache at the address
 */
void SetAssociativeCache::write(uint64_t address, const std::vector<uint8_t>& data) {
    uint64_t offset = get_address_offset(address);
    uint64_t index = get_address_index(address);
    uint64_t tag = get_address_tag(address);

    CacheSet set = cache_sets_[index];
}

/**
 * @brief reads data from the cache
 * @param address the address to read from
 * @param num_bytes the number of bytes to read, starting from the address
 * @return the data read from the cache
 */
std::vector<uint8_t> SetAssociativeCache::read(uint64_t address, uint32_t num_bytes) {
    auto data = std::vector<uint8_t>(num_bytes);

    return data;
}

void SetAssociativeCache::write_byte(uint64_t address, uint8_t byte) {}

void SetAssociativeCache::write_half_word(uint64_t address, uint16_t half_word) {}

void SetAssociativeCache::write_word(uint64_t address, uint32_t word) {}

void SetAssociativeCache::write_double_word(uint64_t address, uint64_t double_word) {}

uint8_t SetAssociativeCache::read_byte(uint64_t address) {
    uint8_t byte = 0;
    return byte;
}

uint16_t SetAssociativeCache::read_half_word(uint64_t address) {
    uint16_t half_word = 0;
    return half_word;
}

uint32_t SetAssociativeCache::read_word(uint64_t address) {
    uint32_t word = 0;
    return word;
}

uint64_t SetAssociativeCache::read_double_word(uint64_t address) {
    uint64_t double_word = 0;
    return double_word;
}

/**
 * @brief reset the whole cache
 */
void SetAssociativeCache::reset() {
    cache_sets_ = std::vector<CacheSet>();
    cache_sets_.reserve(sets_);

    for (int i = 0; i < sets_; i++) {
        cache_sets_[i] = CacheSet(cache_line_size_, ways_);
    }
}
