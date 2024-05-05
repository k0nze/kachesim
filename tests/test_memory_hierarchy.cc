#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "doubly_linked_list/doubly_linked_list.h"
#include "kachesim.h"

using namespace kachesim;

std::string read_file_into_string(const std::string& filename) {
    if (!std::filesystem::exists(filename)) {
        throw std::invalid_argument("File " + filename + " does not exist.");
    }

    std::ifstream file(filename);

    std::stringstream buffer;
    buffer << file.rdbuf();  // Read the entire file into the stringstream

    return buffer.str();  // Return the content of the stringstream as a string
}

int main() {
    std::string yaml_config_string;
    yaml_config_string = read_file_into_string("../data/memory_hierarchy0.yaml");

    auto mh0 = std::make_unique<MemoryHierarchy>(yaml_config_string);

    auto addresses = DoublyLinkedList<uint64_t>();
    std::map<address_t, uint8_t> address_data_map;

    // test full read of top level memory through set associative caches
    // fill test top level memory with random values
    for (int i = 0; i < mh0->top_level_memory->size(); i++) {
        uint8_t random_int = std::rand() % 256;
        mh0->top_level_memory->set(i, random_int);
    }

    // generate ddl with all fake memory addresses
    for (int i = 0; i < mh0->top_level_memory->size(); i++) {
        addresses.insert_tail(i);
    }

    // read all addresses from top level memory through set associative caches
    while (!addresses.empty()) {
        auto address_nodes = addresses.get_nodes();
        size_t node_index = std::rand() % address_nodes.size();
        auto node = address_nodes[node_index];
        uint64_t address = node->value;
        addresses.remove(node);
        auto read_dst = mh0->read(address, 1);

        assert(read_dst.type == DataStorageTransactionType::READ);
        assert(read_dst.address == address);
        assert(read_dst.data.get<uint8_t>() == mh0->top_level_memory->get(address));
    }

    // test full write to top level memory through set associative caches
    // generate data to write to fake memory
    for (int i = 0; i < mh0->top_level_memory->size(); i++) {
        uint8_t random_int = std::rand() % 256;
        address_data_map.insert({i, random_int});
    }

    // generate ddl with all top level memory addresses
    addresses = DoublyLinkedList<uint64_t>();
    for (int i = 0; i < mh0->top_level_memory->size(); i++) {
        addresses.insert_tail(i);
    }

    while (!addresses.empty()) {
        auto address_nodes = addresses.get_nodes();
        size_t node_index = std::rand() % address_nodes.size();
        auto node = address_nodes[node_index];
        uint64_t address = node->value;
        addresses.remove(node);
        Data write_data = Data(1);
        write_data.set<uint8_t>(address_data_map.at(address));
        auto write_dst = mh0->write(address, write_data);

        assert(write_dst.type == DataStorageTransactionType::WRITE);
        assert(write_dst.address == address);
        assert(write_dst.data.get<uint8_t>() == write_data.get<uint8_t>());
    }

    mh0->flush_all_caches();

    // check top level memory values
    for (int i = 0; i < mh0->top_level_memory->size(); i++) {
        assert(mh0->top_level_memory->get(i) == address_data_map.at(i));
    }

    return 0;
}
