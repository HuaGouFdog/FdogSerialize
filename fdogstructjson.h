/*************************************************************************
*       Author:HuaGouFdog(zhangxv)                                       *
*       Email:2506897252@qq.com                                          *
*       Time:2021.11.20                                                  *
*       Copyright 2021 HuaGouFdog(zhangxv)                               *
*       Licensed under the Apache License, Version 2.0 (the "License");  *
*       you may not use this file except in compliance with the License. *
*       You may obtain a copy of the License at                          *
*       http://www.apache.org/licenses/LICENSE-2.0                       *
**************************************************************************/
#ifndef FDOGSTRUCTJSON_H
#define FDOGSTRUCTJSON_H
#include"fdogtype.h"
#include<iostream>
#include<vector>
#include<typeinfo>
#include<regex>
using namespace std;

namespace fsj{

//获取原始名
#define NAME(x) #x

#define NAME_ARRAY(x) #x"_array"

//获取别名
#define ALIASNAME(x) #x

#define NAMESPLIT(x) fdog_##x

#define FUNCNAME(x) get##x

//结构体转json
#define REGISTERED_OBJECT_SERIALIZE(TYPE)\
if (metainfostruct.memberType == NAME(TYPE)){\
    FdogStructToJson(json_, *((TYPE *)((void *)&struct_ + metainfostruct.memberOffset)));\
}

#define REGISTERED_OBJECT_ARRAY_SERIALIZE(TYPE)\
if (metainfostruct.memberType == NAME_ARRAY(TYPE)){\
    json_ = json_ + "{"; \
    FdogStructToJson(json_, *((TYPE *)((void *)&struct_ + metainfostruct.memberOffset + (sizeof(TYPE) * i))));\
    removeLastComma(json_);\
    json_ = json_ + "},";\
}

//json转结构体
#define REGISTERED_OBJECT_DESSERIALIZE(TYPE)\
if (metainfostruct.memberType == NAME(TYPE)){\
    FdogJsonToStruct(*((TYPE *)((void *)&struct_ + metainfostruct.memberOffset)), json_);\
}

#define REGISTERED_OBJECT_ARRAY_DESSERIALIZE(TYPE)\
if (metainfostruct.memberType ==  NAME_ARRAY(TYPE)){\
    FdogJsonToStruct(*((TYPE *)((void *)&struct_ + metainfostruct.memberOffset + (sizeof(TYPE) * i))), json_);\
}

//如若存在嵌套结构体需要序列化应在此添加
#define REGISTERED_OBJECT_SERIALIZE_ALL\
    REGISTERED_OBJECT_SERIALIZE(headmaster)\
    REGISTERED_OBJECT_SERIALIZE(school)\
    REGISTERED_OBJECT_SERIALIZE(teacher)\
    REGISTERED_OBJECT_SERIALIZE(student)\

//如若存在嵌套结构体数组需要序列化应在此添加
#define REGISTERED_OBJECT_ARRAY_SERIALIZE_ALL\
    REGISTERED_OBJECT_ARRAY_SERIALIZE(headmaster)\
    REGISTERED_OBJECT_ARRAY_SERIALIZE(school)\
    REGISTERED_OBJECT_ARRAY_SERIALIZE(teacher)\
    REGISTERED_OBJECT_ARRAY_SERIALIZE(student)\

//如若存在嵌套结构体需要反序列化应在此添加
#define REGISTERED_OBJECT_DESSERIALIZE_ALL\
    REGISTERED_OBJECT_DESSERIALIZE(headmaster)\
    REGISTERED_OBJECT_DESSERIALIZE(school)\
    REGISTERED_OBJECT_DESSERIALIZE(teacher)\
    REGISTERED_OBJECT_DESSERIALIZE(student)\

//如若存在嵌套结构体数组需要反序列化应在此添加
#define REGISTERED_OBJECT_ARRAY_DESSERIALIZE_ALL\
    REGISTERED_OBJECT_ARRAY_DESSERIALIZE(headmaster)\
    REGISTERED_OBJECT_ARRAY_DESSERIALIZE(school)\
    REGISTERED_OBJECT_ARRAY_DESSERIALIZE(teacher)\
    REGISTERED_OBJECT_ARRAY_DESSERIALIZE(student)\


static vector<structInfo> structinfo; //所有结构体信息存储于此

/***********************************
*   返回对应的成员类型(包括基本类型和自定义类型)，数组大小
************************************/
memberAttribute getMemberAttribute(string key){
    memberAttribute resReturn;
    if(baseType.count(key) != 0){
        resReturn.valueType = baseType[key];  
        resReturn.ArraySize = 0;
        return resReturn;
    } else if (basePointerType.count(key) != 0){
        resReturn.valueType = basePointerType[key];
        resReturn.ArraySize = 0;
    } else {
        smatch result;
        regex pattern(patternArray);
        if(regex_search(key, result, pattern)){
            string key_ = result.str(2) + result.str(4);
            if(baseArrayType.count(key_) != 0){
                resReturn.valueType = baseArrayType[key_];
            } else {
                objectArrayType[key_] = result.str(4) + "_array";
                resReturn.valueType = objectArrayType[key_];
            }
            resReturn.ArraySize = atoi(result.str(3).data());
            return resReturn;
        } else {
            smatch result;
            regex pattern(patternObject);
            if(regex_search(key, result, pattern)){
                string str = result.str(2) + " " + result.str(3);
                resReturn.valueType = result.str(3);
                resReturn.ArraySize = atoi(result.str(3).data());
                return resReturn;
            }
        }
    }
    return resReturn;
}

/***********************************
*   删除字符串第一个字符
************************************/
void removeFirstComma(string & return_){
    return_ = return_.substr(1);
}

/***********************************
*   删除字符串最后一个字符
************************************/
void removeLastComma(string & return_){
    return_.pop_back();
}

/***********************************
*   删除浮点数后面多余的0
************************************/
template<class T>
string removeLastZero(T & return_){
    std::ostringstream oss; 
    oss << return_;
    return oss.str();
}

/***********************************
*   删除字符串的数字 这个有一些问题，名字中不能包含数字，要改
************************************/
void removeNumbers(string & return_){
    string::iterator it = return_.begin();
    while (it != return_.end()) {
        if ((*it >= '0') && (*it <= '9')) {
            it = return_.erase(it);
        } else {
            it++;
        }
    }
}

/***********************************
*   查询对应的结构体信息 这里有问题，如果是两个一个的数组就存在问题
************************************/
structInfo & getStructInfo(string structName){
    removeNumbers(structName);
    for(int i = 0; i < structinfo.size(); i++){
        if (structinfo[i].structType == structName){
            return structinfo[i];
        }
    } //空怎么办
}

template<class T>
void setValueByAddress(string type_str, T &info, int vc, string data, int Size = 0){
    std::stringstream ss;
    ss.str(data);
    if(type_str == "bool"){
        ss >> *((bool *)((void *)&info + vc));
    }
    if(type_str == "char" || type_str == "signed char"){
        //可能溢出
        ss >> *((char *)((void *)&info + vc));
    }
    if(type_str == "unsigned char"){
        //可能溢出
        ss >> *((unsigned char *)((void *)&info + vc));
    }
    if(type_str == "int" || type_str == "signed int"){
        ss >> *((int *)((char *)&info + vc));
    }
    if(type_str == "unsigned_int"){
        ss >> *((unsigned int *)((char *)&info + vc));
    }
    if(type_str == "short_int" || type_str == "signed_short_int"){
        ss >> *((short int *)((char *)&info + vc));
    }
    if(type_str == "unsigned_short_int"){
        ss >> *((unsigned short int *)((char *)&info + vc));
    }
    if(type_str == "long_int" || type_str == "signed_long_int"){
        ss >> *((long int *)((char *)&info + vc));
    }
    if(type_str == "unsigned_long_int"){
        ss >> *((unsigned long int *)((char *)&info + vc));
    }
    if(type_str == "long_long_int" || type_str == "signed_long_long_int"){
        ss >> *((long long int *)((char *)&info + vc));
    }
    if(type_str == "unsigned_long_int"){
        ss >> *((unsigned long long  int *)((char *)&info + vc));
    }  
    if(type_str == "float"){
        ss >> *((float *)((char *)&info + vc));
    }
    if(type_str == "double"){
        ss >> *((double *)((char *)&info + vc));
    }
    if(type_str == "long_double"){
        ss >> *((long double *)((char *)&info + vc));
    }
}

template<class T>
void setValueByAddress_S(string type_str, T &info, int vc, string data){
    if(type_str == "char_ptr"){
        *((char **)((void *)&info + vc)) = new char[strlen(data.c_str())];
        strcpy(*((char **)((void *)&info + vc)), data.c_str());
    }
}

/***********************************
*   解析基础类型
************************************/
template<class T>
void JsonToBase(T & struct_, metaInfo & metainfostruct, string json_){

    if (metainfostruct.memberType == "char_ptr"){
        setValueByAddress_S(metainfostruct.memberType, struct_, metainfostruct.memberOffset, json_);
    } else{
        setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, json_);
    }
}

