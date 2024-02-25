#ifndef LEAST_RECENTLY_USED_H
#define LEAST_RECENTLY_USED_H

#include <unordered_map>

#include "../doubly_linked_list/doubly_linked_list.h"
#include "replacement_policy.h"

class LeastRecentlyUsed : public ReplacementPolicy {
public:
    LeastRecentlyUsed();

    void update(uint32_t index);
    uint32_t get_replacement_index();
    void remove(uint32_t index);

private:
    DoublyLinkedList<uint32_t> dll_;
    std::unordered_map<uint32_t, std::shared_ptr<DoublyLinkedListNode<uint32_t>>> map_;
    std::unordered_map<int, int> map2_;

    uint64_t size_;
};

#endif
