#ifndef CACHE_SET_H
#define CACHE_SET_H

#include <cstdint>
#include <memory>
#include <vector>

#include "cache_line.h"
#include "replacement_policy.h"

class CacheSet {
public:
    CacheSet(uint64_t cache_line_size, uint32_t ways);

    int32_t get_line_index_with_tag(uint64_t tag);
    int32_t get_free_line_index();

    Data get_line_data(uint32_t line_index);
    void update_line(uint32_t line_index, uint64_t tag, Data& data);

private:
    std::vector<std::unique_ptr<CacheLine>> lines_;
    std::unique_ptr<ReplacementPolicy> replacement_policy_;
};

#endif
