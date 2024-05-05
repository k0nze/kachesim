#include <cassert>
#include <iostream>
#include <memory>

#include "kachesim/kachesim.h"

using namespace kachesim;

int main() {
    auto cs = std::make_unique<CacheSet>(64, 4, ReplacementPolicyType::LRU);

    return 0;
}
