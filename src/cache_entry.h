class CacheEntry {
public:
    CacheEntry();
    uint64_t tag;
    bool dirty = false;
};
