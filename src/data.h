#ifndef DATA_H
#define DATA_H

#include <algorithm>
#include <cstdint>

class Data {
public:
    Data(uint32_t size);
    ~Data();

    size_t size();

    uint8_t operator[](uint64_t index) const { return data_[index]; }
    uint8_t& operator[](uint64_t index) { return data_[index]; }

    template <typename T>
    void set(T bytes, size_t offset = 0) {
        int max_byte = std::min({(size_t)sizeof(T), size_});

        for (int i = offset; i < max_byte; i++) {
            data_[i] = (bytes >> (8 * i)) & 0xff;
        }

        // erase remaining bytes
        for (int i = max_byte; i < size_; i++) {
            data_[i] = 0;
        }
    }

    template <typename T>
    T get(size_t offset = 0) {
        int max_byte = std::min({(size_t)sizeof(T), size_});

        T bytes = 0;

        for (int i = offset; i < max_byte; i++) {
            bytes |= ((T)data_[i]) << (8 * i);
        }

        return bytes;
    }

private:
    size_t size_;
    uint8_t* data_;
};

#endif
