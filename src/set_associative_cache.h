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
    SetAssociativeCache(std::shared_ptr<DataStorage> next_level_data_storage,
                        bool write_allocate, bool write_back, latency_t miss_latency,
                        latency_t hit_latency, size_t cache_line_size, size_t sets,
                        size_t ways, ReplacementPolicyType replacement_policy_type,
                        size_t multi_line_access = 1);

    bool write_allocate_;
    bool write_back_;

    latency_t miss_latency_;
    latency_t hit_latency_;

    size_t cache_line_size_;
    size_t sets_;
    size_t ways_;
    size_t multi_line_access_;

    size_t size();

    DataStorageTransaction write(address_t address, Data& data);
    DataStorageTransaction read(address_t address, size_t num_bytes);

    bool is_address_cached(address_t address);
    bool is_address_valid(address_t address);
    bool is_address_dirty(address_t address);

    uint8_t get(uint64_t address);

    void reset();

private:
    uint64_t offset_mask_;
    uint64_t index_mask_;
    uint64_t tag_mask_;
    ReplacementPolicyType replacement_policy_type_;

    std::vector<std::unique_ptr<CacheSet>> cache_sets_;

    std::shared_ptr<DataStorage> next_level_data_storage_;

    uint64_t get_address_offset(uint64_t address);
    uint64_t get_address_index(uint64_t address);
    uint64_t get_address_tag(uint64_t address);

    std::map<address_t, Data> align_transaction(address_t address, Data& data);
    DataStorageTransaction aligned_write(address_t address, Data& data);

    DataStorageTransaction fill_data_from_next_level_data_storage(Data& data,
                                                                  uint64_t address,
                                                                  size_t num_bytes);
};

#endif
