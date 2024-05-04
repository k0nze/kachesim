#ifndef MEMORY_HIERARCHY_H
#define MEMORY_HIERARCHY_H

#include <string>

#include "data_storage_transaction.h"

namespace kachesim {
class MemoryHierarchy {
public:
    MemoryHierarchy();
    MemoryHierarchy(const std::string& yaml_config_string);
    DataStorageTransaction write(address_t address, Data& data);
    DataStorageTransaction read(address_t address, size_t num_bytes);
};
}  // namespace kachesim

#endif
