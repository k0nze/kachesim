#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#include "fake_memory.h"

int main() {
    auto fm = std::make_unique<FakeMemory>(1024);

    // write a single byte to each address
    for (int i = 0; i < fm->size(); i++) {
        std::vector<uint8_t> data;
        data.push_back(i);
        fm->write(i, data);
    }

    // read a single byte from each address
    // and check if the data is correct
    for (int i = 0; i < fm->size(); i++) {
        std::vector<uint8_t> data = fm->read(i, 1);
        assert(data[0] == (i % 256));
    }

    return 0;
}
