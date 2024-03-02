#include "data.h"

#include <sstream>

Data::Data(uint32_t size) : size_(size) { data_ = new uint8_t[size_]; }

Data::Data(const std::vector<uint8_t>& data) {
    size_ = data.size();
    data_ = new uint8_t[size_];

    for (int i = 0; i < size_; i++) {
        data_[i] = data[i];
    }
}

Data::Data(const uint8_t* data, size_t size) {
    size_ = size;
    data_ = new uint8_t[size_];

    for (int i = 0; i < size_; i++) {
        data_[i] = data[i];
    }
}

Data::Data(const Data& data) {
    size_ = data.size_;
    data_ = new uint8_t[size_];

    for (int i = 0; i < size_; i++) {
        data_[i] = data[i];
    }
}

uint8_t Data::operator[](uint64_t index) const { return data_[index]; }

uint8_t& Data::operator[](uint64_t index) { return data_[index]; }

bool Data::operator==(Data& d) const {
    if (d.size() != size_) {
        return false;
    }

    for (int i = 0; i < size_; i++) {
        if (d[i] != data_[i]) {
            return false;
        }
    }
    return true;
}

bool Data::operator==(uint64_t i) { return get<uint64_t>() == i; }

std::string Data::to_string() {
    std::stringstream ss;

    ss << "0x";
    for (int i = size_ - 1; i >= 0; i--) {
        ss << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)data_[i];
        if (i != 0) {
            ss << "'";
        }
    }
    ss << std::dec;
    return ss.str();
}

size_t Data::size() const { return size_; }

Data::~Data() { delete[] data_; }
