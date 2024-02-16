#include <cstdint>

enum CacheEntryAccess { hit, miss };

struct CacheEntry {
    uint64_t tag;
    bool dirty = false;
};
