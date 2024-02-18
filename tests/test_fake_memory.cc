#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "fake_memory.h"

int main() {
    auto fm = std::make_unique<FakeMemory>(65536);

    Data d1 = Data(1);
    d1.set<uint8_t>(42);

    fm->write(0, d1);
    auto dst = fm->read(0, 1);
    assert(dst.data->get<uint8_t>() == 42);

    return 0;
}
