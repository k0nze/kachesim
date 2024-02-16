#ifndef CACHE_LINE_H
#define CACHE_LINE_H

#include <cstdint>
#include <vector>

enum CacheLineAccessResult { hit, miss };

/**
 * represents a cache line (also called block) containing N bytes with a tag and a
 * dirty bit
 */
class CacheLine {
public:
    CacheLine(uint64_t size);

    uint64_t tag_;
    bool dirty_ = false;

    std::size_t size() const { return data_.size(); }

    uint8_t operator[](uint64_t index) const { return data_[index]; }
    uint8_t& operator[](uint64_t index) { return data_[index]; }

private:
    std::vector<uint8_t> data_;
};

#endif
