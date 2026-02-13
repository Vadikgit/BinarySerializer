#include "test.h"

#include "types.h"


namespace test {

// ----- ar
std::vector<std::string> & ar::ref_arr(){
	return arr;
}

void ar::encode_arr(std::vector<uint8_t> & bufferToPushBackEncoded){
	for (size_t i = 0; i < arr.size(); i++){
		uint32_t tmpsz = arr[(arr.size() - 1) - i].length();
	for(size_t j = 0; j < tmpsz; j++)
		bufferToPushBackEncoded.push_back(arr[(arr.size() - 1) - i][j]);
	encodeVarInt(IntType::INT32, (uint8_t*)&tmpsz, bufferToPushBackEncoded);
	}
	uint32_t sz = arr.size();

encodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPushBackEncoded);
}

void ar::decode_arr(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){

	uint32_t sz; 
	decodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPopBackEncoded, pos); 
	arr.resize(sz); 

	for(size_t i = 0; i < sz; i++){
		uint32_t tmpsz;
	decodeVarInt(IntType::INT32, (uint8_t*)&tmpsz, bufferToPopBackEncoded, pos);
	arr[i].resize(tmpsz);
	for(size_t j = 0; j < tmpsz; j++)
		arr[i][tmpsz - j - 1] = bufferToPopBackEncoded[pos--];
	}
}

void ar::encode_ar(std::vector<uint8_t> & bufferToPushBackEncoded){

	encode_arr(bufferToPushBackEncoded);

}

void ar::decode_ar(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){
	decode_arr(bufferToPopBackEncoded, pos);

}

// ----- cl1
int64_t cl1::get_f64(){
	return f64;
}

void cl1::set_f64(int64_t v){
	f64 = v;
}

void cl1::encode_f64(std::vector<uint8_t> & bufferToPushBackEncoded){
	encodeFixed64Bit((uint8_t *)&f64, bufferToPushBackEncoded);
}

void cl1::decode_f64(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){
	decodeFixed64Bit((uint8_t *)&f64, bufferToPopBackEncoded, pos);
}

void cl1::encode_cl1(std::vector<uint8_t> & bufferToPushBackEncoded){

	encode_f64(bufferToPushBackEncoded);

}

void cl1::decode_cl1(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){
	decode_f64(bufferToPopBackEncoded, pos);

}

// ----- cl2
int32_t cl2::get_f32(){
	return f32;
}

void cl2::set_f32(int32_t v){
	f32 = v;
}

void cl2::encode_f32(std::vector<uint8_t> & bufferToPushBackEncoded){
	encodeFixed32Bit((uint8_t *)&f32, bufferToPushBackEncoded);
}

void cl2::decode_f32(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){
	decodeFixed32Bit((uint8_t *)&f32, bufferToPopBackEncoded, pos);
}

cl1 & cl2::ref_memberCl1(){
	return memberCl1;
}

void cl2::encode_memberCl1(std::vector<uint8_t> & bufferToPushBackEncoded){
	memberCl1.encode_cl1(bufferToPushBackEncoded);
}

void cl2::decode_memberCl1(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){
	memberCl1.decode_cl1(bufferToPopBackEncoded, pos);
}

int64_t cl2::get_v64(){
	return v64;
}

void cl2::set_v64(int64_t v){
	v64 = v;
}

void cl2::encode_v64(std::vector<uint8_t> & bufferToPushBackEncoded){
	encodeVarInt(IntType::INT64, (uint8_t *)&v64, bufferToPushBackEncoded);
}

void cl2::decode_v64(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){
	decodeVarInt(IntType::INT64, (uint8_t *)&v64, bufferToPopBackEncoded, pos);
}

std::vector<std::string> & cl2::ref_arr(){
	return arr;
}

void cl2::encode_arr(std::vector<uint8_t> & bufferToPushBackEncoded){
	for (size_t i = 0; i < arr.size(); i++){
		uint32_t tmpsz = arr[(arr.size() - 1) - i].length();
	for(size_t j = 0; j < tmpsz; j++)
		bufferToPushBackEncoded.push_back(arr[(arr.size() - 1) - i][j]);
	encodeVarInt(IntType::INT32, (uint8_t*)&tmpsz, bufferToPushBackEncoded);
	}
	uint32_t sz = arr.size();

encodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPushBackEncoded);
}

void cl2::decode_arr(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){

	uint32_t sz; 
	decodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPopBackEncoded, pos); 
	arr.resize(sz); 

	for(size_t i = 0; i < sz; i++){
		uint32_t tmpsz;
	decodeVarInt(IntType::INT32, (uint8_t*)&tmpsz, bufferToPopBackEncoded, pos);
	arr[i].resize(tmpsz);
	for(size_t j = 0; j < tmpsz; j++)
		arr[i][tmpsz - j - 1] = bufferToPopBackEncoded[pos--];
	}
}

std::vector<cl1> & cl2::ref_arrCl1(){
	return arrCl1;
}

void cl2::encode_arrCl1(std::vector<uint8_t> & bufferToPushBackEncoded){
	for (size_t i = 0; i < arrCl1.size(); i++)
		arrCl1[(arrCl1.size() - 1) - i].encode_cl1(bufferToPushBackEncoded);
	
	uint32_t sz = arrCl1.size();

encodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPushBackEncoded);
}

void cl2::decode_arrCl1(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){

	uint32_t sz;
	decodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPopBackEncoded, pos);
	arrCl1.resize(sz);

	for (size_t i = 0; i < sz; i++)
		arrCl1[i].decode_cl1(bufferToPopBackEncoded, pos);
	
}

