#pragma once

#include <vector>
#include <cassert>

inline void encodeFixed64Bit(uint8_t *ptr, std::vector<uint8_t> &bufferToPushBackEncoded)
{
    for (size_t i = 0; i < 8; i++)
        bufferToPushBackEncoded.push_back((*(ptr + i)));
}

inline void decodeFixed64Bit(uint8_t *ptr, const std::vector<uint8_t> &bufferToPopBackEncoded, size_t &pos)
{
    for (size_t i = 0; i < 8; i++)
        *(ptr + i) = bufferToPopBackEncoded[pos + 1 - 8 + i];

    pos -= 8;
}

inline void encodeFixed32Bit(uint8_t *ptr, std::vector<uint8_t> &bufferToPushBackEncoded)
{
    for (size_t i = 0; i < 4; i++)
        bufferToPushBackEncoded.push_back((*(ptr + i)));
}

inline void decodeFixed32Bit(uint8_t *ptr, const std::vector<uint8_t> &bufferToPopBackEncoded, size_t &pos)
{
    for (size_t i = 0; i < 4; i++)
        *(ptr + i) = bufferToPopBackEncoded[pos + 1 - 4 + i];

    pos -= 4;
}

enum IntType
{
    INT32,
    SINT32,
    INT64,
    SINT64
};

inline void encodeVarInt(IntType valType, uint8_t *ptr, std::vector<uint8_t> &bufferToPushBackEncoded)
{
    uint64_t temp = (valType == INT64 || valType == SINT64) ? static_cast<uint64_t>(*((uint64_t *)ptr)) : static_cast<uint32_t>(*((uint32_t *)ptr));

    if (valType == SINT32)
    {
        if (*((int32_t *)ptr) < 0)
            temp = 2 * (llabs(*((int32_t *)ptr))) - 1;
        else
            temp = temp << 1;
    }

    if (valType == SINT64)
    {
        if (*((int64_t *)ptr) < 0)
            temp = 2 * (llabs(*((int64_t *)ptr))) - 1;
        else
            temp = temp << 1;
    }

    size_t firstBytePos = bufferToPushBackEncoded.size();

    do
    {
        bufferToPushBackEncoded.push_back((temp % 128));
        bufferToPushBackEncoded.back() |= 128;

        temp /= 128;
    } while (temp != 0);

    bufferToPushBackEncoded[firstBytePos] &= 127;
}

inline void decodeVarInt(IntType valType, uint8_t *ptr, const std::vector<uint8_t> &bufferToPopBackEncoded, size_t &pos)
{
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
        *((uint64_t *)ptr) = temp;
    else
        *((uint32_t *)ptr) = temp;
}
