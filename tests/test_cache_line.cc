#include <cassert>
#include <iostream>

#include "cache_line.h"

int main() {
    auto cl = std::make_unique<CacheLine<8>>();

    assert(cl->tag == 0);
    assert(cl->dirty == false);
    assert(cl->data.size() == 8);
}
