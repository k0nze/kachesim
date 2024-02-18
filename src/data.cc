#include "data.h"

Data::Data(uint32_t size) : size_(size) { data_ = std::vector<uint8_t>(size_); }

uint32_t Data::size() { return size_; }
