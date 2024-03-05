#include "set_associative_cache.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "common.h"

SetAssociativeCache::SetAssociativeCache(
    const std::string& name, std::shared_ptr<DataStorage> next_level_data_storage,
    bool write_allocate, bool write_back, latency_t miss_latency, latency_t hit_latency,
    size_t cache_line_size, size_t sets, size_t ways,
    ReplacementPolicyType replacement_policy_type, size_t multi_line_access)

    : name_(name),
      next_level_data_storage_(next_level_data_storage),
      write_allocate_(write_allocate),
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

std::string SetAssociativeCache::get_name() { return name_; }

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
inline address_t SetAssociativeCache::get_address_offset(address_t address) {
    return address & offset_mask_;
}

/**
 * @brief calculates the index of the address
 * @param address the address to calculate the index from
 * @return the offset of the address
 */
inline address_t SetAssociativeCache::get_address_index(uint64_t address) {
    return (address & index_mask_) >> clog2(cache_line_size_);
}

/**
 * @brief calculates the tag of the address
 * @param address the address to calculate the tag from
 * @return the tag of the address
 */
inline address_t SetAssociativeCache::get_address_tag(address_t address) {
    return (address & tag_mask_) >> (clog2(cache_line_size_) + clog2(sets_));
}

inline address_t SetAssociativeCache::get_address_from_index_and_tag(address_t index,
                                                                     address_t tag) {
    return (tag << (clog2(sets_) + clog2(cache_line_size_))) |
           (index << clog2(cache_line_size_));
}

/**
 * @brief aligns a transaction to the cache line size
 * @param address the address to align
 * @param data the data to align
 * @return a map of aligned addresses and data
 */
std::map<address_t, Data> SetAssociativeCache::align_write_transaction(
    address_t address, Data& data) {
    std::map<address_t, Data> address_data_map;

    // check if data effects multiple sets and lines
    address_t offset = get_address_offset(address);
    address_t tag = get_address_tag(address);
    address_t index = get_address_index(address);

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
            // this line is needed to end the while loop
            bytes_allocated += cache_line_size_;
        }
    }

    return address_data_map;
}

DataStorageTransaction SetAssociativeCache::fill_data_from_next_level_data_storage(
    Data& data, uint64_t address, size_t num_bytes) {
    address_t offset = get_address_offset(address);

    // load data from next level data storage
    auto next_level_data =
        next_level_data_storage_->read(address - offset, num_bytes).data;

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
    // form offset+data.size() to cache_line_size_
    for (int i = offset + data.size(); i < cache_line_size_; i++) {
        fill_data[i] = next_level_data[i];
    }

    // TODO correct values for latency and hit_level
    latency_t latency = 0;
    uint32_t hit_level = 0;

    DataStorageTransaction dst = {WRITE, address, latency, hit_level, fill_data};

    return dst;
}

/**
 * @brief write data to single cache line
 * @param address the address to write to
 * @param data the data to write
 * @return the transaction result
 */
