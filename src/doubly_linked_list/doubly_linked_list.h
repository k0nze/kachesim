#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <cstdint>
#include <memory>
#include <vector>

template <typename T>
struct DoublyLinkedListNode {
    T value;
    std::shared_ptr<DoublyLinkedListNode<T>> next;
    std::shared_ptr<DoublyLinkedListNode<T>> prev;
};

template <typename T>
class DoublyLinkedList {
public:
    DoublyLinkedList(){};

    std::shared_ptr<DoublyLinkedListNode<T>> insert_head(T value) {
        auto new_node = std::make_shared<DoublyLinkedListNode<T>>();
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

    std::shared_ptr<DoublyLinkedListNode<T>> insert_tail(T value) {
        auto new_node = std::make_shared<DoublyLinkedListNode<T>>();
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

    void remove(std::shared_ptr<DoublyLinkedListNode<T>> node) {
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

    void move_to_head(std::shared_ptr<DoublyLinkedListNode<T>> node) {
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

    void move_to_tail(std::shared_ptr<DoublyLinkedListNode<T>> node) {
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

    T get_head() const { return head_->value; };
    T get_tail() const { return tail_->value; };

    std::vector<std::shared_ptr<DoublyLinkedListNode<T>>> get_nodes() {
        std::vector<std::shared_ptr<DoublyLinkedListNode<T>>> nodes;

        auto current_node = head_;

        if (current_node != nullptr) {
            while (current_node->next != nullptr) {
                nodes.push_back(current_node);
                current_node = current_node->next;
            }
        }

        return nodes;
    }

    uint64_t size() const { return size_; };

private:
    std::shared_ptr<DoublyLinkedListNode<T>> head_ = nullptr;
    std::shared_ptr<DoublyLinkedListNode<T>> tail_ = nullptr;
    uint64_t size_ = 0;
};

#endif
