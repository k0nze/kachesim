#include "memory_hierarchy.h"

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
                auto data_storage_name = Clone(data_storage["name"]);
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

        // order data storages by their dependencies
        std::vector<std::string> data_storage_order;

        // first add FakeMemories because they don't have a dependcy
        for (const auto& data_storage_name : data_storage_names_) {
            if (data_storage_type_map_[data_storage_name].compare("FakeMemory") == 0) {
                data_storage_order.push_back(data_storage_name);
            }
        }

        // CAUTION: This is a naive implementation of topological sort and probably only
        // works for linear memory hierarchies
        while (data_storage_order.size() < data_storage_names_.size()) {
            // add data storages which aren't already added and those next level data
            // storages have already been added
            for (const auto& data_storage_name : data_storage_names_) {
                if (std::find(data_storage_order.begin(), data_storage_order.end(),
                              data_storage_name) == data_storage_order.end()) {
                    if (std::find(data_storage_order.begin(), data_storage_order.end(),
                                  data_storage_dependency_map_[data_storage_name]) !=
                        data_storage_order.end()) {
                        data_storage_order.push_back(data_storage_name);
                    }
                }
            }
        }

        // instantiate data storages
        for (const auto& data_storage_name : data_storage_order) {
            if (data_storage_type_map_[data_storage_name].compare("FakeMemory") == 0) {
                std::shared_ptr<FakeMemory> fake_memory;

                // TODO: fix this in the future
                // since YAML:Nodes seems to be problematic when copying them from the
                // loop where they are accessed iterate a second time over data storages
                // to get node for current data storage
                for (const auto& data_storage : data_storages) {
                    if (data_storage["name"].as<std::string>().compare(
                            data_storage_name) == 0) {
                        fake_memory_from_yaml_node_(data_storage);
                        break;
                    }
                }

                data_storage_map_.insert({data_storage_name, fake_memory});
            }

            else if (data_storage_type_map_[data_storage_name].compare(
                         "SetAssociativeCache") == 0) {
                auto next_level_data_storage =
                    data_storage_map_[data_storage_dependency_map_[data_storage_name]];

                bool write_allocate = true;
                bool write_through = false;
                latency_t miss_latency = 0;
                latency_t hit_latency = 0;
                size_t cache_block_size = 16;
                size_t sets = 4;
                size_t ways = 2;
                ReplacementPolicyType replacement_policy_type =
                    ReplacementPolicyType::LRU;
                size_t multi_block_access = 1;

                // TODO: fix this in the future
                // since YAML:Nodes seems to be problematic when copying them from the
                // loop where they are accessed iterate a second time over data storages
                // to get node for current data storage
                for (const auto& data_storage : data_storages) {
                    if (data_storage["name"].as<std::string>().compare(
                            data_storage_name) == 0) {
                        write_allocate = data_storage["write_allocate"].as<bool>();
                        break;
                    }
                }

                auto set_associative_cache = std::make_shared<SetAssociativeCache>(
                    data_storage_name, next_level_data_storage, write_allocate,
                    write_through, miss_latency, hit_latency, cache_block_size, sets,
                    ways, ReplacementPolicyType::LRU, multi_block_access);
                data_storage_map_.insert({data_storage_name, set_associative_cache});
            }
        }

    } else {
        throw std::runtime_error("No 'nodes' in yaml config");
    }
}

std::shared_ptr<FakeMemory> MemoryHierarchy::fake_memory_from_yaml_node_(
    const YAML::Node& yaml_node) {
    std::string data_storage_name = yaml_node["name"].as<std::string>();
    uint64_t size = yaml_node["size"].as<uint64_t>();
    latency_t read_latency = yaml_node["read_latency"].as<latency_t>();
    latency_t write_latency = yaml_node["write_latency"].as<latency_t>();

    auto fake_memory = std::make_shared<FakeMemory>(data_storage_name, size,
                                                    read_latency, write_latency);
    return fake_memory;
}

std::shared_ptr<SetAssociativeCache>
MemoryHierarchy::set_associative_cache_from_yaml_node_(
    const YAML::Node& yaml_node, std::shared_ptr<DataStorage> next_level_data_storage) {
    return nullptr;
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
