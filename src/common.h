#ifndef COMMON_H
#define COMMON_H

#include <climits>
#include <cmath>

/**
 * @author Siu Ching Pong -Asuka Kenji- https://stackoverflow.com/a/28703383
 */
template <typename R>
static constexpr R bitmask(unsigned int const onecount) {
    return static_cast<R>(-(onecount != 0)) &
           (static_cast<R>(-1) >> ((sizeof(R) * CHAR_BIT) - onecount));
}

uint32_t clog2(uint64_t x);

#endif
