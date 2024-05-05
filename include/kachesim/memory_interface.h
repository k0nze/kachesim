#ifndef MEMORY_INTERFACE_H
#define MEMORY_INTERFACE_H

#include "kachesim/data_storage.h"

namespace kachesim {
class MemoryInterface : public DataStorage {
public:
    MemoryInterface();
    MemoryInterface(const std::string& name);

    virtual ~MemoryInterface() = 0;

    virtual std::string get_name() = 0;
    virtual size_t size() = 0;

    virtual DataStorageTransaction write(address_t address, Data& data) = 0;
    virtual DataStorageTransaction read(address_t address, size_t num_bytes) = 0;

    virtual void set(address_t address, uint8_t) = 0;
    virtual uint8_t get(address_t address) = 0;

    virtual void reset() = 0;

protected:
    latency_t read_latency_ = 0;
    latency_t write_latency_ = 0;
};
}  // namespace kachesim

#endif
