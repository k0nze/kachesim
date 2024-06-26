#include "kachesim/data_storage_transaction.h"

#include <iostream>

namespace kachesim {
DataStorageTransaction::DataStorageTransaction() {}
DataStorageTransaction::DataStorageTransaction(DataStorageTransactionType type,
                                               address_t address, latency_t latency,
                                               int32_t hit_level, Data data)
    : type(type),
      address(address),
      latency(latency),
      hit_level(hit_level),
      data(data) {}
}  // namespace kachesim
