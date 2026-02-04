// BinarySerializer.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

class SchemaFileProcessor
{
public:
	std::string filename;
	std::string printedPublic;
	std::string currentDescribingClassName;
	std::string currentDescribingClassEncoder;
	const std::string fileNameMarker = "filename";
	const std::string typeMarker = "t_";
	const std::string commentMarker = "#";
	bool fileNameRetrieved = false;

	std::ofstream cppFile;
	std::ofstream hFile;

	void processFilename(std::string_view str);
	void processField(std::string_view str);
	void processEndOfClassDefinition();

	std::unordered_set<std::string> definedClasses;
	std::unordered_map<int, std::string> fieldsOfCurrentClass;
	int currentClassId;

	enum types
	{
		T_V_INT32,	// varint
		T_V_SINT32, // varint
		T_V_UINT32, // varint
		T_V_INT64,	// varint
		T_V_SINT64, // varint
		T_V_UINT64, // varint

		T_F_INT32,	// 32bit
		T_F_UINT32, // 32bit
		T_FLOAT,	// 32bit

		T_F_INT64,	// 64bit
		T_F_UINT64, // 64bit
		T_DOUBLE,	// 64bit

		T_ARRAY,  // len
		T_STRING, // len
		T_CLASS,  // len
	};
};

void SchemaFileProcessor::processEndOfClassDefinition()
{
	hFile << printedPublic << "};\n";

	// encoding
	cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << currentDescribingClassName << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n\n";
	for (size_t i = 0; i < fieldsOfCurrentClass.size(); i++)
		cppFile << "\tpos = encode_" << fieldsOfCurrentClass[i] << "(bufferToPushBackEncoded, pos);\n";
	cppFile << "\n\treturn pos;\n}\n";

	// decoding
	cppFile << "\nsize_t " << currentDescribingClassName << "::decode_" << currentDescribingClassName << "(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos){\n";
	for (int i = fieldsOfCurrentClass.size() - 1; i >= 0; i--)
		cppFile << "\tpos = decode_" << fieldsOfCurrentClass[i] << "(bufferToPopBackEncoded, pos);\n";
	cppFile << "\n\treturn pos;}\n";
}

void SchemaFileProcessor::processFilename(std::string_view str)
{
	str.remove_prefix(fileNameMarker.length() + 1);
	filename = str;

	cppFile.open((filename + ".cpp").c_str());
	hFile.open((filename + ".h").c_str());

	fileNameRetrieved = true;

	hFile << "#pragma once\n";
	hFile << "#include <cstdint>\n";
	hFile << "#include <vector>\n";
	hFile << "#include <string>\n";
	hFile << "#include <fstream>\n";
	hFile << "\n\nnamespace " << filename << " {\n";

	cppFile << "#include \"" << filename + ".h" << "\"\n\n";
	cppFile << "#include \"types.h\"\n";
	cppFile << "\n\nnamespace " << filename << " {\n";
}

