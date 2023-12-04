/*
该项目签署了Apache-2.0 License，详情请参见LICENSE
根据 Apache 许可，版本 2.0（“许可”）获得许可
除非遵守许可，否则您不得使用此文件。
Copyright 2021-2023 花狗Fdog(张旭)
*/

#ifndef FSTRUCT_H
#define FSTRUCT_H
#include <iostream>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <mutex>
#include <sstream>
#include <string>
#include <regex>
#include <codecvt>
#include <codecvt>
#include <map>
#include <list>
#include <set>
#include <vector>
#include <deque>
#include <unordered_map>
#ifdef __GNUC__
#include <cxxabi.h>
#elif _MSC_VER
#include <Windows.h>
#endif

using namespace std;

//用于数组类型整体提取
const string arrayRegex = "(\\[(.*?)\\])";
//用于Map类型整体提取
const string mapRegex = "(\\{(.*?)\\})";
//匹配数组
const string patternArray = "((A)(\\d+)_\\d?(\\D+))";
//匹配别名
const string patterAlias = "(\\(A:(.*?)\\))";
//用于结构体整体提取
const string objectRegex = "(\\{(.*?)\\})";
//匹配结构体
const string patternObject = "((\\d+)(\\D+))";

enum ObjectType {
	OBJECT_BASE = 1,
	OBJECT_STRUCT,
	OBJECT_CLASS,
	OBJECT_ARRAY,
	OBJECT_VECTOR,
	OBJECT_MAP,
	OBJECT_UNORDERED_MAP,
	OBJECT_LIST,
	OBJECT_SET,
	OBJECT_DEQUE,
};

/***********************************
*   存储结构体元信息
************************************/
typedef struct MetaInfo {
	string memberName;              	//成员名
	string memberAliasName;         	//成员别名
	string memberType;              	//成员类型
	size_t memberOffset;            	//偏移值
	size_t memberTypeSize;          	//类型大小
	size_t memberArraySize;         	//如果类型是数组，表示数组大小
	int    memberTypeInt;           	//成员类型 数值型
	bool   isPointer;               	//是否是指针
	string first;                   	//如果是map类型 first表示key的类型，如果是其他类型，表示value类型
	string second;                  	//如果是map类型，second表示value类型
	bool   memberIsIgnore = false;    	//是否忽略字段
	bool   memberIsIgnoreLU = false;  	//是否忽略大小写                                                                                                                
}MetaInfo;

/***********************************
*   存储对象元信息
************************************/
typedef struct ObjectInfo {
	string objectType;                      //结构体类型 字符串表示
	int objectTypeInt;                      //结构体类型 数值表示
	int objectSize;                         //结构体大小
	vector<MetaInfo *> metaInfoObjectList;  //结构体元信息
}ObjectInfo;


/***********************************
*   存储成员类型，数组大小
************************************/
struct memberAttribute {
	string valueType;
	int valueTypeInt;   //类型 数值表示
	string first;       //如果是map类型 first表示key的类型，如果是其他类型，表示value类型
	string second;      //如果是map类型，second表示value类型
	int ArraySize;
};

typedef struct FdogMap {
	string first;	//keyType
	string second;  //valueType
};


struct result {
	int code;           //1.正确 0.错误
	string message;     //如果错误，返回错误提示
};

//声明序列化base类
class FdogSerializerBase {
private:
	static mutex * mutex_base;
	static FdogSerializerBase * FdogSerializerbase;

public:
	static FdogSerializerBase * Instance();

	template<class T>
	string removeLastZero(T & return_) {
		std::ostringstream oss;
		oss << return_;
		return oss.str();
	}

