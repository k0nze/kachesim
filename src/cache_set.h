#ifndef CACHE_SET_H
#define CACHE_SET_H

#include <cstdint>
#include <memory>
#include <vector>

#include "cache_block.h"
#include "replacement_policy/replacement_policy.h"

class CacheSet {
public:
    CacheSet(uint64_t cache_line_size, uint32_t ways,
             ReplacementPolicyType replacement_policy_type);

    int32_t get_line_index_with_tag(uint64_t tag);
    int32_t get_free_line_index();

    Data get_line_data(uint32_t line_index);
    uint64_t get_line_tag(uint32_t line_index);
    void update_line(uint32_t line_index, uint64_t tag, Data& data, bool valid = true,
                     bool dirty = true);

    bool is_line_valid(uint32_t line_index);
    bool is_line_dirty(uint32_t line_index);

    void update_replacement_policy(uint32_t line_index);
    uint32_t get_replacement_index();

private:
    std::vector<std::unique_ptr<CacheBlock>> lines_;
    std::shared_ptr<ReplacementPolicy> replacement_policy_;
};

#endif
