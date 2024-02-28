#include "cache_line.h"

#include <string>

#include "common.h"

CacheLine::CacheLine(uint64_t size) : size_(size) { reset(); }
CacheLine::~CacheLine() { delete[] data_; }

void CacheLine::set_valid() { valid_ = true; }

void CacheLine::set_unvalid() { valid_ = false; }

bool CacheLine::is_valid() { return valid_; }

void CacheLine::set_dirty() { dirty_ = true; }

void CacheLine::set_not_dirty() { dirty_ = false; }

bool CacheLine::is_dirty() { return dirty_; }

/**
 * update the cache line with the given tag and data, sets the cache line valid and
 * dirty
 * @param tag the tag of the data
 * @param data the data to stored
 * @throws std::out_of_range if the size of the data does not match the size of the
 * cache line
 */
void CacheLine::update(uint64_t tag, Data& data, bool valid, bool dirty) {
    if (data.size() != size_) {
        std::string err_msg = std::string("data size with tag: ") +
                              int_to_hex<uint64_t>(tag) +
                              std::string("does not match cache line size.");
        THROW_OUT_OF_RANGE(err_msg);
    }

    tag_ = tag;
    valid_ = valid;
    dirty_ = dirty;

    // copy data into data_
    for (int i = 0; i < size_; i++) {
        data_[i] = data[i];
    }
}

uint64_t CacheLine::get_tag() { return tag_; }

Data CacheLine::get_data() {
    auto d = Data(data_, size_);
    return d;
}

void CacheLine::reset() { data_ = new uint8_t[size_]; }
