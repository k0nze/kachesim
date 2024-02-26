#ifndef CACHE_SET_H
#define CACHE_SET_H

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "cache_line.h"
#include "common.h"
#include "replacement_policy/least_recently_used.h"
#include "replacement_policy/replacement_policy.h"

class CacheSet {
public:
    CacheSet(uint64_t cache_line_size, uint32_t ways,
             ReplacementPolicyType replacement_policy_type) {
        lines_.reserve(ways);

        for (int i = 0; i < ways; i++) {
            lines_.push_back(
                std::unique_ptr<CacheLine>(new CacheLine(cache_line_size)));
        }

        switch (replacement_policy_type) {
            case ReplacementPolicyType::LRU:
                replacement_policy_ = std::make_unique<LeastRecentlyUsed>();
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
    int32_t get_line_index_with_tag(uint64_t tag) {
        for (int i = 0; i < lines_.size(); i++) {
            if (!lines_[i]->valid()) {
                return i;
            }
        }

        return -1;
    }

    /**
     * @brief returns the index of a free line in the cache set
     * @return The index of a free line in the cache set, -1 if no line was found
     */
    int32_t get_free_line_index() {
        for (int i = 0; i < lines_.size(); i++) {
            if (!lines_[i]->valid()) {
                return i;
            }
        }

        return -1;
    }

    Data get_line_data(uint32_t line_index) { return lines_[line_index]->get_data(); }

    void update_line(uint32_t line_index, uint64_t tag, Data& data, bool valid = true,
                     bool dirty = true) {
        lines_[line_index]->update(tag, data, valid, dirty);
    }

    void update_replacement_policy(uint32_t line_index) {
        replacement_policy_->update(line_index);
    }

    uint32_t get_replacement_index() {
        std::cout << replacement_policy_->to_string() << std::endl;
        return replacement_policy_->get_replacement_index();
    }

private:
    std::vector<std::unique_ptr<CacheLine>> lines_;
    std::unique_ptr<ReplacementPolicy> replacement_policy_;
};

#endif
