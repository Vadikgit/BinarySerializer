// GeneratedClassesTester.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include <iomanip>
// #include "testStrArr.h"
//  #include "testArrs.h"
#include "test.h"
#include <algorithm>
#include <chrono>

#define tst1
// #define tst1
// #define tst3

#ifdef tst1
void tester1()
{
	/*
	filename test

	t_class ar {
	t_array t_string arr
	}

	t_class cl1 {
	t_f_int64 f64
	}

	t_class cl2 {
	t_f_int32 f32
	cl1 memberCl1
	t_v_int64 v64
	t_array t_string arr
	t_array cl1 arrCl1
	}

	t_class cl3 {
	cl1 memberCl1
	cl2 memberCl2
	t_v_int64 v64
	t_v_int32 v32
	t_float fl
	t_double dbl
	t_string str
	t_v_sint64 s64
	}

	*/

	std::chrono::time_point<std::chrono::system_clock> t1, t2;

	t1 = std::chrono::system_clock::now();

	test::cl3 CLL;
	CLL.ref_memberCl1().set_f64(9);
	CLL.ref_memberCl2().ref_memberCl1().set_f64(19);
	CLL.ref_memberCl2().set_f32(23);
	CLL.ref_memberCl2().set_v64(-3);

	for (size_t i = 0; i < 5; i++)
	{
		std::string str = "wmcwkjnwecnewkcjknenwevnkejrnvrjvi29hnvhvn3h299vnvoi3vbtpbidfnkjFJOFJ(1esdsncknvknijnitnuibwesd;l,l>V<ejwheotrb-=wkwndjhveanjinixjnuj110e92ijeoverjbvnnjkn";
		CLL.ref_memberCl2().ref_arr().push_back(str.substr(i % str.length(), 39));
	}
	for (int64_t i = INT64_MIN; i < INT64_MAX - 100'000'000'000'000'000; i += 100'000'000'000'000'000)
	{
		test::cl1 temp;
		temp.set_f64(i);
		CLL.ref_memberCl2().ref_arrCl1().push_back(temp);
	}

	CLL.set_v32(1025);
	CLL.set_v64(41050);

	CLL.ref_str() = "ffergerg088ky7tgggdtu8b2399999999994356748754iweWEPEKOKF$($#$(J$FJJFWEJFIJEFFFFFFFFFFFFFFFFFFFFWwwc";

	CLL.set_fl(435980.3535f);
	CLL.set_dbl(9845904032.3535345);
	CLL.set_s64(INT32_MIN);

	t2 = std::chrono::system_clock::now();

	std::cout << "Filled " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << std::endl;

	t1 = std::chrono::system_clock::now();
	std::vector<uint8_t> enc;
	// enc.resize(35000);

	CLL.encode_cl3(enc);
	size_t len = enc.size();
	// enc.resize(len);
	t2 = std::chrono::system_clock::now();

	std::cout << "Encoded " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms, " << enc.size() << " " << len << " bytes, "
			  << double(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) / enc.size() << " ms/byte" << std::endl;

	// outf.write((char*)&(enc[0]), enc.size());
	// outf.write((char*)&(enc[0]), 0);
	//-------------------------------------------------------------------------------------------
	test::cl3 CLLDEC;

	t1 = std::chrono::system_clock::now();
	len--;
	CLLDEC.decode_cl3(enc, len);
	t2 = std::chrono::system_clock::now();

	std::cout << "Decoded " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << std::endl;

	std::cout << CLL.ref_memberCl1().get_f64() << '\t' << CLLDEC.ref_memberCl1().get_f64() << '\n';
	std::cout << CLL.ref_memberCl2().ref_memberCl1().get_f64() << '\t' << CLLDEC.ref_memberCl2().ref_memberCl1().get_f64() << '\n';
	std::cout << CLL.ref_memberCl2().get_f32() << '\t' << CLLDEC.ref_memberCl2().get_f32() << '\n';
	std::cout << CLL.ref_memberCl2().get_v64() << '\t' << CLLDEC.ref_memberCl2().get_v64() << "\n\n\n";

	for (size_t i = 0; i < CLLDEC.ref_memberCl2().ref_arr().size(); i++)
	{
		std::cout << i << ' ' << CLLDEC.ref_memberCl2().ref_arr()[i] << "\t" << CLL.ref_memberCl2().ref_arr()[i] << '\n';
	}

	std::cout << "\n\n\n";

	for (int i = 0; i < CLLDEC.ref_memberCl2().ref_arrCl1().size(); i++)
	{
		std::cout << i << ' ' << CLLDEC.ref_memberCl2().ref_arrCl1()[i].get_f64() << "\t" << CLL.ref_memberCl2().ref_arrCl1()[i].get_f64() << '\n';
	}

	std::cout << "\n\n\n"
			  << CLL.get_v32() << '\t' << CLLDEC.get_v32() << '\n';
	std::cout << CLL.get_v64() << '\t' << CLLDEC.get_v64() << '\n';
	std::cout << CLL.ref_str() << '\n'
			  << CLLDEC.ref_str() << '\n';
	std::cout << std::setprecision(32) << CLL.get_fl() << '\t' << CLLDEC.get_fl() << '\n';
	std::cout << std::setprecision(32) << CLL.get_dbl() << '\t' << CLLDEC.get_dbl() << '\n';
	std::cout << CLL.get_s64() << '\t' << CLLDEC.get_s64() << '\n';

	std::cout << "OK " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << std::endl;

	std::cout << "\n\n////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n\n";
}

