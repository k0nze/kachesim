#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include "data_storage_transaction.h"

class DataStorage {
public:
    DataStorage();
    DataStorage(const std::string& name);

    virtual ~DataStorage() = 0;

    virtual std::string get_name() = 0;
    virtual size_t size() = 0;

    virtual DataStorageTransaction write(address_t address, Data& data) = 0;
    virtual DataStorageTransaction read(address_t address, size_t num_bytes) = 0;

    virtual uint8_t get(address_t address) = 0;

    virtual void reset() = 0;
};

#endif
