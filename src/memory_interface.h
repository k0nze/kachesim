#ifndef MEMORY_INTERFACE_H
#define MEMORY_INTERFACE_H

#include "data_storage.h"

class MemoryInterface : public DataStorage {
public:
    virtual ~MemoryInterface() = 0;

    virtual size_t size() = 0;

    virtual DataStorageTransaction write(address_t address, Data& data) = 0;
    virtual DataStorageTransaction read(address_t address, size_t num_bytes) = 0;

    virtual void reset() = 0;

protected:
    latency_t read_latency_ = 0;
    latency_t write_latency_ = 0;
};

#endif
