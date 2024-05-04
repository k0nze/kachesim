#ifndef MEMORY_HIERARCHY_H
#define MEMORY_HIERARCHY_H

#include <map>
#include <string>

#include "data_storage_transaction.h"

namespace kachesim {
class MemoryHierarchy {
public:
    MemoryHierarchy();
    MemoryHierarchy(const std::string& yaml_config_string);
    DataStorageTransaction write(address_t address, Data& data);
    DataStorageTransaction read(address_t address, size_t num_bytes);

private:
    std::vector<std::string> data_storage_names_;
    std::map<std::string, std::string> data_storage_type_map_;
    std::map<std::string, std::string> data_storage_dependency_map_;
};
}  // namespace kachesim

#endif
