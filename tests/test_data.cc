#include <cassert>
#include <iostream>
#include <memory>

#include "data.h"

int main() {
    auto d1 = std::make_unique<Data>(1);

    d1->set_double_word(0xbeef1234abcdef42);
    assert(d1->get_double_word() == 0x42);

    d1->set_word(0xbeef1234);
    assert(d1->get_word() == 0x34);

    d1->set_half_word(0xbeef);
    assert(d1->get_half_word() == 0xef);

    d1->set_byte(0xab);
    assert(d1->get_byte() == 0xab);

    d1->set_int8_t(-12);
    assert(d1->get_int8_t() == -12);

    d1->set_uint8_t(12);
    assert(d1->get_uint8_t() == 12);

    d1->set_int16_t(-4796);
    assert(d1->get_int16_t() == (-4796 & 0xff));

    d1->set_uint16_t(4796);
    assert(d1->get_uint16_t() == (4796 & 0xff));

    d1->set_int32_t(-71914);
    assert(d1->get_int32_t() == (-71914 & 0xff));

    d1->set_uint32_t(71914);
    assert(d1->get_uint32_t() == (71914 & 0xff));

    d1->set_int64_t(-4723642012);
    assert(d1->get_int32_t() == (-4723642012 & 0xff));

    d1->set_uint64_t(4723642012);
    assert(d1->get_uint32_t() == (4723642012 & 0xff));

    auto d2 = std::make_unique<Data>(2);

    d2->set_double_word(0xbeef1234abcdef42);
    assert(d2->get_double_word() == 0xef42);

    d2->set_word(0xbeef1234);
    assert(d2->get_word() == 0x1234);

    d2->set_half_word(0xbeef);
    assert(d2->get_half_word() == 0xbeef);

    d2->set_byte(0xab);
    assert(d2->get_byte() == 0x00ab);

    d2->set_int8_t(-12);
    assert(d2->get_int8_t() == -12);

    d2->set_uint8_t(12);
    assert(d2->get_uint8_t() == 12);

    d2->set_int16_t(-4796);
    assert(d2->get_int16_t() == (int16_t)(-4796 & 0xffff));

    d2->set_uint16_t(4796);
    assert(d2->get_uint16_t() == (4796 & 0xffff));

    d2->set_int32_t(-71914);
    assert(d2->get_int32_t() == (int32_t)(-71914 & 0xffff));

    d2->set_uint32_t(71914);
    assert(d2->get_uint32_t() == (71914 & 0xffff));

    d2->set_int64_t(-4723642012);
    assert(d2->get_int64_t() == (int64_t)(-4723642012 & 0xffff));

    d2->set_uint64_t(4723642012);
    assert(d2->get_uint64_t() == (4723642012 & 0xffff));

    auto d3 = std::make_unique<Data>(3);

    d3->set_double_word(0xbeef1234abcdef42);
    assert(d3->get_double_word() == 0xcdef42);

    d3->set_word(0xbeef1234);
    assert(d3->get_word() == 0x00ef1234);

    d3->set_half_word(0xbeef);
    assert(d3->get_half_word() == 0x00beef);

    d3->set_byte(0xab);
    assert(d3->get_byte() == 0x0000ab);

    d2->set_int8_t(-12);
    assert(d2->get_int8_t() == -12);

    d3->set_uint8_t(12);
    assert(d3->get_uint8_t() == 12);

    d3->set_int16_t(-4796);
    assert(d3->get_int16_t() == (int16_t)(-4796 & 0xffffff));

    d3->set_uint16_t(4796);
    assert(d3->get_uint16_t() == (4796 & 0xffffff));

    d3->set_int32_t(-71914);
    assert(d3->get_int32_t() == (int32_t)(-71914 & 0xffffff));

    d3->set_uint32_t(71914);
    assert(d3->get_uint32_t() == (71914 & 0xffffff));

    d3->set_int64_t(-4723642012);
    assert(d3->get_int64_t() == (int64_t)(-4723642012 & 0xffffff));

    d3->set_uint64_t(4723642012);
    assert(d3->get_uint64_t() == (4723642012 & 0xffffff));

    auto d4 = std::make_unique<Data>(4);

    d4->set_double_word(0xbeef1234abcdef42);
    assert(d4->get_double_word() == 0xabcdef42);

    d4->set_word(0xbeef1234);
    assert(d4->get_word() == 0xbeef1234);

    d4->set_half_word(0xbeef);
    assert(d4->get_half_word() == 0x0000beef);

    d4->set_byte(0xab);
    assert(d4->get_byte() == 0x000000ab);

    d4->set_int8_t(-12);
    assert(d4->get_int8_t() == -12);

    d4->set_uint8_t(12);
    assert(d4->get_uint8_t() == 12);

    d4->set_int16_t(-4796);
    assert(d4->get_int16_t() == -4796);

    d4->set_uint16_t(4796);
    assert(d4->get_uint16_t() == 4796);

    d4->set_int32_t(-71914);
    assert(d4->get_int32_t() == -71914);

    d4->set_uint32_t(71914);
    assert(d4->get_uint32_t() == 71914);

    d4->set_int64_t(-4723642012);
    assert(d4->get_int64_t() == (int64_t)(-4723642012 & 0xffffffff));

    d4->set_uint64_t(4723642012);
    assert(d4->get_uint64_t() == (4723642012 & 0xffffffff));

    auto d6 = std::make_unique<Data>(6);

    d6->set_double_word(0xbeef1234abcdef42);
    assert(d6->get_double_word() == 0x1234abcdef42);

    d6->set_word(0xbeef1234);
    assert(d6->get_word() == 0x0000beef1234);

    d6->set_half_word(0xbeef);
    assert(d6->get_half_word() == 0x00000000beef);

    d6->set_byte(0xab);
    assert(d6->get_byte() == 0x0000000000ab);

    d6->set_int8_t(-12);
    assert(d6->get_int8_t() == -12);

    d6->set_uint8_t(12);
    assert(d6->get_uint8_t() == 12);

    d6->set_int16_t(-4796);
    assert(d6->get_int16_t() == -4796);

    d6->set_uint16_t(4796);
    assert(d6->get_uint16_t() == 4796);

    d6->set_int32_t(-71914);
    assert(d6->get_int32_t() == -71914);

    d6->set_uint32_t(71914);
    assert(d6->get_uint32_t() == 71914);

    d6->set_int64_t(-4723642012);
    assert(d6->get_int64_t() == (int64_t)(-4723642012 & 0xffffffffffff));

    d6->set_uint64_t(4723642012);
    assert(d6->get_uint64_t() == (4723642012 & 0xffffffffffff));

    auto d8 = std::make_unique<Data>(8);

    d8->set_double_word(0xbeef1234abcdef42);
    assert(d8->get_double_word() == 0xbeef1234abcdef42);

    d8->set_word(0xbeef1234);
    assert(d8->get_word() == 0x00000000beef1234);

    d8->set_half_word(0xbeef);
    assert(d8->get_half_word() == 0x000000000000beef);

    d8->set_byte(0xab);
    assert(d8->get_byte() == 0x00000000000000ab);

    d8->set_int8_t(-12);
    assert(d8->get_int8_t() == -12);

    d8->set_uint8_t(12);
    assert(d8->get_uint8_t() == 12);

    d8->set_int16_t(-4796);
    assert(d8->get_int16_t() == -4796);

    d8->set_uint16_t(4796);
    assert(d8->get_uint16_t() == 4796);

    d8->set_int32_t(-71914);
    assert(d8->get_int32_t() == -71914);

    d8->set_uint32_t(71914);
    assert(d8->get_uint32_t() == 71914);

    d8->set_int64_t(-4723642012);
    assert(d8->get_int64_t() == -4723642012);

    d8->set_uint64_t(4723642012);
    assert(d8->get_uint64_t() == 4723642012);

    return 0;
}
