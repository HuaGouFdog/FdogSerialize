/*
����Ŀǩ����Apache-2.0 License��������μ�LICENSE
���� Apache ��ɣ��汾 2.0������ɡ���������
����������ɣ�����������ʹ�ô��ļ���

Copyright 2021-2022 ����Fdog(����)
*/

#include "FStruct.h"

/*

"bool":
"bool*"
"char"
"unsigned char"
"char*"
"unsigned char*"
"int"
"unsigned int"
"int*"
"unsigned int*"
"short"
"unsigned short"
"short*"
"unsigned short*"
"long"
"unsigned long int"
"long*"
"unsigned long*"
"long long"
"unsigned long long"
"long long*"
"unsigned long long*"
"float"
"double"
"long double"
"float*"
"double*"
"long double*"
"string"
"string*"
*/

FdogSerializerBase * FdogSerializerBase::FdogSerializerbase = nullptr;
mutex * FdogSerializerBase::mutex_base = new(mutex);

FdogSerializerBase * FdogSerializerBase::Instance() {
	mutex_base->lock();
	if (FdogSerializerbase == nullptr) {
		FdogSerializerbase = new FdogSerializerBase();
	}
	mutex_base->unlock();
	return FdogSerializerbase;
}


FdogSerializer * FdogSerializer::fdogSerializer = nullptr;
mutex * FdogSerializer::mutex_serialize = new(mutex);

