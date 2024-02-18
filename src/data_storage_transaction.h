#ifndef DATA_STORAGE_TRANSACTION_H
#define DATA_STORAGE_TRANSACTION_H

#include <memory>

#include "data.h"

typedef uint32_t latency_t;
typedef uint64_t address_t;

typedef enum DataStorageTransactionType { READ, WRITE } DataStorageTransactionType;
typedef enum HitMiss { HIT, MISS } HitMiss;

struct DataStorageTransaction {
    DataStorageTransactionType type;
    address_t address;
    latency_t latency;
    uint32_t hit_level;
    std::unique_ptr<Data> data;
};

#endif
