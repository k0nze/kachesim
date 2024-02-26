#include <iostream>
#include <memory>  // Include the memory header for shared_ptr
#include <string>
#include <vector>

#include "../src/replacement_policy/least_recently_used.h"
#include "../src/replacement_policy/replacement_policy.h"

int main() {
    std::vector<std::unique_ptr<ReplacementPolicy>> v;

    v.push_back(std::make_unique<LeastRecentlyUsed>());

    v[0]->update(1);
    v[0]->update(2);
    v[0]->update(3);
    v[0]->update(1);

    std::cout << v[0]->get_replacement_index() << std::endl;

    return 0;
}
