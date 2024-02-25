#include "doubly_linked_list.h"

#include <iostream>

DoublyLinkedList::DoublyLinkedList() {}

uint64_t DoublyLinkedList::get_head() const { return head_->value; }

uint64_t DoublyLinkedList::get_tail() const { return tail_->value; }

uint64_t DoublyLinkedList::size() const { return size_; }

std::shared_ptr<DoublyLinkedListNode<uint64_t>> DoublyLinkedList::insert_head(
    uint64_t value) {
    auto new_node = std::make_shared<DoublyLinkedListNode<uint64_t>>();
    new_node->value = value;

    // check if list is empty
    if (head_ == nullptr) {
        // list is empty
        head_ = new_node;
        tail_ = new_node;
    } else {
        // list is not empty
        new_node->next = head_;
        head_->prev = new_node;
        head_ = new_node;
    }

    size_++;

    return new_node;
}

std::shared_ptr<DoublyLinkedListNode<uint64_t>> DoublyLinkedList::insert_tail(
    uint64_t value) {
    auto new_node = std::make_shared<DoublyLinkedListNode<uint64_t>>();
    new_node->value = value;

    // check if list is empty
    if (head_ == nullptr) {
        // list is empty
        head_ = new_node;
        tail_ = new_node;
    } else {
        // list is not empty
        new_node->prev = tail_;
        tail_->next = new_node;
        tail_ = new_node;
    }

    size_++;

    return new_node;
}

void DoublyLinkedList::remove(std::shared_ptr<DoublyLinkedListNode<uint64_t>> node) {
    // check if node is head
    if (head_ == node && tail_ == node) {
        // node is last node in list
        head_ = nullptr;
        tail_ = nullptr;
    } else if (head_ == node) {
        // node is head
        head_ = node->next;
        head_->prev = nullptr;
    } else if (tail_ == node) {
        // node is tail
        tail_ = node->prev;
        tail_->next = nullptr;
    } else {
        // node is in the middle
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    size_--;
}

void DoublyLinkedList::move_to_head(
    std::shared_ptr<DoublyLinkedListNode<uint64_t>> node) {
    // check if node is not head
    if (head_ != node) {
        // remove node from list
        remove(node);
        // insert node at head
        node->next = head_;
        head_->prev = node;
        head_ = node;
        size_++;
    }
}

void DoublyLinkedList::move_to_tail(
    std::shared_ptr<DoublyLinkedListNode<uint64_t>> node) {
    // check if node is not tail
    if (tail_ != node) {
        // remove node from list
        remove(node);
        // insert node at tail
        node->prev = tail_;
        tail_->next = node;
        tail_ = node;
        size_++;
    }
}
