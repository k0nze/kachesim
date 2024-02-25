#include <cassert>
#include <memory>
#include <unordered_map>

#include "least_recently_used.h"

int main() {
    auto lru = std::make_unique<LeastRecentlyUsed>();

    lru->update(1);
    lru->update(2);
    lru->update(3);
    lru->update(4);
    lru->update(5);
    lru->update(6);

    assert(lru->get_replacement_index() == 1);

    lru->update(4);
    assert(lru->get_replacement_index() == 1);

    lru->update(1);
    assert(lru->get_replacement_index() == 2);

    lru->remove(2);
    assert(lru->get_replacement_index() == 3);

    return 0;
}
