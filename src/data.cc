#include "data.h"

#include <algorithm>

Data::Data(uint32_t size) : size_(size) { data_ = std::vector<uint8_t>(size_); }

uint32_t Data::size() { return size_; }

void Data::set_byte(uint8_t byte) {
    data_[0] = byte;

    // erase remaining bytes
    for (int i = 1; i < size_; i++) {
        data_[i] = 0;
    }
}

void Data::set_half_word(uint16_t half_word) {
    int max_byte = std::min({(uint32_t)2, size_});

    for (int i = 0; i < max_byte; i++) {
        data_[i] = (half_word >> (8 * i)) & 0xff;
    }

    // erase remaining bytes
    for (int i = max_byte; i < size_; i++) {
        data_[i] = 0;
    }
}

void Data::set_word(uint32_t word) {
    int max_byte = std::min({(uint32_t)4, size_});

    for (int i = 0; i < max_byte; i++) {
        data_[i] = (word >> (8 * i)) & 0xff;
    }

    // erase remaining bytes
    for (int i = max_byte; i < size_; i++) {
        data_[i] = 0;
    }
}

void Data::set_double_word(uint64_t double_word) {
    int max_byte = std::min({(uint32_t)8, size_});

    for (int i = 0; i < max_byte; i++) {
        data_[i] = (double_word >> (8 * i)) & 0xff;
    }

    // erase remaining bytes
    for (int i = max_byte; i < size_; i++) {
        data_[i] = 0;
    }
}

void Data::set_int8_t(int8_t a) { set_byte((uint8_t)a); }

void Data::set_uint8_t(uint8_t a) { set_byte(a); }

void Data::set_int16_t(int16_t a) { set_half_word((uint16_t)a); }

void Data::set_uint16_t(uint16_t a) { set_half_word(a); }

void Data::set_int32_t(int32_t a) { set_word((uint32_t)a); }

void Data::set_uint32_t(uint32_t a) { set_word(a); }

void Data::set_int64_t(int64_t a) { set_double_word((uint64_t)a); }

void Data::set_uint64_t(uint64_t a) { set_double_word(a); }

uint8_t Data::get_byte() {
    uint8_t byte = data_[0];
    return byte;
}

uint16_t Data::get_half_word() {
    int max_byte = std::min({(uint32_t)2, size_});

    uint16_t half_word = 0;

    for (int i = 0; i < max_byte; i++) {
        half_word |= ((uint16_t)data_[i]) << (8 * i);
    }

    return half_word;
}

uint32_t Data::get_word() {
    int max_byte = std::min({(uint32_t)4, size_});

    uint32_t word = 0;

    for (int i = 0; i < max_byte; i++) {
        word |= ((uint32_t)data_[i]) << (8 * i);
    }

    return word;
}

uint64_t Data::get_double_word() {
    int max_byte = std::min({(uint32_t)8, size_});

    uint64_t double_word = 0;

    for (int i = 0; i < max_byte; i++) {
        double_word |= ((uint64_t)data_[i]) << (8 * i);
    }

    return double_word;
}

int8_t Data::get_int8_t() { return (int8_t)get_byte(); }

uint8_t Data::get_uint8_t() { return get_byte(); }

int16_t Data::get_int16_t() { return (int16_t)get_half_word(); }

uint16_t Data::get_uint16_t() { return get_half_word(); }

int32_t Data::get_int32_t() { return (int32_t)get_word(); }

uint32_t Data::get_uint32_t() { return get_word(); }

int64_t Data::get_int64_t() { return (int64_t)get_double_word(); }

uint64_t Data::get_uint64_t() { return get_double_word(); }
