#include <array>
#include <cstdint>

enum CacheLineAccessResult { hit, miss };

/**
 * represents a cache line (also called block) containing N bytes with a tag and a
 * dirty bit
 */
template <std::size_t N>
struct CacheLine {
    uint64_t tag;
    bool dirty = false;
    std::array<uint8_t, N> data;
};
