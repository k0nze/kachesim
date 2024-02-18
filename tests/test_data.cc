#include <cassert>
#include <memory>

#include "data.h"

int main() {
    auto d = std::make_unique<Data>(9);

    return 0;
}
