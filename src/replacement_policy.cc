#include "replacement_policy.h"

ReplacementPolicy::ReplacementPolicy(uint64_t indeces) : indeces_(indeces) {}

void ReplacementPolicy::update(uint64_t index) {}

uint64_t ReplacementPolicy::get_replacement_index() { return 0; }
