#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

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

    return 0;
}
