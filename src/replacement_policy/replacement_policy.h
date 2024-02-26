#ifndef REPLACEMENT_POLICY_H
#define REPLACEMENT_POLICY_H

#include <cstdint>
#include <string>

typedef enum ReplacementPolicyType { LRU } ReplacementPolicyType;

class ReplacementPolicy {
public:
    virtual ~ReplacementPolicy() = 0;
    virtual void update(uint32_t index) = 0;
    virtual uint32_t get_replacement_index() = 0;

    virtual std::string to_string() = 0;
};

#endif
