#ifndef CACHE_INTERFACE_H
#define CACHE_INTERFACE_H

#include <cstdint>
#include <vector>

class CacheInterface {
public:
    CacheInterface() {}
    virtual ~CacheInterface() {}

    virtual void write(uint64_t address, const std::vector<uint8_t>& data) = 0;
    virtual std::vector<uint8_t> read(uint64_t address, uint32_t num_bytes) = 0;
};

#endif
