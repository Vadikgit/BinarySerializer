// BinarySerializer.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

class SchemaFileProcessor {
public:
	std::string filename;
	std::string printedPublic;
	std::string currentDescribingClassName;
	std::string currentDescribingClassEncoder;
	const std::string fileNameMarker = "filename";
	const std::string typeMarker = "t_";
	const std::string commentMarker = "#";
	bool fileNameRetrieved;
	
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
		T_V_INT32,   // varint
		T_V_SINT32,  // varint
		T_V_UINT32,  // varint
		T_V_INT64,   // varint
		T_V_SINT64,  // varint
		T_V_UINT64,  // varint

		T_F_INT32,   // 32bit
		T_F_UINT32,  // 32bit
		T_FLOAT,     // 32bit

		T_F_INT64,   // 64bit
		T_F_UINT64,  // 64bit
		T_DOUBLE,    // 64bit

		T_ARRAY,     // len
		T_STRING,    // len
		T_CLASS,     // len
	};

};

void SchemaFileProcessor::processEndOfClassDefinition() {
	hFile << printedPublic << "};\n";

	// encoding
	cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << currentDescribingClassName << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n\n";
	cppFile << "\n\tLen encodedClass;\n\tencodedClass.val.resize(bufferToPushBackEncoded.size() - pos);\n\tsize_t len = 0;\n";

	for (size_t i = 0; i < fieldsOfCurrentClass.size(); i++)
		cppFile << "\tlen = encode_" << fieldsOfCurrentClass[i] << "(encodedClass.val, len);\n";

	cppFile << "\n\tencodedClass.val.resize(len);\n\treturn encodedClass.encodeValue(bufferToPushBackEncoded, pos);\n}\n";

	// decoding
	cppFile << "\nvoid " << currentDescribingClassName << "::decode_" << currentDescribingClassName << "(std::vector<uint8_t> & bufferToPopBackEncoded){\n";
	cppFile << "\tLen Class;\n\tClass.decodeValue(bufferToPopBackEncoded);\n\t";

	for (int i = fieldsOfCurrentClass.size() - 1; i >= 0; i--)
		cppFile << "\tdecode_" << fieldsOfCurrentClass[i] << "(Class.val);\n";

	cppFile << "\n}\n";
}

void SchemaFileProcessor::processFilename(std::string_view str) {
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
	hFile << "#include \"types.h\"\n";

	cppFile << "#include \"" << filename + ".h" << "\"\n\n";
}

