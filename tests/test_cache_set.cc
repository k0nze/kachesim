#include <cassert>
#include <iostream>
#include <memory>

#include "cache_set.h"

int main() {
    auto cs = std::make_unique<CacheSet>(64, 4);

    return 0;
}
