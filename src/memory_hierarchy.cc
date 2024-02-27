#include "memory_hierarchy.h"

#include "yaml-cpp/yaml.h"

MemoryHierarchy::MemoryHierarchy(const std::string& yaml_config_string) {
    YAML::Node config = YAML::Load(yaml_config_string);
}
