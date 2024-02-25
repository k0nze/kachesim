#ifndef REPLACEMENT_POLICY_H
#define REPLACEMENT_POLICY_H

#include <cstdint>

typedef enum ReplacementPolicyType { LRU } ReplacementPolicyType;

class ReplacementPolicy {
public:
    ReplacementPolicy();

    void update(uint64_t index);
    uint64_t get_replacement_index();
};

#endif
