#include <cassert>
#include <memory>
#include <vector>

#include "set_associative_cache.h"

int main() {
    auto sac = std::make_unique<SetAssociativeCache>(false, true, 10, 5, 8, 4, 2);

    assert(sac->write_allocate_ == false);
    assert(sac->write_back_ == true);
    assert(sac->miss_latency_ == 10);
    assert(sac->hit_latency_ == 5);
    assert(sac->cache_line_size_ == 8);
    assert(sac->sets_ == 4);
    assert(sac->ways_ == 2);

    assert(sac->size() == 8 * 4 * 2);

    std::vector<uint8_t> data;
    data.push_back(1);
    data.push_back(2);
    data.push_back(3);
    data.push_back(4);

    sac->write(0xdeadbeef, data);

    return 0;
}
