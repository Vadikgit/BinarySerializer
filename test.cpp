#include "test.h"


// ----- ar
std::vector<std::string> & ar::ref_arr(){
	return arr;
}

std::vector<uint8_t> ar::encode_arr(){
	Len result;
	for(int i = 0; i < arr.size(); i++){
		Len cur;
		for (int j = 0; j < arr[arr.size() - 1 - i].size(); j++){
			cur.val.push_back((uint8_t)arr[arr.size() - 1 - i][j]);
		}
		cur.encodeValue();
		result.val.insert(result.val.begin(), cur.encoded.begin(), cur.encoded.end());
		}
		result.encodeValue();

	return result.encoded;
}

int ar::decode_arr(std::vector<uint8_t> data){
	Len result;
	result.encoded = data;
	int res = result.decodeValue();
	while(result.val.size() != 0){
		Len cur;
		cur.encoded = result.val;
		int cut = cur.decodeValue();
		std::string str;
		for (int j = 0; j < cur.val.size(); j++)
			str.push_back((char) cur.val[j]);

		arr.push_back(str);
		result.val.erase(result.val.begin(), result.val.begin() + cut);
	}
	return res;
}

std::vector<uint8_t> ar::encode_ar(){
	std::vector<uint8_t> res, temp;

	temp = encode_arr();
	res.insert(res.begin(), temp.begin(), temp.end());

	Len encodedClass;
	encodedClass.val = res;
	encodedClass.encodeValue();
	return encodedClass.encoded;
}

int ar::decode_ar(std::vector<uint8_t> data){
	Len Class;
	Class.encoded = data;
	int cutLen, res = Class.decodeValue();
	auto classBytes = Class.val;
		cutLen = decode_arr(classBytes);
	classBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);

	return res;
}

// ----- cl1
int64_t cl1::get_f64(){
	return f64;
}

void cl1::set_f64(int64_t v){
	f64 = v;
}

std::vector<uint8_t> cl1::encode_f64(){
	Fixed64Bit<int64_t> value;
	value.val = get_f64();
	value.encodeValue();
	return value.encoded;
}

int cl1::decode_f64(std::vector<uint8_t> data){
	Fixed64Bit<int64_t> value;
	value.encoded = data;
	int res = value.decodeValue();
	set_f64(value.val);
	return res;
}

std::vector<uint8_t> cl1::encode_cl1(){
	std::vector<uint8_t> res, temp;

	temp = encode_f64();
	res.insert(res.begin(), temp.begin(), temp.end());

	Len encodedClass;
	encodedClass.val = res;
	encodedClass.encodeValue();
	return encodedClass.encoded;
}

int cl1::decode_cl1(std::vector<uint8_t> data){
	Len Class;
	Class.encoded = data;
	int cutLen, res = Class.decodeValue();
	auto classBytes = Class.val;
		cutLen = decode_f64(classBytes);
	classBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);

	return res;
}

// ----- cl2
int32_t cl2::get_f32(){
	return f32;
}

void cl2::set_f32(int32_t v){
	f32 = v;
}

std::vector<uint8_t> cl2::encode_f32(){
	Fixed32Bit<int32_t> value;
	value.val = get_f32();
	value.encodeValue();
	return value.encoded;
}

int cl2::decode_f32(std::vector<uint8_t> data){
	Fixed32Bit<int32_t> value;
	value.encoded = data;
	int res = value.decodeValue();
	set_f32(value.val);
	return res;
}

cl1 & cl2::ref_memberCl1(){
	return memberCl1;
}

std::vector<uint8_t> cl2::encode_memberCl1(){
	return memberCl1.encode_cl1();
}

int cl2::decode_memberCl1(std::vector<uint8_t> data){
	int res = memberCl1.decode_cl1(data);
	return res;
}

int64_t cl2::get_v64(){
	return v64;
}

void cl2::set_v64(int64_t v){
	v64 = v;
}

std::vector<uint8_t> cl2::encode_v64(){
	VarInt<int64_t> value;
	value.val = get_v64();
	value.valType = VarInt<int64_t>::IntType::INT64;
	value.encodeValue();
	return value.encoded;
}

int cl2::decode_v64(std::vector<uint8_t> data){
	VarInt<int64_t> value;
	value.valType = VarInt<int64_t>::IntType::INT64;
	value.encoded = data;
	int res = value.decodeValue(); 
	set_v64(value.val);
	return res;
}

std::vector<std::string> & cl2::ref_arr(){
	return arr;
}