/***********************************
*   解析对象
************************************/
template<class T>
void FdogJsonToStruct(T & struct_, string json_);

template<class T>
void FdogStructToJson(string & json_, T & struct_);

template<class T>
void JsonToObject(T & struct_, metaInfo & metainfostruct, string json_){
    REGISTERED_OBJECT_DESSERIALIZE_ALL
}
/***********************************
*   解析数组
************************************/
template<class T>
void JsonToArray(T & struct_, metaInfo & metainfostruct, string json_){
    removeFirstComma(json_);
    removeLastComma(json_);

    map<string, string>::iterator iter;
    iter = baseArrayType.begin();
    bool isBaseArray = false;
    while(iter != baseArrayType.end()) {
        if(metainfostruct.memberType == iter->second){
            isBaseArray = true;
            break;
        }
        iter++;
    }
    if (isBaseArray){
            std::regex reg(",");
            std::sregex_token_iterator pos(json_.begin(), json_.end(), reg, -1);
            decltype(pos) end;
            int j = 0;
            for (; pos != end; ++pos){
                //这里后面要判断一下值
                size_t subscriptValue = metainfostruct.memberType.find("_array");
                if(metainfostruct.memberType == "char_ptr_array"){
                    setValueByAddress_S(metainfostruct.memberType.substr(0, subscriptValue), struct_, metainfostruct.memberOffset + (j * 8), pos->str());
                }else{
                    setValueByAddress(metainfostruct.memberType.substr(0, subscriptValue), struct_, metainfostruct.memberOffset + (j * 4), pos->str());
                }

                j++;
            }
    }else{
        for(int i = 0; i < metainfostruct.memberArraySize; i++){
            //考虑如何分割数组json {},{}
            REGISTERED_OBJECT_ARRAY_DESSERIALIZE_ALL
        }
    }
}


