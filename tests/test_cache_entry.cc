#include <cassert>
#include <iostream>

#include "cache_entry.h"

int main() {
    auto ce = std::make_unique<CacheEntry>();

    assert(ce->tag == 0);
    assert(ce->dirty == false);
}
