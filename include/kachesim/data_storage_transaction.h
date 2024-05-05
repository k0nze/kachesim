#ifndef DATA_STORAGE_TRANSACTION_H
#define DATA_STORAGE_TRANSACTION_H

#include <memory>

#include "kachesim/data.h"

typedef uint32_t latency_t;
typedef uint64_t address_t;

typedef enum DataStorageTransactionType { READ, WRITE } DataStorageTransactionType;
typedef enum HitMiss { HIT, MISS } HitMiss;

namespace kachesim {
class DataStorageTransaction {
public:
    DataStorageTransaction();
    DataStorageTransaction(DataStorageTransactionType type, address_t address,
                           latency_t latency, int32_t hit_level, Data data);
    DataStorageTransactionType type;
    address_t address;
    latency_t latency;

    /**
     * a hit_level = -1 means a miss and no hit on another level (e.g. full write
     * of cache block without reading from next level data storage)
     * a hit_level = 0 means the hit appeared at the requested level a hit_level > 0
     * means the hit appeared at a higher level, this means there was a miss at the
     * requested
     */
    int32_t hit_level;
    Data data = Data(0);
};
}  // namespace kachesim

#endif
