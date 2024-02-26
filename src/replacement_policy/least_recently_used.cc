#include "least_recently_used.h"

#include <sstream>

LeastRecentlyUsed::LeastRecentlyUsed() {}

void LeastRecentlyUsed::update(uint32_t index) {
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

uint32_t LeastRecentlyUsed::get_replacement_index() {
    // if the hashmap in not full return 0 as default replacement index
    return dll_.get_tail();
}

void LeastRecentlyUsed::remove(uint32_t index) {
    auto node = map_[index];
    dll_.remove(node);
    map_.erase(index);
    size_--;
}

std::string LeastRecentlyUsed::to_string() {
    std::stringstream ss;

    auto dll_nodes = dll_.get_nodes();

    for (auto const& dll_node : dll_nodes) {
        ss << dll_node->value << " ";
    }

    return ss.str();
}
