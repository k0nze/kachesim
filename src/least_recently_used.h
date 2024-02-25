#ifndef LEAST_RECENTLY_USED_H
#define LEAST_RECENTLY_USED_H

#include <unordered_map>

#include "doubly_linked_list/doubly_linked_list.h"
#include "replacement_policy.h"

class LeastRecentlyUsed : public ReplacementPolicy {
public:
    LeastRecentlyUsed();

    void update(uint64_t index);
    uint64_t get_replacement_index();
    void remove(uint64_t index);

private:
    DoublyLinkedList<uint64_t> dll_;
    std::unordered_map<uint64_t, std::shared_ptr<DoublyLinkedListNode<uint64_t>>> map_;
    std::unordered_map<int, int> map2_;

    uint64_t size_;
};

#endif
