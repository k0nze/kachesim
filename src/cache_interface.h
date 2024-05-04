#ifndef CACHE_INTERFACE_H
#define CACHE_INTERFACE_H

#include "data_storage.h"

namespace kachesim {
class CacheInterface : public DataStorage {
public:
    CacheInterface();
    CacheInterface(const std::string& name);

    virtual ~CacheInterface() = 0;

    virtual std::string get_name() = 0;
    virtual size_t size() = 0;

    virtual DataStorageTransaction write(address_t address, Data& data) = 0;
    virtual DataStorageTransaction read(address_t address, size_t num_bytes) = 0;

    virtual uint8_t get(address_t address) = 0;

    virtual void reset() = 0;
};
}  // namespace kachesim

#endif
