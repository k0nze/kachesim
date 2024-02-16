#include "cache_line.h"

CacheLine::CacheLine(uint64_t size) { data_ = std::vector<uint8_t>(size); }
