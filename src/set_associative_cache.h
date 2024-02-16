#ifndef SET_ASSOCIATIVE_CACHE_H
#define SET_ASSOCIATIVE_CACHE_H

#include <cstdint>

class SetAssociativeCache {
public:
    SetAssociativeCache(uint32_t sets);

    uint32_t sets_;
};

#endif
