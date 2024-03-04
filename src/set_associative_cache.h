#ifndef SET_ASSOCIATIVE_CACHE_H
#define SET_ASSOCIATIVE_CACHE_H

#include <map>
#include <memory>

#include "cache_interface.h"
#include "cache_set.h"
#include "data_storage.h"

/**
 * represents a set-associative cache
 *
 * 1-way set associative
 * 8 sets, 1 line per set
 *   +--------+
 * 0 |        |
 *   +--------+
 * 1 |        |
 *   +--------+
 * 2 |        |
 *   +--------+
 * 3 |        |
 *   +--------+
 * 4 |        |
 *   +--------+
 * 5 |        |
 *   +--------+
 * 6 |        |
 *   +--------+
 * 7 |        |
 *   +--------+
 *
 * 2-way set associative
 * 4 sets, 2 lines per set
 *   +--------+
 * 0 |        |
 *   |        |
 *   +--------+
 * 1 |        |
 *   |        |
 *   +--------+
 * 2 |        |
 *   |        |
 *   +--------+
 * 3 |        |
 *   |        |
 *   +--------+
 *
 * 4-way set associative
 * 2 sets, 4 lines per set
 *   +--------+
 * 0 |        |
 *   |        |
 *   |        |
 *   |        |
 *   +--------+
 * 1 |        |
 *   |        |
 *   |        |
 *   |        |
 *   +--------+
 *
 *   ways: is the number of lines per set
 */
class SetAssociativeCache : public CacheInterface {
public:
    SetAssociativeCache(const std::string& name,
                        std::shared_ptr<DataStorage> next_level_data_storage,
                        bool write_allocate, bool write_back, latency_t miss_latency,
                        latency_t hit_latency, size_t cache_line_size, size_t sets,
                        size_t ways, ReplacementPolicyType replacement_policy_type,
                        size_t multi_line_access = 1);

    std::string get_name();
    size_t size();

    DataStorageTransaction write(address_t address, Data& data);
    DataStorageTransaction read(address_t address, size_t num_bytes);

    bool is_address_cached(address_t address);
    bool is_address_valid(address_t address);
    bool is_address_dirty(address_t address);

    uint8_t get(address_t address);

    Data get_cache_line_data(address_t cache_set_index, address_t line_index);
    address_t get_cache_line_tag(address_t cache_set_index, address_t line_index);
    bool is_cache_line_valid(address_t cache_set_index, address_t line_index);
    bool is_cache_line_dirty(address_t cache_set_index, address_t line_index);

    void reset();

private:
    std::string name_;

    bool write_allocate_;
    bool write_back_;

    latency_t miss_latency_;
    latency_t hit_latency_;

    size_t cache_line_size_;
    size_t sets_;
    size_t ways_;
    size_t multi_line_access_;

    address_t offset_mask_;
    address_t index_mask_;
    address_t tag_mask_;
    ReplacementPolicyType replacement_policy_type_;

    std::vector<std::unique_ptr<CacheSet>> cache_sets_;

    std::shared_ptr<DataStorage> next_level_data_storage_;

    address_t get_address_offset(address_t address);
    address_t get_address_index(address_t address);
    address_t get_address_tag(address_t address);
    address_t get_address_from_index_and_tag(address_t index, address_t tag);

    std::map<address_t, Data> align_write_transaction(address_t address, Data& data);
    DataStorageTransaction aligned_write(address_t address, Data& data);

    std::map<address_t, size_t> align_read_transaction(address_t address,
                                                       size_t num_bytes);
    DataStorageTransaction aligned_read(address_t address, size_t num_bytes);

    DataStorageTransaction fill_data_from_next_level_data_storage(Data& data,
                                                                  address_t address,
                                                                  size_t num_bytes);
};

#endif
