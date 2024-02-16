#include <cassert>

#include "cache_line.h"

int main() {
    auto cl = std::make_unique<CacheLine<8>>();

    assert(cl->tag == 0);
    assert(cl->dirty == false);
    assert(cl->data.size() == 8);

    for (int i = 0; i < cl->data.size(); i++) {
        cl->data[i] = 255 - i;
    }

    for (int i = 0; i < cl->data.size(); i++) {
        assert(cl->data[i] == 255 - i);
    }
}
