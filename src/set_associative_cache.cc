#include "set_associative_cache.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <utility>
#include <vector>

#include "common.h"

namespace kachesim {
SetAssociativeCache::SetAssociativeCache(
    const std::string& name, std::shared_ptr<DataStorage> next_level_data_storage,
    bool write_allocate, bool write_through, latency_t miss_latency,
    latency_t hit_latency, size_t cache_block_size, size_t sets, size_t ways,
    ReplacementPolicyType replacement_policy_type, size_t multi_block_access)

    : name_(name),
      next_level_data_storage_(next_level_data_storage),
      write_allocate_(write_allocate),
      write_through_(write_through),
      miss_latency_(miss_latency),
      hit_latency_(hit_latency),
      cache_block_size_(cache_block_size),
      sets_(sets),
      ways_(ways),
      replacement_policy_type_(replacement_policy_type),
      multi_block_access_(multi_block_access) {
    offset_mask_ = bitmask<uint64_t>(clog2(cache_block_size_));
    index_mask_ = bitmask<uint64_t>(clog2(sets_)) << clog2(cache_block_size_);
    tag_mask_ = ~offset_mask_ & ~index_mask_;

    reset();
}

std::string SetAssociativeCache::get_name() { return name_; }

/**
 * @brief Returns the size of the cache in bytes
 * @return The size of the cache in bytes
 */
inline size_t SetAssociativeCache::size() { return sets_ * ways_ * cache_block_size_; }

/*
 * @brief calculates the offset of the address (n LSBs, while 2^n is the cache block
 * size)
 * @param address the address to calculate the offset from
 * @return the offset of the address
 */
inline address_t SetAssociativeCache::get_address_offset(address_t address) {
    return address & offset_mask_;
}

/**
 * @brief calculates the index of the address
 * @param address the address to calculate the index from
 * @return the offset of the address
 */
inline address_t SetAssociativeCache::get_address_index(uint64_t address) {
    return (address & index_mask_) >> clog2(cache_block_size_);
}

/**
 * @brief calculates the tag of the address
 * @param address the address to calculate the tag from
 * @return the tag of the address
 */
inline address_t SetAssociativeCache::get_address_tag(address_t address) {
    return (address & tag_mask_) >> (clog2(cache_block_size_) + clog2(sets_));
}

inline address_t SetAssociativeCache::get_address_from_index_and_tag(address_t index,
                                                                     address_t tag) {
    return (tag << (clog2(cache_block_size_) + clog2(sets_))) |
           (index << clog2(cache_block_size_));
}

/**
 * @brief calculates the latency for a multi block access
 * @param latencies the latencies of all aligned transactions
 * @return the latency of the multi block access
 */
latency_t SetAssociativeCache::calculate_multi_block_access_latency(
    std::vector<latency_t> latencies) {
    std::vector<latency_t> sequential_access_latencies;

    for (int i = 0; i < latencies.size(); i += multi_block_access_) {
        std::vector<latency_t> parallel_access_latencies;
        for (int j = i; j < i + multi_block_access_; j++) {
            if (j < latencies.size()) {
                parallel_access_latencies.push_back(latencies[j]);
            }
        }
        sequential_access_latencies.push_back(*std::max_element(
            parallel_access_latencies.begin(), parallel_access_latencies.end()));
    }

    // sum up sequential access latencies
    return std::accumulate(sequential_access_latencies.begin(),
                           sequential_access_latencies.end(), 0);
}

/**
 * @brief aligns a transaction to the cache block size
 * @param address the address to align
 * @param data the data to align
 * @return a map of aligned addresses and data
 */
std::map<address_t, Data> SetAssociativeCache::align_write_transaction(
    address_t address, Data& data) {
    std::map<address_t, Data> address_data_map;

    // check if data effects multiple sets and blocks
    address_t offset = get_address_offset(address);
    address_t tag = get_address_tag(address);
    address_t index = get_address_index(address);

    // check if only one cache block is affected
    if (offset + data.size() <= cache_block_size_) {
        address_data_map.insert({address, data});
        return address_data_map;
    }

    // split data into cache block size chunks
    // if offset != 0 first piece of data is a partial update
    uint32_t bytes_allocated = cache_block_size_ - offset;

    auto d0 = Data(bytes_allocated);

    for (int i = 0; i < bytes_allocated; i++) {
        d0[i] = data[i];
    }

    address_data_map.insert({address, d0});

    while (bytes_allocated < data.size()) {
        // check if there is data for a whole block
        if (data.size() - bytes_allocated > cache_block_size_) {
            // full update
            Data d = Data(cache_block_size_);
            for (int i = 0; i < cache_block_size_; i++) {
                d[i] = data[i + bytes_allocated];
            }
            address_data_map.insert({address + bytes_allocated, d});
            bytes_allocated += cache_block_size_;
        }

        else {
            // partial update
            Data d = Data(data.size() - bytes_allocated);
            for (int i = 0; i < data.size() - bytes_allocated; i++) {
                d[i] = data[i + bytes_allocated];
            }
            address_data_map.insert({address + bytes_allocated, d});
            // this block is needed to end the while loop
            bytes_allocated += cache_block_size_;
        }
    }

    return address_data_map;
}

DataStorageTransaction SetAssociativeCache::fill_data_from_next_level_data_storage(
    Data& data, uint64_t address, size_t num_bytes) {
    address_t offset = get_address_offset(address);

    // load data from next level data storage
    auto next_level_dst = next_level_data_storage_->read(address - offset, num_bytes);
    auto next_level_data = next_level_dst.data;

    Data fill_data = Data(num_bytes);

    // copy data from next level data storage
    // from 0 to offset
    for (int i = 0; i < offset; i++) {
        fill_data[i] = next_level_data[i];
    }
    // from offset to offset+data.size()
    for (int i = offset; i < offset + data.size(); i++) {
        fill_data[i] = data[i - offset];
    }
    // form offset+data.size() to cache_block_size_
    for (int i = offset + data.size(); i < cache_block_size_; i++) {
        fill_data[i] = next_level_data[i];
    }

    DataStorageTransaction dst = {WRITE, address, next_level_dst.latency,
                                  next_level_dst.hit_level, fill_data};

    return dst;
}

/**
 * @brief write data to single cache block
 * @param address the address to write to
 * @param data the data to write
 * @return the transaction result
 */
DataStorageTransaction SetAssociativeCache::aligned_write(address_t address,
                                                          Data& data) {
    address_t offset = get_address_offset(address);
    address_t tag = get_address_tag(address);
    address_t index = get_address_index(address);

    bool written_back = false;

    int32_t hit_level = -1;
    latency_t latency = 0;

    // check if target cache set already contains tag
    int32_t block_index = cache_sets_[index]->get_block_index_with_tag(tag);

    // TODO: may be move to CacheSet in the future
    if (block_index != -1) {
        // block with tag found -> hit -> update block
        hit_level = 0;
        latency = hit_latency_;

        // check if its a partial write
        if (data.size() != cache_block_size_) {
            // partial write
            Data update_data = Data(cache_block_size_);
            Data cache_block_data = cache_sets_[index]->get_block_data(block_index);

            // copy data from cache block
            // from 0 to offset
            for (int i = 0; i < offset; i++) {
                update_data[i] = cache_block_data[i];
            }
            // from offset to offset+data.size()
            for (int i = offset; i < offset + data.size(); i++) {
                update_data[i] = data[i - offset];
            }
            // from offset+data.size() to cache_block_size_
            for (int i = offset + data.size(); i < cache_block_size_; i++) {
                update_data[i] = cache_block_data[i];
            }

            cache_sets_[index]->update_block(block_index, tag, update_data, true, true);
            cache_sets_[index]->update_replacement_policy(block_index);

            DEBUG_PRINT(
                "> %s w @ 0x%016llx : d=%s / i=%02lld / b=%04d - partial write to "
                "cached block\n",
                name_.c_str(), address, update_data.to_string().c_str(), index,
                block_index);

        } else {
            // full write
            cache_sets_[index]->update_block(block_index, tag, data, true, true);
            cache_sets_[index]->update_replacement_policy(block_index);

            DEBUG_PRINT(
                "> %s w @ 0x%016llx : d=%s / i=%02lld / b=%04d - full write to cached "
                "block\n",
                name_.c_str(), address, data.to_string().c_str(), index, block_index);
        }
    } else {
        // block with tag not found -> miss
        latency = miss_latency_;

        if (write_allocate_) {
            // check if there is a free block
            block_index = cache_sets_[index]->get_free_block_index();

            if (block_index != -1) {
                // free block found -> miss -> write to block
                // check if its a partial write
                if (data.size() != cache_block_size_) {
                    // partial write
                    auto update_dst = fill_data_from_next_level_data_storage(
                        data, address, cache_block_size_);

                    hit_level = update_dst.hit_level + 1;
                    latency += update_dst.latency;

                    Data update_data = update_dst.data;

                    cache_sets_[index]->update_block(block_index, tag, update_data,
                                                     true, true);
                    cache_sets_[index]->update_replacement_policy(block_index);

                    DEBUG_PRINT(
                        "> %s w @ 0x%016llx : d=%s / i=%02lld / b=%04d - partial write "
                        "to "
                        "empty block\n",
                        name_.c_str(), address, update_data.to_string().c_str(), index,
                        block_index);

                } else {
                    // full write
                    cache_sets_[index]->update_block(block_index, tag, data, true,
                                                     true);
                    cache_sets_[index]->update_replacement_policy(block_index);

                    // no hit occured on any other level
                    hit_level = -1;

                    DEBUG_PRINT(
                        "> %s w @ 0x%016llx : d=%s / i=%02lld / b=%04d - full write to "
                        "empty block\n",
                        name_.c_str(), address, data.to_string().c_str(), index,
                        block_index);
                }
            } else {
                // no free block found -> evict block
                block_index = cache_sets_[index]->get_replacement_index();

                // if block is valid and dirty write back to next level data storage
                if (cache_sets_[index]->is_block_valid(block_index) &&
                    cache_sets_[index]->is_block_dirty(block_index)) {
                    Data write_back_data =
                        cache_sets_[index]->get_block_data(block_index);
                    address_t write_back_tag =
                        cache_sets_[index]->get_block_tag(block_index);
                    address_t write_back_address =
                        get_address_from_index_and_tag(index, write_back_tag);
                    auto write_back_dst = next_level_data_storage_->write(
                        write_back_address, write_back_data);

                    // if a write back occurs the latency from the write back
                    // transaction needs to be added
                    latency += write_back_dst.latency;
                }

                if (data.size() != cache_block_size_) {
                    // partial write
                    auto update_dst = fill_data_from_next_level_data_storage(
                        data, address, cache_block_size_);
                    Data update_data = update_dst.data;

                    hit_level = update_dst.hit_level + 1;
                    latency += update_dst.latency;

                    cache_sets_[index]->update_block(block_index, tag, update_data,
                                                     true, true);
                    cache_sets_[index]->update_replacement_policy(block_index);

                    DEBUG_PRINT(
                        "> %s w @ 0x%016llx : d=%s / i=%02lld / b=%04d - partial write "
                        "to "
                        "evicted block\n",
                        name_.c_str(), address, update_data.to_string().c_str(), index,
                        block_index);

                } else {
                    cache_sets_[index]->update_block(block_index, tag, data, true,
                                                     true);
                    cache_sets_[index]->update_replacement_policy(block_index);

                    // no hit occured on any other level
                    hit_level = -1;

                    DEBUG_PRINT(
                        "> %s w @ 0x%016llx : d=%s / i=%02lld / b=%04d - full write to "
                        "evicted block\n",
                        name_.c_str(), address, data.to_string().c_str(), index,
                        block_index);
                }
            }
        } else {
            // write_allocate_ == false
            auto write_back_dst = next_level_data_storage_->write(address, data);
            hit_level = write_back_dst.hit_level + 1;

            // if a write back occurs the latency from the write back transaction needs
            // to be added
            latency += write_back_dst.latency;

            written_back = true;

            DEBUG_PRINT(
                "> %s w @ 0x%016llx : d=%s / i=%02lld - block not cached, write back "
                "only no allocation\n",
                name_.c_str(), address, data.to_string().c_str(), index);
        }
    }

    if (write_through_ && !written_back) {
        auto write_back_dst = next_level_data_storage_->write(address, data);
        // if a write back occurs the latency from the write back transaction needs to
        // be added
        latency += write_back_dst.latency;

        DEBUG_PRINT(
            "> %s w @ 0x%016llx : d=%s / i=%02lld / b=%04d - write through to next "
            "level data storage\n",
            name_.c_str(), address, data.to_string().c_str(), index, block_index);
    }

    DataStorageTransaction dst = {WRITE, address, latency, hit_level, data};

    return dst;
}

/**
 * @brief write data to cache and allows for unaligned access and variable length
 * @param address the address to write to
 * @param data the data to write
 */
DataStorageTransaction SetAssociativeCache::write(address_t address, Data& data) {
    std::map<address_t, Data> address_data_map = align_write_transaction(address, data);

    int32_t hit_level = -1;
    std::vector<latency_t> latencies;

    // itreate over address_data_map and execute and aligned_write
    for (auto& [addr, d] : address_data_map) {
        auto dst = aligned_write(addr, d);

        latencies.push_back(dst.latency);

        // return the highest hit level from all reads
        if (dst.hit_level > hit_level) {
            hit_level = dst.hit_level;
        }
    }

    latency_t latency = calculate_multi_block_access_latency(latencies);

    DataStorageTransaction dst = {WRITE, address, latency, hit_level, data};

    return dst;
}

/**
 * @brief aligns a read transaction to the cache block size
 * @param address the address to align
 * @param num_bytes number of bytes to align
 * @return a map of aligned addresses and size of read
 */
std::map<address_t, size_t> SetAssociativeCache::align_read_transaction(
    address_t address, size_t num_bytes) {
    std::map<address_t, size_t> address_size_map;

    // check if data effects multiple sets and blocks
    address_t offset = get_address_offset(address);
    address_t tag = get_address_tag(address);
    address_t index = get_address_index(address);

    // check if only one cache block is affected
    if (offset + num_bytes <= cache_block_size_) {
        address_size_map.insert({address, num_bytes});
        return address_size_map;
    }

    // split num_bytes into cache block size chunks
    // if offset != 0 first piece of data is a partial read
    uint32_t bytes_allocated = cache_block_size_ - offset;

    address_size_map.insert({address, bytes_allocated});

    while (bytes_allocated < num_bytes) {
        // check if there are bytes for a whole block
        if (num_bytes - bytes_allocated > cache_block_size_) {
            // full read
            address_size_map.insert({address + bytes_allocated, cache_block_size_});
            bytes_allocated += cache_block_size_;
        }

        else {
            // partial read
            address_size_map.insert(
                {address + bytes_allocated, num_bytes - bytes_allocated});
            // this block is needed to end the while loop
            bytes_allocated += cache_block_size_;
        }
    }

    return address_size_map;
}

DataStorageTransaction SetAssociativeCache::aligned_read(address_t address,
                                                         size_t num_bytes) {
    address_t offset = get_address_offset(address);
    address_t tag = get_address_tag(address);
    address_t index = get_address_index(address);

    int32_t hit_level = -1;
    latency_t latency = 0;

    Data read_data = Data(num_bytes);

    // check if target cache set already contains tag
    int32_t block_index = cache_sets_[index]->get_block_index_with_tag(tag);

    // TODO: may be move to CacheSet in the future
    if (block_index != -1) {
        // block with tag found -> hit -> read block
        hit_level = 0;
        latency = hit_latency_;

        Data block_data = cache_sets_[index]->get_block_data(block_index);
        cache_sets_[index]->update_replacement_policy(block_index);

        // copy data from to read_data
        for (int i = 0; i < num_bytes; i++) {
            read_data[i] = block_data[i + offset];
        }

#if DEBUG
        if (num_bytes < cache_block_size_) {
            // partial read
            DEBUG_PRINT(
                "> %s r @ 0x%016llx : d=%s / i=%02lld / b=%04d - partial read of "
                "cached block\n",
                name_.c_str(), address, read_data.to_string().c_str(), index,
                block_index);

        } else {
            // full read
            DEBUG_PRINT(
                "> %s r @ 0x%016llx : d=%s / i=%02lld / b=%04d - full read of cached "
                "block\n",
                name_.c_str(), address, read_data.to_string().c_str(), index,
                block_index);
        }
#endif

    } else {
        // block with tag not found -> miss -> read from next level storage
        // check if there is a free block
        block_index = cache_sets_[index]->get_free_block_index();

        latency = miss_latency_;

        address_t next_level_address = address - offset;

        if (block_index != -1) {
            // free block found -> miss -> write to block
            auto next_level_storage_dst =
                next_level_data_storage_->read(next_level_address, cache_block_size_);

            Data next_level_storage_data = next_level_storage_dst.data;
            hit_level = next_level_storage_dst.hit_level + 1;
            latency += next_level_storage_dst.latency;

            for (int i = 0; i < num_bytes; i++) {
                read_data[i] = next_level_storage_data[i + offset];
            }

            cache_sets_[index]->update_block(block_index, tag, next_level_storage_data,
                                             true, false);
            cache_sets_[index]->update_replacement_policy(block_index);

#if DEBUG
            if (num_bytes < cache_block_size_) {
                // partial read
                DEBUG_PRINT(
                    "> %s r @ 0x%016llx : d=%s / i=%02lld / b=%04d - partial read of "
                    "not cached block\n",
                    name_.c_str(), address, read_data.to_string().c_str(), index,
                    block_index);
            } else {
                // full read
                DEBUG_PRINT(
                    "> %s r @ 0x%016llx : d=%s / i=%02lld / b=%04d - full read of not "
                    "cached block\n",
                    name_.c_str(), address, read_data.to_string().c_str(), index,
                    block_index);
            }
#endif

        } else {
            // no free block found -> evict block -> (write back) -> miss -> write to
            // block
            block_index = cache_sets_[index]->get_replacement_index();

            // if block is valid and dirty write back to next level data storage
            if (cache_sets_[index]->is_block_valid(block_index) &&
                cache_sets_[index]->is_block_dirty(block_index)) {
                Data write_back_data = cache_sets_[index]->get_block_data(block_index);
                address_t write_back_tag =
                    cache_sets_[index]->get_block_tag(block_index);
                address_t write_back_address =
                    get_address_from_index_and_tag(index, write_back_tag);
                auto next_level_storage_dst = next_level_data_storage_->write(
                    write_back_address, write_back_data);

                latency += next_level_storage_dst.latency;
            }

            auto next_level_storage_dst =
                next_level_data_storage_->read(next_level_address, cache_block_size_);

            Data next_level_storage_data = next_level_storage_dst.data;
            hit_level = next_level_storage_dst.hit_level + 1;
            latency += next_level_storage_dst.latency;

            for (int i = 0; i < num_bytes; i++) {
                read_data[i] = next_level_storage_data[i + offset];
            }

            cache_sets_[index]->update_block(block_index, tag, next_level_storage_data,
                                             true, false);
            cache_sets_[index]->update_replacement_policy(block_index);

#if DEBUG
            if (num_bytes < cache_block_size_) {
                // partial read
                DEBUG_PRINT(
                    "> %s r @ 0x%016llx : d=%s / i=%02lld / b=%04d - partial read of "
                    "not cached block with eviction\n",
                    name_.c_str(), address, read_data.to_string().c_str(), index,
                    block_index);
            } else {
                // full read
                DEBUG_PRINT(
                    "> %s r @ 0x%016llx : d=%s / i=%02lld / b=%04d - full read of not "
                    "cached block with eviction\n",
                    name_.c_str(), address, read_data.to_string().c_str(), index,
                    block_index);
            }
#endif
        }
    }

    DataStorageTransaction dst = {READ, address, latency, hit_level, read_data};

    return dst;
}

/**
 * @brief read data from cache
 * @param address the address to read from
 * @param num_bytes the number of bytes to read
 * @return bytes read from cache
 */
DataStorageTransaction SetAssociativeCache::read(address_t address, size_t num_bytes) {
    Data read_data = Data(num_bytes);

    std::map<address_t, size_t> address_size_map =
        align_read_transaction(address, num_bytes);

    int data_index = 0;
    int32_t hit_level = -1;
    std::vector<latency_t> latencies;

    for (auto& [addr, size] : address_size_map) {
        auto dst = aligned_read(addr, size);
        // copy data from read result into read_data
        for (int i = 0; i < size; i++) {
            read_data[data_index++] = dst.data[i];
        }

        latencies.push_back(dst.latency);

        // return the highest hit level from all reads
        if (dst.hit_level > hit_level) {
            hit_level = dst.hit_level;
        }
    }

    latency_t latency = calculate_multi_block_access_latency(latencies);

    DataStorageTransaction dst = {READ, address, latency, hit_level, read_data};
    return dst;
}

/**
 * @brief returns if an address is cached. CAUTION: this method is inteded for debugging
 * and should not be used for a simulation
 * @param address address for which it should be checked if it is cached
 * @return if address is cached
 */
bool SetAssociativeCache::is_address_cached(address_t address) {
    address_t tag = get_address_tag(address);
    address_t index = get_address_index(address);

    // check if target cache set contains  with tag
    int32_t block_index = cache_sets_[index]->get_block_index_with_tag(tag);

    return (block_index != -1);
}

bool SetAssociativeCache::is_address_valid(address_t address) {
    address_t tag = get_address_tag(address);
    address_t index = get_address_index(address);

    // check if target cache set contains  with tag
    int32_t block_index = cache_sets_[index]->get_block_index_with_tag(tag);

    if (block_index == -1) {
        return false;
    }

    return cache_sets_[index]->is_block_valid(block_index);
}

bool SetAssociativeCache::is_address_dirty(address_t address) {
    address_t tag = get_address_tag(address);
    address_t index = get_address_index(address);

    // check if target cache set contains  with tag
    int32_t block_index = cache_sets_[index]->get_block_index_with_tag(tag);

    if (block_index == -1) {
        return false;
    }

    return cache_sets_[index]->is_block_dirty(block_index);
}

/**
 * @brief returns the cached byte or 0. CAUTION: this method is intended for debugging
 * and should not be used in a simulation and check first if address is cached.
 * @param address address of byte to read
 * @return byte read or 0
 */
uint8_t SetAssociativeCache::get(uint64_t address) {
    address_t offset = get_address_offset(address);
    address_t tag = get_address_tag(address);
    address_t index = get_address_index(address);

    // check if target cache set contains  with tag
    int32_t block_index = cache_sets_[index]->get_block_index_with_tag(tag);

    if (block_index != -1) {
        Data cache_block_data = cache_sets_[index]->get_block_data(block_index);
        return cache_block_data.get<uint8_t>(offset);
    }
    return 0;
}

/**
 * @brief returns the data of a cache block. CAUTION: this method is intended for
 * debugging and should not be used in a simulation
 * @param cache_set_index the index of the cache set
 * @param block_index the index of the block
 * @return the data of the cache block
 */
Data SetAssociativeCache::get_cache_block_data(address_t cache_set_index,
                                               address_t block_index) {
    return cache_sets_[cache_set_index]->get_block_data(block_index);
}

/**
 * @brief returns the tag of a cache block. CAUTION: this method is intended for
 * debugging and should not be used in a simulation
 * @param cache_set_index the index of the cache set
 * @param block_index the index of the block
 * @return the tag of the cache block
 */
address_t SetAssociativeCache::get_cache_block_tag(address_t cache_set_index,
                                                   address_t block_index) {
    return cache_sets_[cache_set_index]->get_block_tag(block_index);
}

/**
 * @brief returns if a cache block is valid. CAUTION: this method is intended for
 * debugging and should not be used in a simulation
 * @param cache_set_index the index of the cache set
 * @param block_index the index of the block
 * @return if the cache block is valid
 */
bool SetAssociativeCache::is_cache_block_valid(address_t cache_set_index,
                                               address_t block_index) {
    return cache_sets_[cache_set_index]->is_block_valid(block_index);
}

/**
 * @brief returns if a cache block is dirty. CAUTION: this method is intended for
 * debugging and should not be used in a simulation
 * @param cache_set_index the index of the cache set
 * @param block_index the index of the block
 * @return if the cache block is dirty
 */
bool SetAssociativeCache::is_cache_block_dirty(address_t cache_set_index,
                                               address_t block_index) {
    return cache_sets_[cache_set_index]->is_block_dirty(block_index);
}

/**
 * @brief flush the whole cache, if blocks are dirty they are written back to the next
 * level data storage
 */

DataStorageTransaction SetAssociativeCache::flush() {
    // if nothing needs to be written back to the next leve data storage everything is
    // considered a hit
    int32_t hit_level = 0;

    for (int i = 0; i < sets_; i++) {
        for (int j = 0; j < ways_; j++) {
            if (cache_sets_[i]->is_block_dirty(j) &&
                cache_sets_[i]->is_block_valid(j)) {
                auto tag = cache_sets_[i]->get_block_tag(j);
                auto data = cache_sets_[i]->get_block_data(j);

                address_t address = get_address_from_index_and_tag(i, tag);

                auto next_level_dst = next_level_data_storage_->write(address, data);

                if (next_level_dst.hit_level + 1 > hit_level) {
                    hit_level = next_level_dst.hit_level + 1;
                }
            }
        }
    }
    reset();

    latency_t latency = 0;
    Data data = Data(0);

    DataStorageTransaction dst = {WRITE, 0, latency, hit_level, data};
    return dst;
}

/**
 * @brief reset the whole cache
 */
void SetAssociativeCache::reset() {
    cache_sets_.clear();
    cache_sets_.reserve(sets_);

    for (int i = 0; i < sets_; i++) {
        cache_sets_.push_back(std::unique_ptr<CacheSet>(
            new CacheSet(cache_block_size_, ways_, replacement_policy_type_)));
    }
}
}  // namespace kachesim
