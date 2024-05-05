#include "kachesim/common.h"

namespace kachesim {
uint32_t clog2(uint64_t x) { return (uint32_t)ceil(log2(x)); }
}  // namespace kachesim