DataStorageTransaction SetAssociativeCache::aligned_write(address_t address,
                                                          Data& data) {
    address_t offset = get_address_offset(address);
    address_t tag = get_address_tag(address);
    address_t index = get_address_index(address);

    // check if target cache set already contains tag
    int32_t line_index = cache_sets_[index]->get_line_index_with_tag(tag);

    // TODO add write_allocate
    // TODO add write_back
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

            cache_sets_[index]->update_line(line_index, tag, update_data, true, true);
            cache_sets_[index]->update_replacement_policy(line_index);

            DEBUG_PRINT(
                "> %s w @ 0x%016llx : d=%s / i=%02lld / l=%04d - partial write to "
                "cached line\n",
                name_.c_str(), address, update_data.to_string().c_str(), index,
                line_index);

        } else {
            // full write
            cache_sets_[index]->update_line(line_index, tag, data, true, true);
            cache_sets_[index]->update_replacement_policy(line_index);

            DEBUG_PRINT(
                "> %s w @ 0x%016llx : d=%s / i=%02lld / l=%04d - full write to cached "
                "line\n",
                name_.c_str(), address, data.to_string().c_str(), index, line_index);
        }
    } else {
        // line with tag not found
        // check if there is a free line
        line_index = cache_sets_[index]->get_free_line_index();

        if (line_index != -1) {
            // free line found -> miss -> write to line

            // check if its a partial write
            if (data.size() != cache_line_size_) {
                // partial write
                Data update_data = fill_data_from_next_level_data_storage(
                                       data, address, cache_line_size_)
                                       .data;

                cache_sets_[index]->update_line(line_index, tag, update_data, true,
                                                true);
                cache_sets_[index]->update_replacement_policy(line_index);

                DEBUG_PRINT(
                    "> %s w @ 0x%016llx : d=%s / i=%02lld / l=%04d - partial write to "
                    "empty line\n",
                    name_.c_str(), address, update_data.to_string().c_str(), index,
                    line_index);

            } else {
                // full write
                cache_sets_[index]->update_line(line_index, tag, data, true, true);
                cache_sets_[index]->update_replacement_policy(line_index);

                DEBUG_PRINT(
                    "> %s w @ 0x%016llx : d=%s / i=%02lld / l=%04d - full write to "
                    "empty line\n",
                    name_.c_str(), address, data.to_string().c_str(), index,
                    line_index);
            }
        } else {
            // no free line found -> evict line
            uint32_t line_index = cache_sets_[index]->get_replacement_index();

            // if line is valid and dirty write back to next level data storage
            if (cache_sets_[index]->is_line_valid(line_index) &&
                cache_sets_[index]->is_line_dirty(line_index)) {
                Data write_back_data = cache_sets_[index]->get_line_data(line_index);
                address_t write_back_tag = cache_sets_[index]->get_line_tag(line_index);
                address_t write_back_address =
                    get_address_from_index_and_tag(index, write_back_tag);
                next_level_data_storage_->write(write_back_address, write_back_data);
            }

            if (data.size() != cache_line_size_) {
                // partial write
                Data update_data = fill_data_from_next_level_data_storage(
                                       data, address, cache_line_size_)
                                       .data;

                cache_sets_[index]->update_line(line_index, tag, update_data, true,
                                                true);
                cache_sets_[index]->update_replacement_policy(line_index);

                DEBUG_PRINT(
                    "> %s w @ 0x%016llx : d=%s / i=%02lld / l=%04d - partial write to "
                    "evicted line\n",
                    name_.c_str(), address, update_data.to_string().c_str(), index,
                    line_index);

            } else {
                cache_sets_[index]->update_line(line_index, tag, data, true, true);
                cache_sets_[index]->update_replacement_policy(line_index);

                DEBUG_PRINT(
                    "> %s w @ 0x%016llx : d=%s / i=%02lld / l=%04d - full write to "
                    "evicted line\n",
                    name_.c_str(), address, data.to_string().c_str(), index,
                    line_index);
            }
        }
    }

    uint32_t hit_level = 0;
    latency_t latency = 0;

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

    // itreate over address_data_map and execute and aligned_write
    for (auto& [addr, d] : address_data_map) {
        auto dst = aligned_write(addr, d);
    }

    uint32_t hit_level = 0;
    latency_t latency = 0;

    DataStorageTransaction dst = {WRITE, address, latency, hit_level, data};

    return dst;
}

/**
 * @brief aligns a read transaction to the cache line size
 * @param address the address to align
 * @param num_bytes number of bytes to align
 * @return a map of aligned addresses and size of read
 */
