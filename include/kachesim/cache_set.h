#ifndef CACHE_SET_H
#define CACHE_SET_H

#include <cstdint>
#include <memory>
#include <vector>

#include "kachesim/cache_block.h"
#include "replacement_policy/replacement_policy.h"

namespace kachesim {
class CacheSet {
public:
    CacheSet(uint64_t cache_block_size, uint32_t ways,
             ReplacementPolicyType replacement_policy_type);

    int32_t get_block_index_with_tag(uint64_t tag);
    int32_t get_free_block_index();

    Data get_block_data(uint32_t block_index);
    uint64_t get_block_tag(uint32_t block_index);
    void update_block(uint32_t block_index, uint64_t tag, Data& data, bool valid = true,
                      bool dirty = true);

    bool is_block_valid(uint32_t block_index);
    bool is_block_dirty(uint32_t block_index);

    void update_replacement_policy(uint32_t block_index);
    uint32_t get_replacement_index();

private:
    std::vector<std::unique_ptr<CacheBlock>> blocks_;
    std::shared_ptr<ReplacementPolicy> replacement_policy_;
};
}  // namespace kachesim

#endif
