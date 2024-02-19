#ifndef CACHE_INTERFACE_H
#define CACHE_INTERFACE_H

#include "data_storage.h"

class CacheInterface : DataStorage {
public:
    CacheInterface() {}
    virtual ~CacheInterface() {}

    virtual size_t size() = 0;

    virtual DataStorageTransaction write(address_t address, Data& data) = 0;
    virtual DataStorageTransaction read(address_t address, size_t num_bytes) = 0;

    virtual void reset() = 0;
};

#endif
