#include <cassert>
#include <memory>

#include "set_associative_cache.h"

int main() {
    auto sac = std::make_unique<SetAssociativeCache>(4);

    assert(sac->sets_ == 4);
}
