#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <cstdint>
#include <memory>

template <typename T>
struct DoublyLinkedListNode {
    T value;
    std::shared_ptr<DoublyLinkedListNode<T>> next;
    std::shared_ptr<DoublyLinkedListNode<T>> prev;
};

class DoublyLinkedList {
public:
    DoublyLinkedList();

    std::shared_ptr<DoublyLinkedListNode<uint64_t>> insert_head(uint64_t value);
    std::shared_ptr<DoublyLinkedListNode<uint64_t>> insert_tail(uint64_t value);

    void remove(std::shared_ptr<DoublyLinkedListNode<uint64_t>> node);
    void move_to_head(std::shared_ptr<DoublyLinkedListNode<uint64_t>> node);
    void move_to_tail(std::shared_ptr<DoublyLinkedListNode<uint64_t>> node);

    uint64_t get_head() const;
    uint64_t get_tail() const;

    uint64_t size() const;

private:
    std::shared_ptr<DoublyLinkedListNode<uint64_t>> head_ = nullptr;
    std::shared_ptr<DoublyLinkedListNode<uint64_t>> tail_ = nullptr;
    uint64_t size_ = 0;
};

#endif