void SchemaFileProcessor::processField(std::string_view str)
{
	auto typeStrEnd = str.find(' ');
	auto typeStr = str.substr(0, typeStrEnd);

	str.remove_prefix(typeStrEnd + 1);

	auto nameStr = str;

	if (typeStr == "t_class")
	{
		nameStr = nameStr.substr(0, nameStr.find(' '));
		hFile << "\nclass " << nameStr << " {\nprivate:\n";

		cppFile << "\n// ----- " << nameStr;
		currentDescribingClassName = nameStr;
		definedClasses.emplace(nameStr);
		printedPublic = "\npublic:\n";
		fieldsOfCurrentClass.clear();
		currentClassId = 0;
	}

	else if (typeStr.starts_with("t_f_"))
	{
		typeStr.remove_prefix(std::string("t_f_").length());

		std::string cppTypeStr = std::string(typeStr);
		cppTypeStr += "_t";

		hFile << "\t" << cppTypeStr << " " << nameStr << ";\n";

		printedPublic.append("\t").append(cppTypeStr).append(" get_").append(nameStr).append("();\n");
		cppFile << "\n"
				<< cppTypeStr << " " << currentDescribingClassName << "::get_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

		printedPublic.append("\tvoid set_").append(nameStr).append("(").append(cppTypeStr).append(");\n");
		cppFile << "\nvoid " << currentDescribingClassName << "::set_" << nameStr << "(" << cppTypeStr << " v){\n\t" << nameStr << " = v;\n}\n";

		// encoding
		cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
		cppFile << "\treturn encodeFixed" << typeStr.substr(typeStr.length() - 2, 2) << "Bit((uint8_t *)&" << nameStr << ", bufferToPushBackEncoded, pos);\n}\n";

		// decoding
		cppFile << "\nsize_t " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos){\n";
		cppFile << "\treturn decodeFixed" << typeStr.substr(typeStr.length() - 2, 2) << "Bit((uint8_t *)&" << nameStr << ", bufferToPopBackEncoded, pos);\n}\n";
	}

	else if (typeStr.starts_with("t_v_s"))
	{
		typeStr.remove_prefix(std::string("t_v_s").length());

		std::string cppTypeStr = std::string(typeStr);
		cppTypeStr += "_t";

		hFile << "\t" << cppTypeStr << " " << nameStr << ";\n";

		printedPublic.append("\t").append(cppTypeStr).append(" get_").append(nameStr).append("();\n");
		cppFile << "\n"
				<< cppTypeStr << " " << currentDescribingClassName << "::get_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

		printedPublic.append("\tvoid set_").append(nameStr).append("(").append(cppTypeStr).append(");\n");
		cppFile << "\nvoid " << currentDescribingClassName << "::set_" << nameStr << "(" << cppTypeStr << " v){\n\t" << nameStr << " = v;\n}\n";

		// encoding
		cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
		cppFile << "\treturn encodeVarInt(IntType::SINT" << cppTypeStr.substr(cppTypeStr.length() - 4, 2) << ", (uint8_t *)&" << nameStr << ", bufferToPushBackEncoded, pos);\n}\n";

		// decoding
		cppFile << "\nsize_t " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos){\n";
		cppFile << "\treturn decodeVarInt(IntType::SINT" << cppTypeStr.substr(cppTypeStr.length() - 4, 2) << ", (uint8_t *)&" << nameStr << ", bufferToPopBackEncoded, pos);\n}\n";
	}

	else if (typeStr.starts_with("t_v_"))
	{
		typeStr.remove_prefix(std::string("t_v_").length());

		std::string cppTypeStr = std::string(typeStr);
		cppTypeStr += "_t";

		hFile << "\t" << cppTypeStr << " " << nameStr << ";\n";

		printedPublic.append("\t").append(cppTypeStr).append(" get_").append(nameStr).append("();\n");
		cppFile << "\n"
				<< cppTypeStr << " " << currentDescribingClassName << "::get_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

		printedPublic.append("\tvoid set_").append(nameStr).append("(").append(cppTypeStr).append(");\n");
		cppFile << "\nvoid " << currentDescribingClassName << "::set_" << nameStr << "(" << cppTypeStr << " v){\n\t" << nameStr << " = v;\n}\n";

		// encoding
		cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
		cppFile << "\treturn encodeVarInt(IntType::INT" << cppTypeStr.substr(cppTypeStr.length() - 4, 2) << ", (uint8_t *)&" << nameStr << ", bufferToPushBackEncoded, pos);\n}\n";

		// decoding
		cppFile << "\nsize_t " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos){\n";
		cppFile << "\treturn decodeVarInt(IntType::INT" << cppTypeStr.substr(cppTypeStr.length() - 4, 2) << ", (uint8_t *)&" << nameStr << ", bufferToPopBackEncoded, pos);\n}\n";
	}

	else if (typeStr == "t_float" || typeStr == "t_double")
	{
		typeStr.remove_prefix(typeMarker.length());
		hFile << "\t" << typeStr << " " << nameStr << ";\n";

		printedPublic.append("\t").append(typeStr).append(" get_").append(nameStr).append("();\n");
		cppFile << "\n"
				<< typeStr << " " << currentDescribingClassName << "::get_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

		printedPublic.append("\tvoid set_").append(nameStr).append("(").append(typeStr).append(");\n");
		cppFile << "\nvoid " << currentDescribingClassName << "::set_" << nameStr << "(" << typeStr << " v){\n\t" << nameStr << " = v;\n}\n";

		// encoding
		if (typeStr == "float")
		{
			cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
			cppFile << "\treturn encodeFixed32Bit((uint8_t *)&" << nameStr << ", bufferToPushBackEncoded, pos);\n}\n";
		}
		else
		{
			cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
			cppFile << "\treturn encodeFixed64Bit((uint8_t *)&" << nameStr << ", bufferToPushBackEncoded, pos);\n}\n";
		}

		// decoding
		if (typeStr == "float")
		{
			cppFile << "\nsize_t " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos){\n";
			cppFile << "\treturn decodeFixed32Bit((uint8_t *)&" << nameStr << ", bufferToPopBackEncoded, pos);\n}\n";
		}
		else
		{
			cppFile << "\nsize_t " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos){\n";
			cppFile << "\treturn decodeFixed64Bit((uint8_t *)&" << nameStr << ", bufferToPopBackEncoded, pos);\n}\n";
		}
	}

	else if (definedClasses.find(std::string(typeStr)) != definedClasses.end())
	{
		hFile << "\t" << typeStr << " " << nameStr << ";\n";

		printedPublic.append("\t").append(typeStr).append(" & ref_").append(nameStr).append("();\n");
		cppFile << "\n"
				<< typeStr << " & " << currentDescribingClassName << "::ref_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

		// encoding
		cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
		cppFile << "\treturn " << nameStr << ".encode_" << typeStr << "(bufferToPushBackEncoded, pos);\n}\n";

		// decoding
		cppFile << "\nsize_t " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos){\n";
		cppFile << "\treturn " << nameStr << ".decode_" << typeStr << "(bufferToPopBackEncoded, pos);\n}\n";
	}

	else if (typeStr == "t_array")
	{
		std::string cppTypeStr = "std::vector<";

		auto arrType = nameStr.substr(0, nameStr.find(' '));
		nameStr.remove_prefix(nameStr.find(' ') + 1);

		if (arrType == "t_v_int32" || arrType == "t_v_sint32" || arrType == "t_v_uint32" ||
			arrType == "t_v_int64" || arrType == "t_v_sint64" || arrType == "t_v_uint64")
		{
			arrType.remove_prefix(std::string("t_v_").length());

			std::string enumVal = std::string(arrType);
			std::transform(enumVal.begin(), enumVal.end(), enumVal.begin(), ::toupper);

			enumVal = (enumVal.starts_with('U') ? enumVal.substr(1) : enumVal);

			arrType = (arrType.starts_with('s') ? arrType.substr(1) : arrType);

			cppTypeStr += (std::string(arrType) + std::string("_t>"));

			hFile << "\t" << cppTypeStr << " " << nameStr << ";\n";

			printedPublic.append("\t").append(cppTypeStr).append(" & ref_").append(nameStr).append("();\n");
			cppFile << "\n"
					<< cppTypeStr << " & " << currentDescribingClassName << "::ref_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

			// encoding
			cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
			cppFile << "\tfor (size_t i = 0; i < " << nameStr << ".size(); i++)\n\t\tpos = encodeVarInt(IntType::" << enumVal << ", (uint8_t*)& " << nameStr << "[(" << nameStr << ".size() - 1) - i], bufferToPushBackEncoded, pos);";
			cppFile << "\n\tuint32_t sz = " << nameStr << ".size();\n\nreturn encodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPushBackEncoded, pos);\n}\n";

			// decoding
			cppFile << "\nsize_t " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos){\n";
			cppFile << "\n\tuint32_t sz;\n\tpos = decodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPopBackEncoded, pos);\n\t" << nameStr << ".resize(sz);\n\n\tfor (size_t i = 0; i < sz; i++)\n\t\tpos = decodeVarInt(IntType::" << enumVal << ", (uint8_t*)& " << nameStr << "[i], bufferToPopBackEncoded, pos);";
			cppFile << "\n\nreturn pos;\n}\n";
		}
		else if (arrType == "t_f_int32" || arrType == "t_f_uint32" || arrType == "t_float" ||
				 arrType == "t_f_int64" || arrType == "t_f_uint64" || arrType == "t_double")
		{
			arrType.remove_prefix(std::string("t_").length());
			if (arrType.starts_with("f_"))
				arrType.remove_prefix(std::string("f_").length());

			cppTypeStr += (std::string(arrType) + ((arrType == "float" || arrType == "double") ? "" : "_t") + std::string(">"));

			nameStr.remove_prefix(nameStr.find(' ') + 1);

			hFile << "\t" << cppTypeStr << " " << nameStr << ";\n";

			printedPublic.append("\t").append(cppTypeStr).append(" & ref_").append(nameStr).append("();\n");
			cppFile << "\n"
					<< cppTypeStr << " & " << currentDescribingClassName << "::ref_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

			auto bits = (arrType.ends_with("32") || arrType == "float") ? "32" : "64";

			// encoding
			cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
			cppFile << "\tfor (size_t i = 0; i < " << nameStr << ".size(); i++)\n\t\tpos = encodeFixed" << bits << "Bit((uint8_t*)& " << nameStr << "[(" << nameStr << ".size() - 1) - i], bufferToPushBackEncoded, pos);";
			cppFile << "\n\tuint32_t sz = " << nameStr << ".size();\n\nreturn encodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPushBackEncoded, pos);\n}\n";

			// decoding
			cppFile << "\nsize_t " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos){\n";
			cppFile << "\n\tuint32_t sz;\n\tpos = decodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPopBackEncoded, pos);\n\t" << nameStr << ".resize(sz);\n\n\tfor (size_t i = 0; i < sz; i++)\n\t\tpos = decodeFixed" << bits << "Bit((uint8_t*)& " << nameStr << "[i], bufferToPopBackEncoded, pos);";
			cppFile << "\n\nreturn pos;\n}\n";
		}
		else // string, class
		{

			if (arrType.starts_with("t_"))
			{
				arrType.remove_prefix(std::string("t_").length());
				cppTypeStr += "std::";
			}

			cppTypeStr += (std::string(arrType) + std::string(">"));

			nameStr.remove_prefix(nameStr.find(' ') + 1);

			hFile << "\t" << cppTypeStr << " " << nameStr << ";\n";

			printedPublic.append("\t").append(cppTypeStr).append(" & ref_").append(nameStr).append("();\n");
			cppFile << "\n"
					<< cppTypeStr << " & " << currentDescribingClassName << "::ref_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

			if (arrType == "string")
			{
				/*//encoding
				cppFile << "\nstd::vector<uint8_t> " << currentDescribingClassName << "::encode_" << nameStr << "(){\n";
				cppFile << "\tLen result;\n\tfor(int i = 0; i < " << nameStr << ".size(); i++){\n\t\tLen cur;\n\t\tcur.val = " << nameStr << "[(" << nameStr << ".size() - 1) - i];\n\t\tcur.encodeValue();";
				cppFile << "\t\tArray.val.insert(Array.val.begin(), cur.encoded.begin(), cur.encoded.end());\n\n\t}\n\t";
				cppFile << "Array.encodeValue();\n\treturn Array.encoded;\n}\n";

				// decoding
				cppFile << "\nint " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> data){\n";
				cppFile << "\tLen Array;\n\tArray.encoded = data;\n\tint res = Array.decodeValue();\n\tFixed" << bits << "Bit<" << arrType << ((arrType == "float" || arrType == "double") ? "" : "_t") << "> cur;\n\tcur.encoded = Array.val;\n\t";
				cppFile << "while(cur.encoded.size() != 0){\n\t\tint cut = cur.decodeValue();\n\t\t" << nameStr << ".push_back(cur.val);\n\t\tcur.encoded.erase(cur.encoded.begin(), cur.encoded.begin() + cut);\n\t}";
				cppFile << "\n\treturn res;\n}\n";

				// encoding
				cppFile << "\nstd::vector<uint8_t> " << currentDescribingClassName << "::encode_" << nameStr << "(){\n";
				cppFile << "\n\tLen encodedSmth;\n\tfor (int i = 0; i < " << nameStr << ".size(); i++)\n\t\encodedSmth.val.push_back(" << nameStr << "[i]);\n\encodedSmth.encodeValue();\n\treturn encodedSmth.encoded;\n}\n";

				// decoding
				cppFile << "\nint " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> data){\n";
				cppFile << "\tLen Smth;\n\Smth.encoded = data;\n\tint res = Smth.decodeValue();\n\t";
				cppFile << "for (int i = 0; i < Smth.val.size(); i++)\n\t\t" << nameStr << ".append(1, char(Smth.val[i]));\n\treturn res;\n}\n";*/

				// encoding
				cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
				cppFile << "\tfor (size_t i = 0; i < " << nameStr << ".size(); i++){\n\t\tuint32_t tmpsz = " << nameStr << "[(" << nameStr << ".size() - 1) - i].length();\n\tfor(size_t j = 0; j < tmpsz; j++)" << "\n\t\tbufferToPushBackEncoded[pos++] = " << nameStr << "[(" << nameStr << ".size() - 1) - i][j];\n\tpos = encodeVarInt(IntType::INT32, (uint8_t*)&tmpsz, bufferToPushBackEncoded, pos);\n\t}";
				cppFile << "\n\tuint32_t sz = " << nameStr << ".size();\n\nreturn encodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPushBackEncoded, pos);\n}\n";

				// decoding
				cppFile << "\nsize_t " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos){\n";
				cppFile << "\n\tuint32_t sz; \n\tpos = decodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPopBackEncoded, pos); \n\t" << nameStr << ".resize(sz); \n\n\tfor(size_t i = 0; i < sz; i++){\n\t\tuint32_t tmpsz;\n\tpos = decodeVarInt(IntType::INT32, (uint8_t*)&tmpsz, bufferToPopBackEncoded, pos);\n\t" << nameStr << "[i].resize(tmpsz);\n\tfor(size_t j = 0; j < tmpsz; j++)\n\t\t" << nameStr << "[i][tmpsz - j - 1] = bufferToPopBackEncoded[pos--];\n\t}";
				cppFile << "\n\nreturn pos;\n}\n";
			}
			else
			{
				// encoding
				cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
				cppFile << "\tfor (size_t i = 0; i < " << nameStr << ".size(); i++)\n\t\tpos = " << nameStr << "[(" << nameStr << ".size() - 1) - i].encode_" << arrType << "(bufferToPushBackEncoded, pos);\n\t";
				cppFile << "\n\tuint32_t sz = " << nameStr << ".size();\n\nreturn encodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPushBackEncoded, pos);\n}\n";

				// decoding
				cppFile << "\nsize_t " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos){\n";
				cppFile << "\n\tuint32_t sz;\n\tpos = decodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPopBackEncoded, pos);\n\t" << nameStr << ".resize(sz);\n\n\tfor (size_t i = 0; i < sz; i++)\n\t\tpos = " << nameStr << "[i].decode_" << arrType << "(bufferToPopBackEncoded, pos);\n\t";
				cppFile << "\n\nreturn pos;\n}\n";
			}
		}
	}

	else if (typeStr == "t_string")
	{
		std::string cppTypeStr = "std::string";

		hFile << "\t" << cppTypeStr << " " << nameStr << ";\n";

		printedPublic.append("\t").append(cppTypeStr).append(" & ref_").append(nameStr).append("();\n");
		cppFile << "\n"
				<< cppTypeStr << " & " << currentDescribingClassName << "::ref_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

		// encoding
		cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
		cppFile << "\tfor (size_t i = 0; i < " << nameStr << ".length(); i++)\n\t\tbufferToPushBackEncoded[pos++] = " << nameStr << "[(" << nameStr << ".length() - 1) - i];";
		cppFile << "\n\tuint32_t sz = " << nameStr << ".length();\n\nreturn encodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPushBackEncoded, pos);\n}\n";

		// decoding
		cppFile << "\nsize_t " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos){\n";
		cppFile << "\n\tuint32_t sz;\n\tpos = decodeVarInt(IntType::INT32, (uint8_t*)&sz, bufferToPopBackEncoded, pos);\n\t" << nameStr << ".resize(sz); \n\n\tfor(size_t i = 0; i < sz; i++)\n\t\t" << nameStr << "[i] = bufferToPopBackEncoded[pos--];\n\t";
		cppFile << "\n\nreturn pos;\n}\n";
	}

	if (typeStr != "t_class")
	{
		fieldsOfCurrentClass.emplace(currentClassId, nameStr);
		currentClassId++;

		hFile << "\tsize_t encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos);\n";
		hFile << "\tsize_t decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos);\n\n";
	}
	else
	{
		printedPublic.append("\tsize_t encode_").append(nameStr).append("(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos);\n");
		printedPublic.append("\tsize_t decode_").append(nameStr).append("(std::vector<uint8_t> & bufferToPopBackEncoded, size_t pos);\n\n");
	}
}

int main(int argc, char **argv)
{
	std::string schemeFileName = (argc < 2) ? "schema.bs" : std::string(argv[1]);

	std::ifstream inf(schemeFileName);

	SchemaFileProcessor processor;

	std::string curStr;
	std::string_view curStrView;

	while (inf)
	{
		std::getline(inf, curStr);
		curStrView = curStr;

		if (curStrView.starts_with(processor.commentMarker) == false)
		{
			if (processor.fileNameRetrieved == false && curStrView.starts_with(processor.fileNameMarker) == true)
				processor.processFilename(curStrView);

			if (curStr.starts_with(processor.typeMarker) ||
				processor.definedClasses.find(curStr.substr(0, curStr.find(' '))) != processor.definedClasses.end())
				processor.processField(curStrView);

			if (curStr.starts_with("}"))
			{

				processor.processEndOfClassDefinition();
			}
			std::cout << curStr << '\n';
		}
	}

	processor.cppFile << "\n}";
	processor.hFile << "\n}";
}