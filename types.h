#pragma once

#include <vector>
#include <cassert>

template<class T>
class Fixed32Bit
{
public:
    T val;
    std::vector<uint8_t> encoded;

    void encodeValue() {
        assert(sizeof(val) == 4);

        encoded.clear();

        uint8_t* ptr = (uint8_t*)&val;

        for (size_t i = 0; i < 4; i++)
        {
            encoded.push_back(*ptr);
            ptr++;
        }
    }

    int decodeValue() {
        val = {};

        uint8_t* ptr = (uint8_t*)&val;

        for (size_t i = 0; i < 4; i++)
        {
            *ptr = encoded[i];
            ptr++;
        }
        return 4;
    }
};

template<class T>
class Fixed64Bit
{
public:
    T val;
    std::vector<uint8_t> encoded;

    void encodeValue() {
        assert(sizeof(val) == 8);

        encoded.clear();

        uint8_t* ptr = (uint8_t*)&val;

        for (size_t i = 0; i < 8; i++)
        {
            encoded.push_back(*ptr);
            ptr++;
        }
    }

    int decodeValue() {
        val = {};

        uint8_t* ptr = (uint8_t*)&val;

        for (size_t i = 0; i < 8; i++)
        {
            *ptr = encoded[i];
            ptr++;
        }
        return 8;
    }
};

template<class T>
class VarInt
{
public:
    T val;
    std::vector<uint8_t> encoded;

    enum IntType
    {
        INT32,
        SINT32,
        UINT32,
        INT64,
        SINT64,
        UINT64
    };

    IntType valType;

    void encodeValue() {
        encoded.clear();

        uint64_t temp = (valType == INT64 || valType == SINT64 || valType == UINT64) ? static_cast<uint64_t>(val) : static_cast<uint32_t>(val);

        if (valType == SINT32 || valType == SINT64) {
            if (val < 0)
                temp = 2 * (llabs(val)) - 1;
            else
                temp = temp << 1;

        }

        do {
            encoded.push_back(temp % 128);
            encoded[encoded.size() - 1] |= 128;

            temp /= 128;
        } while (temp != 0);

        encoded[encoded.size() - 1] &= 127;
    }

    int decodeValue() {
        val = {};
        int res = 0;
        uint64_t temp = 0;

        for (res = 0; res < encoded.size(); res++) {
            temp |= (uint64_t((encoded[res] % 128)) << (7 * res));
            if ((encoded[res] & 128) == 0)
                break;
        }
        val = static_cast<T>(temp);

        if (valType == SINT32 || valType == SINT64)
            temp = (temp / 2 + temp % 2) * (-1 + 2 * (temp % 2 == 0));

        val = static_cast<T>(temp);
        return res + 1;
    }

};

class Len
{
public:
    std::vector<uint8_t> val;
    VarInt<uint32_t> length;
    std::vector<uint8_t> encoded;

    void encodeValue() {

        VarInt<uint32_t> resultSize;
        resultSize.val = val.size();
        resultSize.valType = VarInt<uint32_t>::IntType::UINT32;
        resultSize.encodeValue();

        encoded.insert(encoded.begin(), val.begin(), val.end());
        encoded.insert(encoded.begin(), resultSize.encoded.begin(), resultSize.encoded.end());
    }

    int decodeValue() {
        val.clear();

        VarInt<uint32_t> resultSize;
        resultSize.valType = VarInt<uint32_t>::IntType::UINT32;
        resultSize.encoded = encoded;
        int lenLen = resultSize.decodeValue();

        for (size_t i = 0; i < resultSize.val; i++)
            val.push_back(encoded[lenLen + i]);

        return lenLen + val.size();
    }
};