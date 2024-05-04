#include "memory_hierarchy.h"

#include <yaml-cpp/yaml.h>

#include <iostream>

namespace kachesim {

MemoryHierarchy::MemoryHierarchy() = default;

MemoryHierarchy::MemoryHierarchy(const std::string& yaml_config_string) {
    YAML::Node config = YAML::Load(yaml_config_string);

    // access nodes
    if (config["nodes"]) {
        auto nodes = config["nodes"];

        // check if nodes is a sequence
        if (!nodes.IsSequence()) {
            throw std::runtime_error("'nodes' in yaml config is not a sequence");
        }

        // iterate over nodes in nodes
        for (const auto& node : nodes) {
            std::cout << "-" << std::endl;
            // access node name
            if (node["name"]) {
                auto node_name = node["name"];
                auto node_name_str = node_name.as<std::string>();
                std::cout << node_name_str << std::endl;
            } else {
                throw std::runtime_error(
                    "No node does not contain 'name' in yaml config");
            }

            // access node type
            if (node["type"]) {
                auto node_type = node["type"];
                auto node_type_str = node_type.as<std::string>();
                std::cout << node_type_str << std::endl;
            } else {
                throw std::runtime_error(
                    "No node does not contain 'type' in yaml config");
            }
        }

    } else {
        throw std::runtime_error("No 'nodes' in yaml config");
    }
    /*
    for (const auto& pair : config) {
        std::string key = pair.first.as<std::string>();
        std::cout << key << std::endl;
        YAML::Node value = pair.second;

        if(value.IsMap()) {
            std::cout << "Map" << std::endl;
        } else if(value.IsSequence()) {
            std::cout << "Sequence" << std::endl;
        } else {
            std::cout << "Scalar" << std::endl;
        }
    }
    */
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
