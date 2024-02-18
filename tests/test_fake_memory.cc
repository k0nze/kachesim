#include <cassert>
#include <memory>
#include <vector>

#include "fake_memory.h"

int main() {
    auto fm = std::make_unique<FakeMemory>(1024);

    return 0;
}
