#include "memory_hierarchy.h"

#include <algorithm>
#include <iostream>

#include "common.h"

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
                        fake_memory = fake_memory_from_yaml_node_(data_storage);
                        break;
                    }
                }

                data_storage_map_.insert({data_storage_name, fake_memory});
            }

            else if (data_storage_type_map_[data_storage_name].compare(
                         "SetAssociativeCache") == 0) {
                std::shared_ptr<SetAssociativeCache> set_associative_cache;

                auto next_level_data_storage =
                    data_storage_map_[data_storage_dependency_map_[data_storage_name]];

                // TODO: fix this in the future
                // since YAML:Nodes seems to be problematic when copying them from the
                // loop where they are accessed iterate a second time over data storages
                // to get node for current data storage
                for (const auto& data_storage : data_storages) {
                    if (data_storage["name"].as<std::string>().compare(
                            data_storage_name) == 0) {
                        set_associative_cache = set_associative_cache_from_yaml_node_(
                            data_storage, next_level_data_storage);
                        break;
                    }
                }

                data_storage_map_.insert({data_storage_name, set_associative_cache});
            }
        }

        top_level_memory = std::dynamic_pointer_cast<MemoryInterface>(
            data_storage_map_[data_storage_order[0]]);
        first_level_cache_ = std::dynamic_pointer_cast<CacheInterface>(
            data_storage_map_[data_storage_order[data_storage_order.size() - 1]]);
        // set data_storage_names_ to data_storage_order in reverse
        data_storage_names_.clear();
        for (auto it = data_storage_order.rbegin(); it != data_storage_order.rend();
             ++it) {
            data_storage_names_.push_back(*it);
        }

    } else {
        throw std::runtime_error("No 'nodes' in yaml config");
    }
}

std::shared_ptr<FakeMemory> MemoryHierarchy::fake_memory_from_yaml_node_(
    const YAML::Node& yaml_node) {
    std::string name = yaml_node["name"].as<std::string>();
    uint64_t size = yaml_node["size"].as<uint64_t>();
    latency_t read_latency = yaml_node["read_latency"].as<latency_t>();
    latency_t write_latency = yaml_node["write_latency"].as<latency_t>();

    auto fake_memory =
        std::make_shared<FakeMemory>(name, size, read_latency, write_latency);
    return fake_memory;
}

std::shared_ptr<SetAssociativeCache>
MemoryHierarchy::set_associative_cache_from_yaml_node_(
    const YAML::Node& yaml_node, std::shared_ptr<DataStorage> next_level_data_storage) {
    std::string name = yaml_node["name"].as<std::string>();
    bool write_allocate = yaml_node["write_allocate"].as<bool>();
    bool write_through = yaml_node["write_through"].as<bool>();
    latency_t miss_latency = yaml_node["miss_latency"].as<latency_t>();
    latency_t hit_latency = yaml_node["hit_latency"].as<latency_t>();
    size_t cache_block_size = yaml_node["cache_block_size"].as<size_t>();
    size_t sets = yaml_node["sets"].as<size_t>();
    size_t ways = yaml_node["ways"].as<size_t>();
    std::string replacement_policy_str =
        yaml_node["replacement_policy"].as<std::string>();

    ReplacementPolicyType replacement_policy_type;

    if (replacement_policy_str.compare("LRU") == 0) {
        replacement_policy_type = ReplacementPolicyType::LRU;
    } else {
        std::string msg = "replacement_policy '" + replacement_policy_str + "' for '" +
                          name + "' unknown in yaml config";
        THROW_INVALID_ARGUMENT(msg);
    }

    size_t multi_block_access = yaml_node["multi_block_access"].as<size_t>();

    auto set_associative_cache = std::make_shared<SetAssociativeCache>(
        name, next_level_data_storage, write_allocate, write_through, miss_latency,
        hit_latency, cache_block_size, sets, ways, replacement_policy_type,
        multi_block_access);

    return set_associative_cache;
}

DataStorageTransaction MemoryHierarchy::write(address_t address, Data& data) {
    auto read_dst = first_level_cache_->write(address, data);
    return read_dst;
}

DataStorageTransaction MemoryHierarchy::read(address_t address, size_t num_bytes) {
    auto write_dst = first_level_cache_->read(address, num_bytes);
    return write_dst;
}

DataStorageTransaction MemoryHierarchy::flush_all_caches() {
    // strating from the first level cache iterate over all cache levels to flush
    // them. Since the last level is a memory it can't be flushed
    int32_t hit_level = 0;
    latency_t latency = 0;

    for (int i = 0; i < data_storage_names_.size() - 1; i++) {
        auto cache = std::dynamic_pointer_cast<CacheInterface>(
            data_storage_map_[data_storage_names_[i]]);
        auto flush_dst = cache->flush();
        latency += flush_dst.latency;
    }
    Data data = Data(0);
    DataStorageTransaction dst = {WRITE, 0, latency, hit_level, data};

    return dst;
}

void MemoryHierarchy::reset() {}

}  // namespace kachesim