#endif // tst1

#ifdef tst2
/*
filename test

t_class ar {
t_array t_f_int64 arr
}

*/

void tester2(int str)
{

	std::chrono::time_point<std::chrono::system_clock> t1, t2;

	t1 = std::chrono::system_clock::now();

	ar arr;
	arr.ref_arr().resize(str);

	for (int32_t i = 0; i < str; i++)
	{
		arr.ref_arr()[i] = i;
	}

	t2 = std::chrono::system_clock::now();

	std::cout << "Filled " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << std::endl;

	t1 = std::chrono::system_clock::now();
	std::vector<uint8_t> enc;
	// enc.resize(35000000);

	int len = arr.encode_ar(enc, 0);
	enc.resize(len);

	t2 = std::chrono::system_clock::now();

	std::cout << "Encoded " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms, " << enc.size() << " " << len << " bytes, "
			  << double(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) / enc.size() << " ms/byte" << std::endl;

	t1 = std::chrono::system_clock::now();
	std::ofstream outf("file.bin", std::ios::binary);
	outf.write((char *)&(enc[0]), enc.size());
	outf.flush();
	t2 = std::chrono::system_clock::now();
	std::cout << "Saved in file " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms, "
			  << double(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) / enc.size() << " ms/byte" << std::endl;

	//-------------------------------------------------------------------------------------------
	ar decarr;

	t1 = std::chrono::system_clock::now();
	decarr.decode_ar(enc);
	t2 = std::chrono::system_clock::now();

	std::cout << "Decoded " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << std::endl;

	std::cout << "OK " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << std::endl;
}

#endif // t2

#ifdef tst3
/*
filename testArrs

t_class arString {
t_array t_string arr
}

t_class arFint64 {
t_array t_f_int64 arr
}

t_class arVint64 {
t_array t_v_int64 arr
}


t_class cl {
t_f_int32 f32
t_f_int32 f64
t_v_int32 v32
t_v_int64 v64
t_float fl
t_double dbl
}

t_class arClass {
t_array cl arr
}

*/

