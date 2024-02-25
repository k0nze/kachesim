#include "least_recently_used.h"

#include "replacement_policy.h"

LeastRecentlyUsed::LeastRecentlyUsed() {}

void LeastRecentlyUsed::update(uint64_t index) {
    // check if index is in the map_
    if (map_.find(index) == map_.end()) {
        // index is not in map
        auto node = dll_.insert_head(index);
        map_.insert({index, node});
        size_++;
    } else {
        // index is in map
        auto node = map_[index];
        dll_.move_to_head(node);
    }
}

uint64_t LeastRecentlyUsed::get_replacement_index() {
    // if the hashmap in not full return 0 as default replacement index
    return dll_.get_tail();
}

void LeastRecentlyUsed::remove(uint64_t index) {
    auto node = map_[index];
    dll_.remove(node);
    map_.erase(index);
    size_--;
}
