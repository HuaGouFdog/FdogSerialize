/*
����Ŀǩ����Apache-2.0 License��������μ�LICENSE
���� Apache ��ɣ��汾 2.0������ɡ���������
����������ɣ�����������ʹ�ô��ļ���

Copyright 2021-2022 ����Fdog(����)
*/

#ifndef FSTRUCT_H
#define FSTRUCT_H

#include "definition.h"
#include <algorithm>
#ifdef __GNUC__
#include <cxxabi.h>
#endif
#include <cstring>
#include <ctime>
#include <deque>
#include <mutex>
#include <map>
#include <map>
#include <list>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <regex>
#include <codecvt>
#include <Windows.h>
using namespace std;

//������������������ȡ
const string arrayRegex = "(\\[(.*?)\\])";
//����Map����������ȡ
const string mapRegex = "(\\{(.*?)\\})";
//ƥ������
const string patternArray = "((A)(\\d+)_\\d?(\\D+))";
//ƥ�����
const string patterAlias = "(\\(A:(.*?)\\))";
//���ڽṹ��������ȡ
const string objectRegex = "(\\{(.*?)\\})";
//ƥ��ṹ��
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
*   �洢�ṹ��Ԫ��Ϣ
************************************/
typedef struct MetaInfo {
	string memberName;              //��Ա��
	string memberAliasName;         //��Ա����
	string memberType;              //��Ա����
	size_t memberOffset;            //ƫ��ֵ
	size_t memberTypeSize;          //���ʹ�С
	size_t memberArraySize;         //������������飬��ʾ�����С
	int    memberTypeInt;           //��Ա���� ��ֵ��
	bool   isPointer;               //�Ƿ���ָ��
	string first;                   //�����map���� first��ʾkey�����ͣ�������������ͣ���ʾvalue����
	string second;                  //�����map���ͣ�second��ʾvalue����
	bool   memberIsIgnore = false;    //�Ƿ�����ֶ�
	bool   memberIsIgnoreLU = false;  //�Ƿ���Դ�Сд                                                                                                                
}MetaInfo;

/***********************************
*   �洢����Ԫ��Ϣ
************************************/
typedef struct ObjectInfo {
	string objectType;                      //�ṹ������ �ַ�����ʾ
	int objectTypeInt;                      //�ṹ������ ��ֵ��ʾ
	int objectSize;                         //�ṹ���С
	vector<MetaInfo *> metaInfoObjectList;  //�ṹ��Ԫ��Ϣ 
}ObjectInfo;


typedef struct FdogMap {
	string first;
	string second;
};

/***********************************
*   �洢��Ա���ͣ������С
************************************/
struct memberAttribute {
	string valueType;
	int valueTypeInt;   //���� ��ֵ��ʾ
	string first;       //�����map���� first��ʾkey�����ͣ�������������ͣ���ʾvalue����
	string second;      //�����map���ͣ�second��ʾvalue����
	int ArraySize;
};

//�ṹ�����ڷ�����Ϣ
struct result {
	int code;           //1.��ȷ 0.����
	string message;     //������󣬷��ش�����ʾ
};