void tester3(int str)
{

	std::chrono::time_point<std::chrono::system_clock> t1, t2;

	testArrs::arFint64 arfint64;
	arfint64.ref_arr().resize(str);

	for (int32_t i = 0; i < str; i++)
		arfint64.ref_arr()[i] = i;

	t1 = std::chrono::system_clock::now();
	std::vector<uint8_t> enc;
	enc.resize(200'000'000);

	int len = arfint64.encode_arFint64(enc, 0);
	enc.resize(len);

	t2 = std::chrono::system_clock::now();

	std::cout << "arFint64 " << arfint64.ref_arr().size() * sizeof(int64_t) << " bytes ------> encoded " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " mcs, " << enc.size() << " bytes, "
			  << double(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()) / enc.size() << " mcs/byte" << std::endl;

	t1 = std::chrono::system_clock::now();
	std::ofstream outf("file.bin", std::ios::binary);
	outf.write((char *)&(enc[0]), enc.size());
	outf.flush();
	t2 = std::chrono::system_clock::now();
	// std::cout << "Saved in file " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms, "
	//   << double(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) / enc.size() << " ms/byte" << std::endl;

	testArrs::arFint64 arfint642;

	t1 = std::chrono::system_clock::now();
	arfint642.decode_arFint64(enc, enc.size() - 1);
	t2 = std::chrono::system_clock::now();

	std::cout << "\tdecoded " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " mcs, "
			  << double(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()) / enc.size() << " mcs/byte" << std::endl;

	//    std::cout << "equality: " << (arfint64.ref_arr() == arfint642.ref_arr()) << '\n';

	arfint64.ref_arr().clear();
	arfint642.ref_arr().clear();

	//-------------------------------------------------------------------------------------------

	enc.clear();
	testArrs::arVint64 arvint64;
	arvint64.ref_arr().resize(str);

	for (int32_t i = 0; i < str; i++)
		arvint64.ref_arr()[i] = i;

	t1 = std::chrono::system_clock::now();

	enc.resize(200'000'000);

	len = arvint64.encode_arVint64(enc, 0);
	enc.resize(len);

	t2 = std::chrono::system_clock::now();

	std::cout << "arVint64 " << arvint64.ref_arr().size() * sizeof(int64_t) << " bytes ------> encoded " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " mcs, " << enc.size() << " bytes, "
			  << double(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()) / enc.size() << " mcs/byte" << std::endl;

	testArrs::arVint64 arvint642;

	t1 = std::chrono::system_clock::now();
	arvint642.decode_arVint64(enc, enc.size() - 1);
	t2 = std::chrono::system_clock::now();

	std::cout << "\tdecoded " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " mcs, "
			  << double(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()) / enc.size() << " mcs/byte" << std::endl;

	arvint64.ref_arr().clear();

	//-------------------------------------------------------------------------------------------

	enc.clear();

	std::string pastStr = "iuefbvijvrvhwbhfwefew87y4u9u3jdddd2bhjfbh344444444444448ue92uij332d43f43981hbjxandcc0-gbjbjbshuh4b3";

	testArrs::arString arstring;
	arstring.ref_arr().resize(str / pastStr.size());

	for (int32_t i = 0; i < (str / pastStr.size()); i++)
		arstring.ref_arr()[i] = pastStr;

	t1 = std::chrono::system_clock::now();
	enc.resize(200'000'000);

	len = arstring.encode_arString(enc, 0);
	enc.resize(len);

	t2 = std::chrono::system_clock::now();

	std::cout << "arString " << arstring.ref_arr().size() * pastStr.length() << " bytes ------> encoded " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " mcs, " << enc.size() << " bytes, "
			  << double(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()) / enc.size() << " mcs/byte" << std::endl;

	testArrs::arString arstring2;

	t1 = std::chrono::system_clock::now();
	arstring2.decode_arString(enc, enc.size() - 1);
	t2 = std::chrono::system_clock::now();

	std::cout << "\tdecoded " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " mcs, "
			  << double(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()) / enc.size() << " mcs/byte" << std::endl;

	arstring.ref_arr().clear();

	//-------------------------------------------------------------------------------------------

	testArrs::arClass arclass;
	enc.clear();

	arclass.ref_arr().resize(str / sizeof(testArrs::cl));
	testArrs::cl temp;

	for (int32_t i = 0; i < (str / sizeof(testArrs::cl)); i++)
	{
		temp.set_v64(i);
		temp.set_v32(i);
		temp.set_fl(0.001 * i);
		temp.set_dbl(0.001 * i);
		temp.set_f32(i);
		temp.set_f64(i);

		arclass.ref_arr()[i] = temp;
	}
	t1 = std::chrono::system_clock::now();
	enc.resize(200'000'000);

	len = arclass.encode_arClass(enc, 0);
	enc.resize(len);

	t2 = std::chrono::system_clock::now();

	std::cout << "arClass " << arclass.ref_arr().size() * sizeof(testArrs::cl) << " bytes ------> encoded " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " mcs, " << enc.size() << " bytes, "
			  << double(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()) / enc.size() << " mcs/byte" << std::endl;

	testArrs::arClass arclass2;

	t1 = std::chrono::system_clock::now();
	arclass2.decode_arClass(enc, enc.size() - 1);
	t2 = std::chrono::system_clock::now();

	std::cout << "\tdecoded " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " mcs, "
			  << double(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()) / enc.size() << " mcs/byte" << std::endl;
}

#endif // t3

#ifdef tst4
/*
filename testStrArr

t_class ar {
t_array t_v_sint64 arr
}

*/