FdogSerializer::FdogSerializer() {
#ifdef __GNUC__
	//gcc���뻷��
	TypeName["bool"] = "bool";
	TypeName["bool*"] = "bool*";
	TypeName["char"] = "char";
	TypeName["unsigned char"] = "unsigned char";
	TypeName["char*"] = "char*";
	TypeName["unsigned char*"] = "unsigned char*";
	TypeName["int"] = "int";
	TypeName["unsigned int"] = "unsigned int";
	TypeName["int*"] = "int*";
	TypeName["unsigned int*"] = "unsigned int*";
	TypeName["short"] = "short";
	TypeName["unsigned short"] = "unsigned short";
	TypeName["short*"] = "short*";
	TypeName["unsigned short*"] = "unsigned short*";
	TypeName["long"] = "long";
	TypeName["unsigned long int"] = "unsigned long int";
	TypeName["long*"] = "long*";
	TypeName["unsigned long*"] = "unsigned long*";
	TypeName["long long"] = "long long";
	TypeName["unsigned long long"] = "unsigned long long";
	TypeName["long long*"] = "long long*";
	TypeName["unsigned long long*"] = "unsigned long long*";
	TypeName["float"] = "float";
	TypeName["double"] = "double";
	TypeName["long double"] = "long double";
	TypeName["float*"] = "float*";
	TypeName["double*"] = "double*";
	TypeName["long double*"] = "long double*";
	TypeName["std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >"] = "string";
	TypeName["std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*"] = "string*";
	TypeName["std::__cxx11::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >"] = "wstring";
#elif _MSC_VER
	//vc���뻷��
	TypeName["bool"] = "bool";
	TypeName["bool *"] = "bool*";
	TypeName["char"] = "char";
	TypeName["unsigned char"] = "unsigned char";
	TypeName["unsigned char *"] = "char*";
	TypeName["unsigned int"] = "unsigned char*";
	TypeName["int"] = "int";
	TypeName["unsigned int"] = "unsigned int";
	TypeName["int *"] = "int*";
	TypeName["unsigned int *"] = "unsigned int*";
	TypeName["short"] = "short";
	TypeName["unsigned short"] = "unsigned short";
	TypeName["short *"] = "short*";
	TypeName["unsigned short *"] = "unsigned short*";
	TypeName["long"] = "long";
	TypeName["unsigned long"] = "unsigned long int";
	TypeName["long *"] = "long*";
	TypeName["unsigned long *"] = "unsigned long*";
	TypeName["__int64"] = "long long";
	TypeName["unsigned __int64"] = "unsigned long long";
	TypeName["__int64 *"] = "long long*";
	TypeName["unsigned __int64 *"] = "unsigned long long*";
	TypeName["float"] = "float";
	TypeName["double"] = "double";
	TypeName["long double"] = "long double";
	TypeName["float *"] = "float*";
	TypeName["double *"] = "double*";
	TypeName["long double *"] = "long double*";
	TypeName["class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >"] = "string";
	TypeName["class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > *"] = "string*";
	TypeName["td::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >"] = "wstring";
#elif clang
	//clang���뻷��
#endif
	//��һ�־��ǻ�������
	//�ڶ��־����ɻ��������ݱ������ָ������
	//���������Զ������
	//�����־����Զ������ָ��

	//��û��ָ����Դӷ���ֵ���濴��û��*��
	vector<string> baseTypeTemp = {
		"bool", "bool*",
		"char", "unsigned char", "unsigned char*",
		"int", "unsigned int", "int*", "unsigned int*", "unsigned __int64", "unsigned __int64*"
		"short", "unsigned short", "short*", "unsigned short*",
		"long", "unsigned long", "long*", "unsigned long*",
		"long long", "unsigned long long", "long long*", "unsigned long long*",
		"float", "double", "long double", "float*", "double*", "long double*",
		"string","string*","char*", "wstring", "wstring*"
	};
	this->baseType = baseTypeTemp;

	//�з�������Ӧ��ӵ�������ţ����ź��� ^(-|+)? ƥ�为��
	map<string, string> baseRegexTemp = {
		{ "bool", "(\\d+)" },
		{ "float", "(\\d+.\\d+)" },
		{ "double", "(\\d+.\\d+)" },
		{ "long double", "(\\d+.\\d+)" },
		{ "char*", "\"(.*?)\"" },
		{ "string", "\"(.*?)\"" },
		{ "char", "(\\d+)" },{ "unsigned char", "(\\d+)" },
		{ "int", "(\\d+)" },{ "unsigned int", "(\\d+)" },
		{ "short", "(\\d+)" },{ "unsigned short", "(\\d+)" },
		{ "long", "(\\d+)" },{ "unsigned long", "(\\d+)" },
		{ "long long", "(\\d+)" },{ "unsigned long long", "(\\d+)" },
		{ "wstring", "\"(.*?)\"" },
	};
	this->baseRegex = baseRegexTemp;

	//std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > >
	//std::unordered_map<int, int, std::hash<int>, std::equal_to<int>, std::allocator<std::pair<int const, int> > >

	map<int, string> complexRegexTemp = {
#ifdef __GNUC__
		{ 4, "(.*?) (\\[)(\\d+)(\\])" },
		{ 5, "std::vector<(.*?)," },     //����������⣬�����string��ֻ���ȡ����������
		{ 6, "std::map<(.*?), (.*?)," }, //stringҲ��������
		{ 62, "std::map<(.*?), (.*?), (.*?), (.*?)," },
		{ 7, "std::__cxx11::list<(.*?)," },
		{ 8, "std::set<(.*?)," },
		{ 9, "std::deque<(.*?)," },
		{ 10,"std::pair<(.*?) const, (.*?)>" },
		{ 63, "std::unordered_map<(.*?), (.*?), (.*?), (.*?)," },
#elif _MSC_VER
		{ 4, "(.*?) (\\[)(\\d+)(\\])" },
		{ 5, "class std::vector<(.*?)," },     //����������⣬�����string��ֻ���ȡ����������
		{ 53, "td::vector<struct (.*?)," },
		{ 6, "class std::map<(.*?),(.*?)," }, //stringҲ��������
		{ 62, "class std::map<(.*?),(.*?),(.*?),(.*?)," },
		{ 7, "class std::__cxx11::list<(.*?)," },
		{ 8, "class std::set<(.*?)," },
		{ 9, "class std::deque<(.*?)," },
		{ 10,"class std::pair<(.*?) const, (.*?)>" },
		{ 63, "class std::unordered_map<(.*?),(.*?),(.*?),(.*?)," },
#endif
	};
	this->complexRegex = complexRegexTemp;

	ObjectInfo * objectinfo = new ObjectInfo();
	objectinfo->objectType = "NULL";
	objectinfo->objectTypeInt = -1;
	this->objectInfoList.push_back(objectinfo);

	//�Ի�����������ʼ��
	MetaInfo * metainfo = nullptr;
	metainfo = new MetaInfo();
	metainfo->memberType = "bool";
	metainfo->memberTypeSize = sizeof(bool);
	this->baseInfoList.push_back(metainfo);

	metainfo = new MetaInfo();
	metainfo->memberType = "char";
	metainfo->memberTypeSize = sizeof(char);
	this->baseInfoList.push_back(metainfo);

	metainfo = new MetaInfo();
	metainfo->memberType = "unsigned char";
	metainfo->memberTypeSize = sizeof(unsigned char);
	this->baseInfoList.push_back(metainfo);

	metainfo = new MetaInfo();
	metainfo->memberType = "int";
	metainfo->memberTypeSize = sizeof(int);
	this->baseInfoList.push_back(metainfo);

	metainfo = new MetaInfo();
	metainfo->memberType = "unsigned int";
	metainfo->memberTypeSize = sizeof(unsigned int);
	this->baseInfoList.push_back(metainfo);

	metainfo = new MetaInfo();
	metainfo->memberType = "short";
	metainfo->memberTypeSize = sizeof(short);
	this->baseInfoList.push_back(metainfo);

	metainfo = new MetaInfo();
	metainfo->memberType = "unsigned short";
	metainfo->memberTypeSize = sizeof(unsigned short);
	this->baseInfoList.push_back(metainfo);

	metainfo = new MetaInfo();
	metainfo->memberType = "long";
	metainfo->memberTypeSize = sizeof(long);
	this->baseInfoList.push_back(metainfo);

	metainfo = new MetaInfo();
	metainfo->memberType = "unsigned long";
	metainfo->memberTypeSize = sizeof(unsigned long);
	this->baseInfoList.push_back(metainfo);

	metainfo = new MetaInfo();
	metainfo->memberType = "long long";
	metainfo->memberTypeSize = sizeof(long long);
	this->baseInfoList.push_back(metainfo);

	metainfo = new MetaInfo();
	metainfo->memberType = "unsigned long long";
	metainfo->memberTypeSize = sizeof(unsigned long long);
	this->baseInfoList.push_back(metainfo);

	metainfo = new MetaInfo();
	metainfo->memberType = "float";
	metainfo->memberTypeSize = sizeof(float);
	this->baseInfoList.push_back(metainfo);

	metainfo = new MetaInfo();
	metainfo->memberType = "double";
	metainfo->memberTypeSize = sizeof(double);
	this->baseInfoList.push_back(metainfo);

	metainfo = new MetaInfo();
	metainfo->memberType = "long double";
	metainfo->memberTypeSize = sizeof(long double);
	this->baseInfoList.push_back(metainfo);

	metainfo = new MetaInfo();
	metainfo->memberType = "string";
	metainfo->memberTypeSize = sizeof(string);
	this->baseInfoList.push_back(metainfo);

	metainfo = new MetaInfo();
	metainfo->memberType = "char*";
	metainfo->memberTypeSize = sizeof(char*);
	this->baseInfoList.push_back(metainfo);

}

FdogSerializer::~FdogSerializer() {
	//�ͷ��ڴ�
	delete(FdogSerializer::mutex_serialize);
	delete(FdogSerializer::fdogSerializer);
}