std::vector<uint8_t> cl2::encode_arr(){
	Len result;
	for(int i = 0; i < arr.size(); i++){
		Len cur;
		for (int j = 0; j < arr[arr.size() - 1 - i].size(); j++){
			cur.val.push_back((uint8_t)arr[arr.size() - 1 - i][j]);
		}
		cur.encodeValue();
		result.val.insert(result.val.begin(), cur.encoded.begin(), cur.encoded.end());
		}
		result.encodeValue();

	return result.encoded;
}

int cl2::decode_arr(std::vector<uint8_t> data){
	Len result;
	result.encoded = data;
	int res = result.decodeValue();
	while(result.val.size() != 0){
		Len cur;
		cur.encoded = result.val;
		int cut = cur.decodeValue();
		std::string str;
		for (int j = 0; j < cur.val.size(); j++)
			str.push_back((char) cur.val[j]);

		arr.push_back(str);
		result.val.erase(result.val.begin(), result.val.begin() + cut);
	}
	return res;
}

std::vector<cl1> & cl2::ref_arrCl1(){
	return arrCl1;
}

std::vector<uint8_t> cl2::encode_arrCl1(){
	Len result;
	for(int i = 0; i < arrCl1.size(); i++){
		auto encoded = arrCl1[(arrCl1.size() - 1) - i].encode_cl1();
		result.val.insert(result.val.begin(), encoded.begin(), encoded.end());
	}
		result.encodeValue();

	return result.encoded;
}

int cl2::decode_arrCl1(std::vector<uint8_t> data){
	Len result;
	result.encoded = data;
	int res = result.decodeValue();
	while(result.val.size() != 0){
		arrCl1.push_back(cl1{});
		int cut = arrCl1.rbegin()->decode_cl1(result.val);
		result.val.erase(result.val.begin(), result.val.begin() + cut);
	}
	return res;
}

std::vector<uint8_t> cl2::encode_cl2(){
	std::vector<uint8_t> res, temp;

	temp = encode_f32();
	res.insert(res.begin(), temp.begin(), temp.end());
	temp = encode_memberCl1();
	res.insert(res.begin(), temp.begin(), temp.end());
	temp = encode_v64();
	res.insert(res.begin(), temp.begin(), temp.end());
	temp = encode_arr();
	res.insert(res.begin(), temp.begin(), temp.end());
	temp = encode_arrCl1();
	res.insert(res.begin(), temp.begin(), temp.end());

	Len encodedClass;
	encodedClass.val = res;
	encodedClass.encodeValue();
	return encodedClass.encoded;
}

int cl2::decode_cl2(std::vector<uint8_t> data){
	Len Class;
	Class.encoded = data;
	int cutLen, res = Class.decodeValue();
	auto classBytes = Class.val;
		cutLen = decode_arrCl1(classBytes);
	classBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);
	cutLen = decode_arr(classBytes);
	classBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);
	cutLen = decode_v64(classBytes);
	classBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);
	cutLen = decode_memberCl1(classBytes);
	classBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);
	cutLen = decode_f32(classBytes);
	classBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);

	return res;
}

// ----- cl3
cl1 & cl3::ref_memberCl1(){
	return memberCl1;
}

std::vector<uint8_t> cl3::encode_memberCl1(){
	return memberCl1.encode_cl1();
}

int cl3::decode_memberCl1(std::vector<uint8_t> data){
	int res = memberCl1.decode_cl1(data);
	return res;
}

cl2 & cl3::ref_memberCl2(){
	return memberCl2;
}

std::vector<uint8_t> cl3::encode_memberCl2(){
	return memberCl2.encode_cl2();
}

int cl3::decode_memberCl2(std::vector<uint8_t> data){
	int res = memberCl2.decode_cl2(data);
	return res;
}

int64_t cl3::get_v64(){
	return v64;
}

void cl3::set_v64(int64_t v){
	v64 = v;
}

std::vector<uint8_t> cl3::encode_v64(){
	VarInt<int64_t> value;
	value.val = get_v64();
	value.valType = VarInt<int64_t>::IntType::INT64;
	value.encodeValue();
	return value.encoded;
}

int cl3::decode_v64(std::vector<uint8_t> data){
	VarInt<int64_t> value;
	value.valType = VarInt<int64_t>::IntType::INT64;
	value.encoded = data;
	int res = value.decodeValue(); 
	set_v64(value.val);
	return res;
}

int32_t cl3::get_v32(){
	return v32;
}

