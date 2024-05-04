#ifndef MEMORY_HIERARCHY_H
#define MEMORY_HIERARCHY_H

#include <yaml-cpp/yaml.h>

#include <map>
#include <memory>
#include <string>

#include "data_storage.h"
#include "data_storage_transaction.h"
#include "fake_memory.h"
#include "set_associative_cache.h"

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
    std::map<std::string, std::shared_ptr<DataStorage>> data_storage_map_;

    std::shared_ptr<FakeMemory> fake_memory_from_yaml_node_(
        const YAML::Node& yaml_node);
    std::shared_ptr<SetAssociativeCache> set_associative_cache_from_yaml_node_(
        const YAML::Node& yaml_node,
        std::shared_ptr<DataStorage> next_level_data_storage);
};
}  // namespace kachesim

#endif