template<class T>
string getValueByAddress_S(string type_str, T &info, int vc){
    if(type_str == "char_ptr"){
        auto value = *((const char **)((void *)&info + vc));
        string str_value = value;
        return aQuotationMark + str_value  + aQuotationMark;
    }
}
template<class T>
string getValueByAddress(string type_str, T &info, int vc){
    if(type_str == "bool"){
        auto value = *((bool *)((char *)&info + vc));
        if(value){
            return "true";
        }else{
            return "false";
        }
    }
    if(type_str == "char" || type_str == "signed_char"){
        auto value = *((char *)((void *)&info + vc));
        return to_string((int)value);
    }
    if(type_str == "unsigned_char"){
        auto value = *((char *)((void *)&info + vc));
        return to_string((unsigned int)value);
    }
    if(type_str == "int" || type_str == "signed_int"){
        auto value = *((int *)((char *)&info + vc));
        return to_string(value);
    }
    if(type_str == "unsigned_int"){
        auto value = *((unsigned int *)((char *)&info + vc));
        return to_string(value);
    }
    if(type_str == "short_int" || type_str == "signed_short_int"){
        auto value = *((short int *)((char *)&info + vc));
        return to_string(value);
    }
    if(type_str == "unsigned_short_int"){
        auto value = *((unsigned short int *)((char *)&info + vc));
        return to_string(value);
    }
    if(type_str == "long_int" || type_str == "signed_long_int"){
        auto value = *((long int *)((char *)&info + vc));
        return to_string(value);
    }
    if(type_str == "unsigned_long_int"){
        auto value = *((unsigned long int *)((char *)&info + vc));
        return to_string(value);
    }
    if(type_str == "long_long_int" || type_str == "signed_long_long_int"){
        auto value = *((long long int *)((char *)&info + vc));
        return to_string(value);
    }
    if(type_str == "unsigned_long_long_int"){
        auto value = *((unsigned long long int *)((char *)&info + vc));
        return to_string(value);
    }        
    if(type_str == "float"){
        auto value = *((float *)((char *)&info + vc));
        return removeLastZero(value);
    }
    if(type_str == "double"){
        auto value = *((double *)((char *)&info + vc));
        return removeLastZero(value);
    }
    if(type_str == "long_double"){
        auto value = *((long double *)((char *)&info + vc));
        return removeLastZero(value);
    }
    return "";
}

