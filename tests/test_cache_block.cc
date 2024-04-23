#include <cassert>
#include <iostream>
#include <memory>

#include "cache_block.h"

int main() {
    auto cl = std::make_unique<CacheBlock>(8);

    // check that cache block is not valid by default
    assert(!cl->is_valid());

    /*
    assert(cl->size() == 8);
    assert(cl->valid() == false);
    assert(cl->dirty() == false);
    assert(cl->get_tag() == 0);

    auto d = cl->get_data();
    assert(d.size() == 8);

    d[1] = 0x01;
    uint64_t tag = 42;

    cl->update(tag, d);

    auto e = cl->get_data();

    std::cout << std::hex << d.get<uint64_t>() << std::endl;
    std::cout << std::hex << e.get<uint64_t>() << std::endl;
    assert(d == e);

    assert(cl->get_tag() == tag);
    assert(cl->valid() == true);
    assert(cl->dirty() == true);
    */

    return 0;
}
