#include <cassert>
#include <iostream>
#include <memory>

#include "kachesim.h"

using namespace kachesim;

int main() {
    auto dll = std::make_unique<DoublyLinkedList<uint64_t>>();

    auto node_1 = dll->insert_head(1);
    auto node_2 = dll->insert_head(2);
    auto node_3 = dll->insert_head(3);
    auto node_4 = dll->insert_head(4);

    auto nodes = dll->get_nodes();
    assert(nodes.size() == 4);

    assert(dll->size() == 4);
    assert(dll->get_head() == 4);
    assert(dll->get_tail() == 1);

    auto node_5 = dll->insert_tail(5);

    assert(dll->size() == 5);
    assert(dll->get_head() == 4);
    assert(dll->get_tail() == 5);

    dll->remove(node_3);

    assert(dll->size() == 4);
    assert(dll->get_head() == 4);
    assert(dll->get_tail() == 5);

    dll->remove(node_4);
    assert(dll->size() == 3);
    assert(dll->get_head() == 2);
    assert(dll->get_tail() == 5);

    dll->remove(node_5);
    assert(dll->size() == 2);
    assert(dll->get_head() == 2);
    assert(dll->get_tail() == 1);

    dll->remove(node_1);
    assert(dll->size() == 1);
    assert(dll->get_head() == 2);
    assert(dll->get_tail() == 2);

    dll->remove(node_2);
    assert(dll->size() == 0);

    auto node_6 = dll->insert_head(6);
    auto node_7 = dll->insert_head(7);
    auto node_8 = dll->insert_head(8);
    auto node_9 = dll->insert_head(9);

    assert(dll->size() == 4);
    assert(dll->get_head() == 9);
    assert(dll->get_tail() == 6);

    dll->move_to_head(node_9);
    assert(dll->size() == 4);
    assert(dll->get_head() == 9);
    assert(dll->get_tail() == 6);

    dll->move_to_head(node_7);
    assert(dll->size() == 4);
    assert(dll->get_head() == 7);
    assert(dll->get_tail() == 6);

    dll->move_to_head(node_6);
    assert(dll->size() == 4);
    assert(dll->get_head() == 6);
    assert(dll->get_tail() == 8);

    dll->move_to_tail(node_7);
    assert(dll->size() == 4);
    assert(dll->get_head() == 6);
    assert(dll->get_tail() == 7);

    dll->move_to_tail(node_6);
    assert(dll->size() == 4);
    assert(dll->get_head() == 9);
    assert(dll->get_tail() == 6);

    dll->move_to_tail(node_6);
    assert(dll->size() == 4);
    assert(dll->get_head() == 9);
    assert(dll->get_tail() == 6);

    return 0;
}