//�������л�base��
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
		if (valueType == "char*") {
			auto value = *((const char **)((char *)&object + offsetValue));
			string str_value = value;
			return "\"" + str_value + "\"";
		}
		if (valueType == "string") {
			auto value = *((string *)((char *)&object + offsetValue));
			string str_value = value;
			return "\"" + str_value + "\"";
		}
		if (valueType == "wstring") {
			auto value = *((wstring *)((char *)&object + offsetValue));
#ifdef __GNUC__
			wcout << "value = " << value << endl;
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			string str_value = converter.to_bytes(value);
#elif _MSC_VER
			string str_value = FdogSerializer::Instance()->wstring2string(value);
#endif
			return "\"" + str_value + "\"";
		}
		if (valueType == "bool") {
			auto value = *((bool *)((char *)&object + offsetValue));
			if (value) {
				return "true";
			}
			else {
				return "false";
			}
		}
		if (valueType == "char") {
			auto value = *((char *)((char *)&object + offsetValue));
			return to_string((int)value);
		}
		if (valueType == "unsigned char") {
			auto value = *((char *)((char *)&object + offsetValue));
			return to_string((unsigned int)value);
		}
		if (valueType == "int") {
			//cout << "get = " << (int *)((char *)&object + offsetValue) << endl;
			auto value = *((int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "unsigned int") {
			auto value = *((unsigned int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "short") {
			auto value = *((short int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "unsigned short") {
			auto value = *((unsigned short int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "long") {
			auto value = *((long int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "unsigned long") {
			auto value = *((unsigned long int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "long long") {
			auto value = *((long long int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "unsigned long long") {
			auto value = *((unsigned long long int *)((char *)&object + offsetValue));
			return to_string(value);
		}
		if (valueType == "float") {
			auto value = *((float *)((char *)&object + offsetValue));
			return removeLastZero(value);
		}
		if (valueType == "double") {
			auto value = *((double *)((char *)&object + offsetValue));
			return removeLastZero(value);
		}
		if (valueType == "long double") {
			auto value = *((long double *)((char *)&object + offsetValue));
			return removeLastZero(value);
		}
		return "";
	}

	template<class T>
	void setValueByAddress(string valueType, T &object, int offsetValue, string value) {
		//cout << "setValueByAddress offsetValue = " << offsetValue << endl;
		if (valueType == "char*") {
			*((char **)((char *)&object + offsetValue)) = new char[strlen(value.c_str())];
			strcpy(*((char **)((char *)&object + offsetValue)), value.c_str());
		}
		if (valueType == "string") {
			*((string *)((char *)&object + offsetValue)) = value;
		}
		if (valueType == "wstring") {
#ifdef __GNUC__
			cout << "��⵽wstring" << endl;
			std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
			*((wstring *)((char *)&object + offsetValue)) = strCnv.from_bytes(value);
#elif _MSC_VER
			cout << "��⵽wstring" << endl;
			*((wstring *)((char *)&object + offsetValue)) = FdogSerializer::Instance()->string2wstring(value);
#endif
		}
		std::stringstream ss;
		ss.str(value);
		if (valueType == "bool") {
			ss >> *((bool *)((char *)&object + offsetValue));
		}
		if (valueType == "char") {
			ss >> *((char *)((char *)&object + offsetValue));
		}
		if (valueType == "unsigned char") {
			ss >> *((unsigned char *)((char *)&object + offsetValue));
		}
		if (valueType == "int") {
			ss >> *((int *)((char *)&object + offsetValue));
		}
		if (valueType == "unsigned int") {
			ss >> *((unsigned int *)((char *)&object + offsetValue));
		}
		if (valueType == "short") {
			ss >> *((short int *)((char *)&object + offsetValue));
		}
		if (valueType == "unsigned short") {
			ss >> *((unsigned short int *)((char *)&object + offsetValue));
		}
		if (valueType == "long") {
			ss >> *((long int *)((char *)&object + offsetValue));
		}
		if (valueType == "unsigned long") {
			ss >> *((unsigned long int *)((char *)&object + offsetValue));
		}
		if (valueType == "long long") {
			ss >> *((long long int *)((char *)&object + offsetValue));
		}
		if (valueType == "unsigned long long") {
			ss >> *((unsigned long long  int *)((char *)&object + offsetValue));
		}
		if (valueType == "float") {
			ss >> *((float *)((char *)&object + offsetValue));
		}
		if (valueType == "double") {
			ss >> *((double *)((char *)&object + offsetValue));
		}
		if (valueType == "long double") {
			ss >> *((long double *)((char *)&object + offsetValue));
		}
	}

	// ��������תjson
	template<class T>
	void BaseToJson(string & json_, MetaInfo * metainfoobject, T & object_) {
		//cout << "BaseToJson metainfoobject->memberType = " << metainfoobject->memberType << "&metainfoobject->memberName = " << metainfoobject->memberName << endl;
		string value = getValueByAddress(metainfoobject->memberType, object_, metainfoobject->memberOffset);
		if (metainfoobject->memberAliasName != "") {
			json_ = json_ + "\"" + metainfoobject->memberAliasName + "\"" + ":" + value + ",";
		}
		else {
			json_ = json_ + "\"" + metainfoobject->memberName + "\"" + ":" + value + ",";
		}
	}

	// //��������תjson
	template<class T>
	void BaseToJsonA(string & json_, MetaInfo * metainfoobject, T & object_) {
		string value = getValueByAddress(metainfoobject->memberType, object_, metainfoobject->memberOffset);
		json_ = json_ + value + ",";
	}

	// //jsonת��������
	template<class T>
	void JsonToBase(T & object_, MetaInfo * metainfoobject, string json_) {
		setValueByAddress(metainfoobject->memberType, object_, metainfoobject->memberOffset, json_);
	}
};


//BaseTag ArrayTag MapTag ���ڴ�����Serialize��Deserialize�ϵķַ�
//���� base
struct BaseTag {};
//���� array vector list deque set
struct ArrayTag {};
//���� map  
struct MapTag {};
//���� unordered_map
//struct UnorderedMapTag {};

template<typename T> struct TagDispatchTrait {
	using Tag = BaseTag;
};

template<> struct TagDispatchTrait<vector<int>> {
	using Tag = ArrayTag;
};

template<> struct TagDispatchTrait<list<int>> {
	using Tag = ArrayTag;
};

template<> struct TagDispatchTrait<set<int>> {
	using Tag = ArrayTag;
};

template<> struct TagDispatchTrait<deque<int>> {
	using Tag = ArrayTag;
};

template<> struct TagDispatchTrait<map<int, int>> {
	using Tag = MapTag;
};

template<> struct TagDispatchTrait<unordered_map<int, int>> {
	using Tag = MapTag;
};

// ���ַ��ַ������ַ��� ������FSerialize(map) key��ֵ����������ֵ�������Ƿ���ֵ
struct NoStringTag {};
struct StringTag {};

template<typename T> struct TagString {
	using Tag = NoStringTag;
};

template<> struct TagString<char *> {
	using Tag = StringTag;
};

template<> struct TagString<string> {
	using Tag = StringTag;
};

template<typename T>
string F_toString_s(T object, NoStringTag) {
	return to_string(object);
}
template<typename T>
string F_toString_s(T object, StringTag) {
	return object;
}
template<typename T>
string F_toString(T object) {
	return F_toString_s(object, typename TagString<T>::Tag{});
}

// ���ڷ����л�ʱ����Ϊ�յ��������Ҫ�������ݽ�����չ���� Init  ��Ϊ��ֵ�ͷ���ֵ����
struct InitBaseTag {};

struct InitVectorTag {};
struct InitVectorStrTag {};

struct InitDequeTag {};
struct InitDequeStrTag {};

struct InitListTag {};
struct InitListStrTag {};

struct InitMapTag {};
struct InitMapStrTag {};

struct InitSetTag {};
struct InitSetStrTag {};


template<typename T> struct TagSTLType {
	using Tag = InitBaseTag;
};

template<> struct TagSTLType<vector<int>> {
	using Tag = InitVectorTag;
};

template<> struct TagSTLType<vector<char *>> {
	using Tag = InitVectorStrTag;
};

template<> struct TagSTLType<vector<string>> {
	using Tag = InitVectorStrTag;
};

template<> struct TagSTLType<list<int>> {
	using Tag = InitListTag;
};

template<> struct TagSTLType<list<char *>> {
	using Tag = InitListStrTag;
};

template<> struct TagSTLType<list<string>> {
	using Tag = InitListStrTag;
};

template<> struct TagSTLType<set<int>> {
	using Tag = InitSetTag;
};

template<> struct TagSTLType<set<char *>> {
	using Tag = InitSetStrTag;
};

template<> struct TagSTLType<set<string>> {
	using Tag = InitSetStrTag;
};

template<> struct TagSTLType<deque<int>> {
	using Tag = InitDequeTag;
};

template<> struct TagSTLType<deque<char *>> {
	using Tag = InitDequeStrTag;
};

template<> struct TagSTLType<deque<string>> {
	using Tag = InitDequeStrTag;
};

template<> struct TagSTLType<map<int, int>> {
	using Tag = InitMapTag;
};

template<> struct TagSTLType<map<string, int>> {
	using Tag = InitMapStrTag;
};

template<> struct TagSTLType<unordered_map<int, int>> {
	using Tag = InitMapTag;
};

template<> struct TagSTLType<unordered_map<string, int>> {
	using Tag = InitMapStrTag;
};

template<typename T>
void F_init_s(T & object, InitBaseTag, string first, string second = "", string key = "") {

}

template<typename T>
void F_init_s(T & object, InitVectorTag, string first, string second = "", string key = "") {
	//cout << " F_init_s object size = " << object.size() << endl;
	if (first == "char") {
		object.push_back('0');
	}
	else if (first == "unsigned char") {
		object.push_back(0);
	}
	else if (first == "short") {
		object.push_back(0);
	}
	else if (first == "unsigned short") {
		object.push_back(0);
	}
	else if (first == "int") {
		object.push_back(0);
	}
	else if (first == "unsigned int") {
		object.push_back(0);
	}
	else if (first == "long") {
		object.push_back(0);
	}
	else if (first == "unsigned long") {
		object.push_back(0);
	}
	else if (first == "long long") {
		object.push_back(0);
	}
	else if (first == "unsigned long long") {
		object.push_back(0);
	}
	else if (first == "float") {
		object.push_back(0.1f);
	}
	else if (first == "double") {
		object.push_back(0.1);
	}
	else if (first == "long double") {
		object.push_back(0.1);
	}
	else {

	}
}

template<typename T>
void F_init_s(T & object, InitVectorStrTag, string first, string second = "", string key = "") {
	if (first == "char*") {
		object.push_back("");
	}
	else if (first == "string") {
		object.push_back("");
	}
	else {

	}
}

template<typename T>
void F_init_s(T & object, InitDequeTag, string first, string second = "", string key = "") {
	if (first == "char") {
		object.push_back('0');
	}
	else if (first == "unsigned char") {
		object.push_back(0);
	}
	else if (first == "short") {
		object.push_back(0);
	}
	else if (first == "unsigned short") {
		object.push_back(0);
	}
	else if (first == "int") {
		object.push_back(0);
	}
	else if (first == "unsigned int") {
		object.push_back(0);
	}
	else if (first == "long") {
		object.push_back(0);
	}
	else if (first == "unsigned long") {
		object.push_back(0);
	}
	else if (first == "long long") {
		object.push_back(0);
	}
	else if (first == "unsigned long long") {
		object.push_back(0);
	}
	else if (first == "float") {
		object.push_back(0.1f);
	}
	else if (first == "double") {
		object.push_back(0.1);
	}
	else if (first == "long double") {
		object.push_back(0.1);
	}
	else {

	}
}

template<typename T>
void F_init_s(T & object, InitDequeStrTag, string first, string second = "", string key = "") {
	if (first == "char*") {
		object.push_back("");
	}
	else if (first == "string") {
		object.push_back("");
	}
	else {

	}
}

template<typename T>
void F_init_s(T & object, InitListTag, string first, string second = "", string key = "") {
	if (first == "int") {
		object.push_back(0);
	}
	if (first == "char") {
		object.push_back('0');
	}
	else if (first == "unsigned char") {
		object.push_back(0);
	}
	else if (first == "short") {
		object.push_back(0);
	}
	else if (first == "unsigned short") {
		object.push_back(0);
	}
	else if (first == "int") {
		object.push_back(0);
	}
	else if (first == "unsigned int") {
		object.push_back(0);
	}
	else if (first == "long") {
		object.push_back(0);
	}
	else if (first == "unsigned long") {
		object.push_back(0);
	}
	else if (first == "long long") {
		object.push_back(0);
	}
	else if (first == "unsigned long long") {
		object.push_back(0);
	}
	else if (first == "float") {
		object.push_back(0.1f);
	}
	else if (first == "double") {
		object.push_back(0.1);
	}
	else if (first == "long double") {
		object.push_back(0.1);
	}
	else {

	}
}

template<typename T>
void F_init_s(T & object, InitListStrTag, string first, string second = "", string key = "") {
	if (first == "char*") {
		object.push_back("");
	}
	else if (first == "string") {
		object.push_back("");
	}
	else {

	}
}

template<typename T>
void F_init_s(T & object, InitSetTag, string first, string second = "", string key = "") {
	//set�ǲ������ظ��ģ������������
	int a = rand() % 100;
	if (first == "int") {
		object.insert(a);
	}
	if (first == "char") {
		stringstream sstr;
		sstr << a;
		object.insert(sstr.str()[0]);
	}
	else if (first == "unsigned char") {
		object.insert(a);
	}
	else if (first == "short") {
		object.insert(a);
	}
	else if (first == "unsigned short") {
		object.insert(a);
	}
	else if (first == "int") {
		object.insert(a);
	}
	else if (first == "unsigned int") {
		object.insert(a);
	}
	else if (first == "long") {
		object.insert(a);
	}
	else if (first == "unsigned long") {
		object.insert(a);
	}
	else if (first == "long long") {
		object.insert(a);
	}
	else if (first == "unsigned long long") {
		object.insert(a);
	}
	else if (first == "float") {
		object.insert(static_cast<float>(a));
	}
	else if (first == "double") {
		object.insert(static_cast<double>(a));
	}
	else if (first == "long double") {
		object.insert(static_cast<long double>(a));
	}
	else {

	}
}

//�����char * ���ͣ�ÿ��delete֮�� ��ַ������ͬ�ģ����Բ������ڲ�ʹ������㣬Ҫ���������帳��ֵdelete
static vector<char *> temp;

template<typename T>
void F_init_s(T & object, InitSetStrTag, string first, string second = "", string key = "") {
	int a = rand() % 100;
	//cout << "a = " << a << endl;
	if (first == "char*") {
		stringstream sstr;
		sstr << a;
		char * cc = new char[4];
		//cout << "cc = " << (char *)cc << "---"<< *cc << "---" << &cc << " sstr ��ַ��" << &sstr << endl;
		strcpy(cc, (char *)sstr.str().data());
		//cout << "cc = " << (char *)cc << "---"<< *cc << "---" << &cc << " sstr ��ַ��" << &sstr << endl;
		object.insert(cc);
		temp.push_back(cc);
	}
	else if (first == "string") {
		stringstream sstr;
		sstr << a;
		object.insert((char *)sstr.str().c_str());
	}
	else {

	}
	//cout << "���ȣ�" << object.size() << endl;
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
		//string value = key;
		object.insert(make_pair(key, a));
	}
}

template<typename T>
void F_init(T & object, int stlType, string first, string second = "", string key = "") {

	if (stlType == OBJECT_VECTOR) {
		F_init_s(object, typename TagSTLType<T>::Tag{}, first);
	}
	if (stlType == OBJECT_LIST) {
		F_init_s(object, typename TagSTLType<T>::Tag{}, first);
	}
	if (stlType == OBJECT_DEQUE) {
		F_init_s(object, typename TagSTLType<T>::Tag{}, first);
	}
	if (stlType == OBJECT_SET) {
		F_init_s(object, typename TagSTLType<T>::Tag{}, first);
	}
	if (stlType == OBJECT_MAP) {
		F_init_s(object, typename TagSTLType<T>::Tag{}, first, second, key);
	}
	if (stlType == OBJECT_UNORDERED_MAP) {
		//cout << "%$$$$$$$$ ����" << endl;
		F_init_s(object, typename TagSTLType<T>::Tag{}, first, second, key);
	}
}

//��������(�������ͽṹ��/����)
struct BaseAndStructTag {};
struct BaseArrayTag {};

template<typename T> struct TagSTLAAAType {
	using Tag = BaseAndStructTag;
};

// template<> struct TagSTLAAAType<student[2]> {
//     using Tag = BaseArrayTag;
// };

template<> struct TagSTLAAAType<int[2]> {
	using Tag = BaseArrayTag;
};

template<> struct TagSTLAAAType<list<int>> {
	using Tag = BaseAndStructTag;
};

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
	//��ȡʵ��
	static FdogSerializer * Instance();

	//���objectinfo
	void addObjectInfo(ObjectInfo * objectinfo);

	//��ȡ��ӦInfo
	ObjectInfo & getObjectInfo(string objectName);

	//��ȡ����Info ��Ի������ͻ�ȡ
	MetaInfo * getMetaInfo(string TypeName);

	//��Բ�ͬƽ̨��ȡΨһ����ֵ
	std::string getTypeName(string TypeName);

	//���ñ���
	void __setAliasName(string Type, string memberName, string AliasName);
	//�Ƴ�����
	void __removeAliasName(string Type, string memberName);

	//�����Ƿ���Ը��ֶ����л�
	void __setIgnoreField(string Type, string memberName);
	//�Ƴ������ֶ�
	void __removeIgnoreField(string Type, string memberName);

	//�����Ƿ���Դ�Сд
	void __setIgnoreLU(string Type, string memberName);
	//�Ƴ����Դ�Сд
	void __removeIgnoreLU(string Type, string memberName);

	//���ý���ģ��ת�� �ṹ��תjson���������������Ҫ�����jsonת�ṹ������⣬������ڷ��磬���Գ��Խ���ģ��ת��
	void __setFuzzy(string Type);

	void __setAliasNameS(string Type) {};//�˳�

										 //һ�������ö������  �����Ӧ  ����  ԭ��Ա�� ���� һ��ʹ�ã�������������
	template<class T, class ...Args>
	void __setAliasNameS(T Type, T memberName, T AliasName, Args... args) {
		if (memberName == "" && AliasName == "") {
			//������
		}
		//ÿ�λ�ȡ����ֵ
		__setAliasName(Type, memberName, AliasName);
		if (sizeof...(args) == 0) {
			return;
		}
		else if ((sizeof...(args) % 2) != 0) {
			//������
		}
		__setAliasNameS(Type, args...);
	}

	void __setIgnoreFieldS(string Type) {};//�˳�

										   //һ�������ú��Զ���ֶ����л�
	template<class T, class ...Args>
	void __setIgnoreFieldS(T Type, T memberName, Args... args) {
		if (memberName == "") {
			//������
		}
		//ÿ�λ�ȡһ��ֵ
		__setIgnoreField(Type, memberName);
		if (sizeof...(args) == 0) {
			return;
		}
		__setIgnoreFieldS(Type, args...);
	}

	void __setIgnoreLUS(string Type) {}//�˳�

									   //һ�������ú��Զ���ֶεĴ�Сд
	template<class T, class ...Args>
	void __setIgnoreLUS(T Type, T memberName, Args... args) {
		if (memberName == "") {
			//������
		}
		//ÿ�λ�ȡһ��ֵ
		__setIgnoreLU(Type, memberName);
		if (sizeof...(args) == 0) {
			return;
		}
		__setIgnoreLUS(Type, args...);
	}

	//�Ƴ���һ������
	void removeFirstComma(string & return_);
	//�Ƴ����һ������
	void removeLastComma(string & return_);
	//�Ƴ�����
	void removeNumbers(string & return_);

	//��ȡkeyֵ
	string getKey(string json);

	//��ȡ��Ա����
	memberAttribute getMemberAttribute(string key);

	//��ȡobject����
	int getObjectTypeInt(string objectName, string typeName);

	//��ȡ�������� ֻ��base��struct����
	ObjectInfo getObjectInfoByType(string typeName, int objectTypeInt);

	wstring string2wstring(string str);

	string wstring2string(wstring wstr);

	//ͨ���궨����ص���Ϣ��ȡ
	int getObjectTypeByObjectInfo(string objectName);

	//�ж��Ƿ��ǻ�������
	bool isBaseType(string typeName);

	//��map�Ļ������ж��Ƿ��ǻ�������
	bool isBaseTypeByMap(string typeName);

	//�ж��Ƿ�Ϊvector����
	bool isVectorType(string objectName, string typeName);

	//��ȡvector�е�����
	string getTypeOfVector(string objectName, string typeName);

	//�ж��Ƿ�Ϊmap����
	bool isMapType(string objectName, string typeName);

	//�ж��Ƿ�Ϊunordered_map����
	bool isUnorderedMapType(string objectName, string typeName);

	//��ȡmap�е�key��value����
	FdogMap getTypeOfMap(string objectName, string typeName);

	//�ж��Ƿ���list����
	bool isListType(string objectName, string typeName);

	//��ȡlist�е�����
	string getTypeOfList(string objectName, string typeName);

	//�ж��Ƿ���deque����
	bool isDequeType(string objectName, string typeName);

	//�ж��Ƿ���set����
	bool isSetType(string objectName, string typeName);

	//�ж��Ƿ��ǽṹ������
	bool isStructType(string objectName, string typeName);

	//�ж��Ƿ�������
	bool isArrayType(string objectName, string typeName);

	//��������
	vector<string> CuttingArray(string data);

	//�и�
	vector<string> split(string str, string pattern);

	//�ж�json��ʽ�Ƿ���ȷ
	//vector<string> CuttingJson(string json_);
	result CuttingJson(vector<string> & array_json, string json_);
	//�жϷ������Ƿ�ƥ��
	result IsSquareBracket(string json_);

	//�жϻ������Ƿ�ƥ��
	result IsCurlyBraces(string json_);

	//�ж��ܷ������Ƿ�ƥ��
	result isMatch(string json_);

	//�ж�json��ȷ��
	result __JsonValidS(string json_);

	//�ж��ֶ��Ƿ����
	bool __Exist(string json_, string key);

	//��ȡ�ֶε�ֵ
	string __GetStringValue(string json_, string key);

	//��ȡ�ֶε�ֵ
	int __GetIntValue(string json_, string key);

	//��ȡ�ֶε�ֵ
	double __GetDoubleValue(string json_, string key);

	//��ȡ�ֶε�ֵ
	long __GetLongValue(string json_, string key);

	//��ȡ�ֶε�ֵ
	bool __GetBoolValue(string json_, string key);

	//���л�
	template<typename T>
	void Serialize(string & json_, T & object_, string name = "") {
		//cout << "Serialize =" << abi::__cxa_demangle(typeid(T).name(),0,0,0) << endl;
		//ͨ����������T�ж���ʲô�������ͣ�ObjectInfoֻ����ṹ��,�����NULL����ȷ���������Ĳ���struct����
		ObjectInfo objectinfo = FdogSerializer::Instance()->getObjectInfo(FdogSerializer::Instance()->getTypeName(typeid(T).name()));
		//��ȡ��ֻ���ǽṹ�����Ϣ���޷�֪����ʲô�������ͣ����Խ������� objectType��ʵ��һ���ṹ����������
		int objectType = getObjectTypeInt(objectinfo.objectType, FdogSerializer::Instance()->getTypeName(typeid(T).name()));
		if (objectinfo.objectType == "NULL" && objectType != OBJECT_BASE && objectType != OBJECT_STRUCT) {
			//˵������struct���ͺ�base���ͳ��ԣ����Խ�������
			objectinfo = getObjectInfoByType(FdogSerializer::Instance()->getTypeName(typeid(T).name()), objectType);
			objectType = getObjectTypeInt(objectinfo.objectType, FdogSerializer::Instance()->getTypeName(typeid(T).name()));
			//�������objectinfoӦ�û��ǿ� ������objecttype����ֵ�ж�
		}
		int sum = objectinfo.metaInfoObjectList.size();
		//cout << "sum��" << sum << endl;
		int i = 1;
		//��ȡ����objectType�������������ͣ�����������ͽ��в���
		//cout << "objectType type = " << objectType  << " json_ : " << json_ << endl;
		switch (objectType) {
			//��һ�ε��ý�����ʾ�䱾������ ֻ������ �ṹ����Ż�������
		case OBJECT_BASE:
		{
			MetaInfo * metainfo1 = nullptr;
			metainfo1 = getMetaInfo(FdogSerializer::Instance()->getTypeName(typeid(object_).name()));
			if (metainfo1 != nullptr) {
				//cout << "==================" << endl;
				FdogSerializerBase::Instance()->BaseToJsonA(json_, metainfo1, object_);
			}
			else {
				//cout << "��ȡMetaInfoʧ��" << endl;
			}
		}
		break;
		case OBJECT_STRUCT:
		{
			for (auto metainfoObject : objectinfo.metaInfoObjectList) {
				string json_s;
				//cout <<"��Ա���ͣ�" << metainfoObject->memberType << " -- " << metainfoObject->memberTypeInt << " -- " << metainfoObject->first <<":" << metainfoObject->second << " -- " << metainfoObject->memberOffset << endl;
				if (metainfoObject->memberTypeInt == OBJECT_BASE && metainfoObject->memberIsIgnore != true) {
					FdogSerializerBase::Instance()->BaseToJson(json_s, metainfoObject, object_);
					json_ = json_ + json_s;
				}
				if (metainfoObject->memberTypeInt == OBJECT_ARRAY && metainfoObject->memberIsIgnore != true) {
					if (metainfoObject->first == "bool") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(bool)));
							json_s = json_s + value + ",";
						}
					}
					if (metainfoObject->first == "char") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(char)));
							json_s = json_s + value + ",";
						}
					}
					if (metainfoObject->first == "unsigned char") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned char)));
							json_s = json_s + value + ",";
						}
					}
					if (metainfoObject->first == "char*") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(char*)));
							json_s = json_s + value + ",";
						}
					}
					if (metainfoObject->first == "string") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(string)));
							json_s = json_s + value + ",";
						}
					}
					if (metainfoObject->first == "short") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(short)));
							json_s = json_s + value + ",";
						}
					}
					if (metainfoObject->first == "unsigned short") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned short)));
							json_s = json_s + value + ",";
						}
					}
					if (metainfoObject->first == "int") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(int)));
							json_s = json_s + value + ",";
						}
					}
					if (metainfoObject->first == "unsigned int") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned int)));
							json_s = json_s + value + ",";
						}
					}
					if (metainfoObject->first == "long") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(long)));
							json_s = json_s + value + ",";
						}
					}
					if (metainfoObject->first == "unsigned long") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned long)));
							json_s = json_s + value + ",";
						}
					}
					if (metainfoObject->first == "long long") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(long long)));
							json_s = json_s + value + ",";
						}
					}
					if (metainfoObject->first == "unsigned long long") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned long long)));
							json_s = json_s + value + ",";
						}
					}
					if (metainfoObject->first == "float") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(float)));
							json_s = json_s + value + ",";
						}
					}
					if (metainfoObject->first == "double") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(double)));
							json_s = json_s + value + ",";
						}
					}
					if (metainfoObject->first == "long double") {
						for (int i = 0; i < metainfoObject->memberArraySize; i++) {
							string value = FdogSerializerBase::Instance()->getValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(long double)));
							json_s = json_s + value + ",";
						}
					}
					//��������Զ��������
					Serialize_arraytype_judgment_all;

					removeLastComma(json_s);
					json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]" + ",";
				}
				if (metainfoObject->memberTypeInt == OBJECT_VECTOR && metainfoObject->memberIsIgnore != true) {
					//cout << "====��ȡ��ֵ��" << metainfoObject->first << endl;
					if (metainfoObject->first == "char") {
						FSerialize(json_s, *(vector<char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned char") {
						FSerialize(json_s, *(vector<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
					}
					if (metainfoObject->first == "char*") {
						//cout << "zhaodaoleix1" << endl;
						FSerialize(json_s, *(vector<char *> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
					}
					if (metainfoObject->first == "string") {
						//cout << "zhaodaoleix1" << endl;
						FSerialize(json_s, *(vector<string> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
					}
					if (metainfoObject->first == "short") {
						FSerialize(json_s, *(vector<short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned short") {
						FSerialize(json_s, *(vector<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
					}
					if (metainfoObject->first == "int") {
						FSerialize(json_s, *(vector<int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned int") {
						FSerialize(json_s, *(vector<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
					}
					if (metainfoObject->first == "long") {
						FSerialize(json_s, *(vector<long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned long") {
						FSerialize(json_s, *(vector<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
					}
					if (metainfoObject->first == "long long") {
						FSerialize(json_s, *(vector<long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned long long") {
						FSerialize(json_s, *(vector<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
					}
					if (metainfoObject->first == "float") {
						FSerialize(json_s, *(vector<float> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
					}
					if (metainfoObject->first == "double") {
						FSerialize(json_s, *(vector<double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
					}
					if (metainfoObject->first == "long double") {
						FSerialize(json_s, *(vector<long double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
					}
					Serialize_vector_type_judgment_all;
					json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]" + ",";
				}
				if (metainfoObject->memberTypeInt == OBJECT_LIST && metainfoObject->memberIsIgnore != true) {
					//cout << "====��ȡ��ֵ��" << metainfoObject->first << endl;
					if (metainfoObject->first == "bool") {
						FSerialize(json_s, *(list<bool> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<bool>>::Tag{});
					}
					if (metainfoObject->first == "char") {
						FSerialize(json_s, *(list<char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned char") {
						FSerialize(json_s, *(list<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
					}
					if (metainfoObject->first == "char*") {
						FSerialize(json_s, *(list<char *> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
					}
					if (metainfoObject->first == "string") {
						FSerialize(json_s, *(list<string> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
					}
					if (metainfoObject->first == "short") {
						FSerialize(json_s, *(list<short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned short") {
						FSerialize(json_s, *(list<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
					}
					if (metainfoObject->first == "int") {
						FSerialize(json_s, *(list<int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned int") {
						FSerialize(json_s, *(list<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
					}
					if (metainfoObject->first == "long") {
						FSerialize(json_s, *(list<long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned long") {
						FSerialize(json_s, *(list<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
					}
					if (metainfoObject->first == "long long") {
						FSerialize(json_s, *(list<long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned long long") {
						FSerialize(json_s, *(list<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
					}
					if (metainfoObject->first == "float") {
						FSerialize(json_s, *(list<float> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
					}
					if (metainfoObject->first == "double") {
						FSerialize(json_s, *(list<double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
					}
					if (metainfoObject->first == "long double") {
						FSerialize(json_s, *(list<long double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
					}
					Serialize_list_type_judgment_all;
					json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]" + ",";
				}
				if (metainfoObject->memberTypeInt == OBJECT_DEQUE && metainfoObject->memberIsIgnore != true) {
					if (metainfoObject->first == "bool") {
						FSerialize(json_s, *(deque<bool> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<bool>>::Tag{});
					}
					if (metainfoObject->first == "char") {
						FSerialize(json_s, *(deque<char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned char") {
						FSerialize(json_s, *(deque<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
					}
					if (metainfoObject->first == "char*") {
						FSerialize(json_s, *(deque<char *> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
					}
					if (metainfoObject->first == "string") {
						FSerialize(json_s, *(deque<string> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
					}
					if (metainfoObject->first == "short") {
						FSerialize(json_s, *(deque<short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned short") {
						FSerialize(json_s, *(deque<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
					}
					if (metainfoObject->first == "int") {
						FSerialize(json_s, *(deque<int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned int") {
						FSerialize(json_s, *(deque<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
					}
					if (metainfoObject->first == "long") {
						FSerialize(json_s, *(deque<long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned long") {
						FSerialize(json_s, *(deque<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
					}
					if (metainfoObject->first == "long long") {
						FSerialize(json_s, *(deque<long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned long long") {
						FSerialize(json_s, *(deque<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
					}
					if (metainfoObject->first == "float") {
						FSerialize(json_s, *(deque<float> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
					}
					if (metainfoObject->first == "double") {
						FSerialize(json_s, *(deque<double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
					}
					if (metainfoObject->first == "long double") {
						FSerialize(json_s, *(deque<long double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
					}
					Serialize_deque_type_judgment_all;
					json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]" + ",";
				}
				if (metainfoObject->memberTypeInt == OBJECT_SET && metainfoObject->memberIsIgnore != true) {
					if (metainfoObject->first == "bool") {
						FSerialize(json_s, *(set<bool> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<bool>>::Tag{});
					}
					if (metainfoObject->first == "char") {
						FSerialize(json_s, *(set<char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned char") {
						FSerialize(json_s, *(set<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
					}
					if (metainfoObject->first == "char*") {
						FSerialize(json_s, *(set<char *> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
					}
					if (metainfoObject->first == "string") {
						FSerialize(json_s, *(set<string> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
					}
					if (metainfoObject->first == "short") {
						FSerialize(json_s, *(set<short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned short") {
						FSerialize(json_s, *(set<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
					}
					if (metainfoObject->first == "int") {
						FSerialize(json_s, *(set<int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned int") {
						FSerialize(json_s, *(set<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
					}
					if (metainfoObject->first == "long") {
						FSerialize(json_s, *(set<long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned long") {
						FSerialize(json_s, *(set<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
					}
					if (metainfoObject->first == "long long") {
						FSerialize(json_s, *(set<long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
					}
					if (metainfoObject->first == "unsigned long long") {
						FSerialize(json_s, *(set<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
					}
					if (metainfoObject->first == "float") {
						FSerialize(json_s, *(set<float> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
					}
					if (metainfoObject->first == "double") {
						FSerialize(json_s, *(set<double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
					}
					if (metainfoObject->first == "long double") {
						FSerialize(json_s, *(set<long double> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
					}
					Serialize_set_type_judgment_all;
					json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]" + ",";
				}
				if (metainfoObject->memberTypeInt == OBJECT_MAP && metainfoObject->memberIsIgnore != true) {
					if (metainfoObject->first == "char*" && metainfoObject->second == "int") {
						FSerialize(json_s, *(map<char *, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<map<int, int>>::Tag{});
					}
					if (metainfoObject->first == "string" && metainfoObject->second == "int") {
						FSerialize(json_s, *(map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<map<int, int>>::Tag{});
					}
					if (metainfoObject->first == "string" && metainfoObject->second == "float") {
						FSerialize(json_s, *(map<string, float> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<map<int, int>>::Tag{});
					}
					if (metainfoObject->first == "string" && metainfoObject->second == "bool") {
						FSerialize(json_s, *(map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<map<int, int>>::Tag{});
					}
					if (metainfoObject->first == "int" && metainfoObject->second == "int") {
						FSerialize(json_s, *(map<int, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<map<int, int>>::Tag{});
					}
					Serialize_map_type_judgment_all;
					json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "{" + json_s + "}" + ",";
				}
				if (metainfoObject->memberTypeInt == OBJECT_UNORDERED_MAP && metainfoObject->memberIsIgnore != true) {
					//cout << "��OBJECT_UNORDERED_MAP" << endl;
					if (metainfoObject->first == "char*" && metainfoObject->second == "int") {
						FSerialize(json_s, *(unordered_map<char *, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<unordered_map<int, int>>::Tag{});
					}
					if (metainfoObject->first == "string" && metainfoObject->second == "int") {
						FSerialize(json_s, *(unordered_map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<unordered_map<int, int>>::Tag{});
					}
					if (metainfoObject->first == "string" && metainfoObject->second == "float") {
						FSerialize(json_s, *(unordered_map<string, float> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<unordered_map<int, int>>::Tag{});
					}
					if (metainfoObject->first == "string" && metainfoObject->second == "bool") {
						FSerialize(json_s, *(unordered_map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<unordered_map<int, int>>::Tag{});
					}
					if (metainfoObject->first == "int" && metainfoObject->second == "int") {
						FSerialize(json_s, *(unordered_map<int, int> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<unordered_map<int, int>>::Tag{});
					}
					Serialize_map_type_judgment_all;
					json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "{" + json_s + "}" + ",";
				}
				//����Ҫ��������Զ��������
				//��������ڽ���OBJECT_STRUCT
				Serialize_type_judgment_all;
				if (i == sum) {
					if (json_.length() > 0) {
						removeLastComma(json_);
					}
				}
				json_s = "";
				i++;
			}
		}
		break;
		//�������ӻ���STL����
		//��ͨmap��Ҫ�����ﶨ��
		//����ֱ����STL����
		}
	}

	template<typename T>
	void SerializeS(string & json_, T & object_, BaseAndStructTag, string name = "") {
		//cout << "SerializeS1" << endl;
		Serialize(json_, object_, name);
	}

	template<typename T>
	void SerializeS(string & json_, T & object_, BaseArrayTag, string name = "") {
		//cout << "SerializeS2" << endl;
		for (auto & object_one : object_) {
			Serialize(json_, object_one);
		}
	}

	template<typename T>
	void SerializeS_s(string & json_, T & object_, bool isArray, string name = "") {
		// if(isArray){
		//     SerializeS(json_, object_, TagSTLAAAType<int[2]>::Tag{}, name);
		// }else{
		// SerializeS(json_, object_, TagSTLAAAType<list<int>>::Tag{}, name);
		// }
		SerializeS(json_, object_, typename TagSTLAAAType<T>::Tag{}, name);
	}


	//���ڽ����������ͣ�����(ֻ��Ҫ�ж���û��[]����ȷ���ǲ������飬�ṹ��ͻ������Ͷ����߱�[]����)���ṹ��
	template<typename T>
	void FSerialize(string & json_, T & object_, BaseTag, string name = "") {
		bool isArray = isArrayType("", FdogSerializer::Instance()->getTypeName(typeid(T).name()));
		//cout << "�Ƿ������� �� " << isArray << endl;
		SerializeS_s(json_, object_, isArray, name);
		//Serialize(json_, object_, name);
		//������Ҫ�ж����� ����ǻ�������ֱ��ʹ��name ���ǻ������ͣ�����ʹ��
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
	//���ڽ���STL��map���⣩��ʵ����ӿ�Ҳ���Դ���vector�������������޷����������������STL
	template<typename T>
	void FSerialize(string & json_, T & object_, ArrayTag, string name = "") {
		//cout << "����array==========1" << typeid(T).name() << endl;
		for (auto & object_one : object_) {
			//ji
			int objectType = isBaseType(FdogSerializer::Instance()->getTypeName(typeid(object_one).name()));
			if (!objectType) {
				json_ = json_ + "{";
			}
			//cout << "1=====" << endl;
			Serialize(json_, object_one);
			//cout << "����array==========2" << typeid(object_one).name() << endl;
			if (!objectType) {
				json_ = json_ + "},";
			}
		}
		if (json_.length() > 0) {
			removeLastComma(json_);
		}
		//json_ = "{\"" + name + "\":[" + json_ + "]}";
		//���ת������ֱ�Ӿ������飬�����ٶ����ṩһ��������˵����
	}
	//���ڽ���map
	template<typename T>
	void FSerialize(string & json_, T & object_, MapTag, string name = "") {
		int i = 0;
		int len = object_.size();
		for (auto & object_one : object_) {
			//�����������ǽṹ�壬��Ҫ�����ţ��������Ͳ���Ҫ
			json_ = json_ + "\"" + F_toString(object_one.first) + "\"" + ":";
			Serialize(json_, object_one.second);
			removeLastComma(json_);
			json_ = json_ + ",";
			i++;
		}
		removeLastComma(json_);
		//json_ = "{" + json_ + "}";
	}

	//���ڽ���unordered_map
	// template<typename T>
	// void FSerialize(string & json_, T & object_, UnorderedMapTag, string name = ""){
	//     int i = 0;
	//     int len = object_.size();
	//     for(auto & object_one : object_){
	//         //�����������ǽṹ�壬��Ҫ�����ţ��������Ͳ���Ҫ
	//         json_ = json_ + "\"" + F_toString(object_one.first) + "\"" + ":";
	//         Serialize(json_, object_one.second);
	//         removeLastComma(json_);
	//         json_ = json_ + ",";
	//         i++;
	//     }
	//     removeLastComma(json_);
	//     //json_ = "{" + json_ + "}";
	// }

	//�����л�
	template<typename T>
	void Deserialize(T & object_, string & json_, string name = "") {
		//cout << "Deserialize json_ = " << json_ << endl; 
		ObjectInfo & objectinfo = FdogSerializer::Instance()->getObjectInfo(FdogSerializer::Instance()->getTypeName(typeid(T).name()));
		int objectType = getObjectTypeInt(objectinfo.objectType, FdogSerializer::Instance()->getTypeName(typeid(T).name()));
		if (objectinfo.objectType == "NULL" && objectType != OBJECT_BASE && objectType != OBJECT_STRUCT) {
			//˵������struct���ͺ�base���ͳ��ԣ����Խ�������
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
			}
			else {
				pattern = new regex(regex_key + ":" + regex_value);
				// if (metainfoObject->memberIsIgnoreLU == false){
				//     pattern = new regex(regex_key + ":" +regex_value);
				// }else{
				//     pattern = new regex(regex_key + ":" +regex_value,regex::icase);//icase���ں��Դ�Сд
				// }
			}
			//cout << "       �����л���ȡ��regex_value��" << regex_value << "  memberType = " << metainfo1->memberType << endl;
			if (regex_search(json_, result, *pattern)) {
				string value = result.str(2).c_str();
				if (value == "") {
					value = result.str(1).c_str();
				}
				//cout << "@@@@@@@@@@@@@�����л�value = " << value << endl;
				FdogSerializerBase::Instance()->JsonToBase(object_, metainfo1, value);
			}
		}

		if (OBJECT_STRUCT == objectType) {
			for (auto metainfoObject : objectinfo.metaInfoObjectList) {
				//ͨ��������ʽ��ȡ��Ӧ��json
				smatch result;
				string regex_key = "(\"" + metainfoObject->memberName + "\")";
				string regex_value = baseRegex[metainfoObject->memberType];
				cout << "       �����л���ȡ��regex_value��" << regex_value << "  memberType = " << metainfoObject->memberType << endl;
				if (regex_value == "") {
					if (metainfoObject->memberTypeInt == OBJECT_STRUCT) {
						//cout << "------------" << "struct����" << endl;
						regex_value = objectRegex;
					}
					if (metainfoObject->memberTypeInt == OBJECT_ARRAY) {
						//cout << "------------" << "appay����" << endl;
						regex_value = arrayRegex;
					}
					if (metainfoObject->memberTypeInt == OBJECT_VECTOR) {
						//cout << "------------" << "vector����" << endl;
						regex_value = arrayRegex;
					}
					if (metainfoObject->memberTypeInt == OBJECT_LIST) {
						//cout << "------------" << "list����" << endl;
						regex_value = arrayRegex;
					}
					if (metainfoObject->memberTypeInt == OBJECT_SET) {
						//cout << "------------" << "set����" << endl;
						regex_value = arrayRegex;
					}
					if (metainfoObject->memberTypeInt == OBJECT_DEQUE) {
						//cout << "------------" << "deque����" << endl;
						regex_value = arrayRegex;
					}
					if (metainfoObject->memberTypeInt == OBJECT_MAP) {
						//cout << "------------" << "map����" << endl;
						regex_value = mapRegex;
					}
					if (metainfoObject->memberTypeInt == OBJECT_UNORDERED_MAP) {
						//cout << "------------" << "map����" << endl;
						regex_value = mapRegex;
					}
				}
				else {
					//cout << "------------" << "base����" << endl;
				}
				//���ݴ�Сд�ж�
				regex * pattern = nullptr;
				if (metainfoObject->memberIsIgnoreLU == false) {
					pattern = new regex(regex_key + ":" + regex_value);
				}
				else {
					pattern = new regex(regex_key + ":" + regex_value, regex::icase);//icase���ں��Դ�Сд
				}
				if (regex_search(json_, result, *pattern)) {
					string value = result.str(2).c_str();
					//cout << endl << "������ʽ ��ȡ��ֵ��" << value << "   type = " << metainfoObject->memberTypeInt << endl;
					if (metainfoObject->memberTypeInt == OBJECT_BASE && metainfoObject->memberIsIgnore != true) {
						//cout << "�����л�����base��" << value << endl << endl;
						FdogSerializerBase::Instance()->JsonToBase(object_, metainfoObject, value);
					}
					if (metainfoObject->memberTypeInt == OBJECT_ARRAY && metainfoObject->memberIsIgnore != true) {
						vector<string> json_array;
						objectType = isBaseType(metainfoObject->first);
						if (objectType) {
							smatch result;
							regex pattern(arrayRegex);
							if (regex_search(json_, result, pattern)) {
								string value = result.str(2).c_str();
								json_array = split(value, ",");
							}
						}
						else {
							removeFirstComma(json_);
							removeLastComma(json_);
							json_array = FdogSerializer::Instance()->CuttingArray(json_);
						}
						int j = 0;
						if (metainfoObject->first == "bool") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(bool)), json_array[j++]);
							}
						}
						if (metainfoObject->first == "char") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(char)), json_array[j++]);
							}
						}
						if (metainfoObject->first == "unsigned char") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned char)), json_array[j++]);
							}
						}
						if (metainfoObject->first == "char*") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(char*)), json_array[j++]);
							}
						}
						if (metainfoObject->first == "string") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(string)), json_array[j++]);
							}
						}
						if (metainfoObject->first == "short") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(short)), json_array[j++]);
							}
						}
						if (metainfoObject->first == "unsigned short") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned short)), json_array[j++]);
							}
						}
						if (metainfoObject->first == "int") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(int)), json_array[j++]);

							}
						}
						if (metainfoObject->first == "unsigned int") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned int)), json_array[j++]);
							}
						}
						if (metainfoObject->first == "long") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(long)), json_array[j++]);
							}
						}
						if (metainfoObject->first == "unsigned long") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned long)), json_array[j++]);
							}
						}
						if (metainfoObject->first == "long long") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(long long)), json_array[j++]);
							}
						}
						if (metainfoObject->first == "unsigned long long") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(unsigned long long)), json_array[j++]);
							}
						}
						if (metainfoObject->first == "float") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(float)), json_array[j++]);
							}
						}
						if (metainfoObject->first == "double") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(double)), json_array[j++]);
							}
						}
						if (metainfoObject->first == "long double") {
							for (int i = 0; i < metainfoObject->memberArraySize; i++) {
								FdogSerializerBase::Instance()->setValueByAddress(metainfoObject->first, object_, metainfoObject->memberOffset + (i * sizeof(long double)), json_array[j++]);
							}
						}
						Deserialize_arraytype_judgment_all;
					}
					if (metainfoObject->memberTypeInt == OBJECT_VECTOR && metainfoObject->memberIsIgnore != true) {
						if (metainfoObject->first == "char") {
							FDeserialize(*(vector<char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned char") {
							FDeserialize(*(vector<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
						}
						if (metainfoObject->first == "char*") {
							FDeserialize(*(vector<char *> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
						}
						if (metainfoObject->first == "string") {
							//cout << "����string" << endl;
							FDeserialize(*(vector<string> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
						}
						if (metainfoObject->first == "short") {
							FDeserialize(*(vector<short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned short") {
							FDeserialize(*(vector<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
						}
						if (metainfoObject->first == "int") {
							FDeserialize(*(vector<int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned int") {
							FDeserialize(*(vector<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
						}
						if (metainfoObject->first == "long") {
							FDeserialize(*(vector<long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned long") {
							FDeserialize(*(vector<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
						}
						if (metainfoObject->first == "long long") {
							FDeserialize(*(vector<long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned long long") {
							FDeserialize(*(vector<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
						}
						if (metainfoObject->first == "float") {
							FDeserialize(*(vector<float> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
						}
						if (metainfoObject->first == "double") {
							FDeserialize(*(vector<double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
						}
						if (metainfoObject->first == "long double") {
							FDeserialize(*(vector<long double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});
						}
						Deserialize_vector_type_judgment_all;
					}
					if (metainfoObject->memberTypeInt == OBJECT_LIST && metainfoObject->memberIsIgnore != true) {
						if (metainfoObject->first == "bool") {
							FDeserialize(*(list<bool> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<bool>>::Tag{});
						}
						if (metainfoObject->first == "char") {
							FDeserialize(*(list<char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned char") {
							FDeserialize(*(list<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
						}
						if (metainfoObject->first == "char*") {
							FDeserialize(*(list<char *> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
						}
						if (metainfoObject->first == "string") {
							FDeserialize(*(list<string> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
						}
						if (metainfoObject->first == "short") {
							FDeserialize(*(list<short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned short") {
							FDeserialize(*(list<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
						}
						if (metainfoObject->first == "int") {
							//cout << "dasdsa======����list" << endl;                        
							FDeserialize(*(list<int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned int") {
							FDeserialize(*(list<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
						}
						if (metainfoObject->first == "long") {
							FDeserialize(*(list<long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned long") {
							FDeserialize(*(list<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
						}
						if (metainfoObject->first == "long long") {
							FDeserialize(*(list<long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned long long") {
							FDeserialize(*(list<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
						}
						if (metainfoObject->first == "float") {
							FDeserialize(*(list<float> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
						}
						if (metainfoObject->first == "double") {
							FDeserialize(*(list<double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
						}
						if (metainfoObject->first == "long double") {
							FDeserialize(*(list<long double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});
						}
						Deserialize_list_type_judgment_all;
					}
					if (metainfoObject->memberTypeInt == OBJECT_DEQUE && metainfoObject->memberIsIgnore != true) {
						if (metainfoObject->first == "bool") {
							FDeserialize(*(deque<bool> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<bool>>::Tag{});
						}
						if (metainfoObject->first == "char") {
							FDeserialize(*(deque<char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned char") {
							FDeserialize(*(deque<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
						}
						if (metainfoObject->first == "char*") {
							FDeserialize(*(deque<char *> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
						}
						if (metainfoObject->first == "string") {
							FDeserialize(*(deque<string> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
						}
						if (metainfoObject->first == "short") {
							FDeserialize(*(deque<short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned short") {
							FDeserialize(*(deque<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
						}
						if (metainfoObject->first == "int") {
							FDeserialize(*(deque<int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned int") {
							FDeserialize(*(deque<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
						}
						if (metainfoObject->first == "long") {
							FDeserialize(*(deque<long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned long") {
							FDeserialize(*(deque<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
						}
						if (metainfoObject->first == "long long") {
							FDeserialize(*(deque<long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned long long") {
							FDeserialize(*(deque<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
						}
						if (metainfoObject->first == "float") {
							FDeserialize(*(deque<float> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
						}
						if (metainfoObject->first == "double") {
							FDeserialize(*(deque<double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
						}
						if (metainfoObject->first == "long double") {
							FDeserialize(*(deque<long double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});
						}
						Deserialize_deque_type_judgment_all;
					}
					if (metainfoObject->memberTypeInt == OBJECT_SET && metainfoObject->memberIsIgnore != true) {
						if (metainfoObject->first == "bool") {
							FDeserialize(*(set<bool> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<bool>>::Tag{});
						}
						if (metainfoObject->first == "char") {
							FDeserialize(*(set<char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned char") {
							FDeserialize(*(set<unsigned char> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
						}
						if (metainfoObject->first == "char*") {
							FDeserialize(*(set<char *> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
						}
						if (metainfoObject->first == "string") {
							FDeserialize(*(set<string> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
						}
						if (metainfoObject->first == "short") {
							FDeserialize(*(set<short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned short") {
							FDeserialize(*(set<unsigned short> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
						}
						if (metainfoObject->first == "int") {
							FDeserialize(*(set<int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned int") {
							FDeserialize(*(set<unsigned int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
						}
						if (metainfoObject->first == "long") {
							FDeserialize(*(set<long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned long") {
							FDeserialize(*(set<unsigned long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
						}
						if (metainfoObject->first == "long long") {
							FDeserialize(*(set<long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
						}
						if (metainfoObject->first == "unsigned long long") {
							FDeserialize(*(set<unsigned long long> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
						}
						if (metainfoObject->first == "float") {
							FDeserialize(*(set<float> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
						}
						if (metainfoObject->first == "double") {
							FDeserialize(*(set<double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
						}
						if (metainfoObject->first == "long double") {
							FDeserialize(*(set<long double> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});
						}
						Deserialize_set_type_judgment_all;
					}
					if (metainfoObject->memberTypeInt == OBJECT_MAP && metainfoObject->memberIsIgnore != true) {
						if (metainfoObject->first == "char*" && metainfoObject->second == "int") {
							FDeserialize(*(map<char *, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<map<int, int>>::Tag{});
						}
						if (metainfoObject->first == "string" && metainfoObject->second == "int") {
							FDeserialize(*(map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<map<int, int>>::Tag{});
						}
						if (metainfoObject->first == "string" && metainfoObject->second == "float") {
							FDeserialize(*(map<string, float> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<map<int, int>>::Tag{});
						}
						if (metainfoObject->first == "string" && metainfoObject->second == "bool") {
							FDeserialize(*(map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<map<int, int>>::Tag{});
						}
						if (metainfoObject->first == "int" && metainfoObject->second == "int") {
							FDeserialize(*(map<int, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<map<int, int>>::Tag{});
						}
						Deserialize_map_type_judgment_all;
					}
					if (metainfoObject->memberTypeInt == OBJECT_UNORDERED_MAP && metainfoObject->memberIsIgnore != true) {
						if (metainfoObject->first == "char*" && metainfoObject->second == "int") {
							FDeserialize(*(unordered_map<char *, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<unordered_map<int, int>>::Tag{});
						}
						if (metainfoObject->first == "string" && metainfoObject->second == "int") {
							FDeserialize(*(unordered_map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<unordered_map<int, int>>::Tag{});
						}
						if (metainfoObject->first == "string" && metainfoObject->second == "float") {
							FDeserialize(*(unordered_map<string, float> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<unordered_map<int, int>>::Tag{});
						}
						if (metainfoObject->first == "string" && metainfoObject->second == "bool") {
							FDeserialize(*(unordered_map<string, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<unordered_map<int, int>>::Tag{});
						}
						if (metainfoObject->first == "int" && metainfoObject->second == "int") {
							FDeserialize(*(unordered_map<int, int> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<unordered_map<int, int>>::Tag{});
						}
						Deserialize_map_type_judgment_all;
					}
					Deserialize_type_judgment_all;
				}
			}
		}
	}

	template<typename T>
	void DeserializeS(string & json_, T & object_, BaseAndStructTag, string name = "") {
		//cout << "DeserializeS BaseAndStructTag json_ = " << json_ << endl;
		Deserialize(object_, json_, name);
	}

	template<typename T>
	void DeserializeS(string & json_, T & object_, BaseArrayTag, string name = "") {
		vector<string> json_array;
		json_array = FdogSerializer::Instance()->CuttingArray(json_);
		//cout << "----" << json_array.size() << endl;
		for (auto & object_one : object_) {
			Deserialize(object_one, json_);
		}
	}

	template<typename T>
	void DeserializeS_s(T & object_, string & json_, bool isArray, string name = "") {
		// if(isArray){
		//     DeserializeS(json_, object_, TagSTLAAAType<int[2]>::Tag{}, name);
		// }else{

		DeserializeS(json_, object_, typename TagSTLAAAType<T>::Tag{}, name);
		//}
	}

	//���ڽ����������ͣ�����(ֻ��Ҫ�ж���û��[]����ȷ���ǲ������飬�ṹ��ͻ������Ͷ����߱�[]����)���ṹ��
	template<typename T>
	void FDeserialize(T & object_, string & json_, BaseTag, string name = "") {
		//cout << "FDeserialize BaseTag json_ = " << json_ << endl;
		bool isArray = isArrayType("", FdogSerializer::Instance()->getTypeName(typeid(T).name()));
		DeserializeS_s(object_, json_, isArray, name);
	}

	//���ڽ���STL��map���⣩
	template<typename T>
	void FDeserialize(T & object_, string & json_, ArrayTag, string name = "") {
		//cout << "FDeserialize ArrayTag json_ = " << json_ << endl;
		//cout << "�����л�����~array��" << json_  << endl;
		//cout << "����" << abi::__cxa_demangle(typeid(object_).name(),0,0,0) << endl << endl;
		//cout << "���� = " << object_.size() << endl;
		int objectType = -1;
		//���ѭ������ȥ
		for (auto & object_one : object_) {
			//�ж��ڲ������Ƿ�Ϊ��������
			objectType = isBaseType(FdogSerializer::Instance()->getTypeName(typeid(object_one).name()));
			//cout << "objectType=" << objectType << "--" << abi::__cxa_demangle(typeid(object_one).name(),0,0,0)<< endl;
			break;
		}
		//cout << "�ߵ�����1"<<endl;
		vector<string> json_array;
		if (objectType) {
			smatch result;
			regex pattern(arrayRegex);
			if (regex_search(json_, result, pattern)) {
				string value = result.str(2).c_str();
				json_array = split(value, ",");
			}
		}
		else {
			removeFirstComma(json_);
			removeLastComma(json_);
			json_array = FdogSerializer::Instance()->CuttingArray(json_);
		}
		//cout << "�ߵ�����2"<<endl;
		int i = 0;
		//������Ҫע�⣬������STL��������Ϊ0����Ҫ����ָ������ ��Ҫ��취
		//��Ҫ֪�������ڲ����ͣ�Ȼ����Ϊ��������ȥ,�������С����Ҫת���ģ�����Ҫ��ӳ���,STL�����ݻ���
		int len = json_array.size();
		int len2 = object_.size();
		//cout << "changdu:" << len  << "----" << len2 << endl;
		memberAttribute Member = getMemberAttribute(FdogSerializer::Instance()->getTypeName(typeid(T).name()));
		srand((int)time(NULL)); //����set
		for (int i = 0; i < json_array.size(); i++) {
			//cout << "xunhuan1 :" << i << " & object_.size() = " << object_.size() << endl;
			if (json_array.size() > object_.size()) {
				F_init(object_, Member.valueTypeInt, Member.first);
				//cout << "xunhuan2 :" << i << " & object_.size() = " << object_.size() << endl;
			}
			//cout << "xunhuan2 :" << i << " & object_.size() = " << object_.size() << endl;
		}
		//cout << "�ߵ�����3 size = " << object_.size() <<endl;
		for (auto & object_one : object_) {
			//cout << "start " << endl;
			//cout << "json_array[i] = " << json_array[i] << endl;
			//cout << "end " << endl;
			Deserialize(object_one, json_array[i]);
			i++;
		}
		//cout << "�ߵ�����4"<<endl;
		//�����ͷ�init����������ڴ�
		if (1) {
			vector<char *>::iterator it = temp.begin();
			while (it != temp.end()) {
				delete *it;
				++it;
			}
		}
		//cout << "�ߵ�����5"<<endl;
		temp.clear();
		//cout << "�ߵ�����6"<<endl;
	}
	//����map
	template<typename T>
	void FDeserialize(T & object_, string & json_, MapTag) {
		//cout << "�����л�����~map��" << json_ << endl;
		//cout << "����" << abi::__cxa_demangle(typeid(object_).name(),0,0,0) << endl << endl;
		int objectType = -1;
		for (auto & object_one : object_) {
			//�ж��ڲ������Ƿ�Ϊ��������
			objectType = isBaseTypeByMap(FdogSerializer::Instance()->getTypeName(typeid(object_one).name()));
			//cout << "objectType=" << objectType << "--" << abi::__cxa_demangle(typeid(object_one).name(),0,0,0)<< endl;
			break;
		}
		//���������� objectType��ԶΪ0 �ҵڶ��ַ���
		vector<string> json_array;
		if (objectType) {
			smatch result;
			regex pattern(mapRegex);
			if (regex_search(json_, result, pattern)) {
				string value = result.str(2).c_str();
				json_array = split(value, ",");
			}
		}
		else {
			removeFirstComma(json_);
			removeLastComma(json_);
			json_array = FdogSerializer::Instance()->CuttingArray(json_);
		}
		int i = 0;
		int len = json_array.size();
		sort(json_array.begin(), json_array.end());
		//cout << "changdu:" << len << endl;
		memberAttribute Member = getMemberAttribute(FdogSerializer::Instance()->getTypeName(typeid(T).name()));
		for (int i = 0; i < json_array.size(); i++) {
			if (json_array.size() > object_.size()) {
				//cout <<"##### ����" << endl;
				F_init(object_, Member.valueTypeInt, Member.first, Member.second, getKey(json_array[i]));
			}
			//i++;
		}
		//�����и����⣬���ǿ���key��˳��ƥ��
		//����������⣬������STL��������Ϊ0����Ҫ����ָ������
		for (auto & object_one : object_) {
			//cout << "������ object_one.second = " << object_one.second << " json_array[i] = " << json_array[i] << endl;
			//��ȡkey:
			//object_one.first = getKey(json_array[i]);
			Deserialize(object_one.second, json_array[i]);
			i++;
		}
		//�����ͷ�init����������ڴ�
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
		//cout << "FObject json_ = " << json_ << endl;
		FdogSerializer::Instance()->FDeserialize(object_, json_, typename TagDispatchTrait<T>::Tag{}, name);
	}

	template<typename T>
	std::string FJsonToString(T & object_, string json_ = "", string name = "") {
		FdogSerializer::Instance()->FSerialize(json_, object_, typename TagDispatchTrait<T>::Tag{}, name);
		return json_;
	}

	//���ñ���
	void setAliasName(string Type, string memberName, string AliasName);

	//���ö������
	template<class T, class ...Args>
	void setAliasNameS(T Type, T memberName, T AliasName, Args... args) {
		FdogSerializer::Instance()->__setAliasNameS(Type, memberName, AliasName, args...);
	}

	//�����Ƿ���Ը��ֶ����л�
	void setIgnoreField(string Type, string memberName);

	//���ö�������ֶ����л�
	template<class T, class ...Args>
	void setIgnoreFieldS(T Type, T memberName, Args... args) {
		FdogSerializer::Instance()->__setIgnoreFieldS(Type, memberName, args...);
	}

	//�����Ƿ���Դ�Сд
	void setIgnoreLU(string Type, string memberName);

	//���ö�����Դ�Сд
	template<class T, class ...Args>
	void setIgnoreLUS(T Type, T memberName, Args... args) {
		FdogSerializer::Instance()->__setIgnoreLUS(Type, memberName, args...);
	}

	//���ý���ģ��ת�� �ṹ��תjson���������������Ҫ�����jsonת�ṹ������⣬������ڷ��磬���Գ��Խ���ģ��ת��
	void setFuzzy(string Type);

	//�ж�json��ȷ��
	result JsonValidS(string json_);

	//�ж��ֶ��Ƿ����
	bool Exist(string json_, string key);

	//��ȡ�ֶε�ֵ
	string GetStringValue(string json_, string key);

	//��ȡ�ֶε�ֵ
	int GetIntValue(string json_, string key);

	//��ȡ�ֶε�ֵ
	double GetDoubleValue(string json_, string key);

	//��ȡ�ֶε�ֵ
	long GetLongValue(string json_, string key);

	//��ȡ�ֶε�ֵ
	bool GetBoolValue(string json_, string key);

}

#define EXPAND(...) __VA_ARGS__  //1g

#define NAME_MSVC(x) #x

#define NAME(...) EXPAND(NAME_MSVC(__VA_ARGS__))

#define EXTAND_ARGS(args) args //__VA_ARGS__ ��vs�лᱻ��Ϊ��һ��ʵ�Σ�������Ҫ����ú����

#define ARG_N(...) \
    EXTAND_ARGS(ARG_N_(0, __VA_ARGS__, ARG_N_RESQ()))

#define ARG_N_(...) \
    EXTAND_ARGS(ARG_N_M(__VA_ARGS__))  //2g

#define ARG_N_M(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, _11, _12, _13, _14, _15, _16, _17, _18, _19,_20, N,...) N

#define ARG_N_RESQ() 20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0

#define MEMBERTYPE(TYPE, MEMBER) FdogSerializer::Instance()->getMemberAttribute(FdogSerializer::Instance()->getTypeName(typeid(((TYPE *)0)->MEMBER).name()));

#define PLACEHOLDER(placeholder, ...) placeholder

#define REGISTERED_FUCK_MSVC(TYPE, ...) \
do{ \
    ObjectInfo * objectinfo_one = new ObjectInfo();\
    objectinfo_one->objectType = NAME(TYPE);\
    objectinfo_one->objectTypeInt = FdogSerializer::Instance()->getObjectTypeInt(objectinfo_one->objectType, FdogSerializer::Instance()->getTypeName(typeid(TYPE).name()));\
    objectinfo_one->objectSize = sizeof(TYPE);\
    FdogSerializer::Instance()->addObjectInfo(objectinfo_one);\
	EXPAND(REGISTEREDMEMBER_s_1_MSVC(TYPE, PLACEHOLDER(__VA_ARGS__), objectinfo_one->metaInfoObjectList, ARG_N(__VA_ARGS__) - 1, __VA_ARGS__, PLACEHOLDER(##__VA_ARGS__)));\
}while(0); //3g //4g   //    //5g

#define REGISTEREDMEMBER(...) EXPAND(REGISTERED_FUCK_MSVC(##__VA_ARGS__))

#define REGISTEREDMEMBER_s_1(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) EXPAND(REGISTEREDMEMBER_s_2(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE));

#define REGISTEREDMEMBER_s_1_MSVC(...) EXPAND(REGISTEREDMEMBER_s_1(##__VA_ARGS__))

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

#define offsetof_MSVC(...) EXPAND(offsetof(##__VA_ARGS__));

#define MEMBERTYPE_MSVC(...) EXPAND(MEMBERTYPE(##__VA_ARGS__));

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

#endif