void SchemaFileProcessor::processField(std::string_view str) {
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
		cppFile << "\n" << cppTypeStr << " " << currentDescribingClassName << "::get_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

		printedPublic.append("\tvoid set_").append(nameStr).append("(").append(cppTypeStr).append(");\n");
		cppFile << "\nvoid " << currentDescribingClassName << "::set_" << nameStr << "(" << cppTypeStr << " v){\n\t" << nameStr << " = v;\n}\n";

		// encoding
		cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
		cppFile << "\tFixed" << typeStr.substr(typeStr.length() - 2, 2) << "Bit<" << cppTypeStr << "> value;\n\tvalue.val = get_" << nameStr << "();\n\treturn value.encodeValue(bufferToPushBackEncoded, pos);\n}\n";

		// decoding
		cppFile << "\nvoid " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded){\n";
		cppFile << "\tFixed" << typeStr.substr(typeStr.length() - 2, 2) << "Bit<" << cppTypeStr << "> value;\n\tvalue.decodeValue(bufferToPopBackEncoded);\n\tset_" << nameStr << "(value.val);\n}\n";
	}

	else if (typeStr.starts_with("t_v_s"))
	{
		typeStr.remove_prefix(std::string("t_v_s").length());

		std::string cppTypeStr = std::string(typeStr);
		cppTypeStr += "_t";

		hFile << "\t" << cppTypeStr << " " << nameStr << ";\n";

		printedPublic.append("\t").append(cppTypeStr).append(" get_").append(nameStr).append("();\n");
		cppFile << "\n" << cppTypeStr << " " << currentDescribingClassName << "::get_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

		printedPublic.append("\tvoid set_").append(nameStr).append("(").append(cppTypeStr).append(");\n");
		cppFile << "\nvoid " << currentDescribingClassName << "::set_" << nameStr << "(" << cppTypeStr << " v){\n\t" << nameStr << " = v;\n}\n";

		// encoding
		cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
		cppFile << "\tVarInt<" << cppTypeStr << "> value;\n\tvalue.val = get_" << nameStr << "();\n\tvalue.valType = VarInt<" << cppTypeStr << ">::IntType::SINT" << cppTypeStr.substr(cppTypeStr.length() - 4, 2) << ";\n\treturn value.encodeValue(bufferToPushBackEncoded, pos);\n}\n";

		// decoding
		cppFile << "\nvoid " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded){\n";
		cppFile << "\tVarInt<" << cppTypeStr << "> value;\n\tvalue.valType = VarInt<" << cppTypeStr << ">::IntType::SINT" << cppTypeStr.substr(cppTypeStr.length() - 4, 2) << ";\n\tvalue.decodeValue(bufferToPopBackEncoded);\n\tset_" << nameStr << "(value.val);\n}\n";
	}

	else if (typeStr.starts_with("t_v_"))
	{
		typeStr.remove_prefix(std::string("t_v_").length());

		std::string cppTypeStr = std::string(typeStr);
		cppTypeStr += "_t";

		hFile << "\t" << cppTypeStr << " " << nameStr << ";\n";

		printedPublic.append("\t").append(cppTypeStr).append(" get_").append(nameStr).append("();\n");
		cppFile << "\n" << cppTypeStr << " " << currentDescribingClassName << "::get_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

		printedPublic.append("\tvoid set_").append(nameStr).append("(").append(cppTypeStr).append(");\n");
		cppFile << "\nvoid " << currentDescribingClassName << "::set_" << nameStr << "(" << cppTypeStr << " v){\n\t" << nameStr << " = v;\n}\n";

		//encoding
		cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
		cppFile << "\tVarInt<" << cppTypeStr << "> value;\n\tvalue.val = get_" << nameStr << "();\n\tvalue.valType = VarInt<" << cppTypeStr << ">::IntType::INT" << cppTypeStr.substr(cppTypeStr.length() - 4, 2) << ";\n\treturn value.encodeValue(bufferToPushBackEncoded, pos);\n}\n";

		// decoding
		cppFile << "\nvoid " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded){\n";
		cppFile << "\tVarInt<" << cppTypeStr << "> value;\n\tvalue.valType = VarInt<" << cppTypeStr << ">::IntType::INT" << cppTypeStr.substr(cppTypeStr.length() - 4, 2) << ";\n\tvalue.decodeValue(bufferToPopBackEncoded);\n\tset_" << nameStr << "(value.val);\n}\n";
	}

	else if (typeStr == "t_float" || typeStr == "t_double")
	{
		typeStr.remove_prefix(typeMarker.length());
		hFile << "\t" << typeStr << " " << nameStr << ";\n";

		printedPublic.append("\t").append(typeStr).append(" get_").append(nameStr).append("();\n");
		cppFile << "\n" << typeStr << " " << currentDescribingClassName << "::get_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

		printedPublic.append("\tvoid set_").append(nameStr).append("(").append(typeStr).append(");\n");
		cppFile << "\nvoid " << currentDescribingClassName << "::set_" << nameStr << "(" << typeStr << " v){\n\t" << nameStr << " = v;\n}\n";

		//encoding
		if (typeStr == "float")
		{
			cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
			cppFile << "\tFixed32Bit<" << typeStr << "> value;\n\tvalue.val = get_" << nameStr << "();\n\treturn value.encodeValue(bufferToPushBackEncoded, pos);\n}\n";
		}
		else
		{
			cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
			cppFile << "\tFixed64Bit<" << typeStr << "> value;\n\tvalue.val = get_" << nameStr << "();\n\treturn value.encodeValue(bufferToPushBackEncoded, pos);\n}\n";
		}

		// decoding
		if (typeStr == "float")
		{
			cppFile << "\nvoid " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded){\n";
			cppFile << "\tFixed32Bit<" << typeStr << "> value;\n\tvalue.decodeValue(bufferToPopBackEncoded);\n\tset_" << nameStr << "(value.val);\n}\n";
		}
		else
		{
			cppFile << "\nvoid " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded){\n";
			cppFile << "\tFixed64Bit<" << typeStr << "> value;\n\tvalue.decodeValue(bufferToPopBackEncoded);\n\tset_" << nameStr << "(value.val);\n}\n";
		}
	}

	else if (definedClasses.find(std::string(typeStr)) != definedClasses.end())
	{
		hFile << "\t" << typeStr << " " << nameStr << ";\n";

		printedPublic.append("\t").append(typeStr).append(" & ref_").append(nameStr).append("();\n");
		cppFile << "\n" << typeStr << " & " << currentDescribingClassName << "::ref_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";


		//encoding
		cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
		cppFile << "\treturn " << nameStr << ".encode_" << typeStr << "(bufferToPushBackEncoded, pos);\n}\n";

		// decoding
		cppFile << "\nvoid " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded){\n";
		cppFile << "\t" << nameStr << ".decode_" << typeStr << "(bufferToPopBackEncoded);\n}\n";

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

			arrType = (arrType.starts_with('s') ? arrType.substr(1) : arrType);
			
			cppTypeStr += (std::string(arrType) + std::string("_t>"));


			hFile << "\t" << cppTypeStr << " " << nameStr << ";\n";

			printedPublic.append("\t").append(cppTypeStr).append(" & ref_").append(nameStr).append("();\n");
			cppFile << "\n" << cppTypeStr << " & " << currentDescribingClassName << "::ref_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

			//encoding
			cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
			cppFile << "\tLen Array;\n\tArray.val.resize(bufferToPushBackEncoded.size() - pos);\n\tsize_t len = 0;\n\tVarInt<" << arrType << "_t> cur;\n\tfor(int i = 0; i < " << nameStr << ".size(); i++){\n\t\tcur.val = " << nameStr << "[(" << nameStr << ".size() - 1) - i];\n\t\tcur.valType = VarInt<" << arrType << "_t>::IntType::" << enumVal << ";\n\t\tlen = cur.encodeValue(Array.val, len);";
			cppFile << "\n\n\t}\n\t"; 
			cppFile << "Array.val.resize(len);\n\treturn Array.encodeValue(bufferToPushBackEncoded, pos);\n}\n";
			
			// decoding
			cppFile << "\nvoid " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded){\n"; 
			cppFile << "\tLen Array;\n\tArray.decodeValue(bufferToPopBackEncoded);\n\tVarInt<" << arrType << "_t> cur;\n\tcur.valType = VarInt<" << arrType << "_t>::IntType::" << enumVal << ";\n\t";
			cppFile << "while(Array.val.size() != 0){\n\t\tcur.decodeValue(Array.val);\n\t\t" << nameStr << ".push_back(cur.val);\n\t}";
			cppFile << "\n}\n";
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
			cppFile << "\n" << cppTypeStr << " & " << currentDescribingClassName << "::ref_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

			auto bits = (arrType.ends_with("32") || arrType == "float") ? "32" : "64";

			//encoding
			cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
			cppFile << "\tLen Array;\n\tArray.val.resize(bufferToPushBackEncoded.size() - pos);\n\tsize_t len = 0;\n\tFixed" << bits << "Bit<" << arrType << ((arrType == "float" || arrType == "double") ? "" : "_t") << "> cur;\n\tfor(int i = 0; i < " << nameStr << ".size(); i++){\n\t\tcur.val = " << nameStr << "[(" << nameStr << ".size() - 1) - i];\n\t\tlen = cur.encodeValue(Array.val, len);";
			cppFile << "\n\n\t}\n\t";
			cppFile << "Array.val.resize(len);\n\treturn Array.encodeValue(bufferToPushBackEncoded, pos);\n}\n";

			// decoding
			cppFile << "\nvoid " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded){\n";
			cppFile << "\tLen Array;\n\tArray.decodeValue(bufferToPopBackEncoded);\n\tFixed" << bits << "Bit<" << arrType << ((arrType == "float" || arrType == "double") ? "" : "_t") << "> cur;\n\t";
			cppFile << "while(Array.val.size() != 0){\n\t\tcur.decodeValue(Array.val);\n\t\t" << nameStr << ".push_back(cur.val);\n\t}\n}\n";
		}
		else // string, class
		{
			
			if (arrType.starts_with("t_")) {
				arrType.remove_prefix(std::string("t_").length());
				cppTypeStr += "std::";
			}

			cppTypeStr += (std::string(arrType) + std::string(">"));

			nameStr.remove_prefix(nameStr.find(' ') + 1);



			hFile << "\t" << cppTypeStr << " " << nameStr << ";\n";

			printedPublic.append("\t").append(cppTypeStr).append(" & ref_").append(nameStr).append("();\n");
			cppFile << "\n" << cppTypeStr << " & " << currentDescribingClassName << "::ref_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

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

				//encoding
				cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
				cppFile << "\tLen result;\n\tresult.val.resize(bufferToPushBackEncoded.size() - pos);\n\tsize_t len = 0;\n\tLen cur;\n\tfor(int i = 0; i < " << nameStr << ".size(); i++){\n\t\tcur.val.resize(" << nameStr << "[i].size());\n\t\tfor (int j = 0; j < " << nameStr << "[" << nameStr << ".size() - 1 - i].size(); j++){\n\t\t\tcur.val[j] = (uint8_t)" << nameStr << "[" << nameStr << ".size() - 1 - i][j];\n\t\t}\n\t\tlen = cur.encodeValue(result.val, len);\n\t\t}\n\t";
				cppFile << "\tresult.val.resize(len);\n\treturn result.encodeValue(bufferToPushBackEncoded, pos);\n\n}\n";

				// decoding
				cppFile << "\nvoid " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded){\n";
				cppFile << "\tLen result;\n\tresult.decodeValue(bufferToPopBackEncoded);\n\t";
				cppFile << "while(result.val.size() != 0){\n\t\tLen cur;\n\t\tcur.decodeValue(result.val);\n\t\tstd::string str;\n\t\tfor (int j = 0; j < cur.val.size(); j++)\n\t\t\tstr.push_back((char) cur.val[j]);\n\n\t\t" << nameStr << ".push_back(str);\n\t}\n}\n";
			}
			else
			{
				//encoding
				cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
				cppFile << "\tLen result;\n\tresult.val.resize(bufferToPushBackEncoded.size() - pos);\n\tsize_t len = 0;\n\tfor(int i = 0; i < " << nameStr << ".size(); i++){\n\t\tlen = " << nameStr << "[(" << nameStr << ".size() - 1) - i].encode_" << arrType << "(result.val, len);\n\t\t}\n";
				cppFile << "\t\tresult.val.resize(len);\t\treturn result.encodeValue(bufferToPushBackEncoded, pos);\n\n}\n";

				// decoding
				cppFile << "\nvoid " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded){\n";
				cppFile << "\tLen result;\n\tresult.decodeValue(bufferToPopBackEncoded);\n\t";
				cppFile << "while(result.val.size() != 0){\n\t\t" << nameStr << ".push_back(" << arrType << "{});\n\t\t" << nameStr << ".rbegin()->decode_" << arrType << "(result.val);\n\t}\n}\n";
			}
		}
	
	}

	else if (typeStr == "t_string")
	{
		std::string cppTypeStr = "std::string";

		hFile << "\t" << cppTypeStr << " " << nameStr << ";\n";

		printedPublic.append("\t").append(cppTypeStr).append(" & ref_").append(nameStr).append("();\n");
		cppFile << "\n" << cppTypeStr << " & " << currentDescribingClassName << "::ref_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

		// encoding
		cppFile << "\nsize_t " << currentDescribingClassName << "::encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos){\n";
		cppFile << "\n\tLen encodedString;\n\tencodedString.val.resize(" << nameStr << ".length());\n\tfor (int i = 0; i < " << nameStr << ".length(); i++)\n\t\tencodedString.val[i] = " << nameStr << "[i];\n\treturn encodedString.encodeValue(bufferToPushBackEncoded, pos);\n}\n";

		// decoding
		cppFile << "\nvoid " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded){\n";
		cppFile << "\tLen String;\n\tString.decodeValue(bufferToPopBackEncoded);\n\t";
		cppFile << "for (int i = 0; i < String.val.size(); i++)\n\t\t" << nameStr << ".append(1, char(String.val[i]));\n}\n";
	}


	if (typeStr != "t_class")
	{
		fieldsOfCurrentClass.emplace(currentClassId, nameStr);
		currentClassId++;

		hFile << "\tsize_t encode_" << nameStr << "(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos);\n";
		hFile << "\tvoid decode_" << nameStr << "(std::vector<uint8_t> & bufferToPopBackEncoded);\n\n";
	}
	else
	{
		printedPublic.append("\tsize_t encode_").append(nameStr).append("(std::vector<uint8_t> & bufferToPushBackEncoded, size_t pos);\n");
		printedPublic.append("\tvoid decode_").append(nameStr).append("(std::vector<uint8_t> & bufferToPopBackEncoded);\n\n");
	}

}

int main()
{
    std::ifstream inf("schema.bs");

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

			if (curStr.starts_with("}")) {
				
				processor.processEndOfClassDefinition();
			}
			std::cout << curStr << '\n';
		}
	}
}