void tester4(int str)
{

	std::chrono::time_point<std::chrono::system_clock> t1, t2;

	testStrArr::ar arvsint64;
	arvsint64.ref_arr().resize(str);

	for (int32_t i = 0; i < str; i++)
		arvsint64.ref_arr()[i] = i;

	t1 = std::chrono::system_clock::now();
	std::vector<uint8_t> enc;
	enc.resize(500'000'000);

	int len = arvsint64.encode_ar(enc, 0);
	enc.resize(len);

	t2 = std::chrono::system_clock::now();

	std::cout << "arvsint64 encoded " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms, " << len << " bytes, "
			  << double(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) / enc.size() << " ms/byte" << std::endl;

	testStrArr::ar arvsint642;

	t1 = std::chrono::system_clock::now();
	arvsint642.decode_ar(enc, enc.size() - 1);
	t2 = std::chrono::system_clock::now();

	std::cout << "arvsint64 decoded " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms, " << len << " bytes, "
			  << double(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) / enc.size() << " ms/byte" << std::endl;

	std::cout << "equality: " << (arvsint64.ref_arr() == arvsint642.ref_arr()) << '\n';
}

#endif // t4

int main()
{
	// std::ofstream outf("file.bin", std::ios::binary);

	/*std::vector<uint8_t> buffer;
	buffer.resize(1000);

	int32_t vr;
	vr = INT32_MIN;
	int len = encodeFixed32Bit((uint8_t*)&vr, buffer, 0);
	buffer.resize(len);

	for (size_t i = 0; i < buffer.size(); i++)
	{
		std::cout << (int) buffer[i] << ' ';
	}

	int32_t vr1;

	len = decodeFixed32Bit((uint8_t*)&vr1, buffer, len - 1);

	std::cout << '\n' << vr << '\n' << vr1;*/

	/*std::vector<uint8_t> buffer;
	buffer.resize(100);

	int64_t vr = INT64_MIN;

	int len = encodeVarInt(IntType::INT64, (uint8_t *) &vr, buffer, 0);
	buffer.resize(len);

	for (size_t i = 0; i < buffer.size(); i++)
	{
		std::cout << (int)buffer[i] << ' ';
	}

	int64_t vr1;

	decodeVarInt(IntType::INT64, (uint8_t*)&vr1, buffer, len - 1);


	std::cout << '\n' << vr << '\n' << vr1 << '\n';*/

	/*std::chrono::time_point<std::chrono::system_clock> t1, t2;

	std::vector<int64_t> lntest;

	t1 = std::chrono::system_clock::now();
	lntest.resize(75'000'000);

	for (int i = 0; i < 75'000'000; i++)
	{
		lntest[i] = (i % 256);
	}
	t2 = std::chrono::system_clock::now();
	std::cout << "Filled " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms " << double(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) / lntest.size() / 8 << " ms/byte" << std::endl;

	//lntest.val = { 4,23,46, 12,35,2,12,64,23 };

	t1 = std::chrono::system_clock::now();
	std::vector<uint8_t> buffer;
	buffer.resize(750'000'000);
	//std::cout << lntest.encodeValue(buffer, 0) <<'\n';
	size_t len = encodeLen((uint8_t*)&(lntest[0]), lntest.size()*(sizeof(int64_t)), buffer, 0);
	buffer.resize(len);

	t2 = std::chrono::system_clock::now();

	//for (size_t i = 0; i < buffer.size(); i++)
//        std::cout << ' ' << (int)buffer[i];


	std::cout << "Encoded " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms " << buffer.size() << " bytes, " << double(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) / buffer.size() << " ms/byte" << std::endl;



	t1 = std::chrono::system_clock::now();
	std::ofstream outf("file.bin", std::ios::binary);
	outf.write((char*)&(buffer[0]), buffer.size());
	outf.flush();
	t2 = std::chrono::system_clock::now();
	std::cout << "Saved in file " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms, "
		<< double(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) / buffer.size() << " ms/byte" << std::endl;



	std::vector<int64_t> lntest2;
	lntest2.resize(75'000'000);
	t1 = std::chrono::system_clock::now();
	len--;
	decodeLen((uint8_t*)&(lntest2[0]), buffer, len);
	t2 = std::chrono::system_clock::now();
	std::cout << "Decoded " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms " << buffer.size() << " bytes, " << double(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) / buffer.size() << " ms/byte" << std::endl;


	std::cout << "equality:\n" << (lntest == lntest2) << '\n';*/

	/*ar t;

	for (int32_t i = 0; i < 22; i++) {

		t.ref_arr().push_back(std::string("gfht") + std::string(i % 5, char(i + 'a')));
	}

	auto enc = t.encode_ar();

	outf.write((char*)&(enc[0]), enc.size());


	ar t2;

	t2.decode_ar(enc);

	for (size_t i = 0; i < t2.ref_arr().size(); i++)
	{
		std::cout << t2.ref_arr()[i] << ' ' << t.ref_arr()[i] << std::endl;
	}*/

#ifdef tst2
	for (int i = 4000000; i <= 4000000; i++)
	{
		std::cout << "\n"
				  << i << "\n";
		for (size_t j = 0; j < 1; j++)
		{
			tester2(i);
		}
	}
#endif // t2

#ifdef tst1
	for (int i = 1; i <= 1; i++)
	{
		std::cout << "\n"
				  << i << "\n";
		for (size_t j = 0; j < 1; j++)
		{
			tester1();
		}
	}
#endif // tst1

#ifdef tst3

	for (int64_t i = 20; i <= 20'000'000; i *= 10)
	{
		std::cout << "\n"
				  << i << "\n";
		for (size_t j = 0; j < 1; j++)
		{
			tester3(i);

			std::cout << "\n\n///////////////////////////////////////////////\n\n";
		}
	}

#endif // tst3

#ifdef tst4
	for (int64_t i = 60; i <= 60'000'000; i *= 10)
	{
		std::cout << "\n"
				  << i << "\n";
		for (size_t j = 0; j < 1; j++)
		{
			tester4(i);

			std::cout << "\n\n///////////////////////////////////////////////\n\n";
		}
	}
#endif // tst4

	/*
	std::chrono::time_point<std::chrono::system_clock> t1, t2;

	{

		t1 = std::chrono::system_clock::now();

		std::vector<int> v;

		for (size_t i = 0; i < 1'000'000; i++)
		{
			v.push_back(i);
		}

		t2 = std::chrono::system_clock::now();

		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms " << double(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) / v.size() << " ms/byte" << std::endl;

	}

	{   t1 = std::chrono::system_clock::now();

	std::vector<int> v;
	v.reserve(1'000'000);

	for (size_t i = 0; i < 1'000'000; i++)
	{
		//if (i % 10000 == 0)
		  //  std::cout << "\n" << v.capacity();

		v.emplace_back(i);
	}

	t2 = std::chrono::system_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms " << double(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) / v.size() << " ms/byte" << std::endl;
	}

	{
		t1 = std::chrono::system_clock::now();

		std::vector<int> v;
		v.resize(1'000'000);

		for (size_t i = 0; i < 1'000'000; i++)
		{
			v[i] = i;
		}

		t2 = std::chrono::system_clock::now();

		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms " << double(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) / v.size() << " ms/byte" << std::endl;
	}
	{
		t1 = std::chrono::system_clock::now();

		std::vector<int> v;

		for (size_t i = 0; i < 1'000'000; i++)
		{
			if (i >= v.size())
			{
				v.resize(v.size() * 2 + 1);
			}
			v[i] = i;
		}

		t2 = std::chrono::system_clock::now();

		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms " << double(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) / v.size() << " ms/byte" << std::endl;
	}

	{

		std::vector<int> v, v1;

		for (size_t i = 0; i < 1'000'000; i++)
		{

			v.push_back(i);
		}

		t1 = std::chrono::system_clock::now();

		v1.insert(v1.end(), v.begin(), v.end());

		t2 = std::chrono::system_clock::now();

		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms " << double(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) / v.size() << " ms/byte" << std::endl;
	}

	*/

	std::chrono::time_point<std::chrono::system_clock> t1, t2;

	t1 = std::chrono::system_clock::now();

	std::vector<uint64_t> arr1;

	for (int64_t i = 0; i < 10'000'000; i++)
	{
		arr1.push_back(i);
	}

	t2 = std::chrono::system_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << '\n';

	t1 = std::chrono::system_clock::now();

	std::vector<uint64_t> arr2;

	arr2.reserve(10'000'000);

	for (int64_t i = 0; i < 10'000'000; i++)
	{
		arr1.push_back(i);
	}

	t2 = std::chrono::system_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << '\n';

	t1 = std::chrono::system_clock::now();

	std::vector<uint64_t> arr3;
	arr3.resize(10'000'000);

	for (int64_t i = 0; i < 10'000'000; i++)
	{
		arr1[i] = i;
	}

	t2 = std::chrono::system_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << '\n';
}