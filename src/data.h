#ifndef DATA_H
#define DATA_H

#include <string.h>

#include <algorithm>
#include <cstdint>

class Data {
public:
    Data(uint32_t size);
    ~Data();

    size_t size();

    uint8_t operator[](uint64_t index) const { return data_[index]; }
    uint8_t& operator[](uint64_t index) { return data_[index]; }

    /**
     * @brief set value to bytes starting from offset
     * @param bytes bytes to be set
     * @param offset default 0
     */
    template <typename T>
    void set(T bytes, size_t offset = 0) {
        int max_byte = std::min({(size_t)sizeof(T), size_});
        memcpy(data_, &bytes, max_byte);
    }

    /**
     * @brief returns value as type T from offset
     * @param offset default 0
     * @return value of type T
     */
    template <typename T>
    T get(size_t offset = 0) {
        int max_byte = std::min({(size_t)sizeof(T), size_});
        T bytes = 0;
        memcpy(&bytes, data_, max_byte);
        return bytes;
    }

private:
    size_t size_;
    uint8_t* data_;
};

#endif
