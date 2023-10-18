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
	cppFile << "\nstd::vector<uint8_t> " << currentDescribingClassName << "::encode_" << currentDescribingClassName << "(){\n";
	cppFile << "\tstd::vector<uint8_t> res, temp;\n\n";
	for (size_t i = 0; i < fieldsOfCurrentClass.size(); i++)
	{
		cppFile << "\ttemp = encode_" << fieldsOfCurrentClass[i] << "();\n";
		cppFile << "\tres.insert(res.begin(), temp.begin(), temp.end());\n";
	}

	cppFile << "\n\tLen encodedClass;\n\tencodedClass.val = res;\n\tencodedClass.encodeValue();\n\treturn encodedClass.encoded;\n}\n";

	// decoding
	cppFile << "\nint " << currentDescribingClassName << "::decode_" << currentDescribingClassName << "(std::vector<uint8_t> data){\n";
	cppFile << "\tLen Class;\n\tClass.encoded = data;\n\tint cutLen, res = Class.decodeValue();\n\tauto classBytes = Class.val;\n\t";

	for (int i = fieldsOfCurrentClass.size() - 1; i >= 0; i--)
	{
		cppFile << "\tcutLen = decode_" << fieldsOfCurrentClass[i] << "(classBytes);\n";
		cppFile << "\tclassBytes.erase(classBytes.begin(), classBytes.begin() + cutLen);\n";
	}

	cppFile << "\n\treturn res;\n}\n";
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

		//encoding
		cppFile << "\nstd::vector<uint8_t> " << currentDescribingClassName << "::encode_" << nameStr << "(){\n";
		cppFile << "\tFixed" << typeStr.substr(typeStr.length() - 2, 2) << "Bit<" << cppTypeStr << "> value;\n\tvalue.val = get_" << nameStr << "();\n\tvalue.encodeValue();";
		cppFile << "\n\treturn value.encoded;\n}\n";

		// decoding
		cppFile << "\nint " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> data){\n";
		cppFile << "\tFixed" << typeStr.substr(typeStr.length() - 2, 2) << "Bit<" << cppTypeStr << "> value;\n\tvalue.encoded = data;\n\tint res = value.decodeValue();\n\tset_" << nameStr << "(value.val);";
		cppFile << "\n\treturn res;\n}\n";
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
		cppFile << "\nstd::vector<uint8_t> " << currentDescribingClassName << "::encode_" << nameStr << "(){\n";
		cppFile << "\tVarInt<" << cppTypeStr << "> value;\n\tvalue.val = get_" << nameStr << "();\n\tvalue.valType = VarInt<" << cppTypeStr << ">::IntType::SINT" << cppTypeStr.substr(cppTypeStr.length() - 4, 2) << ";\n\tvalue.encodeValue();";
		cppFile << "\n\treturn value.encoded;\n}\n";

		// decoding
		cppFile << "\nint " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> data){\n";
		cppFile << "\tVarInt<" << cppTypeStr << "> value;\n\tvalue.valType = VarInt<" << cppTypeStr << ">::IntType::SINT" << cppTypeStr.substr(cppTypeStr.length() - 4, 2) << ";\n\tvalue.encoded = data;" << "\n\tint res = value.decodeValue(); \n\tset_" << nameStr << "(value.val);";
		cppFile << "\n\treturn res;\n}\n";
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
		cppFile << "\nstd::vector<uint8_t> " << currentDescribingClassName << "::encode_" << nameStr << "(){\n";
		cppFile << "\tVarInt<" << cppTypeStr << "> value;\n\tvalue.val = get_" << nameStr << "();\n\tvalue.valType = VarInt<" << cppTypeStr << ">::IntType::INT" << cppTypeStr.substr(cppTypeStr.length() - 4, 2) << ";\n\tvalue.encodeValue();";
		cppFile << "\n\treturn value.encoded;\n}\n";

		// decoding
		cppFile << "\nint " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> data){\n";
		cppFile << "\tVarInt<" << cppTypeStr << "> value;\n\tvalue.valType = VarInt<" << cppTypeStr << ">::IntType::INT" << cppTypeStr.substr(cppTypeStr.length() - 4, 2) << ";\n\tvalue.encoded = data;" << "\n\tint res = value.decodeValue(); \n\tset_" << nameStr << "(value.val);";
		cppFile << "\n\treturn res;\n}\n";
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
			cppFile << "\nstd::vector<uint8_t> " << currentDescribingClassName << "::encode_" << nameStr << "(){\n";
			cppFile << "\tFixed32Bit<" << typeStr << "> value;\n\tvalue.val = get_" << nameStr << "();\n\tvalue.encodeValue();";
			cppFile << "\n\treturn value.encoded;\n}\n";
		}
		else
		{
			cppFile << "\nstd::vector<uint8_t> " << currentDescribingClassName << "::encode_" << nameStr << "(){\n";
			cppFile << "\tFixed64Bit<" << typeStr << "> value;\n\tvalue.val = get_" << nameStr << "();\n\tvalue.encodeValue();";
			cppFile << "\n\treturn value.encoded;\n}\n";
		}

		// decoding
		if (typeStr == "float")
		{
			cppFile << "\nint " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> data){\n";
			cppFile << "\tFixed32Bit<" << typeStr << "> value;\n\tvalue.encoded = data;\n\tint res = value.decodeValue();\n\tset_" << nameStr << "(value.val);";
			cppFile << "\n\treturn res;\n}\n";
		}
		else
		{
			cppFile << "\nint " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> data){\n";
			cppFile << "\tFixed64Bit<" << typeStr << "> value;\n\tvalue.encoded = data;\n\tint res = value.decodeValue();\n\tset_" << nameStr << "(value.val);";
			cppFile << "\n\treturn res;\n}\n";
		}
	}

	else if (definedClasses.find(std::string(typeStr)) != definedClasses.end())
	{
		hFile << "\t" << typeStr << " " << nameStr << ";\n";

		printedPublic.append("\t").append(typeStr).append(" & ref_").append(nameStr).append("();\n");
		cppFile << "\n" << typeStr << " & " << currentDescribingClassName << "::ref_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";


		//encoding
		cppFile << "\nstd::vector<uint8_t> " << currentDescribingClassName << "::encode_" << nameStr << "(){\n";
		cppFile << "\treturn " << nameStr << ".encode_" << typeStr << "();\n}\n";

		// decoding
		cppFile << "\nint " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> data){\n";
		cppFile << "\tint res = " << nameStr << ".decode_" << typeStr << "(data);";
		cppFile << "\n\treturn res;\n}\n";

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
			cppFile << "\nstd::vector<uint8_t> " << currentDescribingClassName << "::encode_" << nameStr << "(){\n";
			cppFile << "\tLen Array;\n\tfor(int i = 0; i < " << nameStr << ".size(); i++){\n\t\tVarInt<" << arrType << "_t> cur;\n\t\tcur.val = " << nameStr << "[(" << nameStr << ".size() - 1) - i];\n\t\tcur.valType = VarInt<" << arrType << "_t>::IntType::" << enumVal << ";\n\t\tcur.encodeValue();";
			cppFile << "\t\tArray.val.insert(Array.val.begin(), cur.encoded.begin(), cur.encoded.end());\n\n\t}\n\t"; 
			cppFile << "Array.encodeValue();\n\treturn Array.encoded;\n}\n";
			
			// decoding
			cppFile << "\nint " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> data){\n"; 
			cppFile << "\tLen Array;\n\tArray.encoded = data;\n\tint res = Array.decodeValue();\n\tVarInt<" << arrType << "_t> cur;\n\tcur.valType = VarInt<" << arrType << "_t>::IntType::" << enumVal << ";\n\tcur.encoded = Array.val;\n\t";
			cppFile << "while(cur.encoded.size() != 0){\n\t\tint cut = cur.decodeValue();\n\t\t" << nameStr << ".push_back(cur.val);\n\t\tcur.encoded.erase(cur.encoded.begin(), cur.encoded.begin() + cut);\n\t}";
			cppFile << "\n\treturn res;\n}\n";
		}
		else if (arrType == "t_f_int32" || arrType == "t_f_uint32" || arrType == "t_float" ||
				 arrType == "t_f_int64" || arrType == "t_f_uint64" || arrType == "t_double")
		{
			arrType.remove_prefix(std::string("t_").length());
			if (arrType.starts_with("f_"))
				arrType.remove_prefix(std::string("f_").length());
		
			cppTypeStr += (std::string(arrType) + std::string(">"));

			nameStr.remove_prefix(nameStr.find(' ') + 1);

			hFile << "\t" << cppTypeStr << " " << nameStr << ";\n";

			printedPublic.append("\t").append(cppTypeStr).append(" & ref_").append(nameStr).append("();\n");
			cppFile << "\n" << cppTypeStr << " & " << currentDescribingClassName << "::ref_" << nameStr << "(){\n\treturn " << nameStr << ";\n}\n";

			auto bits = (arrType.ends_with("32") || arrType == "float") ? "32" : "64";

			//encoding
			cppFile << "\nstd::vector<uint8_t> " << currentDescribingClassName << "::encode_" << nameStr << "(){\n";
			cppFile << "\tLen Array;\n\tfor(int i = 0; i < " << nameStr << ".size(); i++){\n\t\tFixed" << bits << "Bit<" << arrType << ((arrType == "float" || arrType == "double") ? "" : "_t") << "> cur;\n\t\tcur.val = " << nameStr << "[(" << nameStr << ".size() - 1) - i];\n\t\tcur.encodeValue();";
			cppFile << "\t\tArray.val.insert(Array.val.begin(), cur.encoded.begin(), cur.encoded.end());\n\n\t}\n\t";
			cppFile << "Array.encodeValue();\n\treturn Array.encoded;\n}\n";

			// decoding
			cppFile << "\nint " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> data){\n";
			cppFile << "\tLen Array;\n\tArray.encoded = data;\n\tint res = Array.decodeValue();\n\tFixed" << bits << "Bit<" << arrType << ((arrType == "float" || arrType == "double") ? "" : "_t") << "> cur;\n\tcur.encoded = Array.val;\n\t";
			cppFile << "while(cur.encoded.size() != 0){\n\t\tint cut = cur.decodeValue();\n\t\t" << nameStr << ".push_back(cur.val);\n\t\tcur.encoded.erase(cur.encoded.begin(), cur.encoded.begin() + cut);\n\t}";
			cppFile << "\n\treturn res;\n}\n";
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
				cppFile << "\nstd::vector<uint8_t> " << currentDescribingClassName << "::encode_" << nameStr << "(){\n";
				cppFile << "\tLen result;\n\tfor(int i = 0; i < " << nameStr << ".size(); i++){\n\t\tLen cur;\n\t\tfor (int j = 0; j < " << nameStr << "[" << nameStr << ".size() - 1 - i].size(); j++){\n\t\t\tcur.val.push_back((uint8_t)" << nameStr << "[" << nameStr << ".size() - 1 - i][j]);\n\t\t}\n\t\tcur.encodeValue();\n\t\tresult.val.insert(result.val.begin(), cur.encoded.begin(), cur.encoded.end());\n\t\t}\n\t";
				cppFile << "\tresult.encodeValue();\n";
				cppFile << "\n\treturn result.encoded;\n}\n";

				// decoding
				cppFile << "\nint " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> data){\n";
				cppFile << "\tLen result;\n\tresult.encoded = data;\n\tint res = result.decodeValue();\n\t";
				cppFile << "while(result.val.size() != 0){\n\t\tLen cur;\n\t\tcur.encoded = result.val;\n\t\tint cut = cur.decodeValue();\n\t\tstd::string str;\n\t\tfor (int j = 0; j < cur.val.size(); j++)\n\t\t\tstr.push_back((char) cur.val[j]);\n\n\t\t" << nameStr << ".push_back(str);\n\t\tresult.val.erase(result.val.begin(), result.val.begin() + cut);\n\t}";
				cppFile << "\n\treturn res;\n}\n";
			}
			else
			{
				//encoding
				cppFile << "\nstd::vector<uint8_t> " << currentDescribingClassName << "::encode_" << nameStr << "(){\n";
				cppFile << "\tLen result;\n\tfor(int i = 0; i < " << nameStr << ".size(); i++){\n\t\tauto encoded = " << nameStr << "[(" << nameStr << ".size() - 1) - i].encode_" << arrType << "();\n\t\tresult.val.insert(result.val.begin(), encoded.begin(), encoded.end());\n\t}\n";
				cppFile << "\t\tresult.encodeValue();\n";
				cppFile << "\n\treturn result.encoded;\n}\n";

				// decoding
				cppFile << "\nint " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> data){\n";
				cppFile << "\tLen result;\n\tresult.encoded = data;\n\tint res = result.decodeValue();\n\t";
				cppFile << "while(result.val.size() != 0){\n\t\t" << nameStr << ".push_back(" << arrType << "{});\n\t\tint cut = " << nameStr << ".rbegin()->decode_" << arrType << "(result.val);\n\t\tresult.val.erase(result.val.begin(), result.val.begin() + cut);\n\t}";
				cppFile << "\n\treturn res;\n}\n";
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
		cppFile << "\nstd::vector<uint8_t> " << currentDescribingClassName << "::encode_" << nameStr << "(){\n";
		cppFile << "\n\tLen encodedString;\n\tfor (int i = 0; i < " << nameStr << ".length(); i++)\n\t\tencodedString.val.push_back(" << nameStr << "[i]);\n\tencodedString.encodeValue();\n\treturn encodedString.encoded;\n}\n";

		// decoding
		cppFile << "\nint " << currentDescribingClassName << "::decode_" << nameStr << "(std::vector<uint8_t> data){\n";
		cppFile << "\tLen String;\n\tString.encoded = data;\n\tint res = String.decodeValue();\n\t";
		cppFile << "for (int i = 0; i < String.val.size(); i++)\n\t\t" << nameStr << ".append(1, char(String.val[i]));\n\treturn res;\n}\n";
	}


	if (typeStr != "t_class")
	{
		fieldsOfCurrentClass.emplace(currentClassId, nameStr);
		currentClassId++;

		hFile << "\tstd::vector<uint8_t> encode_" << nameStr << "();\n";
		hFile << "\tint decode_" << nameStr << "(std::vector<uint8_t> data);\n\n";
	}
	else
	{
		printedPublic.append("\tstd::vector<uint8_t> encode_").append(nameStr).append("();\n");
		printedPublic.append("\tint decode_").append(nameStr).append("(std::vector<uint8_t> data);\n\n");
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