#ifndef LEAST_RECENTLY_USED_H
#define LEAST_RECENTLY_USED_H

#include <string>
#include <unordered_map>

#include "../doubly_linked_list/doubly_linked_list.h"
#include "replacement_policy.h"

namespace kachesim {
class LeastRecentlyUsed : public ReplacementPolicy {
public:
    LeastRecentlyUsed();
    ~LeastRecentlyUsed();

    void update(uint32_t index);
    uint32_t get_replacement_index();
    void remove(uint32_t index);

    std::string to_string();

private:
    DoublyLinkedList<uint32_t> dll_;
    std::unordered_map<uint32_t, std::shared_ptr<DoublyLinkedListNode<uint32_t>>> map_;

    uint64_t size_;
};
}  // namespace kachesim

#endif