FdogSerializer * FdogSerializer::Instance() {
	mutex_serialize->lock();
	if (fdogSerializer == nullptr) {
		fdogSerializer = new FdogSerializer();
	}
	mutex_serialize->unlock();
	return fdogSerializer;
}

void FdogSerializer::addObjectInfo(ObjectInfo * objectinfo) {
	this->objectInfoList.push_back(objectinfo);
}

ObjectInfo & FdogSerializer::getObjectInfo(string objectName) {
	for (auto objectinfo : FdogSerializer::Instance()->objectInfoList) {
		if (objectinfo->objectType == objectName) {
			return *objectinfo;
		}
	}
	return *(FdogSerializer::Instance()->objectInfoList[0]);
}

MetaInfo * FdogSerializer::getMetaInfo(string TypeName) {
	//cout << "getMetaInfo - TypeName = "  << TypeName << endl;
	for (auto metainfo : FdogSerializer::Instance()->baseInfoList) {
		if (metainfo->memberType == TypeName) {
			return metainfo;
		}
	}
	return nullptr;
}

std::string FdogSerializer::getTypeName(string TypeName) {
#ifdef __GNUC__
	TypeName = abi::__cxa_demangle(TypeName.c_str(), 0, 0, 0);
#elif _MSC_VER
	if (TypeName.find("struct ") != string::npos && TypeName.find("class std::basic_string<char,struct ") == string::npos) {
		//ֻ�����string���ݲ�ȷ��MSVC�������������Ƿ��gcc�г���
		TypeName = TypeName.substr(7);
	}
	//���msvcӦ����ϵͳ�ӿ� ����struct
#endif
	auto iter = this->TypeName.find(TypeName);
	if (iter != this->TypeName.end()) {
		//cout << "getTypeName = " << iter->second << endl;
		return iter->second;
	}
	//cout << "getTypeName = " << TypeName << endl;
	return TypeName;
}

void FdogSerializer::__setAliasName(string Type, string memberName, string aliasName) {
	ObjectInfo & objectinfo = this->getObjectInfo(Type);
	for (auto metainfoObject : objectinfo.metaInfoObjectList) {
		if (metainfoObject->memberName == memberName) {
			metainfoObject->memberAliasName = aliasName;
			break;
		}
	}
}
void FdogSerializer::__removeAliasName(string Type, string memberName) {
	ObjectInfo & objectinfo = this->getObjectInfo(Type);
	for (auto metainfoObject : objectinfo.metaInfoObjectList) {
		if (metainfoObject->memberName == memberName) {
			metainfoObject->memberAliasName = "";
			break;
		}
	}
}

void FdogSerializer::__setIgnoreField(string Type, string memberName) {
	ObjectInfo & objectinfo = this->getObjectInfo(Type);
	for (auto metainfoObject : objectinfo.metaInfoObjectList) {
		if (metainfoObject->memberName == memberName) {
			metainfoObject->memberIsIgnore = true;
			break;
		}
	}
}

void FdogSerializer::__removeIgnoreField(string Type, string memberName) {
	ObjectInfo & objectinfo = this->getObjectInfo(Type);
	for (auto metainfoObject : objectinfo.metaInfoObjectList) {
		if (metainfoObject->memberName == memberName) {
			metainfoObject->memberIsIgnore = false;
			break;
		}
	}
}

void FdogSerializer::__setIgnoreLU(string Type, string memberName) {
	ObjectInfo & objectinfo = this->getObjectInfo(Type);
	for (auto metainfoObject : objectinfo.metaInfoObjectList) {
		if (metainfoObject->memberName == memberName) {
			metainfoObject->memberIsIgnoreLU = true;
			break;
		}
	}
}

void FdogSerializer::__removeIgnoreLU(string Type, string memberName) {
	ObjectInfo & objectinfo = this->getObjectInfo(Type);
	for (auto metainfoObject : objectinfo.metaInfoObjectList) {
		if (metainfoObject->memberName == memberName) {
			metainfoObject->memberIsIgnoreLU = false;
			break;
		}
	}
}

void FdogSerializer::__setFuzzy(string Type) {

}

void Fdog::setAliasName(string Type, string memberName, string AliasName) {
	FdogSerializer::Instance()->__setAliasName(Type, memberName, AliasName);
}

void Fdog::setIgnoreField(string Type, string memberName) {
	FdogSerializer::Instance()->__setIgnoreField(Type, memberName);
}

void Fdog::setIgnoreLU(string Type, string memberName) {
	FdogSerializer::Instance()->__setIgnoreLU(Type, memberName);
}

void Fdog::setFuzzy(string Type) {
	FdogSerializer::Instance()->__setFuzzy(Type);
}

//��ȡkeyֵ
string FdogSerializer::getKey(string json) {
	smatch result;
	regex pattern("\"(.*)\":(.*)");
	if (regex_search(json, result, pattern)) {
		string key = result.str(1).c_str();
		return key;
	}
	return "";
}