std::map<address_t, size_t> SetAssociativeCache::align_read_transaction(
    address_t address, size_t num_bytes) {
    std::map<address_t, size_t> address_size_map;

    // check if data effects multiple sets and lines
    address_t offset = get_address_offset(address);
    address_t tag = get_address_tag(address);
    address_t index = get_address_index(address);

    // check if only one cache line is affected
    if (offset + num_bytes <= cache_line_size_) {
        address_size_map.insert({address, num_bytes});
        return address_size_map;
    }

    // split num_bytes into cache line size chunks
    // if offset != 0 first piece of data is a partial read
    uint32_t bytes_allocated = cache_line_size_ - offset;

    address_size_map.insert({address, bytes_allocated});

    while (bytes_allocated < num_bytes) {
        // check if there are bytes for a whole line
        if (num_bytes - bytes_allocated > cache_line_size_) {
            // full read
            address_size_map.insert({address + bytes_allocated, cache_line_size_});
            bytes_allocated += cache_line_size_;
        }

        else {
            // partial read
            address_size_map.insert(
                {address + bytes_allocated, num_bytes - bytes_allocated});
            // this line is needed to end the while loop
            bytes_allocated += cache_line_size_;
        }
    }

    return address_size_map;
}

DataStorageTransaction SetAssociativeCache::aligned_read(address_t address,
                                                         size_t num_bytes) {
    address_t offset = get_address_offset(address);
    address_t tag = get_address_tag(address);
    address_t index = get_address_index(address);

    Data read_data = Data(num_bytes);

    // check if target cache set already contains tag
    int32_t line_index = cache_sets_[index]->get_line_index_with_tag(tag);

    // TODO: may be move to CacheSet in the future
    if (line_index != -1) {
        // line with tag found -> hit -> read line
        Data line_data = cache_sets_[index]->get_line_data(line_index);
        cache_sets_[index]->update_replacement_policy(line_index);

        // copy data from to read_data
        for (int i = 0; i < num_bytes; i++) {
            read_data[i] = line_data[i + offset];
        }

#if DEBUG
        if (num_bytes < cache_line_size_) {
            // partial read
            DEBUG_PRINT(
                "> %s r @ 0x%016llx : d=%s / i=%02lld / l=%04d - partial read of "
                "cached line\n",
                name_.c_str(), address, read_data.to_string().c_str(), index,
                line_index);

        } else {
            // full read
            DEBUG_PRINT(
                "> %s r @ 0x%016llx : d=%s / i=%02lld / l=%04d - full read of chached "
                "line\n",
                name_.c_str(), address, read_data.to_string().c_str(), index,
                line_index);
        }
#endif

    } else {
        // line with tag not found -> miss -> read from next level storage
        // check if there is a free line
        line_index = cache_sets_[index]->get_free_line_index();

        address_t next_level_address = address - offset;
        Data next_level_storage_data =
            next_level_data_storage_->read(next_level_address, cache_line_size_).data;

        for (int i = 0; i < num_bytes; i++) {
            read_data[i] = next_level_storage_data[i + offset];
        }

        if (line_index != -1) {
            // free line found -> miss -> write to line
            cache_sets_[index]->update_line(line_index, tag, next_level_storage_data,
                                            true, false);
            cache_sets_[index]->update_replacement_policy(line_index);

#if DEBUG
            if (num_bytes < cache_line_size_) {
                // partial read
                DEBUG_PRINT(
                    "> %s r @ 0x%016llx : d=%s / i=%02lld / l=%04d - partial read of "
                    "not cached line\n",
                    name_.c_str(), address, read_data.to_string().c_str(), index,
                    line_index);
            } else {
                // full read
                DEBUG_PRINT(
                    "> %s r @ 0x%016llx : d=%s / i=%02lld / l=%04d - full read not of "
                    "cached line\n",
                    name_.c_str(), address, read_data.to_string().c_str(), index,
                    line_index);
            }
#endif

        } else {
            // no free line found -> evict line
            line_index = cache_sets_[index]->get_replacement_index();
            cache_sets_[index]->update_line(line_index, tag, next_level_storage_data,
                                            true, false);
            cache_sets_[index]->update_replacement_policy(line_index);

#if DEBUG
            if (num_bytes < cache_line_size_) {
                // partial read
                DEBUG_PRINT(
                    "> %s r @ 0x%016llx : d=%s / i=%02lld / l=%04d - partial read of "
                    "not cached line with eviction\n",
                    name_.c_str(), address, read_data.to_string().c_str(), index,
                    line_index);
            } else {
                // full read
                DEBUG_PRINT(
                    "> %s r @ 0x%016llx : d=%s / i=%02lld / l=%04d - full read of not "
                    "cached line with eviction\n",
                    name_.c_str(), address, read_data.to_string().c_str(), index,
                    line_index);
            }
#endif
        }
    }

    uint32_t hit_level = 0;
    latency_t latency = 0;

    std::cout << "read_data: " << read_data.to_string() << std::endl;

    DataStorageTransaction dst = {READ, address, latency, hit_level, read_data};

    std::cout << "dst.data: " << dst.data.to_string() << std::endl;

    return dst;
}

