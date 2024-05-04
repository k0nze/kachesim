#include "memory_hierarchy.h"

#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <iostream>

namespace kachesim {

MemoryHierarchy::MemoryHierarchy() = default;

MemoryHierarchy::MemoryHierarchy(const std::string& yaml_config_string) {
    YAML::Node config = YAML::Load(yaml_config_string);

    // access data_storages
    if (config["data_storages"]) {
        auto data_storages = config["data_storages"];

        // check if nodes is a sequence
        if (!data_storages.IsSequence()) {
            throw std::runtime_error(
                "'data_storages' in yaml config is not a sequence");
        }

        // iterate over data_storages to collect names, types and dependencies
        for (const auto& data_storage : data_storages) {
            std::string data_storage_name_str;
            std::string data_storage_type_str;

            // access node name
            if (data_storage["name"]) {
                auto data_storage_name = data_storage["name"];
                data_storage_name_str = data_storage_name.as<std::string>();
                data_storage_names_.push_back(data_storage_name_str);
            } else {
                throw std::runtime_error(
                    "No data_storage does not contain 'name' in yaml config");
            }

            // access data storage type
            if (data_storage["type"]) {
                auto data_storage_type = data_storage["type"];
                data_storage_type_str = data_storage_type.as<std::string>();
                data_storage_type_map_.insert(
                    {data_storage_name_str, data_storage_type_str});
            } else {
                throw std::runtime_error(
                    "No data_storage does not contain 'type' in yaml config");
            }

            if (data_storage_type_str.compare("SetAssociativeCache") == 0) {
                if (data_storage["next_level_data_storage"]) {
                    data_storage_dependency_map_.insert(
                        {data_storage_name_str,
                         data_storage["next_level_data_storage"].as<std::string>()});

                } else {
                    throw std::runtime_error(
                        "No data_storage of type SetAssociativeCache does not contain "
                        "'next_level_data_storage' in yaml config");
                }
            }
        }

        // check data_storage_dependency_map_
        for (const auto& [data_storages_name, next_level_data_storage_name] :
             data_storage_dependency_map_) {
            auto it = std::find(data_storage_names_.begin(), data_storage_names_.end(),
                                next_level_data_storage_name);
            if (it == data_storage_names_.end()) {
                throw std::runtime_error("No data_storage with name " +
                                         next_level_data_storage_name +
                                         " in yaml config");
            }
        }

    } else {
        throw std::runtime_error("No 'nodes' in yaml config");
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