/***********************************
*   ���ض�Ӧ�ĳ�Ա����(�����������ͺ��Զ�������)�������С
************************************/
memberAttribute FdogSerializer::getMemberAttribute(string typeName) {
	cout << "getMemberAttribute = " <<typeName << endl;
	memberAttribute resReturn;
	smatch result;
	if (FdogSerializer::isBaseType(typeName)) {
		resReturn.valueType = typeName;
		resReturn.ArraySize = 0;
		resReturn.valueTypeInt = OBJECT_BASE;
	}
	else if (FdogSerializer::isVectorType("", typeName)) {
		cout << "=========>>1  typeName ============= = " << typeName << endl;
		resReturn.valueType = typeName;
		regex pattern(complexRegex[5]);
		regex pattern2(complexRegex[53]);
		if (regex_search(typeName, result, pattern)) {
			string value = result.str(1).c_str();
			//cout << "=========>>1  isVectorType ��ʱ������ ============= = " << value << endl;
			//�����
			if (value == "string") {
				resReturn.first = "string";
			}
			else {
				resReturn.first = value;
			}
		} else if (regex_search(typeName, result, pattern2)) {
			string value = result.str(1).c_str();
			cout << "=========>>2  isVectorType ��ʱ������ ============= = " << value << endl;
			if (value == "string") {
				resReturn.first = "string";
			}
			else {
				resReturn.first = value;
			}
		}
		resReturn.valueTypeInt = OBJECT_VECTOR;
	}
	else if (FdogSerializer::isMapType("", typeName)) {
		resReturn.valueType = typeName;
		regex pattern(complexRegex[62]);
		regex pattern2(complexRegex[63]);
		cout << "ֵ=" << typeName << endl;
		if (regex_search(typeName, result, pattern)) {
			string value = result.str(1).c_str();
			if (value == "std::__cxx11::basic_string<char" || value == "class std::basic_string<char") {
				resReturn.first = "string";
				string value2 = result.str(4).c_str();
				resReturn.second = value2;
			}
			else {
				resReturn.first = value;
				string value2 = result.str(2).c_str();
				resReturn.second = value2;
			}
			//cout << "=========>>2  isMapType " << resReturn.first << endl;
		}
		else {
			regex pattern2(complexRegex[63]);
			if (regex_search(typeName, result, pattern)) {
				string value = result.str(1).c_str();
				string value2 = result.str(2).c_str();
				resReturn.first = value;
				resReturn.second = value2;
			}
		}
		resReturn.valueTypeInt = OBJECT_MAP;
	}
	else if (FdogSerializer::isUnorderedMapType("", typeName)) {
		resReturn.valueType = typeName;
		regex pattern2(complexRegex[63]);
		if (regex_search(typeName, result, pattern2)) {
			string value = result.str(1).c_str();
			//cout << "&&&&&&& = " << result.str(0).c_str() << endl;
			if (value == "std::basic_string<char" || value == "class std::basic_string<char") {
				resReturn.first = "string";
				string value2 = result.str(4).c_str();
				resReturn.second = value2;
			}
			else {
				resReturn.first = value;
				string value2 = result.str(2).c_str();
				resReturn.second = value2;
			}
			//cout << "=========>>3  isMapType " << resReturn.first << endl; 
		}
		else {

		}
		resReturn.valueTypeInt = OBJECT_UNORDERED_MAP;
	}
	else if (FdogSerializer::isListType("", typeName)) {
		resReturn.valueType = typeName;
		regex pattern(complexRegex[7]);
		if (regex_search(typeName, result, pattern)) {
			string value = result.str(1).c_str();
			//cout << "=========>>1  isListType " << value << endl;
			resReturn.first = value;
		}
		resReturn.valueTypeInt = OBJECT_LIST;
		//getTypeOfList
	}
	else if (FdogSerializer::isSetType("", typeName)) {
		resReturn.valueType = typeName;
		regex pattern(complexRegex[8]);
		if (regex_search(typeName, result, pattern)) {
			string value = result.str(1).c_str();
			//cout << "=========>>1  isSetType " << value << endl;
			resReturn.first = value;
		}
		resReturn.valueTypeInt = OBJECT_SET;
		//getTypeOfList
	}
	else if (FdogSerializer::isDequeType("", typeName)) {
		resReturn.valueType = typeName;
		regex pattern(complexRegex[9]);
		if (regex_search(typeName, result, pattern)) {
			string value = result.str(1).c_str();
			//cout << "=========>>1  isDequeType " << value << endl;
			resReturn.first = value;
		}
		resReturn.valueTypeInt = OBJECT_DEQUE;
		//getTypeOfList
	}
	else if (FdogSerializer::isArrayType("", typeName)) {
		resReturn.valueType = typeName;
		// ObjectInfo objectinfo = getObjectInfoByType(typeName, OBJECT_ARRAY);  
		// resReturn.valueType = objectinfo.objectType;
		regex pattern(complexRegex[4]);
		if (regex_search(typeName, result, pattern)) {
			string value = result.str(1).c_str();
			//cout << "��������:" << value << " ";
			string value2 = result.str(3).c_str();
			//cout << "�����С:" << value2;
			resReturn.first = value;
			resReturn.ArraySize = atoi(value2.data());
		}
		resReturn.valueTypeInt = OBJECT_ARRAY;
	}
	else {
		resReturn.valueType = typeName;
		resReturn.ArraySize = 0;
		resReturn.valueTypeInt = OBJECT_STRUCT;
	}

	return resReturn;
}

int FdogSerializer::getObjectTypeInt(string objectName, string typeName) {
	//cout << "����getObjectTypeInt typeName = " <<objectName << " -- " << typeName << endl;
	if (FdogSerializer::Instance()->isBaseType(typeName)) {
		return OBJECT_BASE;
	}
	if (FdogSerializer::Instance()->isVectorType(objectName, typeName)) {
		return OBJECT_VECTOR;
	}
	if (FdogSerializer::Instance()->isMapType(objectName, typeName)) {
		return OBJECT_MAP;
	}
	if (FdogSerializer::Instance()->isListType(objectName, typeName)) {
		return OBJECT_LIST;
	}
	if (FdogSerializer::Instance()->isStructType(objectName, typeName)) {
		//����Ҳ�������� �ж�
		return OBJECT_STRUCT;
	}
	if (FdogSerializer::Instance()->isArrayType(objectName, typeName)) {
		return OBJECT_ARRAY;
	}
}

