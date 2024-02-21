#ifndef COMMON_H
#define COMMON_H

#include <climits>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

/**
 * @author Siu Ching Pong -Asuka Kenji- https://stackoverflow.com/a/28703383
 */
template <typename R>
static constexpr R bitmask(unsigned int const onecount) {
    return static_cast<R>(-(onecount != 0)) &
           (static_cast<R>(-1) >> ((sizeof(R) * CHAR_BIT) - onecount));
}

/**
 * @author Kornel Kisielewicz https://stackoverflow.com/a/5100745
 */
template <typename T>
std::string int_to_hex(T i) {
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << i;
    return stream.str();
}

uint32_t clog2(uint64_t x);

#define THROW_OUT_OF_RANGE(msg)                                                   \
    throw std::out_of_range((std::string(msg)) + " at " + std::string(__FILE__) + \
                            ":" + std::to_string(__LINE__));

#define THROW_INVALID_ARGUMENT(msg)                                                   \
    throw std::invalid_argument((std::string(msg)) + " at " + std::string(__FILE__) + \
                                ":" + std::to_string(__LINE__));

#endif
