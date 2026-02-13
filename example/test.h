#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>


namespace test {

class ar {
private:
	std::vector<std::string> arr;
	void encode_arr(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_arr(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);


public:
	void encode_ar(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_ar(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);

	std::vector<std::string> & ref_arr();
};

class cl1 {
private:
	int64_t f64;
	void encode_f64(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_f64(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);


public:
	void encode_cl1(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_cl1(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);

	int64_t get_f64();
	void set_f64(int64_t);
};

class cl2 {
private:
	int32_t f32;
	void encode_f32(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_f32(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);

	cl1 memberCl1;
	void encode_memberCl1(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_memberCl1(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);

	int64_t v64;
	void encode_v64(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_v64(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);

	std::vector<std::string> arr;
	void encode_arr(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_arr(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);

	std::vector<cl1> arrCl1;
	void encode_arrCl1(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_arrCl1(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);


public:
	void encode_cl2(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_cl2(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);

	int32_t get_f32();
	void set_f32(int32_t);
	cl1 & ref_memberCl1();
	int64_t get_v64();
	void set_v64(int64_t);
	std::vector<std::string> & ref_arr();
	std::vector<cl1> & ref_arrCl1();
};

class cl3 {
private:
	cl1 memberCl1;
	void encode_memberCl1(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_memberCl1(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);

	cl2 memberCl2;
	void encode_memberCl2(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_memberCl2(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);

	int64_t v64;
	void encode_v64(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_v64(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);

	int32_t v32;
	void encode_v32(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_v32(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);

	float fl;
	void encode_fl(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_fl(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);

	double dbl;
	void encode_dbl(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_dbl(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);

	std::string str;
	void encode_str(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_str(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);

	int64_t s64;
	void encode_s64(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_s64(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);


public:
	void encode_cl3(std::vector<uint8_t> & bufferToPushBackEncoded);
	void decode_cl3(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos);

	cl1 & ref_memberCl1();
	cl2 & ref_memberCl2();
	int64_t get_v64();
	void set_v64(int64_t);
	int32_t get_v32();
	void set_v32(int32_t);
	float get_fl();
	void set_fl(float);
	double get_dbl();
	void set_dbl(double);
	std::string & ref_str();
	int64_t get_s64();
	void set_s64(int64_t);
};

}