ObjectInfo FdogSerializer::getObjectInfoByType(string typeName, int objectTypeInt) {
	smatch result;
	regex pattern(complexRegex[objectTypeInt]);
	switch (objectTypeInt)
	{
	case OBJECT_VECTOR:
		//��ȡֵ
		if (regex_search(typeName, result, pattern)) {
			string value = result.str(1).c_str();
			//cout << "-----" << value << endl;
			return getObjectInfo(value);
			//���˸������ͣ���Ӧ���л�������
		}
		break;
	case OBJECT_LIST:
		if (regex_search(typeName, result, pattern)) {
			string value = result.str(1).c_str();
			return getObjectInfo(value);
		}
		break;
	case OBJECT_MAP:
		if (regex_search(typeName, result, pattern)) {
			string value = result.str(1).c_str();
			//cout << "-----" << value << endl;
			return getObjectInfo(value);
		}
		break;
	case OBJECT_ARRAY:
		if (regex_search(typeName, result, pattern)) {
			string value = result.str(1).c_str();
			//cout << "-----" << value << endl;
			return getObjectInfo(value);
		}
		break;
	default:
		break;
	}
}

wstring FdogSerializer::string2wstring(string str)
{
	wstring result;
	//��ȡ��������С��������ռ䣬��������С���ַ�����  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //����ַ�����β  
									//ɾ��������������ֵ  
	result.append(buffer);
	delete[] buffer;
	return result;
}

string FdogSerializer::wstring2string(wstring wstr)
{
	string result;
	//��ȡ��������С��������ռ䣬��������С�°��ֽڼ����  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//���ֽڱ���ת���ɶ��ֽڱ���  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//ɾ��������������ֵ  
	result.append(buffer);
	delete[] buffer;
	return result;
}

int FdogSerializer::getObjectTypeByObjectInfo(string objectName) {
	return getObjectInfo(objectName).objectTypeInt;
}

bool FdogSerializer::isBaseType(string typeName) {
	//cout << "isBaseType = " << typeName << endl;
	vector<string>::iterator result = find(baseType.begin(), baseType.end(), typeName);
	if (result != baseType.end()) {
		return true;
	}
	return false;
}
//����������⣬�������ǻ��������ǲ�������ǵ�stl����char* string���ͻ������

bool FdogSerializer::isBaseTypeByMap(string typeName) {
	smatch result;
	regex pattern(complexRegex[10]);
	if (regex_search(typeName, result, pattern)) {
		string value = result.str(2).c_str();
		vector<string>::iterator result = find(baseType.begin(), baseType.end(), value);
		if (result != baseType.end()) {
			return true;
		}
		// return result.str(1).c_str();
		//         result.str(2).c_str();
	}
	return false;
}

//�ж��Ƿ�Ϊvector����
bool FdogSerializer::isVectorType(string objectName, string typeName) {
#ifdef __GNUC__
	auto x = typeName.find("std::vector<");
	if (x != string::npos && x == 0) {
		return true;
	}
#elif _MSC_VER
	auto x = typeName.find("class std::vector<");
	if (x != string::npos && x == 0) {
		return true;
	}
	auto y = typeName.find("td::vector<");
	if (y != string::npos && y == 0) {
		return true;
	}
#endif
	return false;
}

//�ж��Ƿ�Ϊmap����
bool FdogSerializer::isMapType(string objectName, string typeName) {
#ifdef __GNUC__
	auto x = typeName.find("std::map<");
#elif _MSC_VER
	auto x = typeName.find("class std::map<");
#endif

	if (x != string::npos && x == 0) {
		return true;
	}
	return false;
}

bool FdogSerializer::isUnorderedMapType(string objectName, string typeName) {
#ifdef __GNUC__
	auto x = typeName.find("std::unordered_map<");
#elif _MSC_VER
	auto x = typeName.find("class std::unordered_map<");
#endif

	if (x != string::npos && x == 0) {
		return true;
	}
	return false;
}

//�ж��Ƿ���list����
bool FdogSerializer::isListType(string objectName, string typeName) {
	auto x = typeName.find("std::__cxx11::list");
	if (x != string::npos && x == 0) {
		return true;
	}
	return false;
}

bool FdogSerializer::isSetType(string objectName, string typeName) {
	auto x = typeName.find("std::set<");
	if (x != string::npos && x == 0) {
		return true;
	}
	return false;
}

bool FdogSerializer::isDequeType(string objectName, string typeName) {
	auto x = typeName.find("std::deque<");
	if (x != string::npos && x == 0) {
		return true;
	}
	return false;
}


bool FdogSerializer::isStructType(string objectName, string typeName) {
	if (objectName == typeName) {
		return true;
	}
	return false;
}

//�����鷵�ض���ֵ���������鷵��0 
bool FdogSerializer::isArrayType(string objectName, string typeName) {
	memberAttribute resReturn;
	smatch result;
	resReturn.valueType = typeName;
	regex pattern(complexRegex[4]);
	if (regex_search(typeName, result, pattern)) {
		string value = result.str(2).c_str();
		string value2 = result.str(4).c_str();
		//cout << "value = " << value << endl;
		//cout << "value2 = " << value2 << endl;
		if ((value + value2) == "[]") {
			return true;
		}
	}
	return false;
}

void FdogSerializer::removeFirstComma(string & return_) {
	return_ = return_.substr(1);
}

void FdogSerializer::removeLastComma(string & return_) {
	return_.pop_back();
}

void FdogSerializer::removeNumbers(string & return_) {
	string::iterator it = return_.begin();
	while (it != return_.end()) {
		if ((*it >= '0') && (*it <= '9')) {
			it = return_.erase(it);
		}
		else {
			it++;
		}
	}
}

vector<string> FdogSerializer::CuttingArray(string data) {
	int sum = 0;
	int first = 0;
	int end = 0;
	vector<string> StrArray;
	int len = data.length();
	for (int i = 0; i <= len; i++) {
		if (data[i] == '{') {
			sum++;
			if (sum == 1) {
				first = i;
			}
		}
		if (data[i] == '}') {
			sum--;
			if (sum == 0) {
				end = i;
				string da = data.substr(first, end - first + 1);
				StrArray.push_back(da);
			}
		}
	}
	return StrArray;
}

