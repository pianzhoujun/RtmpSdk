#pragma once

#include <cstdint>
#include <cstring>
#include <string>

inline void encode_fixed8(char* dst, uint8_t value) {
    uint8_t* const buffer = reinterpret_cast<uint8_t*>(dst);
    std::memcpy(buffer, &value, sizeof(value));
}

inline void encode_fixed32(char* dst, uint32_t value) {
    uint8_t* const buffer = reinterpret_cast<uint8_t*>(dst);
    std::memcpy(buffer, &value, sizeof(value));
}

inline void put_fixed32(std::string* dst, uint32_t value) {
    char buf[sizeof(value)];
    encode_fixed32(buf, value);
    dst->append(buf, sizeof(buf));
}

inline void put_fixed8(std::string* dst, uint8_t value) {
    char buf[sizeof(value)];
    encode_fixed8(buf, value);
    dst->append(buf, sizeof(buf));
}

inline uint32_t decode_fixed32(const char* ptr) {
    const uint8_t* const buffer = reinterpret_cast<const uint8_t*>(ptr);
    uint32_t result;
    std::memcpy(&result, buffer, sizeof(result));

    return result;
}