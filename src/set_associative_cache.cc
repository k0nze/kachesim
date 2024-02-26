#include "set_associative_cache.h"

#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "common.h"

SetAssociativeCache::SetAssociativeCache(bool write_allocate, bool write_back,
                                         latency_t miss_latency, latency_t hit_latency,
                                         size_t cache_line_size, size_t sets,
                                         size_t ways,
                                         ReplacementPolicyType replacement_policy_type,
                                         size_t multi_line_access)
    : write_allocate_(write_allocate),
      write_back_(write_back),
      miss_latency_(miss_latency),
      hit_latency_(hit_latency),
      cache_line_size_(cache_line_size),
      sets_(sets),
      ways_(ways),
      replacement_policy_type_(replacement_policy_type),
      multi_line_access_(multi_line_access) {
    offset_mask_ = bitmask<uint64_t>(clog2(cache_line_size_));
    index_mask_ = bitmask<uint64_t>(clog2(sets_)) << clog2(cache_line_size_);
    tag_mask_ = ~offset_mask_ & ~index_mask_;

    reset();
}

/**
 * @brief Returns the size of the cache in bytes
 * @return The size of the cache in bytes
 */
size_t SetAssociativeCache::size() { return sets_ * ways_ * cache_line_size_; }

/**
 * @brief calculates the offset of the address (n LSBs, while 2^n is the cache line
 * size)
 * @param address the address to calculate the offset from
 * @return the offset of the address
 */
inline uint64_t SetAssociativeCache::get_address_offset(uint64_t address) {
    return address & offset_mask_;
}

/**
 * @brief calculates the index of the address
 * @param address the address to calculate the index from
 * @return the offset of the address
 */
inline uint64_t SetAssociativeCache::get_address_index(uint64_t address) {
    return (address & index_mask_) >> clog2(cache_line_size_);
}

/**
 * @brief calculates the tag of the address
 * @param address the address to calculate the tag from
 * @return the tag of the address
 */
inline uint64_t SetAssociativeCache::get_address_tag(uint64_t address) {
    return (address & tag_mask_) >> (clog2(cache_line_size_) + clog2(sets_));
}

/**
 * @brief aligns a transaction to the cache line size
 * @param address the address to align
 * @param data the data to align
 * @return a map of aligned addresses and data
 */
std::map<address_t, Data> SetAssociativeCache::align_transaction(address_t address,
                                                                 Data& data) {
    std::map<address_t, Data> address_data_map;

    // check if data effects multiple sets and lines
    uint64_t offset = get_address_offset(address);
    uint64_t tag = get_address_tag(address);
    uint64_t index = get_address_index(address);

    // check if only one cache line is affected
    if (offset + data.size() <= cache_line_size_) {
        address_data_map.insert({address, data});
        return address_data_map;
    }

    // split data into cache line size chunks
    // if offset != 0 first piece of data is a partial update
    uint32_t bytes_allocated = cache_line_size_ - offset;

    auto d0 = Data(bytes_allocated);

    for (int i = 0; i < bytes_allocated; i++) {
        d0[i] = data[i];
    }

    address_data_map.insert({address, d0});

    while (bytes_allocated < data.size()) {
        // check if there is data for a whole line
        if (data.size() - bytes_allocated > cache_line_size_) {
            // full update
            Data d = Data(cache_line_size_);
            for (int i = 0; i < cache_line_size_; i++) {
                d[i] = data[i + bytes_allocated];
            }
            address_data_map.insert({address + bytes_allocated, d});
            bytes_allocated += cache_line_size_;
        }

        else {
            // partial update
            Data d = Data(data.size() - bytes_allocated);
            for (int i = 0; i < data.size() - bytes_allocated; i++) {
                d[i] = data[i + bytes_allocated];
            }
            address_data_map.insert({address + bytes_allocated, d});
            bytes_allocated += cache_line_size_;
        }
    }

    return address_data_map;
}

/**
 * @brief write data to single cache line
 * @param address the address to write to
 * @param data the data to write
 * @return the transaction result
 */
