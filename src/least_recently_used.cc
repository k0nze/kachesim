#include "least_recently_used.h"

#include "replacement_policy.h"

LeastRecentlyUsed::LeastRecentlyUsed(uint64_t indeces) : ReplacementPolicy(indeces) {}

void LeastRecentlyUsed::update(uint64_t index) {}

uint64_t LeastRecentlyUsed::get_replacement_index() { return 0; }