vector<string> FdogSerializer::split(string str, string pattern) {
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;
	int size = str.size();

	for (int i = 0; i<size; i++)
	{
		pos = str.find(pattern, i);
		if (pos<size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

//�ж�json��ʽ�Ƿ���ȷ
result FdogSerializer::CuttingJson(vector<string> & json_array, string json_) {
	result res;
	//�������������ɶ�json��Ϣ�ķָֻ�ָ��һ�㣬���������Ϣ���ж�㣬���η���
	int status = -1; //0 ��ʾ��һ����������  1 ��ʾ��������(������Ƕ��) 2 ��ʾ����  3 ��ʾ����
	int first = 0;  //�ַ�����ʼλ��
	int end = 0;    //�ַ�������λ��
	int len = json_.length();
	int sum = 0;
	int xiabiao = 0; //��¼ƥ���±�
	bool isadd = false;
	for (int i = 0; i < len; i++) {
		//cout << "i = " << i << endl;
		if (json_[i] == '"') {
			if (sum == 0 && status == -1) {
				status = 0;
				first = i;
				//cout << "i = " << i << endl;
			}
		}
		if (status == 3 && json_[i] == '{' && isadd == true) {
			sum++;
		}

		if (status == 4 && json_[i] == '[' && isadd == true) {
			sum++;
		}

		if (json_[i] == ',' && json_[i - 1] == '"' && status == 2) {
			cout << ",������\" �� status = 2 ����" << endl;
		}

		if (json_[i] == '"' && json_[i + 1] == ':' && json_[i + 2] == '{') {
			if (status == 0) {
				//cout << "status = 3" << endl;
				status = 3;
				//sum++;
				isadd = true;
			}
		}
		else if (json_[i] == '"' && json_[i + 1] == ':' && json_[i + 2] == '[') {
			if (status == 0) {
				status = 4;
				//sum++;
				isadd = true;
			}
		}
		else if (json_[i] == '"' && json_[i + 1] == ':' && json_[i + 2] == '"') {
			if (status == 0) {
				status = 1;
				sum++;
				//cout << "sum+1 =" << sum << endl;
				xiabiao = i + 2;
				isadd = true;
			}
		}
		else if (json_[i] == '"' && json_[i + 1] == ':') {
			if (status == 0) {
				status = 2;
				sum++;
				isadd = true;
			}
		}
		if (json_[i] == '"') {
			if (json_[i + 1] == ',' && status == 1) {
				sum--;
				if (sum == 0) {
					end = i;
					string da = json_.substr(first, end - first + 1);
					//cout << "��ȡ����ֵ1��" << da << endl;
					status = -1;
					json_array.push_back(da);
				}
			}
			else if (json_[i + 1] == '}' && status == 1) {
				sum--;
				//cout << "sum-1 =" << sum << endl;
				if (sum == 0) {
					end = i;
					string da = json_.substr(first, end - first + 1);
					//cout << "��ȡ����ֵ1��" << da << endl;
					status = -1;
					json_array.push_back(da);
				}
			}
			else if (json_[i + 1] == ']' && status == 1) {
				sum--;
				if (sum == 0) {
					end = i;
					string da = json_.substr(first, end - first + 1);
					//cout << "��ȡ����ֵ1��" << da << endl;
					status = -1;
					json_array.push_back(da);
				}
			}
			else {
				if (sum - 1 == 0 && xiabiao < i) {
					//cout << "�ַ����Ҳ���ƥ��ֵ" << endl;
				}
			}
			if (json_[i + 1] == ']' && status == 4) {
				sum--;
				if (sum == 0) {
					end = i + 1;
					string da = json_.substr(first, end - first + 1);
					//cout << "��ȡ����ֵ3��" << da << endl;
					status = -1;
					json_array.push_back(da);
				}
			}
		}
		if ((status == 2 || status == 3 || status == 4) && sum != 0) {
			if (status == 2 && json_[i - 1] != '"' && json_[i] == ',' && json_[i + 1] == '"') {
				sum--;
				if (sum == 0) {
					end = i;
					string da = json_.substr(first, end - first);
					if (da.npos != da.find(",")) {
						cout << "ʧ��" << endl;
					}
					//cout << "��ȡ����ֵ4��" << da << endl;
					status = -1;
					json_array.push_back(da);
				}
				continue;
			}
			if (status == 3 && json_[i] == '}' && json_[i + 1] == ',') {
				sum--;
				if (sum == 0) {
					end = i;
					string da = json_.substr(first, end - first + 1);
					//cout << "��ȡ����ֵ5��" << da << endl;
					status = -1;
					json_array.push_back(da);
				}
				continue;
			}
			//3 �� 6���������ǲ��Ƕ�������
			if (status == 4 && json_[i] == ']' && json_[i + 1] == ',') {
				sum--;
				if (sum == 0) {
					end = i;
					string da = json_.substr(first, end - first + 1);
					//cout << "��ȡ����ֵ6��" << da << endl;
					status = -1;
					json_array.push_back(da);
				}
				continue;
			}
			if ((status == 3 || status == 2) && json_[i] == '}') {
				if (isadd == true) {
					sum--;
				}
				if (sum == 0) {
					end = i;
					if (status == 2 && json_[i - 1] != '"') {
						string da = json_.substr(first, end - first);
						if (da.npos != da.find(",")) {
							cout << "ʧ��" << endl;
						}
						//cout << "��ȡ����ֵ71��" << da << endl;
						status = -1;
						json_array.push_back(da);
					}
					else {
						string da = json_.substr(first, end - first + 1);
						//cout << "��ȡ����ֵ72��" << da << endl;
						status = -1;
						json_array.push_back(da);
					}
				}
				continue;
			}
			if (status == 4 && json_[i] == ']') {
				sum--;
				if (sum == 0) {
					end = i;
					string da = json_.substr(first, end - first + 1);
					//cout << "��ȡ����ֵ8��" << da << endl;
					status = -1;
					json_array.push_back(da);
				}
				continue;
			}
		}
	}
	if (status != -1) {
		cout << "ƥ��ʧ�����ַ���<";
		int a = first;
		for (int i = 0; i < (json_.length() - a > 20 ? 20 : json_.length() - a); i++) {
			cout << json_[a++];
		}
		cout << ">����" << endl;
	}
	else {
		int count = 0;
		for (int i = 0; i < json_array.size(); i++) {
			count = count + json_array[i].length() + 1;
		}
		if (count + 1 != json_.length()) {
			cout << "���ȳ��� :" << json_ << "---ԭ�ַ������ȣ�" << json_.length() << "---���ڳ���:" << count + 1 << endl;
		}
	}
	if (1) {
		string json_a = "{";
		for (int i = 0; i < json_array.size(); i++) {
			if (i + 1 == json_array.size()) {
				json_a = json_a + json_array[i];
				break;
			}
			json_a = json_a + json_array[i] + ",";
		}
		json_a = json_a + "}";
		if (json_ != json_a) {
			cout << "�Ӷ��Ż�ԭ����ƥ��" << json_a << endl;
		}
	}
	res.code = 0;
	return res;
}

//�жϷ������Ƿ�ƥ��
result FdogSerializer::IsSquareBracket(string json_) {
	result res;
	char start = json_[0];
	char end = json_[json_.length() - 1];
	if (start != '[' || end != ']') {
		res.code = -1;
		res.message = "��ͷ���βȱ�ٷ�����";
		return res;
	}
	return res;
}

//�жϻ������Ƿ�ƥ��
result FdogSerializer::IsCurlyBraces(string json_) {
	result res;
	char start = json_[0];
	char end = json_[json_.length() - 1];
	if (start != '{' || end != '}') {
		res.code = -1;
		res.message = "��ͷ���βȱ�ٻ�����";
		return res;
	}
	res.code = 0;
	return res;
}

//�ж��ܷ������Ƿ�ƥ��
result FdogSerializer::isMatch(string json_) {
	result res;
	int h_sum = 0;
	int f_sum = 0;
	int i;
	for (i = 0; i < json_.length(); i++) {
		if (json_[i] == '{') {
			h_sum++;
		}
		if (json_[i] == '}') {
			h_sum--;
		}
		if (json_[i] == '[') {
			f_sum++;
		}
		if (json_[i] == ']') {
			f_sum--;
		}
	}
	if (h_sum == f_sum && f_sum == 0) {
		res.code = 0;
		return res;
	}
	res.code = -1;
	if (h_sum > 0) {
		res.message = "ȱ�ٻ�����";
	}
	else if (f_sum > 0) {
		res.message = "ȱ�ٷ�����";
	}
	return res;
}

//�ж�json��ȷ��
result FdogSerializer::__JsonValidS(string json_) {
	result res;
	//������һ������Ƿ�ƥ��
	res = IsCurlyBraces(json_);
	if (res.code != 0) {
		return res;
	}
	//����ܷ������Ƿ�ƥ��([]{})
	res = isMatch(json_);
	if (res.code != 0) {
		return res;
	}
	vector<string> array_json;
	res = CuttingJson(array_json, json_);
	// if(res.code != 0){
	//     return res;
	// }

	// if(array_json.size() == 0){
	//     res.code = 0;
	//     return res;
	// }
	return res;
}

//�ж��ֶ��Ƿ����
bool FdogSerializer::__Exist(string json_, string key) {
	auto num = key.find(".");
	if (num != key.npos) {
		if (json_.find(key.substr(0, num)) != json_.npos) {
			//������Ҫ���ҵ��������Ҷ�Ӧ�ַ���
			string resp = "";
			string jsonNew = "\"" + key.substr(0, num) + "\":";
			cout << "n = " << num << endl;
			auto num2 = json_.find(jsonNew);
			if (num2 != json_.npos) {
				if (json_[num2 + jsonNew.length()] == '{') {
					string json_2 = json_.substr(num2 + jsonNew.length());
					int len = json_2.length();
					int sum = 0;
					int first = 0;
					int end = 0;
					for (int i = 0; i <= len; i++) {
						if (json_2[i] == '{') {
							sum++;
							if (sum == 1) {
								first = i;
							}
						}
						if (json_2[i] == '}') {
							sum--;
							if (sum == 0) {
								end = i;
							}
						}
					}
					return __Exist(json_2.substr(first, end - first + 1), key.substr(num + 1));
				}
				else if (json_[num2 + jsonNew.length()] == '[') {
					string json_2 = json_.substr(num2 + jsonNew.length());
					int len = json_2.length();
					int sum = 0;
					int first = 0;
					int end = 0;
					for (int i = 0; i <= len; i++) {
						if (json_2[i] == '[') {
							sum++;
							if (sum == 1) {
								first = i;
							}
						}
						if (json_2[i] == ']') {
							sum--;
							if (sum == 0) {
								end = i;
							}
						}
					}
					return __Exist(json_2.substr(first, end - first + 1), key.substr(num + 1));
				}
				else {
					return false;
				}
			}
		}
		else {
			return false;
		}
	}
	else {
		int x = json_.find(key);
		if (x != json_.npos) {
			if (json_[x - 1] == '"' && json_[x + key.length()] == '"') {
				return true;
			}
			else {
				return false;
			}

		}
		else {
			return false;
		}
	}
	return false;
}

//��ȡ�ֶε�ֵ
string FdogSerializer::__GetStringValue(string json_, string key) {
	auto num = key.find(".");
	if (num != key.npos) {
		if (json_.find(key.substr(0, num)) != json_.npos) {
			//������Ҫ���ҵ��������Ҷ�Ӧ�ַ���
			string resp = "";
			string jsonNew = "\"" + key.substr(0, num) + "\":";
			cout << "n = " << num << endl;
			auto num2 = json_.find(jsonNew);
			if (num2 != json_.npos) {
				if (json_[num2 + jsonNew.length()] == '{') {
					string json_2 = json_.substr(num2 + jsonNew.length());
					int len = json_2.length();
					int sum = 0;
					int first = 0;
					int end = 0;
					for (int i = 0; i <= len; i++) {
						if (json_2[i] == '{') {
							sum++;
							if (sum == 1) {
								first = i;
							}
						}
						if (json_2[i] == '}') {
							sum--;
							if (sum == 0) {
								end = i;
							}
						}
					}
					return __GetStringValue(json_2.substr(first, end - first + 1), key.substr(num + 1));
				}
				else if (json_[num2 + jsonNew.length()] == '[') {
					string json_2 = json_.substr(num2 + jsonNew.length());
					int len = json_2.length();
					int sum = 0;
					int first = 0;
					int end = 0;
					for (int i = 0; i <= len; i++) {
						if (json_2[i] == '[') {
							sum++;
							if (sum == 1) {
								first = i;
							}
						}
						if (json_2[i] == ']') {
							sum--;
							if (sum == 0) {
								end = i;
							}
						}
					}
					return __GetStringValue(json_2.substr(first, end - first + 1), key.substr(num + 1));
				}
				else {
					return "";
				}
			}
		}
		else {
			return "";
		}
	}
	else {
		int x = json_.find(key);
		if (x != json_.npos) {
			if (json_[x - 1] == '"' && json_[x + key.length()] == '"') {
				// //�������Ϳ���ʹ��������ʽ��ȡ
				string res = "(\"" + key + "\":)((\"(.*?)\")|([+-]?([0-9]*\\.?[0-9]+|[0-9]+\\.?[0-9]*)([eE][+-]?[0-9]+)?))";
				string res2 = "(\"" + key + "\":)((true)|(false))";
				smatch result;
				regex pattern(res);
				if (regex_search(json_, result, pattern)) {
					return result.str(2);
				}
				regex pattern2(res2);
				if (regex_search(json_, result, pattern2)) {
					return result.str(2);
				}
				return "";
			}
			else {
				return "";
			}

		}
		else {
			return "";
		}
	}
	return "";
}

//��ȡ�ֶε�ֵ
int FdogSerializer::__GetIntValue(string json_, string key) {
	string value = __GetStringValue(json_, key);
	if (value == "") {
		return -1;
	}
	else {
		return atoi(value.c_str());
	}
}

//��ȡ�ֶε�ֵ
double FdogSerializer::__GetDoubleValue(string json_, string key) {
	string value = __GetStringValue(json_, key);
	if (value == "") {
		return -1.0;
	}
	else {
		return atof(value.c_str());
	}
}

//��ȡ�ֶε�ֵ
long FdogSerializer::__GetLongValue(string json_, string key) {
	string value = __GetStringValue(json_, key);
	if (value == "") {
		return -1.0l;
	}
	else {
		return atol(value.c_str());
	}
}

//��ȡ�ֶε�ֵ
bool FdogSerializer::__GetBoolValue(string json_, string key) {
	string value = __GetStringValue(json_, key);
	if (value == "true") {
		return 1;
	}
	else {
		return 0;
	}
	return 0;
}

string FdogSerializer::getTypeOfVector(string objectName, string typeName) {
	smatch result;
	regex pattern(complexRegex[5]);
	if (regex_search(typeName, result, pattern)) {
		return result.str(1).c_str();
	}
	return "";
}

FdogMap FdogSerializer::getTypeOfMap(string objectName, string typeName) {
	smatch result;
	regex pattern(complexRegex[5]);
	FdogMap fdogMap;
	if (regex_search(typeName, result, pattern)) {
		fdogMap.first = result.str(1).c_str();
		fdogMap.second;
		return fdogMap;
	}
	return fdogMap;
}

string FdogSerializer::getTypeOfList(string objectName, string typeName) {
	smatch result;
	regex pattern(complexRegex[5]);
	if (regex_search(typeName, result, pattern)) {
		return result.str(1).c_str();
	}
	return "";
}

//�ж�json��ȷ��
result Fdog::JsonValidS(string json_) {
	return FdogSerializer::Instance()->__JsonValidS(json_);
}

//�ж��ֶ��Ƿ����
bool Fdog::Exist(string json_, string key) {
	return FdogSerializer::Instance()->__Exist(json_, key);
}

//��ȡ�ֶε�ֵ
string Fdog::GetStringValue(string json_, string key) {
	return FdogSerializer::Instance()->__GetStringValue(json_, key);
}

//��ȡ�ֶε�ֵ
int Fdog::GetIntValue(string json_, string key) {
	return FdogSerializer::Instance()->__GetIntValue(json_, key);
}

//��ȡ�ֶε�ֵ
double Fdog::GetDoubleValue(string json_, string key) {
	return FdogSerializer::Instance()->__GetDoubleValue(json_, key);
}

//��ȡ�ֶε�ֵ
long Fdog::GetLongValue(string json_, string key) {
	return FdogSerializer::Instance()->__GetLongValue(json_, key);
}

//��ȡ�ֶε�ֵ
bool Fdog::GetBoolValue(string json_, string key) {
	return FdogSerializer::Instance()->__GetBoolValue(json_, key);
}