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

size_t Data::size() { return size_; }
