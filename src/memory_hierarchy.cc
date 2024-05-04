#include "memory_hierarchy.h"

#include <yaml-cpp/yaml.h>

namespace kachesim {

MemoryHierarchy::MemoryHierarchy() = default;

MemoryHierarchy::MemoryHierarchy(std::string yaml_config_string) {
    std::string s = "name: YAML from libcurl";
    std::stringstream ss(s);
    YAML::Parser parser(ss);
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
