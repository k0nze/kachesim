#include "data.h"

Data::Data(uint32_t size) : size_(size) { data_ = new uint8_t[size_]; }
Data::~Data() { delete[] data_; }

size_t Data::size() { return size_; }