template<class T>
void BaseToJson(string & json_, metaInfo & metainfostruct, T & struct_){
    string value;
    if (metainfostruct.memberType == "char_ptr"){
        value = getValueByAddress_S(metainfostruct.memberType, struct_, metainfostruct.memberOffset);
    } else{
        value = getValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset);
    }
    json_ = json_ + aQuotationMark + metainfostruct.memberName + aQuotationMark + ":" + value + ",";
}

template<class T>
void ObjectToJson(string & json_, metaInfo & metainfostruct, T & struct_){
    REGISTERED_OBJECT_SERIALIZE_ALL
}

template<class T>
void ArrayToJson(string & json_, metaInfo & metainfostruct, T & struct_){
    map<string, string>::iterator iter;
    iter = baseArrayType.begin();
    bool isBaseArray = false;
    while(iter != baseArrayType.end()) {
        if(metainfostruct.memberType == iter->second){
            isBaseArray = true;
            break;
        }
        iter++;
    }
    if (isBaseArray){
        string quotationMark = "\"";
        string json_2;
        json_2 =  json_2 + aQuotationMark + metainfostruct.memberName + aQuotationMark + ":" + squareBracketsL;
        for(int i = 0; i < metainfostruct.memberArraySize; i++){
            //这里要按照类型选取不同的值
            string value;
            size_t subscriptValue = metainfostruct.memberType.find("_array");
            if(metainfostruct.memberType == "char_ptr_array"){
                value = getValueByAddress_S(metainfostruct.memberType.substr(0, subscriptValue), struct_, metainfostruct.memberOffset + (i * 8));
            }else{
                value = getValueByAddress(metainfostruct.memberType.substr(0, subscriptValue), struct_, metainfostruct.memberOffset + (i * 4));
            }
            json_2 = json_2 + value + ",";
        }
        removeLastComma(json_2);
    json_ = json_ + json_2 + squareBracketsR + ",";

    }else{
        json_ = json_ + "\"" + metainfostruct.memberName + "\"" + ":[";
        for(int i = 0; i < metainfostruct.memberArraySize; i++){
            REGISTERED_OBJECT_ARRAY_SERIALIZE_ALL
        }
        removeLastComma(json_);
        json_ = json_ + "],";
    }
}


enum MetainfoType {
    FDOGBASE = 1,
    FDOGOBJECT,
    FDOGARRAY
};

/***********************************
*   获取成员类型 1：基础类型 2：自定义类型 3：数组
************************************/
int getMemberVType(string type_str, int Size){
    if(Size > 0){
        return FDOGARRAY;
    }
    map<string, string>::iterator iter;
    iter = baseType.begin();
    bool isObject = true;
    while(iter != baseType.end()) {
        if(type_str == iter->second){
            isObject = false;
            break;
        }
        iter++;
    }
    iter = basePointerType.begin();
    while(iter != basePointerType.end()) {
        if(type_str == iter->second){
            isObject = false;
            break;
        }
        iter++;
    }
    if(isObject){
        return FDOGOBJECT;
    }
    return FDOGBASE;
}