void cl3::set_v32(int32_t v){
	v32 = v;
}

std::vector<uint8_t> cl3::encode_v32(){
	VarInt<int32_t> value;
	value.val = get_v32();
	value.valType = VarInt<int32_t>::IntType::INT32;
	value.encodeValue();
	return value.encoded;
}

int cl3::decode_v32(std::vector<uint8_t> data){
	VarInt<int32_t> value;
	value.valType = VarInt<int32_t>::IntType::INT32;
	value.encoded = data;
	int res = value.decodeValue(); 
	set_v32(value.val);
	return res;
}

float cl3::get_fl(){
	return fl;
}

void cl3::set_fl(float v){
	fl = v;
}

std::vector<uint8_t> cl3::encode_fl(){
	Fixed32Bit<float> value;
	value.val = get_fl();
	value.encodeValue();
	return value.encoded;
}

int cl3::decode_fl(std::vector<uint8_t> data){
	Fixed32Bit<float> value;
	value.encoded = data;
	int res = value.decodeValue();
	set_fl(value.val);
	return res;
}

double cl3::get_dbl(){
	return dbl;
}

void cl3::set_dbl(double v){
	dbl = v;
}

std::vector<uint8_t> cl3::encode_dbl(){
	Fixed64Bit<double> value;
	value.val = get_dbl();
	value.encodeValue();
	return value.encoded;
}

int cl3::decode_dbl(std::vector<uint8_t> data){
	Fixed64Bit<double> value;
	value.encoded = data;
	int res = value.decodeValue();
	set_dbl(value.val);
	return res;
}

std::string & cl3::ref_str(){
	return str;
}

std::vector<uint8_t> cl3::encode_str(){

	Len encodedString;
	for (int i = 0; i < str.length(); i++)
		encodedString.val.push_back(str[i]);
	encodedString.encodeValue();
	return encodedString.encoded;
}

int cl3::decode_str(std::vector<uint8_t> data){
	Len String;
	String.encoded = data;
	int res = String.decodeValue();
	for (int i = 0; i < String.val.size(); i++)
		str.append(1, char(String.val[i]));
	return res;
}

int64_t cl3::get_s64(){
	return s64;
}

void cl3::set_s64(int64_t v){
	s64 = v;
}

std::vector<uint8_t> cl3::encode_s64(){
	VarInt<int64_t> value;
	value.val = get_s64();
	value.valType = VarInt<int64_t>::IntType::SINT64;
	value.encodeValue();
	return value.encoded;
}

int cl3::decode_s64(std::vector<uint8_t> data){
	VarInt<int64_t> value;
	value.valType = VarInt<int64_t>::IntType::SINT64;
	value.encoded = data;
	int res = value.decodeValue(); 
	set_s64(value.val);
	return res;
}

std::vector<uint8_t> cl3::encode_cl3(){
	std::vector<uint8_t> res, temp;

	temp = encode_memberCl1();
	res.insert(res.begin(), temp.begin(), temp.end());
	temp = encode_memberCl2();
	res.insert(res.begin(), temp.begin(), temp.end());
	temp = encode_v64();
	res.insert(res.begin(), temp.begin(), temp.end());
	temp = encode_v32();
	res.insert(res.begin(), temp.begin(), temp.end());
	temp = encode_fl();
	res.insert(res.begin(), temp.begin(), temp.end());
	temp = encode_dbl();
	res.insert(res.begin(), temp.begin(), temp.end());
	temp = encode_str();
	res.insert(res.begin(), temp.begin(), temp.end());
	temp = encode_s64();
	res.insert(res.begin(), temp.begin(), temp.end());

	Len encodedClass;
	encodedClass.val = res;
	encodedClass.encodeValue();
	return encodedClass.encoded;
}

int cl3::decode_cl3(std::vector<uint8_t> data){
	Len Class;
	Class.encoded = data;
	int cutLen, res = Class.decodeValue();
	auto classBytes = Class.val;
		cutLen = decode_s64(classBytes);
	classBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);
	cutLen = decode_str(classBytes);
	classBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);
	cutLen = decode_dbl(classBytes);
	classBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);
	cutLen = decode_fl(classBytes);
	classBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);
	cutLen = decode_v32(classBytes);
	classBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);
	cutLen = decode_v64(classBytes);
	classBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);
	cutLen = decode_memberCl2(classBytes);
	classBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);
	cutLen = decode_memberCl1(classBytes);
	classBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);

	return res;
}
