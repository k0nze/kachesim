#include "data.h"

Data::Data(uint32_t size) : size_(size) { data_ = new uint8_t[size_]; }

Data::Data(const std::vector<uint8_t>& data) {
    size_ = data.size();
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

Data::~Data() { delete[] data_; }

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

size_t Data::size() { return size_; }
