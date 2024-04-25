#include <cassert>
#include <iostream>
#include <limits>
#include <memory>

#include "kachesim.h"

using namespace kachesim;

int main() {
    auto d1 = std::make_unique<Data>(1);

    d1->set<uint64_t>(0xbeef1234abcdef42);
    assert(d1->get<uint64_t>() == (uint64_t)0x42);

    d1->set<uint32_t>(0xbeef1234);
    assert(d1->get<uint32_t>() == (uint32_t)0x34);

    d1->set<uint16_t>(0xbeef);
    assert(d1->get<uint16_t>() == (uint16_t)0xef);

    d1->set<uint8_t>(0xab);
    assert(d1->get<uint8_t>() == (uint8_t)0xab);

    d1->set<int8_t>(-12);
    assert(d1->get<int8_t>() == (int8_t)-12);

    d1->set<uint8_t>(12);
    assert(d1->get<uint8_t>() == (uint8_t)12);

    d1->set<int16_t>(-4796);
    assert(d1->get<int16_t>() == (-4796 & 0xff));

    d1->set<uint16_t>(4796);
    assert(d1->get<uint16_t>() == (4796 & 0xff));

    d1->set<int32_t>(-71914);
    assert(d1->get<int32_t>() == (-71914 & 0xff));

    d1->set<uint32_t>(71914);
    assert(d1->get<uint32_t>() == (71914 & 0xff));

    d1->set<int64_t>(-4723642012);
    assert(d1->get<int32_t>() == (-4723642012 & 0xff));

    d1->set<uint64_t>(4723642012);
    assert(d1->get<uint32_t>() == (4723642012 & 0xff));

    auto d2 = std::make_unique<Data>(2);

    d2->set<uint64_t>(0xbeef1234abcdef42);
    assert(d2->get<uint64_t>() == (uint64_t)0xef42);

    d2->set<uint32_t>(0xbeef1234);
    assert(d2->get<uint32_t>() == (uint32_t)0x1234);

    d2->set<uint16_t>(0xbeef);
    assert(d2->get<uint16_t>() == (uint16_t)0xbeef);

    d2->set<uint8_t>(0xab);
    assert(d2->get<uint8_t>() == (uint8_t)0x00ab);

    d2->set<int8_t>(-12);
    assert(d2->get<int8_t>() == (int8_t)-12);

    d2->set<uint8_t>(12);
    assert(d2->get<uint8_t>() == (uint8_t)12);

    d2->set<int16_t>(-4796);
    assert(d2->get<int16_t>() == (int16_t)(-4796 & 0xffff));

    d2->set<uint16_t>(4796);
    assert(d2->get<uint16_t>() == (uint16_t)(4796 & 0xffff));

    d2->set<int32_t>(-71914);
    assert(d2->get<int32_t>() == (int32_t)(-71914 & 0xffff));

    d2->set<uint32_t>(71914);
    assert(d2->get<uint32_t>() == (uint32_t)(71914 & 0xffff));

    d2->set<int64_t>(-4723642012);
    assert(d2->get<int64_t>() == (int64_t)(-4723642012 & 0xffff));

    d2->set<uint64_t>(4723642012);
    assert(d2->get<uint64_t>() == (uint64_t)(4723642012 & 0xffff));

    auto d3 = std::make_unique<Data>(3);

    d3->set<uint64_t>(0xbeef1234abcdef42);
    assert(d3->get<uint64_t>() == 0xcdef42);

    d3->set<uint32_t>(0xbeef1234);
    assert(d3->get<uint32_t>() == (uint32_t)0x00ef1234);

    d3->set<uint16_t>(0xbeef);
    assert(d3->get<uint16_t>() == (uint16_t)0x00beef);

    d3->set<uint8_t>(0xab);
    assert(d3->get<uint8_t>() == (uint8_t)0x0000ab);

    d2->set<int8_t>(-12);
    assert(d2->get<int8_t>() == -12);

    d3->set<uint8_t>(12);
    assert(d3->get<uint8_t>() == 12);

    d3->set<int16_t>(-4796);
    assert(d3->get<int16_t>() == (int16_t)(-4796 & 0xffffff));

    d3->set<uint16_t>(4796);
    assert(d3->get<uint16_t>() == (4796 & 0xffffff));

    d3->set<int32_t>(-71914);
    assert(d3->get<int32_t>() == (int32_t)(-71914 & 0xffffff));

    d3->set<uint32_t>(71914);
    assert(d3->get<uint32_t>() == (71914 & 0xffffff));

    d3->set<int64_t>(-4723642012);
    assert(d3->get<int64_t>() == (int64_t)(-4723642012 & 0xffffff));

    d3->set<uint64_t>(4723642012);
    assert(d3->get<uint64_t>() == (4723642012 & 0xffffff));

    auto d4 = std::make_unique<Data>(4);

    d4->set<uint64_t>(0xbeef1234abcdef42);
    assert(d4->get<uint64_t>() == 0xabcdef42);

    d4->set<uint32_t>(0xbeef1234);
    assert(d4->get<uint32_t>() == 0xbeef1234);

    d4->set<uint16_t>(0xbeef);
    assert(d4->get<uint16_t>() == 0x0000beef);

    d4->set<uint8_t>(0xab);
    assert(d4->get<uint8_t>() == 0x000000ab);

    d4->set<int8_t>(-12);
    assert(d4->get<int8_t>() == -12);

    d4->set<uint8_t>(12);
    assert(d4->get<uint8_t>() == 12);

    d4->set<int16_t>(-4796);
    assert(d4->get<int16_t>() == -4796);

    d4->set<uint16_t>(4796);
    assert(d4->get<uint16_t>() == 4796);

    d4->set<int32_t>(-71914);
    assert(d4->get<int32_t>() == -71914);

    d4->set<uint32_t>(71914);
    assert(d4->get<uint32_t>() == 71914);

    d4->set<int64_t>(-4723642012);
    assert(d4->get<int64_t>() == (int64_t)(-4723642012 & 0xffffffff));

    d4->set<uint64_t>(4723642012);
    assert(d4->get<uint64_t>() == (4723642012 & 0xffffffff));

    auto d6 = std::make_unique<Data>(6);

    d6->set<uint64_t>(0xbeef1234abcdef42);
    assert(d6->get<uint64_t>() == 0x1234abcdef42);

    d6->set<uint32_t>(0xbeef1234);
    assert(d6->get<uint32_t>() == 0x0000beef1234);

    d6->set<uint16_t>(0xbeef);
    assert(d6->get<uint16_t>() == 0x00000000beef);

    d6->set<uint8_t>(0xab);
    assert(d6->get<uint8_t>() == 0x0000000000ab);

    d6->set<int8_t>(-12);
    assert(d6->get<int8_t>() == -12);

    d6->set<uint8_t>(12);
    assert(d6->get<uint8_t>() == 12);

    d6->set<int16_t>(-4796);
    assert(d6->get<int16_t>() == -4796);

    d6->set<uint16_t>(4796);
    assert(d6->get<uint16_t>() == 4796);

    d6->set<int32_t>(-71914);
    assert(d6->get<int32_t>() == -71914);

    d6->set<uint32_t>(71914);
    assert(d6->get<uint32_t>() == 71914);

    d6->set<int64_t>(-4723642012);
    assert(d6->get<int64_t>() == (int64_t)(-4723642012 & 0xffffffffffff));

    d6->set<uint64_t>(4723642012);
    assert(d6->get<uint64_t>() == (4723642012 & 0xffffffffffff));

    auto d8 = std::make_unique<Data>(8);

    d8->set<uint64_t>(0xbeef1234abcdef42);
    assert(d8->get<uint64_t>() == (uint64_t)0xbeef1234abcdef42);

    d8->set<uint32_t>(0xbeef1234);
    assert(d8->get<uint32_t>() == 0x00000000beef1234);

    d8->set<uint16_t>(0xbeef);
    assert(d8->get<uint16_t>() == 0x000000000000beef);

    d8->set<uint8_t>(0xab);
    assert(d8->get<uint8_t>() == 0x00000000000000ab);

    d8->set<int8_t>(-12);
    assert(d8->get<int8_t>() == -12);

    d8->set<uint8_t>(12);
    assert(d8->get<uint8_t>() == 12);

    d8->set<int16_t>(-4796);
    assert(d8->get<int16_t>() == -4796);

    d8->set<uint16_t>(4796);
    assert(d8->get<uint16_t>() == 4796);

    d8->set<int32_t>(-71914);
    assert(d8->get<int32_t>() == -71914);

    d8->set<uint32_t>(71914);
    assert(d8->get<uint32_t>() == 71914);

    d8->set<int64_t>(-4723642012);
    assert(d8->get<int64_t>() == -4723642012);

    d8->set<uint64_t>(4723642012);
    assert(d8->get<uint64_t>() == 4723642012);

    d8->set<uint32_t>(0x04030201, 1);
    assert(d8->get<uint64_t>(0) == 0x0403020100);
    assert(d8->get<uint64_t>(1) == 0x04030201);

    d8->set<float>(3.14);
    assert(d8->get<float>() > 3.14 - std::numeric_limits<float>::epsilon());
    assert(d8->get<float>() < 3.14 + std::numeric_limits<float>::epsilon());

    d8->set<float>(3.14, 1);
    assert(d8->get<float>(1) > 3.14 - std::numeric_limits<float>::epsilon());
    assert(d8->get<float>(1) < 3.14 + std::numeric_limits<float>::epsilon());

    d8->set<double>(42.23);
    assert(d8->get<double>() > 42.23 - std::numeric_limits<float>::epsilon());
    assert(d8->get<double>() < 42.23 + std::numeric_limits<float>::epsilon());

    auto d12 = std::make_unique<Data>(12);

    d12->set<double>(42.23, 3);
    assert(d12->get<double>(3) > 42.23 - std::numeric_limits<float>::epsilon());
    assert(d12->get<double>(3) < 42.23 + std::numeric_limits<float>::epsilon());

    auto a = std::make_unique<Data>(8);
    auto b = std::make_unique<Data>(8);

    a->set<uint64_t>(2342);
    b->set<uint64_t>(2342);

    assert(*a == *b);

    uint8_t* ds = new uint8_t[8];

    for (int i = 0; i < 8; i++) {
        ds[i] = i + 1;
    }

    auto c = std::make_unique<Data>(ds, 8);

    assert(c->get<uint64_t>() == 0x0807060504030201);

    auto d = Data(8);
    d[3] = 42;

    assert(d[3] == 42);
    assert(d.get<uint8_t>(3) == 42);

    return 0;
}
