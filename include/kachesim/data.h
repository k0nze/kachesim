#ifndef DATA_H
#define DATA_H

#include <string.h>

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>

namespace kachesim {
class Data {
public:
    Data(uint32_t size);
    Data(const std::vector<uint8_t>& data);
    Data(const uint8_t* data, size_t size);
    Data(const Data& data);
    ~Data();

    size_t size() const;

    uint8_t operator[](uint64_t index) const;
    uint8_t& operator[](uint64_t index);

    bool operator==(Data& d) const;
    bool operator==(uint64_t i);

    std::string to_string();

    friend std::ostream& operator<<(std::ostream& os, Data d) {
        os << d.to_string();
        return os;
    }

    /**
     * @brief set value to bytes starting from offset
     * @param bytes bytes to be set
     * @param offset default 0
     */
    template <typename T>
    void set(T bytes, size_t offset = 0, bool clear = true) {
        int max_byte = std::min({(size_t)sizeof(T), size_ - offset});
        // clear data_
        if (clear) {
            memset(data_, 0, size_);
        }
        // copy bytes in to data_
        memcpy(data_ + offset, &bytes, max_byte);
    }

    /**
     * @brief returns value as type T from offset
     * @param offset default 0
     * @return value of type T
     */
    template <typename T>
    T get(size_t offset = 0) {
        int max_byte = std::min({(size_t)sizeof(T), size_ - offset});
        T bytes = 0;
        memcpy(&bytes, data_ + offset, max_byte);
        return bytes;
    }

private:
    size_t size_;
    uint8_t* data_;
};
}  // namespace kachesim

#endif
