#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>


namespace testStrArr {

class ar {
private:
	std::vector<int64_t> arr;
	size_t encode_arr(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos);
	size_t decode_arr(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos);


public:
	size_t encode_ar(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos);
	size_t decode_ar(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos);

	std::vector<int64_t> & ref_arr();
};

}