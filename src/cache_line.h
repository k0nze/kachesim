#include <cstdint>

enum CacheLineAccessResult { hit, miss };

template <std::size_t N>
struct CacheLine {
    uint64_t tag;
    bool dirty = false;
    std::array<uint8_t, N> data;
};