/***********************************
*   Json转Struct接口
************************************/
template<typename T>
void FdogJsonToStruct(T & struct_, string json_){
    structInfo a = getStructInfo(typeid(T).name());
    for(int i = 0; i < a.metainfoStruct.size(); i++){
        int Vtype = getMemberVType(a.metainfoStruct[i].memberType, a.metainfoStruct[i].memberArraySize);
        smatch result;
        string regex_key = "(\"" + a.metainfoStruct[i].memberName +"\")";
        string regex_value = baseRegex[a.metainfoStruct[i].memberType];
        if(regex_value == ""){
            if(Vtype == 2){
                regex_value = objectRegex;
            }else{
                regex_value = arrayRegex;
            }
        }
        regex pattern(regex_key + ":" +regex_value);
        if(regex_search(json_, result, pattern)){
        auto value = result.str(2).c_str();
            switch(Vtype){
                case FDOGBASE:
                    //cout << "基础类型" << a.metainfoStruct[i].memberType << "=" << value << endl;
                    JsonToBase(struct_, a.metainfoStruct[i], value); //基础类型 提供数据
                    break;
                case FDOGOBJECT:
                    //cout << "对象类型" << a.metainfoStruct[i].memberType << "=" << result.str(2).c_str() << endl;
                    JsonToObject(struct_, a.metainfoStruct[i], result.str(2).c_str()); 
                    break;
                case FDOGARRAY:
                    //cout << "数组类型" << a.metainfoStruct[i].memberType << "=" << result.str(2).c_str() << endl;
                    JsonToArray(struct_, a.metainfoStruct[i], result.str(2).c_str()); //数组类型 
                    break;
            }
        }
    }
}
template<typename T>
T getheadmaster(T &){}
/***********************************
*   Struct转Json接口
************************************/
template<typename T>
void FdogStructToJson(string & json_, T & struct_){
    //获取传进来结构体对应的元信息
    structInfo info = getStructInfo(typeid(T).name());
    string curlyBracketL = "{";
    string curlyBracketR = "}";
    for(int i = 0; i < info.metainfoStruct.size(); i++){
        int Vtype = getMemberVType(info.metainfoStruct[i].memberType, info.metainfoStruct[i].memberArraySize);
            switch(Vtype){
                case FDOGBASE:
                    //cout << "基础类型" << info.metainfoStruct[i].memberType << endl;
                    BaseToJson(json_, info.metainfoStruct[i], struct_);
                    break;
                case FDOGOBJECT:
                    //cout << "对象类型" << info.metainfoStruct[i].memberType << endl;
                    json_ = json_ + aQuotationMark + info.metainfoStruct[i].memberName + aQuotationMark + ":" + curlyBracketL;
                    ObjectToJson(json_, info.metainfoStruct[i], struct_); 
                    removeLastComma(json_);
                    json_ = json_ + curlyBracketR + ",";
                    break;
                case FDOGARRAY:
                    //cout << "数组类型" << info.metainfoStruct[i].memberType << endl;
                    ArrayToJson(json_, info.metainfoStruct[i], struct_); 
                    break;
            }
    }
}

//序列化 struct=>json
template<typename T>
void FdogSerialize(string & json_, T & struct_){
    FdogStructToJson(json_, struct_);
    removeLastComma(json_);
    json_ = curlyBracketL + json_ + curlyBracketR;
}
//反序列化 json=>struct
template<typename T>
void FdogDesSerialize(T & struct_, string & json_){
    FdogJsonToStruct(struct_, json_);
}

#define ARG_N(...) \
    ARG_N_(0, ##__VA_ARGS__, ARG_N_RESQ()) 

#define ARG_N_(...) \
    ARG_N_M(__VA_ARGS__)

#define ARG_N_M(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, _11, _12, _13, _14, _15, _16, _17, _18, _19,_20, N,...) N

#define ARG_N_RESQ() 20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0

//#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)  //获取偏移值

#define MEMBERTYPE(TYPE, MEMBER) getMemberAttribute(typeid(((TYPE *)0)->MEMBER).name())//获取类型  //可以直接使用函数 去掉MEMBERTYPE

#define PLACEHOLDER(placeholder, ...) placeholder

#define REGISTEREDMEMBER(TYPE, ...) \
do{ \
    structInfo structinfo_one;\
    structinfo_one.structType = NAME(TYPE);\
    REGISTEREDMEMBER_s_1(TYPE, PLACEHOLDER(__VA_ARGS__), structinfo_one.metainfoStruct, ARG_N(__VA_ARGS__) - 1, ##__VA_ARGS__, PLACEHOLDER(__VA_ARGS__));\
    structinfo.push_back(structinfo_one);\
}while(0);

#define REGISTEREDMEMBER_s_1(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_2(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_2(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_3(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_3(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_4(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_4(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_5(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_5(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_6(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_6(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_7(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_7(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_8(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_8(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_9(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_9(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_10(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_10(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_11(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_11(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_12(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_12(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_13(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_13(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_14(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_14(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_15(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_15(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_16(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_16(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_17(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_17(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_18(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_18(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_19(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_19(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_20(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_20(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1);

#define REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg) \
    do{\
        metaInfo metainfo_one;\
        metainfo_one.memberName = NAME(arg);\
        metainfo_one.memberAliasName = ALIASNAME(arg);\
        metainfo_one.memberOffset = offsetof(TYPE, arg);\
        memberAttribute resReturn = MEMBERTYPE(TYPE, arg);\
        metainfo_one.memberType = resReturn.valueType;\
        metainfo_one.memberTypeSize = sizeof(TYPE);\
        metainfo_one.memberArraySize = resReturn.ArraySize;\
        metainfoStruct.push_back(metainfo_one);\
    }while(0);
}
#endif