void cl2::encode_cl2(std::vector<uint8_t> & bufferToPushBackEncoded){

	encode_f32(bufferToPushBackEncoded);
	encode_memberCl1(bufferToPushBackEncoded);
	encode_v64(bufferToPushBackEncoded);
	encode_arr(bufferToPushBackEncoded);
	encode_arrCl1(bufferToPushBackEncoded);

}

void cl2::decode_cl2(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){
	decode_arrCl1(bufferToPopBackEncoded, pos);
	decode_arr(bufferToPopBackEncoded, pos);
	decode_v64(bufferToPopBackEncoded, pos);
	decode_memberCl1(bufferToPopBackEncoded, pos);
	decode_f32(bufferToPopBackEncoded, pos);

}

// ----- cl3
cl1 & cl3::ref_memberCl1(){
	return memberCl1;
}

void cl3::encode_memberCl1(std::vector<uint8_t> & bufferToPushBackEncoded){
	memberCl1.encode_cl1(bufferToPushBackEncoded);
}

void cl3::decode_memberCl1(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){
	memberCl1.decode_cl1(bufferToPopBackEncoded, pos);
}

cl2 & cl3::ref_memberCl2(){
	return memberCl2;
}

void cl3::encode_memberCl2(std::vector<uint8_t> & bufferToPushBackEncoded){
	memberCl2.encode_cl2(bufferToPushBackEncoded);
}

void cl3::decode_memberCl2(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){
	memberCl2.decode_cl2(bufferToPopBackEncoded, pos);
}

int64_t cl3::get_v64(){
	return v64;
}

void cl3::set_v64(int64_t v){
	v64 = v;
}

void cl3::encode_v64(std::vector<uint8_t> & bufferToPushBackEncoded){
	encodeVarInt(IntType::INT64, (uint8_t *)&v64, bufferToPushBackEncoded);
}

void cl3::decode_v64(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){
	decodeVarInt(IntType::INT64, (uint8_t *)&v64, bufferToPopBackEncoded, pos);
}

int32_t cl3::get_v32(){
	return v32;
}

void cl3::set_v32(int32_t v){
	v32 = v;
}

void cl3::encode_v32(std::vector<uint8_t> & bufferToPushBackEncoded){
	encodeVarInt(IntType::INT32, (uint8_t *)&v32, bufferToPushBackEncoded);
}

void cl3::decode_v32(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){
	decodeVarInt(IntType::INT32, (uint8_t *)&v32, bufferToPopBackEncoded, pos);
}

float cl3::get_fl(){
	return fl;
}

void cl3::set_fl(float v){
	fl = v;
}

void cl3::encode_fl(std::vector<uint8_t> & bufferToPushBackEncoded){
	encodeFixed32Bit((uint8_t *)&fl, bufferToPushBackEncoded);
}

void cl3::decode_fl(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){
	decodeFixed32Bit((uint8_t *)&fl, bufferToPopBackEncoded, pos);
}

double cl3::get_dbl(){
	return dbl;
}

void cl3::set_dbl(double v){
	dbl = v;
}

void cl3::encode_dbl(std::vector<uint8_t> & bufferToPushBackEncoded){
	encodeFixed64Bit((uint8_t *)&dbl, bufferToPushBackEncoded);
}

void cl3::decode_dbl(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){
	decodeFixed64Bit((uint8_t *)&dbl, bufferToPopBackEncoded, pos);
}

std::string & cl3::ref_str(){
	return str;
}

void cl3::encode_str(std::vector<uint8_t> & bufferToPushBackEncoded){
	for (size_t i = 0; i < str.length(); i++)
		bufferToPushBackEncoded.push_back(str[(str.length() - 1) - i]);
	uint32_t sz = str.length();

encodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPushBackEncoded);
}

void cl3::decode_str(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){

	uint32_t sz;
	decodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPopBackEncoded, pos);
	str.resize(sz); 

	for(size_t i = 0; i < sz; i++)
		str[i] = bufferToPopBackEncoded[pos--];
	
}

int64_t cl3::get_s64(){
	return s64;
}

void cl3::set_s64(int64_t v){
	s64 = v;
}

void cl3::encode_s64(std::vector<uint8_t> & bufferToPushBackEncoded){
	encodeVarInt(IntType::SINT64, (uint8_t *)&s64, bufferToPushBackEncoded);
}

void cl3::decode_s64(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){
	decodeVarInt(IntType::SINT64, (uint8_t *)&s64, bufferToPopBackEncoded, pos);
}

void cl3::encode_cl3(std::vector<uint8_t> & bufferToPushBackEncoded){

	encode_memberCl1(bufferToPushBackEncoded);
	encode_memberCl2(bufferToPushBackEncoded);
	encode_v64(bufferToPushBackEncoded);
	encode_v32(bufferToPushBackEncoded);
	encode_fl(bufferToPushBackEncoded);
	encode_dbl(bufferToPushBackEncoded);
	encode_str(bufferToPushBackEncoded);
	encode_s64(bufferToPushBackEncoded);

}

void cl3::decode_cl3(const std::vector<uint8_t> & bufferToPopBackEncoded, size_t & pos){
	decode_s64(bufferToPopBackEncoded, pos);
	decode_str(bufferToPopBackEncoded, pos);
	decode_dbl(bufferToPopBackEncoded, pos);
	decode_fl(bufferToPopBackEncoded, pos);
	decode_v32(bufferToPopBackEncoded, pos);
	decode_v64(bufferToPopBackEncoded, pos);
	decode_memberCl2(bufferToPopBackEncoded, pos);
	decode_memberCl1(bufferToPopBackEncoded, pos);

}

}