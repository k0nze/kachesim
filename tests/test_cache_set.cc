#include <cassert>
#include <iostream>
#include <memory>

#include "cache_set.h"
#include "replacement_policy/replacement_policy.h"

using namespace kachesim;

int main() {
    auto cs = std::make_unique<CacheSet>(64, 4, ReplacementPolicyType::LRU);

    return 0;
}