DataStorageTransaction SetAssociativeCache::aligned_write(address_t address,
                                                          Data& data) {
    uint64_t offset = get_address_offset(address);
    uint64_t tag = get_address_tag(address);
    uint64_t index = get_address_index(address);

    // check if target cache set already contains tag
    int32_t line_index = cache_sets_[index]->get_line_index_with_tag(tag);

    // TODO: may be move to CacheSet in the future
    if (line_index != -1) {
        // line with tag found -> update line

        // check if its a partial write
        if (data.size() != cache_line_size_) {
            // partial write
            Data update_data = Data(cache_line_size_);
            Data cache_line_data = cache_sets_[index]->get_line_data(line_index);

            // copy data from cache line
            // from 0 to offset
            for (int i = 0; i < offset; i++) {
                update_data[i] = cache_line_data[i];
            }
            // from offset to offset+data.size()
            for (int i = offset; i < offset + data.size(); i++) {
                update_data[i] = data[i - offset];
            }
            // from offset+data.size() to cache_line_size_
            for (int i = offset + data.size(); i < cache_line_size_; i++) {
                update_data[i] = cache_line_data[i];
            }

            std::cout << "update data:     " << update_data.get<uint64_t>()
                      << std::endl;

            cache_sets_[index]->update_line(line_index, tag, update_data, true, true);
            std::cout << "Partial update @ address/index: " << std::hex << address
                      << "/" << index << ", d: " << update_data.get<uint64_t>()
                      << std::endl;
            cache_sets_[index]->update_replacement_policy(line_index);
        } else {
            // full write
            cache_sets_[index]->update_line(line_index, tag, data, true, true);
            std::cout << "Full update @ address/index:    " << std::hex << address
                      << "/" << index << ", d: " << data.get<uint64_t>() << std::endl;
            cache_sets_[index]->update_replacement_policy(line_index);
        }
    } else {
        // check if there is a free line
        line_index = cache_sets_[index]->get_free_line_index();

        if (line_index != -1) {
            // free line found -> miss -> write to line

            // check if its a partial write
            if (data.size() != cache_line_size_) {
                // partial write
                Data update_data = Data(cache_line_size_);

                if (offset != 0) {
                    // load lower bytes from memory
                    // TODO
                    for (int i = 0; i < offset; i++) {
                        // fake data not from memory
                        update_data[i] = 0xff;
                    }
                    for (int i = offset; i < cache_line_size_; i++) {
                        update_data[i] = data[i - offset];
                    }
                } else {
                    for (int i = 0; i < data.size(); i++) {
                        update_data[i] = data[i];
                    }
                    // load upper bytes from memory
                    // TODO
                    for (int i = data.size(); i < cache_line_size_; i++) {
                        // fake data not from memory
                        update_data[i] = 0xff;
                    }
                }

                cache_sets_[index]->update_line(line_index, tag, update_data, true,
                                                true);
                std::cout << "Partial write to empty line @ address/index: " << std::hex
                          << address << "/" << index
                          << ", d: " << update_data.get<uint64_t>() << std::endl;
                cache_sets_[index]->update_replacement_policy(line_index);
            } else {
                // full write
                cache_sets_[index]->update_line(line_index, tag, data, true, true);
                std::cout << "Full write to empty line @ address/index:    " << std::hex
                          << address << "/" << index << ", d: " << data.get<uint64_t>()
                          << std::endl;
                cache_sets_[index]->update_replacement_policy(line_index);
            }
        } else {
            // no free line found -> evict line
            std::cout << "No free line found" << std::endl;
            uint32_t replacement_index = cache_sets_[index]->get_replacement_index();

            // write to line
            // TODO
        }
    }

    uint32_t hit_level = 0;
    latency_t latency = 0;

    DataStorageTransaction dst = {WRITE, address, latency, hit_level,
                                  std::unique_ptr<Data>(new Data(data))};

    return dst;
}

/**
 * @brief write data to cache and allows for unaligned access and variable length
 * @param address the address to write to
 * @param data the data to write
 */
DataStorageTransaction SetAssociativeCache::write(address_t address, Data& data) {
    std::map<address_t, Data> address_data_map = align_transaction(address, data);

    // itreate over address_data_map and execute and aligned_write
    for (auto& [addr, d] : address_data_map) {
        auto dst = aligned_write(addr, d);
    }

    uint32_t hit_level = 0;
    latency_t latency = 0;

    DataStorageTransaction dst = {WRITE, address, latency, hit_level,
                                  std::unique_ptr<Data>(new Data(data))};

    return dst;
}

/**
 * @brief read data from cache
 * @param address the address to read from
 * @param num_bytes the number of bytes to read
 * @return bytes read from cache
 */
DataStorageTransaction SetAssociativeCache::read(address_t address, size_t num_bytes) {
    auto data = std::unique_ptr<Data>(new Data(num_bytes));

    uint32_t hit_level = 0;
    latency_t latency = 0;
    DataStorageTransaction dst = {READ, address, latency, hit_level, std::move(data)};
    return dst;
}

/**
 * @brief reset the whole cache
 */
void SetAssociativeCache::reset() {
    cache_sets_.reserve(sets_);

    for (int i = 0; i < sets_; i++) {
        cache_sets_.push_back(std::unique_ptr<CacheSet>(
            new CacheSet(cache_line_size_, ways_, replacement_policy_type_)));
    }
}
