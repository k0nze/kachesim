#include "cache_set.h"

#include <iostream>
#include <memory>

#include "common.h"
#include "least_recently_used.h"

CacheSet::CacheSet(uint64_t cache_line_size, uint32_t ways,
                   ReplacementPolicyType replacement_policy_type) {
    lines_.reserve(ways);

    for (int i = 0; i < ways; i++) {
        lines_.push_back(std::unique_ptr<CacheLine>(new CacheLine(cache_line_size)));
    }

    switch (replacement_policy_type) {
        case ReplacementPolicyType::LRU:
            replacement_policy_ = std::make_unique<LeastRecentlyUsed>(ways);
            break;
        default:
            THROW_INVALID_ARGUMENT("invalid ReplacementPolicyType");
            break;
    }
}

/**
 * @brief returns the index of a line in the cache set with the given tag
 * @return The index of line with given tag, -1 if no line with tag was found
 */
int32_t CacheSet::get_line_index_with_tag(uint64_t tag) {
    for (int i = 0; i < lines_.size(); i++) {
        if (lines_[i]->get_tag() == tag) {
            return i;
        }
    }

    return -1;
}

/**
 * @brief returns the index of a free line in the cache set
 * @return The index of a free line in the cache set, -1 if no line was found
 */
int32_t CacheSet::get_free_line_index() {
    for (int i = 0; i < lines_.size(); i++) {
        if (!lines_[i]->valid()) {
            return i;
        }
    }

    return -1;
}

Data CacheSet::get_line_data(uint32_t line_index) {
    return lines_[line_index]->get_data();
}

void CacheSet::update_line(uint32_t line_index, uint64_t tag, Data& data) {
    lines_[line_index]->update(tag, data);
}

void CacheSet::update_replacement_policy(uint32_t line_index) {
    replacement_policy_->update(line_index);
}

uint32_t CacheSet::get_replacement_index() {
    return replacement_policy_->get_replacement_index();
}