/**
 * @brief read data from cache
 * @param address the address to read from
 * @param num_bytes the number of bytes to read
 * @return bytes read from cache
 */
DataStorageTransaction SetAssociativeCache::read(address_t address, size_t num_bytes) {
    Data data = Data(num_bytes);

    std::map<address_t, size_t> address_size_map =
        align_read_transaction(address, num_bytes);

    for (auto& [addr, size] : address_size_map) {
        auto dst = aligned_read(addr, size);
    }

    uint32_t hit_level = 0;
    latency_t latency = 0;
    DataStorageTransaction dst = {READ, address, latency, hit_level, data};
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
    int32_t line_index = cache_sets_[index]->get_line_index_with_tag(tag);

    return (line_index != -1);
}

bool SetAssociativeCache::is_address_valid(address_t address) {
    address_t tag = get_address_tag(address);
    address_t index = get_address_index(address);

    // check if target cache set contains  with tag
    int32_t line_index = cache_sets_[index]->get_line_index_with_tag(tag);

    if (line_index == -1) {
        return false;
    }

    return cache_sets_[index]->is_line_valid(line_index);
}

bool SetAssociativeCache::is_address_dirty(address_t address) {
    address_t tag = get_address_tag(address);
    address_t index = get_address_index(address);

    // check if target cache set contains  with tag
    int32_t line_index = cache_sets_[index]->get_line_index_with_tag(tag);

    if (line_index == -1) {
        return false;
    }

    return cache_sets_[index]->is_line_dirty(line_index);
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
    int32_t line_index = cache_sets_[index]->get_line_index_with_tag(tag);

    if (line_index != -1) {
        Data cache_line_data = cache_sets_[index]->get_line_data(line_index);
        return cache_line_data.get<uint8_t>(offset);
    }
    return 0;
}

/**
 * @brief returns the data of a cache line. CAUTION: this method is intended for
 * debugging and should not be used in a simulation
 * @param cache_set_index the index of the cache set
 * @param line_index the index of the line
 * @return the data of the cache line
 */
Data SetAssociativeCache::get_cache_line_data(address_t cache_set_index,
                                              address_t line_index) {
    return cache_sets_[cache_set_index]->get_line_data(line_index);
}

/**
 * @brief returns the tag of a cache line. CAUTION: this method is intended for
 * debugging and should not be used in a simulation
 * @param cache_set_index the index of the cache set
 * @param line_index the index of the line
 * @return the tag of the cache line
 */
address_t SetAssociativeCache::get_cache_line_tag(address_t cache_set_index,
                                                  address_t line_index) {
    return cache_sets_[cache_set_index]->get_line_tag(line_index);
}

/**
 * @brief returns if a cache line is valid. CAUTION: this method is intended for
 * debugging and should not be used in a simulation
 * @param cache_set_index the index of the cache set
 * @param line_index the index of the line
 * @return if the cache line is valid
 */
bool SetAssociativeCache::is_cache_line_valid(address_t cache_set_index,
                                              address_t line_index) {
    return cache_sets_[cache_set_index]->is_line_valid(line_index);
}

/**
 * @brief returns if a cache line is dirty. CAUTION: this method is intended for
 * debugging and should not be used in a simulation
 * @param cache_set_index the index of the cache set
 * @param line_index the index of the line
 * @return if the cache line is dirty
 */
bool SetAssociativeCache::is_cache_line_dirty(address_t cache_set_index,
                                              address_t line_index) {
    return cache_sets_[cache_set_index]->is_line_dirty(line_index);
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
