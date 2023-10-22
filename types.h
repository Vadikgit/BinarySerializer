#pragma once

#include <vector>
#include <cassert>

template<class T>
class Fixed32Bit
{
public:
    T val;

    void encodeValue(std::vector<uint8_t> & bufferToPushBackEncoded) {
        assert(sizeof(val) == 4);

        uint8_t* ptr = (uint8_t*)&val;

        for (size_t i = 0; i < 4; i++)
            bufferToPushBackEncoded.push_back(*(ptr++));
    }

    void decodeValue(std::vector<uint8_t> & bufferToPopBackEncoded) {
        val = {};

        uint8_t* ptr = (uint8_t*)&val;

        for (size_t i = 0; i < 4; i++)
            *(ptr++) = bufferToPopBackEncoded[bufferToPopBackEncoded.size() - 4 + i];
   
        bufferToPopBackEncoded.resize(bufferToPopBackEncoded.size() - 4);
    }
};

template<class T>
class Fixed64Bit
{
public:
    T val;

    void encodeValue(std::vector<uint8_t>& bufferToPushBackEncoded) {
        assert(sizeof(val) == 8);

        uint8_t* ptr = (uint8_t*)&val;

        for (size_t i = 0; i < 8; i++)
            bufferToPushBackEncoded.push_back(*(ptr++));
    }

    void decodeValue(std::vector<uint8_t>& bufferToPopBackEncoded) {
        val = {};

        uint8_t* ptr = (uint8_t*)&val;

        for (size_t i = 0; i < 8; i++)
            *(ptr++) = bufferToPopBackEncoded[bufferToPopBackEncoded.size() - 8 + i];

        bufferToPopBackEncoded.resize(bufferToPopBackEncoded.size() - 8);
    }
};

template<class T>
class VarInt
{
public:
    T val;

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

    void encodeValue(std::vector<uint8_t>& bufferToPushBackEncoded) {

        uint64_t temp = (valType == INT64 || valType == SINT64 || valType == UINT64) ? static_cast<uint64_t>(val) : static_cast<uint32_t>(val);

        if (valType == SINT32 || valType == SINT64) {
            if (val < 0)
                temp = 2 * (llabs(val)) - 1;
            else
                temp = temp << 1;
        }

        int firstBytePos = bufferToPushBackEncoded.size();
        
        do {
            bufferToPushBackEncoded.push_back(temp % 128);
            bufferToPushBackEncoded[bufferToPushBackEncoded.size() - 1] |= 128;

            temp /= 128;
        } while (temp != 0);

        bufferToPushBackEncoded[firstBytePos] &= 127;
    }

    void decodeValue(std::vector<uint8_t>& bufferToPopBackEncoded) {
        val = {};
        int res = 1;
        uint64_t temp = 0;

        for (res = 1; res <= bufferToPopBackEncoded.size(); res++) 
            if ((bufferToPopBackEncoded[bufferToPopBackEncoded.size() - res] & 128) == 0)
                break;

        for (int i = 0; i < res; i++) 
            temp |= (uint64_t((bufferToPopBackEncoded[bufferToPopBackEncoded.size() - res + i] % 128)) << (7 * i));

        val = static_cast<T>(temp);

        if (valType == SINT32 || valType == SINT64)
            temp = (temp / 2 + temp % 2) * (-1 + 2 * (temp % 2 == 0));

        val = static_cast<T>(temp);

        bufferToPopBackEncoded.resize(bufferToPopBackEncoded.size() - res);
    }

};

class Len
{
public:
    std::vector<uint8_t> val;
    VarInt<uint64_t> length;

    void encodeValue(std::vector<uint8_t>& bufferToPushBackEncoded) {

        VarInt<uint64_t> resultSize;
        resultSize.val = val.size();
        resultSize.valType = VarInt<uint64_t>::IntType::UINT64;

        bufferToPushBackEncoded.insert(bufferToPushBackEncoded.end(), val.begin(), val.end());
        resultSize.encodeValue(bufferToPushBackEncoded);
    }

    void decodeValue(std::vector<uint8_t>& bufferToPopBackEncoded) {
        val.clear();

        VarInt<uint64_t> resultSize;
        resultSize.valType = VarInt<uint64_t>::IntType::UINT64;
        
        resultSize.decodeValue(bufferToPopBackEncoded);

        for (size_t i = 0; i < resultSize.val; i++)
            val.push_back(bufferToPopBackEncoded[bufferToPopBackEncoded.size() - (resultSize.val) + i]);

        bufferToPopBackEncoded.resize(bufferToPopBackEncoded.size() - val.size());
    }
};