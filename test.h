#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include "types.h"

class ar {
private:
	std::vector<std::string> arr;
	std::vector<uint8_t> encode_arr();
	int decode_arr(std::vector<uint8_t> data);


public:
	std::vector<uint8_t> encode_ar();
	int decode_ar(std::vector<uint8_t> data);

	std::vector<std::string> & ref_arr();
};

class cl1 {
private:
	int64_t f64;
	std::vector<uint8_t> encode_f64();
	int decode_f64(std::vector<uint8_t> data);


public:
	std::vector<uint8_t> encode_cl1();
	int decode_cl1(std::vector<uint8_t> data);

	int64_t get_f64();
	void set_f64(int64_t);
};

class cl2 {
private:
	int32_t f32;
	std::vector<uint8_t> encode_f32();
	int decode_f32(std::vector<uint8_t> data);

	cl1 memberCl1;
	std::vector<uint8_t> encode_memberCl1();
	int decode_memberCl1(std::vector<uint8_t> data);

	int64_t v64;
	std::vector<uint8_t> encode_v64();
	int decode_v64(std::vector<uint8_t> data);

	std::vector<std::string> arr;
	std::vector<uint8_t> encode_arr();
	int decode_arr(std::vector<uint8_t> data);

	std::vector<cl1> arrCl1;
	std::vector<uint8_t> encode_arrCl1();
	int decode_arrCl1(std::vector<uint8_t> data);


public:
	std::vector<uint8_t> encode_cl2();
	int decode_cl2(std::vector<uint8_t> data);

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
	std::vector<uint8_t> encode_memberCl1();
	int decode_memberCl1(std::vector<uint8_t> data);

	cl2 memberCl2;
	std::vector<uint8_t> encode_memberCl2();
	int decode_memberCl2(std::vector<uint8_t> data);

	int64_t v64;
	std::vector<uint8_t> encode_v64();
	int decode_v64(std::vector<uint8_t> data);

	int32_t v32;
	std::vector<uint8_t> encode_v32();
	int decode_v32(std::vector<uint8_t> data);

	float fl;
	std::vector<uint8_t> encode_fl();
	int decode_fl(std::vector<uint8_t> data);

	double dbl;
	std::vector<uint8_t> encode_dbl();
	int decode_dbl(std::vector<uint8_t> data);

	std::string str;
	std::vector<uint8_t> encode_str();
	int decode_str(std::vector<uint8_t> data);

	int64_t s64;
	std::vector<uint8_t> encode_s64();
	int decode_s64(std::vector<uint8_t> data);


public:
	std::vector<uint8_t> encode_cl3();
	int decode_cl3(std::vector<uint8_t> data);

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
