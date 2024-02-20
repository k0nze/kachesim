#ifndef LEAST_RECENTLY_USED_H
#define LEAST_RECENTLY_USED_H

#include "replacement_policy.h"

class LeastRecentlyUsed : public ReplacementPolicy {
public:
    LeastRecentlyUsed(uint64_t indeces);

    void update(uint64_t index);
    uint64_t get_replacement_index();

private:
    uint64_t indeces_;
};

#endif
