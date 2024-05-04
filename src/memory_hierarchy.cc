#include "memory_hierarchy.h"

#include <yaml-cpp/yaml.h>

#include <iostream>

namespace kachesim {

MemoryHierarchy::MemoryHierarchy() = default;

MemoryHierarchy::MemoryHierarchy(const std::string& yaml_config_string) {
    // std::stringstream ss(yaml_config_string);
    // YAML::Parser parser(ss);

    YAML::Node config = YAML::Load(yaml_config_string);

    for (const auto& pair : config) {
        std::string key = pair.first.as<std::string>();
        std::cout << key << std::endl;
    }
}

DataStorageTransaction MemoryHierarchy::write(address_t address, Data& data) {
    DataStorageTransaction dst;
    return dst;
}

DataStorageTransaction MemoryHierarchy::read(address_t address, size_t num_bytes) {
    DataStorageTransaction dst;
    return dst;
}
}  // namespace kachesim