	template<class T>
	string getValueByAddress(string valueType, T & object, int offsetValue) {
		if (valueType == "char") {
			auto value = *((char *)((char *)&object + offsetValue));
			return to_string((int)value);
		}
		if (valueType == "char*") {
			if (((const char **)((char *)&object + offsetValue)) == NULL) {
				return "fdog_serialize_uninitialized";
			}
			auto value = *((const char **)((char *)&object + offsetValue));
			string str_value = value;
			return "\"" + str_value + "\"";
		}
		if (valueType == "string") {
			auto value = *((string *)((char *)&object + offsetValue));
			string str_value = value;
			return "\"" + str_value + "\"";
		}
		if (valueType == "string*") {
			if (*((string **)((char **)&object + offsetValue)) == NULL) {
				return "fdog_serialize_uninitialized";
			}
			auto value = **((string **)((char *)&object + offsetValue));
			string str_value = value;
			return "\"" + str_value + "\"";
		}
		if (valueType == "wstring") {
			auto value = *((wstring *)((char *)&object + offsetValue));
		#ifdef __GNUC__
					std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
					string str_value = converter.to_bytes(value);
		#elif _MSC_VER
					//string str_value = FdogSerializer::Instance()->wstring2string(value);
		#endif
			//return "\"" + str_value + "\"";
		}
		if (valueType == "bool") {
			auto value = *((bool *)((char *)&object + offsetValue));
			if (value) {
				return "true";
			} else {
				return "false";
			}
		}
		if (valueType == "bool*") {
			if (*((bool **)((char **)&object + offsetValue)) == NULL) {
				return "fdog_serialize_uninitialized";
			}
			auto value = **((bool **)((char *)&object + offsetValue));
			if (value) {
				return "true";
			} else {
				return "false";
			}
		}
		if (valueType == "unsigned char") {
			auto value = *((char *)((char *)&object + offsetValue));
			return to_string((unsigned int)value);
		}
		if (valueType == "unsigned char*") {
			if (*((unsigned char **)((char **)&object + offsetValue)) == NULL) {
				return "fdog_serialize_uninitialized";
			}
			auto value = **((char **)((char *)&object + offsetValue));
			return to_string((unsigned int)value);
		}
		if (valueType == "int") {
			auto value = *((int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "int*") {
			if (*((int **)((char **)&object + offsetValue)) == NULL) {
				return "fdog_serialize_uninitialized";
			}
			auto value = **((int **)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "unsigned int") {
			auto value = *((unsigned int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "unsigned int*") {
			if (*((unsigned int **)((char **)&object + offsetValue)) == NULL) {
				return "fdog_serialize_uninitialized";
			}
			auto value = **((unsigned int **)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "short") {
			auto value = *((short int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "short*") {
			if (*((short **)((char **)&object + offsetValue)) == NULL) {
				return "fdog_serialize_uninitialized";
			}
			auto value = **((short int **)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "unsigned short") {
			auto value = *((unsigned short int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "unsigned short*") {
			if (*((unsigned short **)((char **)&object + offsetValue)) == NULL) {
				return "fdog_serialize_uninitialized";
			}
			auto value = **((unsigned short int **)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "long") {
			auto value = *((long int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "long*") {
			if (*((long **)((char **)&object + offsetValue)) == NULL) {
				return "fdog_serialize_uninitialized";
			}
			auto value = **((long int **)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "unsigned long") {
			auto value = *((unsigned long int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "unsigned long*") {
			if (*((unsigned long **)((char **)&object + offsetValue)) == NULL) {
				return "fdog_serialize_uninitialized";
			}
			auto value = **((unsigned long int **)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "long long") {
			auto value = *((long long int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "long long*") {
			if (*((long long **)((char **)&object + offsetValue)) == NULL) {
				return "fdog_serialize_uninitialized";
			}
			auto value = **((long long int **)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "unsigned long long") {
			auto value = *((unsigned long long int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "unsigned long long*") {
			if (*((unsigned long long **)((char **)&object + offsetValue)) == NULL) {
				return "fdog_serialize_uninitialized";
			}
			auto value = **((unsigned long long int **)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "float") {
			auto value = *((float *)((char *)&object + offsetValue));
			return removeLastZero(value);
		}
		if (valueType == "float*") {
			if (*((float **)((char **)&object + offsetValue)) == NULL) {
				return "fdog_serialize_uninitialized";
			}
			auto value = **((float **)((char *)&object + offsetValue));
			return removeLastZero(value);
		}
		if (valueType == "double") {
			auto value = *((double *)((char *)&object + offsetValue));
			return removeLastZero(value);
		}
		if (valueType == "double*") {
			if (*((double **)((char **)&object + offsetValue)) == NULL) {
				return "fdog_serialize_uninitialized";
			}
			auto value = **((double **)((char *)&object + offsetValue));
			return removeLastZero(value);
		}
		if (valueType == "long double") {
			auto value = *((long double *)((char *)&object + offsetValue));
			return removeLastZero(value);
		}
		if (valueType == "long double*") {
			if (*((long double **)((char **)&object + offsetValue)) == NULL) {
				return "fdog_serialize_uninitialized";
			}
			auto value = **((long double **)((char *)&object + offsetValue));
			return removeLastZero(value);
		}
		return "";
	}

	template<class T>
	void setValueByAddress(string valueType, T &object, int offsetValue, string value) {
		if (valueType == "char*") {
			//这种写法有问题
			*((char **)((char *)&object + offsetValue)) = new char[strlen(value.c_str())];
			strcpy(*((char **)((char *)&object + offsetValue)), value.c_str());
		}
		if (valueType == "string") {
			*((string *)((char *)&object + offsetValue)) = value;
		}
		if (valueType == "string*") {
			if (*((string **)((char **)&object + offsetValue)) == NULL) {
				*((string **)((char **)&object + offsetValue)) = new string();
			}
			**((string **)((char *)&object + offsetValue)) = value;
		}
		if (valueType == "wstring") {
		#ifdef __GNUC__
					std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
					*((wstring *)((char *)&object + offsetValue)) = strCnv.from_bytes(value);
		#elif _MSC_VER
					//*((wstring *)((char *)&object + offsetValue)) = FdogSerializer::Instance()->string2wstring(value);
		#endif
		}
		std::stringstream ss;
		ss.str(value);
		if (valueType == "char") {
			ss >> *((char *)((char *)&object + offsetValue));
		}
		if (valueType == "bool") {
			ss >> *((bool *)((char *)&object + offsetValue));
		}
		if (valueType == "bool*") {
			if (*((bool **)((char **)&object + offsetValue)) == NULL) {
				*((bool **)((char **)&object + offsetValue)) = new bool();
			}
			ss >> **((bool **)((char *)&object + offsetValue));
		}
		if (valueType == "unsigned char") {
			ss >> *((unsigned char *)((char *)&object + offsetValue));
		}
		if (valueType == "unsigned char*") {
			if (*((unsigned char **)((char **)&object + offsetValue)) == NULL) {
				*((unsigned char **)((char **)&object + offsetValue)) = new unsigned char();
			}
			ss >> **((unsigned char **)((char *)&object + offsetValue));
		}
		if (valueType == "int") {
			ss >> *((int *)((char *)&object + offsetValue));
		}
		if (valueType == "int*") {
			if (*((int **)((char **)&object + offsetValue)) == NULL) {
				*((int **)((char **)&object + offsetValue)) = new int();
			}
			ss >> **((int **)((char *)&object + offsetValue));
		}
		if (valueType == "unsigned int") {
			ss >> *((unsigned int *)((char *)&object + offsetValue));
		}
		if (valueType == "unsigned int*") {
			if (*((unsigned int **)((char **)&object + offsetValue)) == NULL) {
				*((unsigned int **)((char **)&object + offsetValue)) = new unsigned int();
			}
			ss >> **((unsigned int **)((char *)&object + offsetValue));
		}
		if (valueType == "short") {
			ss >> *((short int *)((char *)&object + offsetValue));
		}
		if (valueType == "short*") {
			if (*((short **)((char **)&object + offsetValue)) == NULL) {
				*((short **)((char **)&object + offsetValue)) = new short();
			}
			ss >> **((short int **)((char *)&object + offsetValue));
		}
		if (valueType == "unsigned short") {
			ss >> *((unsigned short int *)((char *)&object + offsetValue));
		}
		if (valueType == "unsigned short*") {
			if (*((unsigned short **)((char **)&object + offsetValue)) == NULL) {
				*((unsigned short **)((char **)&object + offsetValue)) = new unsigned short();
			}
			ss >> **((unsigned short int **)((char *)&object + offsetValue));
		}
		if (valueType == "long") {
			ss >> *((long int *)((char *)&object + offsetValue));
		}
		if (valueType == "long*") {
			if (*((long **)((char **)&object + offsetValue)) == NULL) {
				*((long **)((char **)&object + offsetValue)) = new long();
			}
			ss >> **((long int **)((char **)&object + offsetValue));
		}
		if (valueType == "unsigned long") {
			ss >> *((unsigned long int *)((char *)&object + offsetValue));
		}
		if (valueType == "unsigned long*") {
			if (*((unsigned long **)((char **)&object + offsetValue)) == NULL) {
				*((unsigned long **)((char **)&object + offsetValue)) = new unsigned long();
			}
			ss >> **((unsigned long int **)((char *)&object + offsetValue));
		}
		if (valueType == "long long") {
			ss >> *((long long int *)((char *)&object + offsetValue));
		}
		if (valueType == "long long*") {
			if (*((long long int **)((char **)&object + offsetValue)) == NULL) {
				*((long long int **)((char **)&object + offsetValue)) = new long long int();
			}
			ss >> **((long long int **)((char *)&object + offsetValue));
		}
		if (valueType == "unsigned long long") {
			ss >> *((unsigned long long  int *)((char *)&object + offsetValue));
		}
		if (valueType == "unsigned long long*") {
			if (*((unsigned long long **)((char **)&object + offsetValue)) == NULL) {
				*((unsigned long long **)((char **)&object + offsetValue)) = new unsigned long long();
			}
			ss >> **((unsigned long long  int **)((char *)&object + offsetValue));
		}
		if (valueType == "float") {
			ss >> *((float *)((char *)&object + offsetValue));
		}
		if (valueType == "float*") {
			if (*((float **)((char **)&object + offsetValue)) == NULL) {
				*((float **)((char **)&object + offsetValue)) = new float();
			}
			ss >> **((float **)((char **)&object + offsetValue));
		}
		if (valueType == "double") {
			ss >> *((double *)((char *)&object + offsetValue));
		}
		if (valueType == "double*") {
			if (*((double **)((char **)&object + offsetValue)) == NULL) {
				*((double **)((char **)&object + offsetValue)) = new double();
			}
			ss >> **((double **)((char **)&object + offsetValue));
		}
		if (valueType == "long double") {
			ss >> *((long double *)((char *)&object + offsetValue));
		}
		if (valueType == "long double*") {
			if (*((long double **)((char **)&object + offsetValue)) == NULL) {
				*((long double **)((char **)&object + offsetValue)) = new long double();
			}
			ss >> **((long double **)((char **)&object + offsetValue));
		}
	}

	// 基础类型转json
	template<class T>
	void BaseToJson(string & json_, MetaInfo * metainfoobject, T & object_) {
		string value = getValueByAddress(metainfoobject->memberType, object_, metainfoobject->memberOffset);
		if (value == "fdog_serialize_uninitialized") {
			return;
		}
		if (metainfoobject->memberAliasName != "") {
			json_ = json_ + "\"" + metainfoobject->memberAliasName + "\"" + ":" + value + ",";
		} else {
			json_ = json_ + "\"" + metainfoobject->memberName + "\"" + ":" + value + ",";
		}
	}

	// //基础类型转json
	template<class T>
	void BaseToJsonA(string & json_, MetaInfo * metainfoobject, T & object_) {
		string value = getValueByAddress(metainfoobject->memberType, object_, metainfoobject->memberOffset);
		if (value == "fdog_serialize_uninitialized") {
			return;
		}
		json_ = json_ + value + ",";
	}

	// //json转基础类型
	template<class T>
	void JsonToBase(T & object_, MetaInfo * metainfoobject, string json_) {
		setValueByAddress(metainfoobject->memberType, object_, metainfoobject->memberOffset, json_);
	}
};


//BaseTag ArrayTag MapTag 用于处理在Serialize，Deserialize上的分发
//处理 base
struct BaseTag {};
//处理 array vector list deque set
struct ArrayTag {};
//处理 map  
struct MapTag {};

template<typename T> struct TagDispatchTrait {
	using Tag = ArrayTag;
};

//处理基础类型
template<> struct TagDispatchTrait<bool> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<bool*> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<char> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<char*> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<unsigned char> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<unsigned char*> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<int> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<int*> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<unsigned int> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<unsigned int*> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<short> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<short*> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<unsigned short> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<unsigned short*> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<long> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<long*> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<unsigned long> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<unsigned long*> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<long long> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<long long*> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<unsigned long long> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<unsigned long long*> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<float> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<float*> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<double> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<double*> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<long double> {using Tag = BaseTag;};
template<> struct TagDispatchTrait<long double*> {using Tag = BaseTag;};

template<> struct TagDispatchTrait<vector<int>> {using Tag = ArrayTag;};
template<> struct TagDispatchTrait<list<int>> {using Tag = ArrayTag;};
template<> struct TagDispatchTrait<set<int>> {using Tag = ArrayTag;};
template<> struct TagDispatchTrait<deque<int>> {using Tag = ArrayTag;};
template<> struct TagDispatchTrait<map<int, int>> {using Tag = MapTag;};
template<> struct TagDispatchTrait<unordered_map<int, int>> {using Tag = MapTag;};

// 区分非字符串和字符串 处理在FSerialize(map) key的值，可能是数值，可能是非数值
struct NoStringTag {};
struct StringTag {};

template<typename T> struct TagString {using Tag = NoStringTag;};
template<> struct TagString<char *> {using Tag = StringTag;};
template<> struct TagString<string> {using Tag = StringTag;};

template<typename T>
string F_toString_s(T object, NoStringTag) {
	return to_string(object);
}
template<typename T>
string F_toString_s(T object, StringTag) {
	return std::string("\"") + object + std::string("\"");
}
template<typename T>
string F_toString(T object) {
	return F_toString_s(object, typename TagString<T>::Tag{});
}

// 用于反序列化时对象为空的情况，需要根据数据进行扩展长度 Init  分为数值和非数值类型
struct InitBaseTag {};
struct InitStructTag {};
struct InitVectorTag {};
struct InitVectorStrTag {};

struct InitDequeTag {};
struct InitDequeStrTag {};

struct InitListTag {};
struct InitListStrTag {};

struct InitMapBaseTag{};
struct InitMapTag {};
struct InitMapStrTag {};

struct InitSetTag {};
struct InitSetStrTag {};


template<typename T> struct TagSTLType {using Tag = InitBaseTag;};
template<> struct TagSTLType<vector<int>> {using Tag = InitVectorTag;};
template<> struct TagSTLType<vector<char *>> {using Tag = InitVectorStrTag;};
template<> struct TagSTLType<vector<string>> {using Tag = InitVectorStrTag;};
template<> struct TagSTLType<list<int>> {using Tag = InitListTag;};
template<> struct TagSTLType<list<char *>> {using Tag = InitListStrTag;};
template<> struct TagSTLType<list<string>> {using Tag = InitListStrTag;};
template<> struct TagSTLType<set<int>> {using Tag = InitSetTag;};
template<> struct TagSTLType<set<char *>> {using Tag = InitSetStrTag;};
template<> struct TagSTLType<set<string>> {using Tag = InitSetStrTag;};
template<> struct TagSTLType<deque<int>> {using Tag = InitDequeTag;};
template<> struct TagSTLType<deque<char *>> {using Tag = InitDequeStrTag;};
template<> struct TagSTLType<deque<string>> {using Tag = InitDequeStrTag;};
template<> struct TagSTLType<map<int, int>> {using Tag = InitMapTag;};
template<> struct TagSTLType<map<string, int>> {using Tag = InitMapStrTag;};
template<> struct TagSTLType<unordered_map<int, int>> {using Tag = InitMapTag;};
template<> struct TagSTLType<unordered_map<string, int>> {using Tag = InitMapStrTag;};

template<typename T>
void F_init_s(T & object, InitBaseTag, string first, string second = "", string key = "") {
	typename T::value_type a;
	//object.push_back(a)
	//走到这里的都是自定义容器类型
}

template<typename T>
void F_init_s(T & object, InitStructTag, string first, string second = "", string key = "") {
	object.resize(2);
}

template<typename T>
void F_init_s(T & object, InitVectorTag, string first, string second = "", string key = "") {
	if (first == "char") {
		object.push_back('0');
	} else if (first == "unsigned char") {
		object.push_back(0);
	} else if (first == "short") {
		object.push_back(0);
	} else if (first == "unsigned short") {
		object.push_back(0);
	} else if (first == "int") {
		object.push_back(0);
	} else if (first == "unsigned int") {
		object.push_back(0);
	} else if (first == "long") {
		object.push_back(0);
	} else if (first == "unsigned long") {
		object.push_back(0);
	} else if (first == "long long") {
		object.push_back(0);
	} else if (first == "unsigned long long") {
		object.push_back(0);
	} else if (first == "float") {
		object.push_back(0.1f);
	} else if (first == "double") {
		object.push_back(0.1);
	} else if (first == "long double") {
		object.push_back(0.1);
	} else {

	}
}

template<typename T>
void F_init_s(T & object, InitVectorStrTag, string first, string second = "", string key = "") {
	if (first == "char*") {
		object.push_back("");
	} else if (first == "string") {
		object.push_back("");
	} else {
	}
}

template<typename T>
void F_init_s(T & object, InitDequeTag, string first, string second = "", string key = "") {
	if (first == "char") {
		object.push_back('0');
	} else if (first == "unsigned char") {
		object.push_back(0);
	} else if (first == "short") {
		object.push_back(0);
	} else if (first == "unsigned short") {
		object.push_back(0);
	} else if (first == "int") {
		object.push_back(0);
	} else if (first == "unsigned int") {
		object.push_back(0);
	} else if (first == "long") {
		object.push_back(0);
	} else if (first == "unsigned long") {
		object.push_back(0);
	} else if (first == "long long") {
		object.push_back(0);
	} else if (first == "unsigned long long") {
		object.push_back(0);
	} else if (first == "float") {
		object.push_back(0.1f);
	} else if (first == "double") {
		object.push_back(0.1);
	} else if (first == "long double") {
		object.push_back(0.1);
	} else {

	}
}

template<typename T>
void F_init_s(T & object, InitDequeStrTag, string first, string second = "", string key = "") {
	if (first == "char*") {
		object.push_back("");
	} else if (first == "string") {
		object.push_back("");
	} else {

	}
}

template<typename T>
void F_init_s(T & object, InitListTag, string first, string second = "", string key = "") {
	if (first == "int") {
		object.push_back(0);
	} else if (first == "char") {
		object.push_back('0');
	} else if (first == "unsigned char") {
		object.push_back(0);
	} else if (first == "short") {
		object.push_back(0);
	} else if (first == "unsigned short") {
		object.push_back(0);
	} else if (first == "int") {
		object.push_back(0);
	} else if (first == "unsigned int") {
		object.push_back(0);
	} else if (first == "long") {
		object.push_back(0);
	} else if (first == "unsigned long") {
		object.push_back(0);
	} else if (first == "long long") {
		object.push_back(0);
	} else if (first == "unsigned long long") {
		object.push_back(0);
	} else if (first == "float") {
		object.push_back(0.1f);
	} else if (first == "double") {
		object.push_back(0.1);
	} else if (first == "long double") {
		object.push_back(0.1);
	} else {

	}
}

template<typename T>
void F_init_s(T & object, InitListStrTag, string first, string second = "", string key = "") {
	if (first == "char*") {
		object.push_back("");
	} else if (first == "string") {
		object.push_back("");
	} else {

	}
}

template<typename T>
void F_init_s(T & object, InitSetTag, string first, string second = "", string key = "") {
	//set是不可以重复的，可以拿随机数
	int a = rand() % 100;
	if (first == "int") {
		object.insert(a);
	}
	if (first == "char") {
		stringstream sstr;
		sstr << a;
		object.insert(sstr.str()[0]);
	} else if (first == "unsigned char") {
		object.insert(a);
	} else if (first == "short") {
		object.insert(a);
	} else if (first == "unsigned short") {
		object.insert(a);
	} else if (first == "int") {
		object.insert(a);
	} else if (first == "unsigned int") {
		object.insert(a);
	} else if (first == "long") {
		object.insert(a);
	} else if (first == "unsigned long") {
		object.insert(a);
	} else if (first == "long long") {
		object.insert(a);
	} else if (first == "unsigned long long") {
		object.insert(a);
	} else if (first == "float") {
		object.insert(static_cast<float>(a));
	} else if (first == "double") {
		object.insert(static_cast<double>(a));
	} else if (first == "long double") {
		object.insert(static_cast<long double>(a));
	} else {

	}
}

//如果是char * 类型，每次delete之后 地址都是相同的，所以不能在内部使用完归零，要在外面整体赋完值delete
static vector<char *> temp;

template<typename T>
void F_init_s(T & object, InitSetStrTag, string first, string second = "", string key = "") {
	int a = rand() % 100;
	if (first == "char*") {
		stringstream sstr;
		sstr << a;
		char * cc = new char[4];
		strcpy(cc, (char *)sstr.str().data());
		object.insert(cc);
		temp.push_back(cc);
	} else if (first == "string") {
		stringstream sstr;
		sstr << a;
		object.insert((char *)sstr.str().c_str());
	} else {

	}
}

template<typename T>
void F_init_s(T & object, InitMapBaseTag, string first, string second = "", string key = "") {
	//object.resize(2);
}

template<typename T>
void F_init_s(T & object, InitMapTag, string first, string second = "", string key = "") {
	int a = atoi(key.c_str());
	if (first == "int" && second == "int") {
		object.insert(make_pair(a, a));
	}
}

template<typename T>
void F_init_s(T & object, InitMapStrTag, string first, string second = "", string key = "") {
	int a = rand() % 100;
	if (first == "string" && second == "int") {
		stringstream sstr;
		sstr << a;
		object.insert(make_pair(key, a));
	}
}


template<typename T> 
void my_fun(T &obj, string first, string second = "", string key = "") 
{ 
} 

template<typename T> 
void my_fun(std::vector<T> &obj, string first, string second = "", string key = "") 
{ 
	typename std::vector<T>::value_type a;
	obj.push_back(a);
}

template<typename T> 
void my_fun(std::set<T> &obj, string first, string second = "", string key = "") 
{ 
} 

template<typename X> 
void my_fun(std::map<int, X> &obj, string first, string second = "", string key = "") 
{ 
	int a = atoi(key.c_str());
	int * d = &a;
	X b;
	obj.insert(make_pair(*d, b));
} 

template<typename X> 
void my_fun(std::map<string, X> &obj, string first, string second = "", string key = "") 
{ 
	X b;
	obj.insert(make_pair(key, b));
} 

template<typename T, typename X> 
void my_fun(std::map<T,X> &obj, string first, string second = "", string key = "") 
{ 
	typename std::map<T,X>::value_type a;
	T c;
	T d;
	X b;
	X e;
	obj.insert(make_pair(c, b));
	obj.insert(make_pair(d, e));
} 

template<typename T>
void F_init(T & object, int stlType, string first, string second = "", string key = "") {
	my_fun(object, first, second, key); 
}

//用于解析
struct StructTag_zx {};
struct BaseTag_zx {};
struct ArrayTag_zx {};

template<typename T> struct TagDispatchTrait_zx {
	using Tag = BaseTag_zx;
};

//GCC 5在编译时会将std::string类型按c++11下std::__cxx11::basic_string<char> 来处理
//这里可能还需要判断一下是不是GCC5以下的版本
#ifdef __GNUC__
// template<> struct TagDispatchTrait_zx<const class std::__cxx11::basic_string<char>> {
// 	using Tag = BaseTag_zx;
// };
// template<> struct TagDispatchTrait_zx<class std::__cxx11::list<bool>> {
// 	using Tag = BaseTag_zx;
// };
#elif _MSC_VER
// template<> struct TagDispatchTrait_zx<const class std::basic_string<char>> {
// 	using Tag = BaseTag_zx;
// };

// template<> struct TagDispatchTrait_zx<class std::list<bool>> {
// 	using Tag = BaseTag_zx;
// };
#endif


//用于区分(基础类型结构体/数组)
// struct BaseAndStructTag {};
// struct BaseArrayTag {};

// template<typename T> struct TagSTLAAAType {
// 	using Tag = BaseAndStructTag;
// };

//这里有问题，如何能拿到数组大小呢
// template<> struct TagSTLAAAType<int[2]> {
// 	using Tag = BaseArrayTag;
// };

// template<> struct TagSTLAAAType<list<int>> {
// 	using Tag = BaseAndStructTag;
// };

class FdogSerializer {

private:
	static mutex * mutex_serialize;
	static FdogSerializer * fdogSerializer;
	vector<ObjectInfo *> objectInfoList;
	vector<MetaInfo *> baseInfoList;
	vector<string> baseType;
	map<string, string> baseRegex;
	map<int, string> complexRegex;
	map<string, string> TypeName;
	FdogSerializer();
	~FdogSerializer();

public:
	//获取实例
	static FdogSerializer * Instance();

	//添加objectinfo
	void addObjectInfo(ObjectInfo * objectinfo);

	//获取对应Info
	ObjectInfo & getObjectInfo(string objectName);

	//获取对于Info 针对基础类型获取
	MetaInfo * getMetaInfo(string TypeName);

	//针对不同平台获取唯一类型值
	std::string getTypeName(string TypeName);

	//设置别名
	void __setAliasName(string Type, string memberName, string AliasName);
	//移除别名
	void __removeAliasName(string Type, string memberName);

	//设置是否忽略该字段序列化
	void __setIgnoreField(string Type, string memberName);
	//移除忽略字段
	void __removeIgnoreField(string Type, string memberName);

	//设置是否忽略大小写
	void __setIgnoreLU(string Type, string memberName);
	//移除忽略大小写
	void __removeIgnoreLU(string Type, string memberName);

	//设置进行模糊转换 结构体转json不存在这个问题主要是针对json转结构体的问题，如果存在分歧，可以尝试进行模糊转换
	void __setFuzzy(string Type);

	//废弃
	void __setAliasNameS(string Type) {};//退出

	//一次性设置多个别名  必须对应  类型  原成员名 别名 一旦使用，最少三个参数
	template<class T, class ...Args>
	void __setAliasNameS(T Type, T memberName, T AliasName, Args... args) {
		if (memberName == "" && AliasName == "") {
			//有问题
		}
		//每次获取两个值
		__setAliasName(Type, memberName, AliasName);
		if (sizeof...(args) == 0) {
			return;
		}
		else if ((sizeof...(args) % 2) != 0) {
			//有问题
		}
		__setAliasNameS(Type, args...);
	}

	//废弃
	void __setIgnoreFieldS(string Type) {};//退出

	//一次性设置忽略多个字段序列化
	template<class T, class ...Args>
	void __setIgnoreFieldS(T Type, T memberName, Args... args) {
		if (memberName == "") {
			//有问题
		}
		//每次获取一个值
		__setIgnoreField(Type, memberName);
		if (sizeof...(args) == 0) {
			return;
		}
		__setIgnoreFieldS(Type, args...);
	}

	//废弃
	void __setIgnoreLUS(string Type) {}//退出

	//一次性设置忽略多个字段的大小写
	template<class T, class ...Args>
	void __setIgnoreLUS(T Type, T memberName, Args... args) {
		if (memberName == "") {
			//有问题
		}
		//每次获取一个值
		__setIgnoreLU(Type, memberName);
		if (sizeof...(args) == 0) {
			return;
		}
		__setIgnoreLUS(Type, args...);
	}

	//移除第一个逗号
	void removeFirstComma(string & return_);
	//移除最后一个逗号
	void removeLastComma(string & return_);
	//移除数字
	void removeNumbers(string & return_);

	//截取字符串
	string getFdogString(string a, string b);

	//获取key值
	string getKey(string json);

	//获取成员属性
	memberAttribute getMemberAttribute(string key);

	//获取object类型
	int getObjectTypeInt(string objectName, string typeName);

	//获取基础类型 只有base和struct两种
	ObjectInfo getObjectInfoByType(string typeName, int objectTypeInt);

	#ifdef __GNUC__
	#elif _MSC_VER
		//wstring string2wstring(string str);
		//string wstring2string(wstring wstr);
	#endif

	//通过宏定义加载的信息获取
	int getObjectTypeByObjectInfo(string objectName);

	//判断是否是基础类型
	bool isBaseType(string typeName);

	//在map的基础上判断是否是基础类型
	bool isBaseTypeByMap(string typeName);

	//判断是否为vector类型
	bool isVectorType(string objectName, string typeName);

	//获取vector中的类型
	string getTypeOfVector(string objectName, string typeName);

	//判断是否为map类型
	bool isMapType(string objectName, string typeName);

	//判断是否为unordered_map类型
	bool isUnorderedMapType(string objectName, string typeName);

	//获取map中的key，value类型
	FdogMap getTypeOfMap(string objectName, string typeName);

	//判断是否是list类型
	bool isListType(string objectName, string typeName);

	//获取list中的类型
	string getTypeOfList(string objectName, string typeName);

	//判断是否是deque类型
	bool isDequeType(string objectName, string typeName);

	//判断是否是set类型
	bool isSetType(string objectName, string typeName);

	//判断是否是结构体类型
	bool isStructType(string objectName, string typeName);

	//判断是否是数组
	bool isArrayType(string objectName, string typeName);

	//解析数组
	vector<string> CuttingArray(string data);

	//切割
	vector<string> split(string str, string pattern);

	//判断json格式是否正确
	result CuttingJson(vector<string> & array_json, string json_);

	//判断方括号是否匹配
	result IsSquareBracket(string json_);

	//判断花括号是否匹配
	result IsCurlyBraces(string json_);

	//判断总符号数是否匹配
	result isMatch(string json_);

	//判断json正确性
	result __JsonValidS(string json_);

	//判断字段是否存在
	bool __Exist(string json_, string key);

	//获取字段的值
	string __GetStringValue(string json_, string key);

	//获取字段的值
	int __GetIntValue(string json_, string key);

	//获取字段的值
	double __GetDoubleValue(string json_, string key);

	//获取字段的值
	long __GetLongValue(string json_, string key);

	//获取字段的值
	bool __GetBoolValue(string json_, string key);

	//序列化
	template<typename T>
	void Serialize(string & json_, T & object_, string name = "") {
		//通过传进来的T判断是什么复合类型，ObjectInfo只保存结构体,如果是NULL可以确定传进来的不是struct类型
		ObjectInfo objectinfo = FdogSerializer::Instance()->getObjectInfo(FdogSerializer::Instance()->getTypeName(typeid(T).name()));
		//获取的只能是结构体的信息，无法知道是什么复合类型，尝试解析类型 objectType其实是一个结构体类型名称
		int objectType = getObjectTypeInt(objectinfo.objectType, FdogSerializer::Instance()->getTypeName(typeid(T).name()));
		if (objectinfo.objectType == "NULL" && objectType != OBJECT_BASE && objectType != OBJECT_STRUCT) {
			//说明不是struct类型和base类型尝试，尝试解析类型
			objectinfo = getObjectInfoByType(FdogSerializer::Instance()->getTypeName(typeid(T).name()), objectType);
			objectType = getObjectTypeInt(objectinfo.objectType, FdogSerializer::Instance()->getTypeName(typeid(T).name()));
			//这里这个objectinfo应该还是空 所以拿objecttype的数值判断
		}
		int sum = objectinfo.metaInfoObjectList.size();
		int i = 1;
		//获取到的objectType才是真正的类型，根据这个类型进行操作
		switch (objectType) {
			//第一次调用进来表示其本身类型 只有两种 结构体或着基础类型
		case OBJECT_BASE:
		{
			MetaInfo * metainfo1 = nullptr;
			metainfo1 = getMetaInfo(FdogSerializer::Instance()->getTypeName(typeid(object_).name()));
			if (metainfo1 != nullptr) {
				FdogSerializerBase::Instance()->BaseToJsonA(json_, metainfo1, object_);
			} else {
			}
		}
		break;
		case OBJECT_STRUCT:
		{
			for (auto metainfoObject : objectinfo.metaInfoObjectList) {
				bool isCall = false;
				string json_s;
				//添加一个判断 如果是普通类型，或者容器中是普通类型走普通接口，剩下的直接走类接口，提高效率
				//cout <<"成员类型：" << metainfoObject->memberType << " -- " << metainfoObject->memberTypeInt << " -- " << metainfoObject->first <<":" << metainfoObject->second << " -- " << metainfoObject->memberOffset << endl;
				if (metainfoObject->memberTypeInt == OBJECT_BASE && metainfoObject->memberIsIgnore != true) {
					FdogSerializerBase::Instance()->BaseToJson(json_s, metainfoObject, object_);
					json_ = json_ + json_s;
				} else if (metainfoObject->memberTypeInt == OBJECT_ARRAY && metainfoObject->memberIsIgnore != true) {
					if (metainfoObject->first == "bool") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(bool)));
							json_s = json_s + value + ",";
						}
						isCall = true;
					} else if (metainfoObject->first == "char") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(char)));
							json_s = json_s + value + ",";
						}
						isCall = true;
					}
					if (metainfoObject->first == "unsigned char") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned char)));
							json_s = json_s + value + ",";
						}
					} else if (metainfoObject->first == "char*") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(char*)));
							json_s = json_s + value + ",";
						}
						isCall = true;
					}
					if (metainfoObject->first == "string") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(string)));
							json_s = json_s + value + ",";
						}
					} else if (metainfoObject->first == "short") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(short)));
							json_s = json_s + value + ",";
						}
						isCall = true;
					} else if (metainfoObject->first == "unsigned short") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned short)));
							json_s = json_s + value + ",";
						}
						isCall = true;
					} else if (metainfoObject->first == "int") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(int)));
							json_s = json_s + value + ",";
						}
						isCall = true;
					} else if (metainfoObject->first == "unsigned int") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned int)));
							json_s = json_s + value + ",";
						}
						isCall = true;
					} else if (metainfoObject->first == "long") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(long)));
							json_s = json_s + value + ",";
						}
						isCall = true;
					} else if (metainfoObject->first == "unsigned long") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned long)));
							json_s = json_s + value + ",";
						}
						isCall = true;
					} else if (metainfoObject->first == "long long") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(long long)));
							json_s = json_s + value + ",";
						}
						isCall = true;
					} else if (metainfoObject->first == "unsigned long long") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned long long)));
							json_s = json_s + value + ",";
						}
						isCall = true;
					} else if (metainfoObject->first == "float") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(float)));
							json_s = json_s + value + ",";
						}
						isCall = true;
					} else if (metainfoObject->first == "double") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(double)));
							json_s = json_s + value + ",";
						}
						isCall = true;
					} else if (metainfoObject->first == "long double") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(long double)));
							json_s = json_s + value + ",";
						}
						isCall = true;
					}
					if (isCall) {
						json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]" + ",";
					}
					json_s = "";
				} else if (metainfoObject->memberTypeInt == OBJECT_VECTOR && metainfoObject->memberIsIgnore != true) {
					//cout << "====OBJECT_VECTOR获取的值：" << metainfoObject->first << " name = " << typeid(object_).name() << " metainfoObject->memberOffset = " << metainfoObject->memberOffset<< endl;
					if (metainfoObject->first == "char") {
						FSerialize(json_s, *(vector<char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned char") {
						FSerialize(json_s, *(vector<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "char*") {
						FSerialize(json_s, *(vector<char *> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "string") {
						FSerialize(json_s, *(vector<string> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "short") {
						FSerialize(json_s, *(vector<short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned short") {
						FSerialize(json_s, *(vector<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "int") {
						FSerialize(json_s, *(vector<int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned int") {
						FSerialize(json_s, *(vector<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "long") {
						FSerialize(json_s, *(vector<long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned long") {
						FSerialize(json_s, *(vector<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "long long") {
						FSerialize(json_s, *(vector<long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned long long") {
						FSerialize(json_s, *(vector<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "float") {
						FSerialize(json_s, *(vector<float> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "double") {
						FSerialize(json_s, *(vector<double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "long double") {
						FSerialize(json_s, *(vector<long double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
						isCall = true;
					}                                                                                                                                                                                                                                           
					if (isCall) {
						json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]" + ",";
					}
					json_s = "";
				} else if (metainfoObject->memberTypeInt == OBJECT_LIST && metainfoObject->memberIsIgnore != true) {
					if (metainfoObject->first == "bool") {
						FSerialize(json_s, *(list<bool> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<bool>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "char") {
						FSerialize(json_s, *(list<char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned char") {
						FSerialize(json_s, *(list<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "char*") {
						FSerialize(json_s, *(list<char *> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "string") {
						FSerialize(json_s, *(list<string> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "short") {
						FSerialize(json_s, *(list<short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned short") {
						FSerialize(json_s, *(list<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "int") {
						FSerialize(json_s, *(list<int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned int") {
						FSerialize(json_s, *(list<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "long") {
						FSerialize(json_s, *(list<long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned long") {
						FSerialize(json_s, *(list<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "long long") {
						FSerialize(json_s, *(list<long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned long long") {
						FSerialize(json_s, *(list<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "float") {
						FSerialize(json_s, *(list<float> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "double") {
						FSerialize(json_s, *(list<double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "long double") {
						FSerialize(json_s, *(list<long double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
						isCall = true;
					}
					if (isCall) {
						json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]" + ",";
					}
					json_s = "";
				} else if (metainfoObject->memberTypeInt == OBJECT_DEQUE && metainfoObject->memberIsIgnore != true) {
					if (metainfoObject->first == "bool") {
						FSerialize(json_s, *(deque<bool> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<bool>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "char") {
						FSerialize(json_s, *(deque<char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned char") {
						FSerialize(json_s, *(deque<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "char*") {
						FSerialize(json_s, *(deque<char *> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "string") {
						FSerialize(json_s, *(deque<string> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "short") {
						FSerialize(json_s, *(deque<short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned short") {
						FSerialize(json_s, *(deque<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "int") {
						FSerialize(json_s, *(deque<int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned int") {
						FSerialize(json_s, *(deque<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "long") {
						FSerialize(json_s, *(deque<long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned long") {
						FSerialize(json_s, *(deque<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "long long") {
						FSerialize(json_s, *(deque<long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned long long") {
						FSerialize(json_s, *(deque<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "float") {
						FSerialize(json_s, *(deque<float> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "double") {
						FSerialize(json_s, *(deque<double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "long double") {
						FSerialize(json_s, *(deque<long double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
						isCall = true;
					}
					if (isCall) {
						json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]" + ",";
					}
					json_s = "";
				} else if (metainfoObject->memberTypeInt == OBJECT_SET && metainfoObject->memberIsIgnore != true) {
					if (metainfoObject->first == "bool") {
						FSerialize(json_s, *(set<bool> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<bool>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "char") {
						FSerialize(json_s, *(set<char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned char") {
						FSerialize(json_s, *(set<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "char*") {
						FSerialize(json_s, *(set<char *> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "string") {
						FSerialize(json_s, *(set<string> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "short") {
						FSerialize(json_s, *(set<short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned short") {
						FSerialize(json_s, *(set<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "int") {
						FSerialize(json_s, *(set<int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned int") {
						FSerialize(json_s, *(set<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "long") {
						FSerialize(json_s, *(set<long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned long") {
						FSerialize(json_s, *(set<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "long long") {
						FSerialize(json_s, *(set<long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "unsigned long long") {
						FSerialize(json_s, *(set<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "float") {
						FSerialize(json_s, *(set<float> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "double") {
						FSerialize(json_s, *(set<double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "long double") {
						FSerialize(json_s, *(set<long double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
						isCall = true;
					}
					if (isCall) {
						json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]" + ",";
					}
					json_s = "";
				} else if (metainfoObject->memberTypeInt == OBJECT_MAP && metainfoObject->memberIsIgnore != true) {
					if (metainfoObject->first == "char*" && metainfoObject->second == "int") {
						FSerialize(json_s, *(map<char *, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<map<int, int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "string" && metainfoObject->second == "int") {
						FSerialize(json_s, *(map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<map<int, int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "string" && metainfoObject->second == "float") {
						FSerialize(json_s, *(map<string, float> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<map<int, int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "string" && metainfoObject->second == "bool") {
						FSerialize(json_s, *(map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<map<int, int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "int" && metainfoObject->second == "int") {
						FSerialize(json_s, *(map<int, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<map<int, int>>::Tag{});
						isCall = true;
					} 
					if (isCall) {
						json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "{" + json_s + "}" + ",";
					}
					json_s = "";
				} else if (metainfoObject->memberTypeInt == OBJECT_UNORDERED_MAP && metainfoObject->memberIsIgnore != true) {
					if (metainfoObject->first == "char*" && metainfoObject->second == "int") {
						FSerialize(json_s, *(unordered_map<char *, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<unordered_map<int, int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "string" && metainfoObject->second == "int") {
						FSerialize(json_s, *(unordered_map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<unordered_map<int, int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "string" && metainfoObject->second == "float") {
						FSerialize(json_s, *(unordered_map<string, float> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<unordered_map<int, int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "string" && metainfoObject->second == "bool") {
						FSerialize(json_s, *(unordered_map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<unordered_map<int, int>>::Tag{});
						isCall = true;
					} else if (metainfoObject->first == "int" && metainfoObject->second == "int") {
						FSerialize(json_s, *(unordered_map<int, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<unordered_map<int, int>>::Tag{});
						isCall = true;
					}
					if (isCall) {
						json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "{" + json_s + "}" + ",";
					}
					json_s = "";
				}
				if (!isCall) {
					Serialize_struct(json_, json_s, object_, metainfoObject, typename TagDispatchTrait_zx<T>::Tag{});
				}

				if (i == sum) {
					if (json_.length() > 0) {
						removeLastComma(json_);
					}
				}
				json_s = "";
				i++;
			}
			//json_ = "{" + json_ + "}";
		}
		break;
		}
	}

	//自定义结构体
	template<typename T>
	void Serialize_struct(string & json_, string & json_s, T & object_, MetaInfo * metainfoObject, StructTag_zx) {
		object_.Serialize_struct_s(json_, json_s, object_, metainfoObject);
		//json_ = "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_ + "]" + ",";
	}
	template<typename T>
	void Serialize_struct(string & json_, string & json_s, T & object_, MetaInfo * metainfoObject, BaseTag_zx) {
	}

	//用于解析基础类型，数组(只需要判断有没有[]就能确定是不是数组，结构体和基础类型都不具备[]条件)，结构体
	template<typename T>
	void FSerialize(string & json_, T & object_, BaseTag, string name = "") {
		Serialize(json_, object_, name);
		//这里需要判断类型 如果是基础类型直接使用name 不是基础类型，可以使用
		if (isBaseType(FdogSerializer::Instance()->getTypeName(typeid(T).name()))) {
			removeLastComma(json_);
			json_ = "{\"" + name + "\":" + json_ + "}";
			return;
		}
		if (isArrayType("", FdogSerializer::Instance()->getTypeName(typeid(T).name()))) {
			removeLastComma(json_);
			json_ = "{\"" + name + "\":" + "[" + json_ + "]" + "}";
			return;
		}
		json_ = "{" + json_ + "}";
	}
	//用于解析STL（map除外）其实上面接口也可以处理vector，但其他类型无法处理，所以这个处理STL
	template<typename T>
	void FSerialize(string & json_, T & object_, ArrayTag, string name = "") {
		for (auto & object_one : object_) {
			//ji
			int objectType = isBaseType(FdogSerializer::Instance()->getTypeName(typeid(object_one).name()));
			if (!objectType) {
				json_ = json_ + "{";
			}
			Serialize(json_, object_one);

			if (!objectType) {
				json_ = json_ + "},";
			}
		}
		if (json_.length() > 0) {
			removeLastComma(json_);
		}
		//如果转换对象直接就是数组，可以再额外提供一个，或者说其他
	}

	//用于解析map
	template<typename T>
	void FSerialize(string & json_, T & object_, MapTag, string name = "") {
		int i = 0;
		for (auto & object_one : object_) {
			//看情况，如果是结构体，需要花括号，基本类型不需要
			json_ = json_ + "\"" + F_toString(object_one.first) + "\":";
			string json_s = "";
			Serialize(json_s, object_one.second);
			int objectType = isBaseType(FdogSerializer::Instance()->getTypeName(typeid(object_one).name()));
			if (!objectType) { 
				json_ = json_ + "{" + json_s + "}";
				json_ = json_ + ",";
			} else {
				json_ = json_ + json_s;
			}
			i++;
		}
		removeLastComma(json_);
	}

	//用于过度
	// template<typename T>
	// void FSerialize(string & json_, T & object_, BaseAndStructTag, string name = ""){
	//     //什么都不做
	// }

	//反序列化
	template<typename T>
	void Deserialize(T & object_, string & json_, string name = "") {
		ObjectInfo & objectinfo = FdogSerializer::Instance()->getObjectInfo(FdogSerializer::Instance()->getTypeName(typeid(T).name()));
		int objectType = getObjectTypeInt(objectinfo.objectType, FdogSerializer::Instance()->getTypeName(typeid(T).name()));
		if (objectinfo.objectType == "NULL" && objectType != OBJECT_BASE && objectType != OBJECT_STRUCT) {
			//说明不是struct类型和base类型尝试，尝试解析类型
			objectinfo = getObjectInfoByType(FdogSerializer::Instance()->getTypeName(typeid(T).name()), objectType);
			objectType = getObjectTypeInt(objectinfo.objectType, FdogSerializer::Instance()->getTypeName(typeid(T).name()));
		}
		if (OBJECT_BASE == objectType) {
			MetaInfo * metainfo1 = getMetaInfo(FdogSerializer::Instance()->getTypeName(typeid(object_).name()));
			smatch result;
			string regex_key;
			string regex_value = baseRegex[metainfo1->memberType];
			regex * pattern;
			if (name == "") {
				pattern = new regex(regex_value);
			} else {
				pattern = new regex(regex_key + ":" + regex_value);
				// if (metainfoObject->memberIsIgnoreLU == false){
				//     pattern = new regex(regex_key + ":" +regex_value);
				// }else{
				//     pattern = new regex(regex_key + ":" +regex_value,regex::icase);//icase用于忽略大小写
				// }
			}
			if (regex_search(json_, result, *pattern)) {
				string value = result.str(2).c_str();
				if (value == "") {
					value = result.str(1).c_str();
				}
				FdogSerializerBase::Instance()->JsonToBase(object_, metainfo1, value);
			}
		}

		if (OBJECT_STRUCT == objectType) {
			for (auto metainfoObject : objectinfo.metaInfoObjectList) {
				bool isCall = false;
				//通过正则表达式获取对应的json
				smatch result;
				string regex_key = "(\"" + metainfoObject->memberName + "\")";
				string regex_value = baseRegex[metainfoObject->memberType];
				if (regex_value == "") {
					if (metainfoObject->memberTypeInt == OBJECT_STRUCT) {
						regex_value = objectRegex;
					} else if (metainfoObject->memberTypeInt == OBJECT_ARRAY) {
						regex_value = arrayRegex;
					} else if (metainfoObject->memberTypeInt == OBJECT_VECTOR) {
						regex_value = arrayRegex;
					} else if (metainfoObject->memberTypeInt == OBJECT_LIST) {
						regex_value = arrayRegex;
					} else if (metainfoObject->memberTypeInt == OBJECT_SET) {
						regex_value = arrayRegex;
					} else if (metainfoObject->memberTypeInt == OBJECT_DEQUE) {
						regex_value = arrayRegex;
					} else if (metainfoObject->memberTypeInt == OBJECT_MAP) {
						regex_value = mapRegex;
					} else if (metainfoObject->memberTypeInt == OBJECT_UNORDERED_MAP) {
						regex_value = mapRegex;
					}
				} else {
				}
				//删除正则表达式，用代码获取对应数据
				//根据大小写判断
				regex * pattern = nullptr;
				if (metainfoObject->memberIsIgnoreLU == false) {
					pattern = new regex(regex_key + ":" + regex_value);
				} else {
					pattern = new regex(regex_key + ":" + regex_value, regex::icase);//icase用于忽略大小写
				}
				if (regex_search(json_, result, *pattern)) {
					string value = result.str(2).c_str();
					if (metainfoObject->memberTypeInt == OBJECT_BASE && metainfoObject->memberIsIgnore != true) {
						FdogSerializerBase::Instance()->JsonToBase(object_, metainfoObject, value);
					} else if (metainfoObject->memberTypeInt == OBJECT_ARRAY && metainfoObject->memberIsIgnore != true) {
						vector<string> json_array;
						objectType = isBaseType(metainfoObject->first);
						if (objectType) {
							smatch result;
							regex pattern(arrayRegex);
							if (regex_search(json_, result, pattern)) {
								string value = result.str(2).c_str();
								json_array = split(value, ",");
							}
						} else {
							removeFirstComma(json_);
							removeLastComma(json_);
							json_array = FdogSerializer::Instance()->CuttingArray(json_);
						}
						int j = 0;
						if (metainfoObject->first == "bool") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(bool)), json_array[j++]);
							}
							isCall = true;
						} else if (metainfoObject->first == "char") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(char)), json_array[j++]);
							}
							isCall = true;
						} else if (metainfoObject->first == "unsigned char") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned char)), json_array[j++]);
							}
							isCall = true;
						} else if (metainfoObject->first == "char*") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(char*)), json_array[j++]);
							}
							isCall = true;
						} else if (metainfoObject->first == "string") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(string)), json_array[j++]);
							}
							isCall = true;
						} else if (metainfoObject->first == "short") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(short)), json_array[j++]);
							}
							isCall = true;
						} else if (metainfoObject->first == "unsigned short") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned short)), json_array[j++]);
							}
							isCall = true;
						} else if (metainfoObject->first == "int") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(int)), json_array[j++]);
							}
							isCall = true;
						} else if (metainfoObject->first == "unsigned int") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned int)), json_array[j++]);
							}
							isCall = true;
						} else if (metainfoObject->first == "long") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(long)), json_array[j++]);
							}
							isCall = true;
						} else if (metainfoObject->first == "unsigned long") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned long)), json_array[j++]);
							}
							isCall = true;
						} else if (metainfoObject->first == "long long") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(long long)), json_array[j++]);
							}
							isCall = true;
						} else if (metainfoObject->first == "unsigned long long") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned long long)), json_array[j++]);
							}
							isCall = true;
						} else if (metainfoObject->first == "float") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(float)), json_array[j++]);
							}
							isCall = true;
						} else if (metainfoObject->first == "double") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(double)), json_array[j++]);
							}
							isCall = true;
						} else if (metainfoObject->first == "long double") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(long double)), json_array[j++]);
							}
							isCall = true;
						}
					} else if (metainfoObject->memberTypeInt == OBJECT_VECTOR && metainfoObject->memberIsIgnore != true) {
						if (metainfoObject->first == "char") {
							FDeserialize(*(vector<char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned char") {
							FDeserialize(*(vector<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "char*") {
							FDeserialize(*(vector<char *> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "string") {
							FDeserialize(*(vector<string> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "short") {
							FDeserialize(*(vector<short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned short") {
							FDeserialize(*(vector<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "int") {
							FDeserialize(*(vector<int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned int") {
							FDeserialize(*(vector<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "long") {
							FDeserialize(*(vector<long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned long") {
							FDeserialize(*(vector<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "long long") {
							FDeserialize(*(vector<long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned long long") {
							FDeserialize(*(vector<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "float") {
							FDeserialize(*(vector<float> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "double") {
							FDeserialize(*(vector<double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "long double") {
							FDeserialize(*(vector<long double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
							isCall = true;
						}
					} else if (metainfoObject->memberTypeInt == OBJECT_LIST && metainfoObject->memberIsIgnore != true) {
						if (metainfoObject->first == "bool") {
							FDeserialize(*(list<bool> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<bool>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "char") {
							FDeserialize(*(list<char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned char") {
							FDeserialize(*(list<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "char*") {
							FDeserialize(*(list<char *> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "string") {
							FDeserialize(*(list<string> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "short") {
							FDeserialize(*(list<short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned short") {
							FDeserialize(*(list<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "int") {                      
							FDeserialize(*(list<int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned int") {
							FDeserialize(*(list<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "long") {
							FDeserialize(*(list<long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned long") {
							FDeserialize(*(list<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "long long") {
							FDeserialize(*(list<long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned long long") {
							FDeserialize(*(list<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "float") {
							FDeserialize(*(list<float> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "double") {
							FDeserialize(*(list<double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "long double") {
							FDeserialize(*(list<long double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
							isCall = true;
						}
					} else if (metainfoObject->memberTypeInt == OBJECT_DEQUE && metainfoObject->memberIsIgnore != true) {
						if (metainfoObject->first == "bool") {
							FDeserialize(*(deque<bool> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<bool>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "char") {
							FDeserialize(*(deque<char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned char") {
							FDeserialize(*(deque<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "char*") {
							FDeserialize(*(deque<char *> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "string") {
							FDeserialize(*(deque<string> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "short") {
							FDeserialize(*(deque<short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned short") {
							FDeserialize(*(deque<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "int") {
							FDeserialize(*(deque<int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned int") {
							FDeserialize(*(deque<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "long") {
							FDeserialize(*(deque<long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned long") {
							FDeserialize(*(deque<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "long long") {
							FDeserialize(*(deque<long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned long long") {
							FDeserialize(*(deque<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "float") {
							FDeserialize(*(deque<float> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "double") {
							FDeserialize(*(deque<double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "long double") {
							FDeserialize(*(deque<long double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
							isCall = true;
						}
					} else if (metainfoObject->memberTypeInt == OBJECT_SET && metainfoObject->memberIsIgnore != true) {
						if (metainfoObject->first == "bool") {
							FDeserialize(*(set<bool> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<bool>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "char") {
							FDeserialize(*(set<char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned char") {
							FDeserialize(*(set<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "char*") {
							FDeserialize(*(set<char *> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "string") {
							FDeserialize(*(set<string> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "short") {
							FDeserialize(*(set<short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned short") {
							FDeserialize(*(set<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "int") {
							FDeserialize(*(set<int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned int") {
							FDeserialize(*(set<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "long") {
							FDeserialize(*(set<long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned long") {
							FDeserialize(*(set<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "long long") {
							FDeserialize(*(set<long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "unsigned long long") {
							FDeserialize(*(set<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "float") {
							FDeserialize(*(set<float> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "double") {
							FDeserialize(*(set<double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "long double") {
							FDeserialize(*(set<long double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
							isCall = true;
						}
					} else if (metainfoObject->memberTypeInt == OBJECT_MAP && metainfoObject->memberIsIgnore != true) {
						//string v = getFdogString("\"" + metainfoObject->memberName + "\":", json_);
						//value = "{12:{\"name\":\"zhangxu\",\"age\":21},13:{\"name\":\"yujing\",\"age\":21}}";
						if (metainfoObject->first == "char*" && metainfoObject->second == "int") {
							FDeserialize(*(map<char *, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<map<int, int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "string" && metainfoObject->second == "int") {
							FDeserialize(*(map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<map<int, int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "string" && metainfoObject->second == "float") {
							FDeserialize(*(map<string, float> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<map<int, int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "string" && metainfoObject->second == "bool") {
							FDeserialize(*(map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<map<int, int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "int" && metainfoObject->second == "int") {
							FDeserialize(*(map<int, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<map<int, int>>::Tag{});
							isCall = true;
						}
					} else if (metainfoObject->memberTypeInt == OBJECT_UNORDERED_MAP && metainfoObject->memberIsIgnore != true) {
						if (metainfoObject->first == "char*" && metainfoObject->second == "int") {
							FDeserialize(*(unordered_map<char *, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<unordered_map<int, int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "string" && metainfoObject->second == "int") {
							FDeserialize(*(unordered_map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<unordered_map<int, int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "string" && metainfoObject->second == "float") {
							FDeserialize(*(unordered_map<string, float> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<unordered_map<int, int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "string" && metainfoObject->second == "bool") {
							FDeserialize(*(unordered_map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<unordered_map<int, int>>::Tag{});
							isCall = true;
						} else if (metainfoObject->first == "int" && metainfoObject->second == "int") {
							FDeserialize(*(unordered_map<int, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<unordered_map<int, int>>::Tag{});
							isCall = true;
						}
					}

					if (!isCall) {
						Deserialize_struct(object_, value, metainfoObject, typename TagDispatchTrait_zx<T>::Tag{});
					}
				}
			}
		}
	}
	//自定义结构体
	template<typename T>
	void Deserialize_struct(T & object_, string & value, MetaInfo * metainfoObject, StructTag_zx) {
		object_.Deserialize_struct_s(object_, value, metainfoObject);
	}
	template<typename T>
	void Deserialize_struct(T & object_, string & value, MetaInfo * metainfoObject, BaseTag_zx) {
	}

	//用于解析基础类型，数组(只需要判断有没有[]就能确定是不是数组，结构体和基础类型都不具备[]条件)，结构体
	template<typename T>
	void FDeserialize(T & object_, string & json_, BaseTag, string name = "") {
		Deserialize(object_, json_, name);
	}

	//用于解析STL（map除外）
	template<typename T>
	void FDeserialize(T & object_, string & json_, ArrayTag, string name = "") {
		memberAttribute Member = getMemberAttribute(FdogSerializer::Instance()->getTypeName(typeid(T).name()));
		int objectType = 0;
		objectType = isBaseType(Member.first);
		vector<string> json_array;
		if (objectType) {
			//搞基础
			smatch result;
			regex pattern(arrayRegex);
			if (regex_search(json_, result, pattern)) {
				string value = result.str(2).c_str();
				json_array = split(value, ",");
			}
		}
		else {
			//搞对象
			removeFirstComma(json_);
			removeLastComma(json_);
			json_array = FdogSerializer::Instance()->CuttingArray(json_);
		}
		int i = 0;
		//这里需要注意，进来的STL容器长度为0，需要重新指定长度 需要想办法
		//需要知道容易内部类型，然后作为参数传进去,如果长度小于需要转换的，就需要添加长度,STL有扩容机制
		int len = json_array.size();
		int len2 = object_.size();
		srand((int)time(NULL)); //用于set
		for (int i = 0; i < json_array.size(); i++) {
			if (json_array.size() > object_.size()) {
				F_init(object_, Member.valueTypeInt, Member.first);
			}
		}
		for (auto & object_one : object_) {
			if (i > len) {
				break;
			}
			Deserialize(object_one, json_array[i]);
			i++;
		}
		//这里释放init里面申请的内存
		if (1) {
			vector<char *>::iterator it = temp.begin();
			while (it != temp.end()) {
				delete *it;
				++it;
			}
		}
		temp.clear();
	}
	//用于map
	template<typename T>
	void FDeserialize(T & object_, string & json_, MapTag) {
		memberAttribute Member = getMemberAttribute(FdogSerializer::Instance()->getTypeName(typeid(T).name()));
		int objectType = 0;
		objectType = isBaseType(Member.first);
		for (auto & object_one : object_) {
			//判断内部类型是否为基础类型
			objectType = isBaseTypeByMap(FdogSerializer::Instance()->getTypeName(typeid(object_one).name()));
			break;
		}
		//这里有问题 objectType永远为0 找第二种方法
		vector<string> json_array;
		vector<string> json_key;
		objectType = 0;
		if (objectType) {
			smatch result;
			regex pattern(mapRegex);
			if (regex_search(json_, result, pattern)) {
				string value = result.str(2).c_str();
				json_array = split(value, ",");
			}
		} else {
			removeFirstComma(json_);
			removeLastComma(json_);
			json_array = FdogSerializer::Instance()->CuttingArray(json_);
			json_key.push_back("12");
			json_key.push_back("13");
		}
		int i = 0;
		int len = json_array.size();
		sort(json_array.begin(), json_array.end());
		for (int i = 0; i < json_array.size(); i++) {
			if (json_array.size() > object_.size()) {
				F_init(object_, Member.valueTypeInt, Member.first, Member.second, json_key[i]);
			}
		}
		//这里有个问题，就是可能key的顺序不匹配
		//这里存在问题，进来的STL容器长度为0，需要重新指定长度
		for (auto & object_one : object_) {
			Deserialize(object_one.second, json_array[i]);
			i++;
		}
		//这里释放init里面申请的内存
		if (Member.valueTypeInt == OBJECT_MAP) {
			vector<char *>::iterator it = temp.begin();
			while (it != temp.end()) {
				delete *it;
				++it;
			}
		}
		temp.clear();
	}
};

namespace Fdog {

	template<typename T>
	void FJson(string & json_, T & object_, string name = "") {
		FdogSerializer::Instance()->FSerialize(json_, object_, typename TagDispatchTrait<T>::Tag{}, name);
	}

	template<typename T>
	void FObject(T & object_, string & json_, string name = "") {
		FdogSerializer::Instance()->FDeserialize(object_, json_, typename TagDispatchTrait<T>::Tag{}, name);
	}

	template<typename T>
	std::string FJsonToString(T & object_, string json_ = "", string name = "") {
		FdogSerializer::Instance()->FSerialize(json_, object_, typename TagDispatchTrait<T>::Tag{}, name);
		return json_;
	}

	//设置别名
	void setAliasName(string Type, string memberName, string AliasName);

	//设置多个别名
	template<class T, class ...Args>
	void setAliasNameS(T Type, T memberName, T AliasName, Args... args) {
		FdogSerializer::Instance()->__setAliasNameS(Type, memberName, AliasName, args...);
	}

	//设置是否忽略该字段序列化
	void setIgnoreField(string Type, string memberName);

	//设置多个忽略字段序列化
	template<class T, class ...Args>
	void setIgnoreFieldS(T Type, T memberName, Args... args) {
		FdogSerializer::Instance()->__setIgnoreFieldS(Type, memberName, args...);
	}

	//设置是否忽略大小写
	void setIgnoreLU(string Type, string memberName);

	//设置多个忽略大小写
	template<class T, class ...Args>
	void setIgnoreLUS(T Type, T memberName, Args... args) {
		FdogSerializer::Instance()->__setIgnoreLUS(Type, memberName, args...);
	}

	//设置进行模糊转换 结构体转json不存在这个问题主要是针对json转结构体的问题，如果存在分歧，可以尝试进行模糊转换
	void setFuzzy(string Type);

	//判断json正确性
	result JsonValidS(string json_);

	//判断字段是否存在
	bool Exist(string json_, string key);

	//获取字段的值
	string GetStringValue(string json_, string key);

	//获取字段的值
	int GetIntValue(string json_, string key);

	//获取字段的值
	double GetDoubleValue(string json_, string key);

	//获取字段的值
	long GetLongValue(string json_, string key);

	//获取字段的值
	bool GetBoolValue(string json_, string key);

}

#define EXPAND(...) __VA_ARGS__  //1g

#define NAME_MSVC(x) #x

#define NAME(...) EXPAND(NAME_MSVC(__VA_ARGS__))

#define EXTAND_ARGS(args) args //__VA_ARGS__ 在vs中会被认为是一个实参，所以需要定义该宏过渡

#define ARG_N(...) \
    EXTAND_ARGS(ARG_N_(0, __VA_ARGS__, ARG_N_RESQ()))

#define ARG_N_(...) \
    EXTAND_ARGS(ARG_N_M(__VA_ARGS__))  //2g

#define ARG_N_M(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, _11, _12, _13, _14, _15, _16, _17, _18, _19,_20, N,...) N

#define ARG_N_RESQ() 20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0

#define MEMBERTYPE(TYPE, MEMBER) FdogSerializer::Instance()->getMemberAttribute(FdogSerializer::Instance()->getTypeName(typeid(((TYPE *)0)->MEMBER).name()));

#define PLACEHOLDER(placeholder, ...) placeholder

#ifdef __GNUC__

#define REGISTERED_FUCK_MSVC(TYPE, ...) \
do{ \
    ObjectInfo * objectinfo_one = new ObjectInfo();\
    objectinfo_one->objectType = NAME(TYPE);\
    objectinfo_one->objectTypeInt = FdogSerializer::Instance()->getObjectTypeInt(objectinfo_one->objectType, FdogSerializer::Instance()->getTypeName(typeid(TYPE).name()));\
    objectinfo_one->objectSize = sizeof(TYPE);\
    FdogSerializer::Instance()->addObjectInfo(objectinfo_one);\
	EXPAND(REGISTEREDMEMBER_s_1_MSVC(TYPE, PLACEHOLDER(__VA_ARGS__), objectinfo_one->metaInfoObjectList, ARG_N(__VA_ARGS__) - 1, __VA_ARGS__, PLACEHOLDER(__VA_ARGS__)));\
}while(0);

#elif _MSC_VER

#define REGISTERED_FUCK_MSVC(TYPE, ...) \
do{ \
    ObjectInfo * objectinfo_one = new ObjectInfo();\
    objectinfo_one->objectType = NAME(TYPE);\
    objectinfo_one->objectTypeInt = FdogSerializer::Instance()->getObjectTypeInt(objectinfo_one->objectType, FdogSerializer::Instance()->getTypeName(typeid(TYPE).name()));\
    objectinfo_one->objectSize = sizeof(TYPE);\
    FdogSerializer::Instance()->addObjectInfo(objectinfo_one);\
	EXPAND(REGISTEREDMEMBER_s_1_MSVC(TYPE, PLACEHOLDER(__VA_ARGS__), objectinfo_one->metaInfoObjectList, ARG_N(__VA_ARGS__) - 1, __VA_ARGS__, PLACEHOLDER(##__VA_ARGS__)));\
}while(0);

#endif

#define REGISTEREDMEMBER(...) EXPAND(REGISTERED_FUCK_MSVC(##__VA_ARGS__))

#define SERIALIZE_FUCK_MSVC(TYPE, ...) \
    SERIALIZE_FUCK_MSVC_s_1(TYPE, PLACEHOLDER(__VA_ARGS__), ARG_N(__VA_ARGS__) - 1, __VA_ARGS__, PLACEHOLDER(__VA_ARGS__));

#define SERIALIZE_FUCK_MSVC_s_1(TYPE, ...)\
    EXPAND(SERIALIZE_s_1(TYPE, __VA_ARGS__))

#define SERIALIZE_s_1(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_2(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_2(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_3(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_3(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_4(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_4(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_5(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_5(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_6(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_6(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_7(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_7(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_8(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_8(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_9(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_9(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_10(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_10(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_11(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_11(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_12(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_12(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_13(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_13(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_14(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_14(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_15(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_15(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_16(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_16(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_17(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_17(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_18(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_18(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size); if (size > 0)EXPAND(SERIALIZE_s_19(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define SERIALIZE_s_19(TYPE, PLACE, size, arg1, ...)\
    SERIALIZE_s(TYPE, arg1, size);

#define DESERIALIZE_FUCK_MSVC(TYPE, ...) \
    DESERIALIZE_FUCK_MSVC_s_1(TYPE, PLACEHOLDER(__VA_ARGS__), ARG_N(__VA_ARGS__) - 1, __VA_ARGS__, PLACEHOLDER(__VA_ARGS__));

#define DESERIALIZE_FUCK_MSVC_s_1(TYPE, ...)\
    EXPAND(DESERIALIZE_s_1(TYPE, __VA_ARGS__))

#define DESERIALIZE_s_1(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_2(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_2(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_3(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_3(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_4(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_4(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_5(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_5(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_6(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_6(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_7(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_7(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_8(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_8(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_9(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_9(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_10(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_10(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_11(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_11(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_12(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_12(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_13(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_13(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_14(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_14(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_15(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_15(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_16(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_16(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_17(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_17(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_18(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_18(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size); EXPAND(DESERIALIZE_s_19(TYPE, PLACE, size-1, __VA_ARGS__, PLACE));

#define DESERIALIZE_s_19(TYPE, PLACE, size, arg1, ...)\
    DESERIALIZE_s(TYPE, arg1, size);

#define SERIALIZE_s(TYPE, arg1, size)\
    if (size > -1 && metainfoObject->memberTypeInt == OBJECT_STRUCT && metainfoObject->memberType == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name())) {\
        Serialize_struct_Temp(json_, json_s, object_, object_.arg1, metainfoObject);\
    } else if (metainfoObject->memberTypeInt == OBJECT_ARRAY && metainfoObject->first == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name())) {\
        Serialize_array_Temp(json_, json_s, object_, object_.arg1, metainfoObject);\
    }else if (size > -1 && metainfoObject->memberTypeInt == OBJECT_VECTOR && metainfoObject->memberType == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name())) {\
		Serialize_vector_Temp(json_, json_s, object_, object_.arg1, metainfoObject);\
    } else if (size > -1 && metainfoObject->memberTypeInt == OBJECT_LIST && metainfoObject->first == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name())) {\
        Serialize_list_Temp(json_, object_, object_.arg1, metainfoObject);\
    } else if (size > -1 && metainfoObject->memberTypeInt == OBJECT_SET && metainfoObject->first == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name())) {\
        Serialize_set_Temp(json_, object_, object_.arg1, metainfoObject);\
    } else if (size > -1 && metainfoObject->memberTypeInt == OBJECT_DEQUE && metainfoObject->first == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name())) {\
        Serialize_deque_Temp(json_, object_, object_.arg1, metainfoObject);\
    } else if (size > -1 && metainfoObject->memberTypeInt == OBJECT_MAP && metainfoObject->memberType == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name())) {\
        Serialize_map_Temp(json_, json_s, object_, object_.arg1, metainfoObject);\
    } else if (size > -1 && metainfoObject->memberTypeInt == OBJECT_UNORDERED_MAP && metainfoObject->memberType == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name())) {\
        Serialize_unordered_map_Temp(json_, object_, object_.arg1, metainfoObject);\
    };\

#define DESERIALIZE_s(TYPE, arg1, size)\
    if (size > -1 && metainfoObject->memberTypeInt == OBJECT_STRUCT && metainfoObject->memberType == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name()) && metainfoObject->memberIsIgnore != true && size > -1) {\
        Deserialize_struct_Temp(object_, object_.arg1, value, metainfoObject);\
    } else if (metainfoObject->memberTypeInt == OBJECT_ARRAY && metainfoObject->memberType == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name())) {\
        Deserialize_array_Temp(object_, object_.arg1, value, metainfoObject);\
    } else if (metainfoObject->memberTypeInt == OBJECT_VECTOR && metainfoObject->memberType == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name()) && size > -1) {\
		Deserialize_vector_Temp(object_, object_.arg1, value, metainfoObject);\
    } else if (metainfoObject->memberTypeInt == OBJECT_LIST && metainfoObject->memberType == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name()) && size > -1) {\
        Deserialize_list_Temp(object_, object_.arg1, value, metainfoObject);\
    } else if (metainfoObject->memberTypeInt == OBJECT_SET && metainfoObject->memberType == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name()) && size > -1) {\
        Deserialize_set_Temp(object_, object_.arg1, value, metainfoObject);\
    } else if (metainfoObject->memberTypeInt == OBJECT_DEQUE && metainfoObject->memberType == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name()) && size > -1) {\
        Deserialize_deque_Temp(object_, object_.arg1, value, metainfoObject);\
    } else if (metainfoObject->memberTypeInt == OBJECT_MAP && metainfoObject->memberType == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name()) && size > -1) {\
		Deserialize_map_Temp(object_, object_.arg1, value, metainfoObject);\
    } else if (metainfoObject->memberTypeInt == OBJECT_UNORDERED_MAP && metainfoObject->memberType == FdogSerializer::Instance()->getTypeName(typeid(object_.arg1).name())) {\
        Deserialize_unordered_map_Temp(object_, object_.arg1, value, metainfoObject);\
    };\

#define REGISTERED_MEMBER_S(...)\
public:\
EXPAND(PLACEHOLDER(__VA_ARGS__))() {\
	EXPAND(PLACEHOLDER(__VA_ARGS__)):InitFdogSerialize();\
};\
EXPAND(REGISTEREDMEMBER(__VA_ARGS__))

#define REGISTERED_MEMBER_C(...)\
EXPAND(REGISTEREDMEMBER(__VA_ARGS__))

#define REGISTEREDMEMBER(...)\
public:\
template<typename T, typename X>\
void Serialize_struct_Temp(string & json_, string & json_s, T & object_, X & object_2, MetaInfo * metainfoObject) {\
    FdogSerializer::Instance()->Serialize(json_s, object_2);\
    json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "{" + json_s + "}" + ",";\
};\
template<typename T, typename X>\
void Deserialize_struct_Temp(T & object_, X & object_2, string & value, MetaInfo * metainfoObject) {\
    FdogSerializer::Instance()->Deserialize(object_2, value);\
};\
template<typename T, typename X>\
void Serialize_array_Temp(string & json_, string & json_s, T & object_, X & object_2, MetaInfo * metainfoObject) {\
    for(int i = 0; i < metainfoObject->memberArraySize; i++){\
        string json_z = "";\
        FdogSerializer::Instance()->Serialize(json_z, *(X *)((char *)&object_ + metainfoObject->memberOffset + (i * sizeof(X))));\
        json_ = json_ + "{" + json_z + "}" + ",";\
    }\
};\
template<typename T, typename X>\
void Deserialize_array_Temp(T & object_, X & object_2, string & value, MetaInfo * metainfoObject) {\
    vector<string> json_array;\
    int j = 0;\
    for(int i = 0; i < metainfoObject->memberArraySize; i++){\
        FdogSerializer::Instance()->Deserialize(*(X *)((char *)&object_ + metainfoObject->memberOffset + (i * sizeof(X))), json_array[j++]);\
    }\
};\
template<typename T, typename X>\
void Serialize_vector_Temp(string & json_, string & json_s,  T & object_, X & object_2, MetaInfo * metainfoObject) {\
    FdogSerializer::Instance()->FSerialize(json_s, *(X *)((char *)&object_ + metainfoObject->memberOffset), typename TagDispatchTrait<X>::Tag{});\
	json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]" + ",";\
};\
template<typename T, typename X>\
void Deserialize_vector_Temp(T & object_, X & object_2, string & value, MetaInfo * metainfoObject) {\
    FdogSerializer::Instance()->FDeserialize(*(X *)((char *)&object_ + metainfoObject->memberOffset), value, typename TagDispatchTrait<X>::Tag{});\
};\
template<typename T, typename X>\
void Serialize_list_Temp(string & json_, T & object_, X & object_2, MetaInfo * metainfoObject) {\
    FdogSerializer::Instance()->FSerialize(json_, *(X *)((char *)&object_ + metainfoObject->memberOffset), typename TagDispatchTrait<X>::Tag{});\
};\
template<typename T, typename X>\
void Deserialize_list_Temp(T & object_, X & object_2, string & value, MetaInfo * metainfoObject) {\
    FdogSerializer::Instance()->FDeserialize(*(X *)((char *)&object_ + metainfoObject->memberOffset), value, typename TagDispatchTrait<X>::Tag{});\
};\
template<typename T, typename X>\
void Serialize_set_Temp(string & json_, T & object_, X & object_2, MetaInfo * metainfoObject) {\
    FdogSerializer::Instance()->FSerialize(json_, *(X *)((char *)&object_ + metainfoObject->memberOffset), typename TagDispatchTrait<X>::Tag{});\
};\
template<typename T, typename X>\
void Deserialize_set_Temp( T & object_, X & object_2, string & value, MetaInfo * metainfoObject) {\
    FdogSerializer::Instance()->FDeserialize(*(X *)((char *)&object_ + metainfoObject->memberOffset), value, typename TagDispatchTrait<X>::Tag{});\
};\
template<typename T, typename X>\
void Serialize_deque_Temp(string & json_, T & object_, X & object_2, MetaInfo * metainfoObject) {\
    FdogSerializer::Instance()->FSerialize(json_, *(X *)((char *)&object_ + metainfoObject->memberOffset), typename TagDispatchTrait<X>::Tag{});\
};\
template<typename T, typename X>\
void Deserialize_deque_Temp(T & object_, X & object_2, string & value, MetaInfo * metainfoObject) {\
    FdogSerializer::Instance()->FDeserialize(*(X *)((char *)&object_ + metainfoObject->memberOffset), value, typename TagDispatchTrait<X>::Tag{});\
};\
template<typename T, typename X>\
void Serialize_map_Temp(string & json_, string & json_s, T & object_, X & object_2, MetaInfo * metainfoObject) {\
    FdogSerializer::Instance()->FSerialize(json_s, *(X *)((char *)&object_ + metainfoObject->memberOffset), typename TagDispatchTrait<X>::Tag{});\
	json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "{" + json_s + "}" + ",";\
};\
template<typename T, typename X>\
void Deserialize_map_Temp(T & object_, X & object_2, string & value, MetaInfo * metainfoObject) {\
    FdogSerializer::Instance()->FDeserialize(*(X *)((char *)&object_ + metainfoObject->memberOffset), value, typename TagDispatchTrait<X>::Tag{});\
};\
template<typename T, typename X>\
void Serialize_unordered_map_Temp(string & json_, T & object_, X & object_2, MetaInfo * metainfoObject) {\
    FdogSerializer::Instance()->FSerialize(json_, *(X *)((char *)&object_ + metainfoObject->memberOffset), typename TagDispatchTrait<X>::Tag{});\
};\
template<typename T, typename X>\
void Deserialize_unordered_map_Temp(T & object_, X & object_2, string & value, MetaInfo * metainfoObject) {\
    FdogSerializer::Instance()->FDeserialize(*(X *)((char *)&object_ + metainfoObject->memberOffset), value, typename TagDispatchTrait<X>::Tag{});\
};\
template<typename T>\
void Serialize_struct_s(string & json_, string & json_s, T & object_, MetaInfo * metainfoObject){\
    EXPAND(SERIALIZE_FUCK_MSVC(__VA_ARGS__));\
};\
template<typename T>\
void Deserialize_struct_s(T & object_, string & value, MetaInfo * metainfoObject){\
    EXPAND(DESERIALIZE_FUCK_MSVC(__VA_ARGS__));\
};\
static int InitFdogSerialize() {\
    EXPAND(REGISTERED_FUCK_MSVC(__VA_ARGS__));\
    return 1;\
};

#define REGISTEREDMEMBER_s_1(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_2(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));


#ifdef __GNUC__
#define REGISTEREDMEMBER_s_1_MSVC(...) EXPAND(REGISTEREDMEMBER_s_1(__VA_ARGS__))
#elif _MSC_VER
#define REGISTEREDMEMBER_s_1_MSVC(...) EXPAND(REGISTEREDMEMBER_s_1(##__VA_ARGS__))
#endif

#define REGISTEREDMEMBER_s_2(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_3(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_3(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_4(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_4(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_5(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_5(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_6(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_6(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_7(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_7(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_8(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_8(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_9(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_9(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_10(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_10(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_11(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_11(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_12(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_12(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_13(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_13(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_14(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_14(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_15(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_15(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_16(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_16(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_17(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_17(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_18(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_18(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_19(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_19(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_20(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_20(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1);

#ifdef __GNUC__
#define MEMBERTYPE_MSVC(...) EXPAND(MEMBERTYPE(__VA_ARGS__));
#define offsetof_MSVC(...) EXPAND(offsetof(__VA_ARGS__));
#elif _MSC_VER
#define MEMBERTYPE_MSVC(...) EXPAND(MEMBERTYPE(##__VA_ARGS__));
#define offsetof_MSVC(...) EXPAND(offsetof(##__VA_ARGS__));
#endif

#define REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg) \
    do{\
        MetaInfo * metainfo_one = new MetaInfo();\
        metainfo_one->memberName = NAME(arg);\
        metainfo_one->memberAliasName = "";\
        metainfo_one->memberOffset = offsetof_MSVC(TYPE, arg);\
        memberAttribute resReturn = MEMBERTYPE_MSVC(TYPE, arg);\
        metainfo_one->memberType = resReturn.valueType;\
        metainfo_one->memberTypeSize = sizeof(TYPE);\
        metainfo_one->memberArraySize = resReturn.ArraySize;\
        metainfo_one->memberTypeInt = resReturn.valueTypeInt;\
        metainfo_one->first = resReturn.first;\
        metainfo_one->second = resReturn.second;\
        metainfo_one->memberIsIgnore = false;\
        metainfo_one->memberIsIgnoreLU = false;\
        metaInfoObjectList.push_back(metainfo_one);\
    }while(0);

#define REGISTERE_CONTAINER_V(object)\
template<> struct TagDispatchTrait<vector<object>> {using Tag = ArrayTag;};\
template<> struct TagSTLType<vector<object>> {using Tag = InitStructTag;};\
\
template<> struct TagDispatchTrait<set<object>> {using Tag = ArrayTag;};\
template<> struct TagSTLType<set<object>> {using Tag = InitStructTag;};\
\
template<> struct TagDispatchTrait<deque<object>> {using Tag = ArrayTag;};\
template<> struct TagSTLType<deque<object>> {using Tag = InitStructTag;};\
\
template<> struct TagDispatchTrait<list<object>> {using Tag = ArrayTag;};\
template<> struct TagSTLType<list<object>> {using Tag = InitStructTag;};\

#define REGISTERE_CONTAINER_M(object, object2)\
template<> struct TagDispatchTrait<map<object, object2>> {using Tag = MapTag;};\
template<> struct TagSTLType<map<object, object2>> {using Tag = InitMapBaseTag;};\
\
template<> struct TagDispatchTrait<unordered_map<object, object2>> {using Tag = MapTag;};\
template<> struct TagSTLType<unordered_map<object, object2>> {using Tag = InitMapBaseTag;};\

#define F_CLASS(x) \
class x;\
template<> struct TagDispatchTrait_zx<x> {using Tag = StructTag_zx;};\
template<> struct TagDispatchTrait<x> {using Tag = BaseTag;};\
class x\

#endif