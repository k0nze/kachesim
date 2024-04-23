#include "cache_set.h"

#include <iostream>
#include <memory>

#include "common.h"
#include "replacement_policy/least_recently_used.h"

CacheSet::CacheSet(uint64_t cache_block_size, uint32_t ways,
                   ReplacementPolicyType replacement_policy_type) {
    blocks_.reserve(ways);

    for (int i = 0; i < ways; i++) {
        blocks_.push_back(
            std::unique_ptr<CacheBlock>(new CacheBlock(cache_block_size)));
    }

    switch (replacement_policy_type) {
        case ReplacementPolicyType::LRU:
            replacement_policy_ = std::make_shared<LeastRecentlyUsed>();
            break;
        default:
            THROW_INVALID_ARGUMENT("invalid ReplacementPolicyType");
            break;
    }
}

/**
 * @brief returns the index of a block in the cache set with the given tag
 * @return The index of block with given tag, -1 if no block with tag was found
 */
int32_t CacheSet::get_block_index_with_tag(uint64_t tag) {
    for (int i = 0; i < blocks_.size(); i++) {
        if (blocks_[i]->get_tag() == tag && blocks_[i]->is_valid()) {
            return i;
        }
    }

    return -1;
}

/**
 * @brief returns the index of a free block in the cache set
 * @return The index of a free block in the cache set, -1 if no block was found
 */
int32_t CacheSet::get_free_block_index() {
    for (int i = 0; i < blocks_.size(); i++) {
        if (!blocks_[i]->is_valid()) {
            return i;
        }
    }

    return -1;
}

Data CacheSet::get_block_data(uint32_t block_index) {
    return blocks_[block_index]->get_data();
}

uint64_t CacheSet::get_block_tag(uint32_t block_index) {
    return blocks_[block_index]->get_tag();
}

void CacheSet::update_block(uint32_t block_index, uint64_t tag, Data& data, bool valid,
                            bool dirty) {
    blocks_[block_index]->update(tag, data, valid, dirty);
}

bool CacheSet::is_block_valid(uint32_t block_index) {
    return blocks_[block_index]->is_valid();
}

bool CacheSet::is_block_dirty(uint32_t block_index) {
    return blocks_[block_index]->is_dirty();
}

void CacheSet::update_replacement_policy(uint32_t block_index) {
    replacement_policy_->update(block_index);
}

uint32_t CacheSet::get_replacement_index() {
    return replacement_policy_->get_replacement_index();
}
