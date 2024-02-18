#ifndef DATA_H
#define DATA_H

#include <cstdint>
#include <vector>

class Data {
public:
    Data(uint32_t size);

    uint32_t size();

    uint8_t operator[](uint64_t index) const { return data_[index]; }
    uint8_t& operator[](uint64_t index) { return data_[index]; }

    void set_byte(uint8_t byte);
    void set_half_word(uint16_t half_word);
    void set_word(uint32_t word);
    void set_double_word(uint64_t double_word);

    void set_int8_t(int8_t a);
    void set_uint8_t(uint8_t a);

    void set_int16_t(int16_t a);
    void set_uint16_t(uint16_t a);

    void set_int32_t(int32_t a);
    void set_uint32_t(uint32_t a);

    void set_int64_t(int64_t a);
    void set_uint64_t(uint64_t a);

    void set_float(float f);
    void set_double(double d);

    uint8_t get_byte();
    uint16_t get_half_word();
    uint32_t get_word();
    uint64_t get_double_word();

    int8_t get_int8_t();
    uint8_t get_uint8_t();

    int16_t get_int16_t();
    uint16_t get_uint16_t();

    int32_t get_int32_t();
    uint32_t get_uint32_t();

    int64_t get_int64_t();
    uint64_t get_uint64_t();

    float get_float();
    double get_double();

private:
    uint32_t size_;
    std::vector<uint8_t> data_;
};

#endif
