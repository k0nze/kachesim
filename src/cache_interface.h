#ifndef CACHE_INTERFACE_H
#define CACHE_INTERFACE_H

#include "data_storage.h"

class CacheInterface : public DataStorage {
public:
    CacheInterface();
    CacheInterface(const std::string& name);

    virtual ~CacheInterface() = 0;

    virtual size_t size() = 0;

    virtual DataStorageTransaction write(address_t address, Data& data) = 0;
    virtual DataStorageTransaction read(address_t address, size_t num_bytes) = 0;

    virtual uint8_t get(uint64_t address) = 0;

    virtual void reset() = 0;
};

#endif
