#include "testStrArr.h"

#include "types.h"


namespace testStrArr {

// ----- ar
std::vector<int64_t> & ar::ref_arr(){
	return arr;
}

size_t ar::encode_arr(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){
	for (size_t i = 0; i < arr.size(); i++)
		pos = encodeVarInt(IntType::SINT64, (uint8_t*)& arr[(arr.size() - 1) - i], bufferToPushBackEncoded, pos);
	uint32_t sz = arr.size();

return encodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPushBackEncoded, pos);
}

size_t ar::decode_arr(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos){

	uint32_t sz;
	pos = decodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPopBackEncoded, pos);
	arr.resize(sz);

	for (size_t i = 0; i < sz; i++)
		pos = decodeVarInt(IntType::SINT64, (uint8_t*)& arr[i], bufferToPopBackEncoded, pos);

return pos;
}

size_t ar::encode_ar(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){

	pos = encode_arr(bufferToPushBackEncoded, pos);

	return pos;
}

size_t ar::decode_ar(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos){
	pos = decode_arr(bufferToPopBackEncoded, pos);

	return pos;}

}