#ifndef REPLACEMENT_POLICY_H
#define REPLACEMENT_POLICY_H

#include <cstdint>

enum ReplacementPolicyType { LRU };

class ReplacementPolicy {
public:
    ReplacementPolicy(uint64_t indeces);

    void update(uint64_t index);
    uint64_t get_replacement_index();

protected:
    uint64_t indeces_;
};

#endif
