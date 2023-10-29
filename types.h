#pragma once

#include <vector>
#include <cassert>

inline size_t encodeFixed64Bit(uint8_t* ptr, std::vector<uint8_t>& bufferToPushBackEncoded, size_t pos) {

    for (size_t i = 0; i < 8; i++)
        bufferToPushBackEncoded[pos + i] = (*(ptr + i));

    return pos + 8;
}

inline size_t decodeFixed64Bit(uint8_t* ptr, std::vector<uint8_t>& bufferToPopBackEncoded, size_t pos) {

    for (size_t i = 0; i < 8; i++)
        *(ptr + i) = bufferToPopBackEncoded[pos - 8 + i + 1];

    return pos - 8;
}

inline size_t encodeFixed32Bit(uint8_t* ptr, std::vector<uint8_t>& bufferToPushBackEncoded, size_t pos) {

    for (size_t i = 0; i < 4; i++)
        bufferToPushBackEncoded[pos + i] = (*(ptr + i));

    return pos + 4;
}

inline size_t decodeFixed32Bit(uint8_t* ptr, std::vector<uint8_t>& bufferToPopBackEncoded, size_t pos) {

    for (size_t i = 0; i < 4; i++)
        *(ptr + i) = bufferToPopBackEncoded[pos - 4 + i + 1];

    return pos - 4;
}

enum IntType
{
    INT32,
    SINT32,
    INT64,
    SINT64
};

inline size_t encodeVarInt(IntType valType, uint8_t* ptr, std::vector<uint8_t>& bufferToPushBackEncoded, size_t pos) {

    uint64_t temp = (valType == INT64 || valType == SINT64) ? static_cast<uint64_t>(*((uint64_t*)ptr)) : static_cast<uint32_t>(*((uint32_t*)ptr));

    if (valType == SINT32) {
        if (*((int32_t*)ptr) < 0)
            temp = 2 * (llabs(*((int32_t*)ptr))) - 1;
        else
            temp = temp << 1;
    }

    if (valType == SINT64) {
        if (*((int64_t*)ptr) < 0)
            temp = 2 * (llabs(*((int64_t*)ptr))) - 1;
        else
            temp = temp << 1;
    }

    size_t firstBytePos = pos;

    do {
        bufferToPushBackEncoded[pos++] = (temp % 128);
        bufferToPushBackEncoded[pos - 1] |= 128;

        temp /= 128;
    } while (temp != 0);

    bufferToPushBackEncoded[firstBytePos] &= 127;

    return pos;
}

inline size_t decodeVarInt(IntType valType, uint8_t* ptr, std::vector<uint8_t>& bufferToPopBackEncoded, size_t pos) {

    uint64_t temp = 0;

    size_t firstPos = pos;
    for (; firstPos > 0; firstPos--)
        if ((bufferToPopBackEncoded[firstPos] & 128) == 0)
            break;

    do
        temp |= (uint64_t((bufferToPopBackEncoded[pos] % 128)) << (7 * (pos - firstPos)));
    while (pos-- != firstPos);


    if (valType == SINT32 || valType == SINT64)
        temp = (temp / 2 + temp % 2) * (-1 + 2 * (temp % 2 == 0));

    if (valType == INT64 || valType == SINT64)
        *((uint64_t*)ptr) = temp;
    else
        *((uint32_t*)ptr) = temp;

    return firstPos - 1;
}
