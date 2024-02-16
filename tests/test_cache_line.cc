#include <cassert>
#include <memory>

#include "cache_line.h"

int main() {
    auto cl = std::make_unique<CacheLine>(8);

    assert(cl->tag_ == 0);
    assert(cl->dirty_ == false);
    assert(cl->size() == 8);

    for (int i = 0; i < cl->size(); i++) {
        cl->operator[](i) = 255 - i;
    }

    for (int i = 0; i < cl->size(); i++) {
        assert(cl->operator[](i) == 255 - i);
    }

    return 0;
}
