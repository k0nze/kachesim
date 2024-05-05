#include <cassert>
#include <iostream>

#include "kachesim/kachesim.h"

using namespace kachesim;

DataStorageTransaction test_function() {
    Data data = Data(9);

    for (int i = 0; i < 9; i++) {
        data[i] = i + 1;
    }

    DataStorageTransaction dst = {READ, 0x0042, 23, 1, data};
    return dst;
}

int main() {
    DataStorageTransaction dst = test_function();

    assert(dst.type == READ);
    assert(dst.address == 0x0042);
    assert(dst.latency == 23);
    assert(dst.hit_level == 1);

    for (int i = 0; i < 9; i++) {
        assert(dst.data[i] == i + 1);
    }

    std::cout << dst.data << std::endl;

    return 0;
}
