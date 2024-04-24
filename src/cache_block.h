#ifndef CACHE_BLOCK_H
#define CACHE_BLOCK_H

#include <cstdint>
#include <vector>

#include "data.h"

namespace kachesim {
/**
 * represents a cache block (also called line) containing N bytes with a tag and a
 * dirty bit
 */
class CacheBlock {
public:
    CacheBlock(uint64_t size);
    ~CacheBlock();

    std::size_t size() const { return size_; }

    void set_valid();
    void set_unvalid();
    bool is_valid();

    void set_dirty();
    void set_not_dirty();
    bool is_dirty();

    void update(uint64_t tag, Data& data, bool valid = true, bool dirty = true);
    uint64_t get_tag();
    Data get_data();

    // uint8_t operator[](uint64_t index) const { return data_[index]; }
    // uint8_t& operator[](uint64_t index) { return data_[index]; }

    void reset();

private:
    uint8_t* data_;

    size_t size_;
    uint64_t tag_;
    bool dirty_ = false;
    bool valid_ = false;
};
}  // namespace kachesim

#endif
