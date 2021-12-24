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

#define NAMESPLIT(x) fdog_##x

#define FUNCNAME(x) get##x

#define REGISTERED(TYPE) \
    extern TYPE NAMESPLIT(TYPE);\
    TYPE FUNCNAME(TYPE)(){ TYPE value; return value;}\


REGISTERED(headmaster);
REGISTERED(school);
REGISTERED(teacher);
REGISTERED(student);


enum function {
    HEADMASTER = 1,
    SCHOOL,
};

#define BUILDSWITCH     \
        switch(value)   \
        {               \
        case HEADMASTER:\
            FdogJsonToStruct(*((decltype(getheadmaster()) *)((void *)&struct_ + metainfostruct.memberOffset)), json_);\
            break;      \
        case SCHOOL:    \
            FdogJsonToStruct(*((decltype(getschool()) *)((void *)&struct_ + metainfostruct.memberOffset)), json_);\
            break;      \
        };

static vector<structInfo> structinfo; //所有结构体信息存储于此

/***********************************
*   返回对应的成员类型(包括基本类型和自定义类型)，数组大小
************************************/
valueTyle getValueTyle(string key){  // 改  MemberAttribute  getMemberAttribute
    valueTyle res;
    if(BaseValueType.count(key) != 0){
        res.valueType = BaseValueType[key];  //memberType
        res.ArraySize = 0;                   //memberArraySize
        return res;                          //Attribute_return
    } else if (BasePointerValueType.count(key) != 0){
        res.valueType = BasePointerValueType[key];
        res.ArraySize = 0;
    } else {
        smatch result;
        regex pattern("((A)(\\d+)_\\d?(\\D+))"); //这个改成string PatternArray
        if(regex_search(key, result, pattern)){
            string key_ = result.str(2) + result.str(4);
            if(BaseArrayValueType.count(key_) != 0){
                res.valueType = BaseArrayValueType[key_];
            } else {
                ObjectArrayValueType[key_] = result.str(4) + "_array";
                res.valueType = ObjectArrayValueType[key_];
            }
            res.ArraySize = atoi(result.str(3).data());
            return res;
        } else {
            smatch result;
            regex pattern("((\\d+)(\\D+))");  //这个改成string PatternObject
            if(regex_search(key, result, pattern)){
                string str = result.str(2) + " " + result.str(3);
                res.valueType = result.str(3);
                res.ArraySize = atoi(result.str(3).data());
                return res;
            }
        }
    }
    return res;
}
void RemoveFirstComma(string & return_){  //第一个字符小写，返回值可以为void 或者bool
    return_ = return_.substr(1);
}
/***********************************
*   删除字符串最后一个逗号
************************************/
void RemoveLastComma(string & return_){   //第一个字符小写，返回值可以为void 或者bool
    return_.pop_back();
}

void RemoveLastZero(string & return_){    //删除浮点数后面的0

	// if (NULL == strchr(return_, '.'))
	// {
    //     return;
    // }
	// int length = strlen(return_.c_str());
	// for (int i = length - 1; i > 0; --i)
	// {
	// 	if ('\0' == return_[i])
	// 	{
	// 		continue;
	// 	}
	// 	else if ('0' == return_[i])
	// 	{
	// 		return_[i] = '\0';
	// 	}
	// 	else if ('.' == return_[i])// 小数点之后全为零
	// 	{
	// 		return_[i] = '\0';
	// 		break;
	// 	}
	// 	else// 小数点后有非零数字
	// 	{
	// 		break;
	// 	}
	// }
}
/***********************************
*   查询对应的结构体信息 这里有问题，如果是两个一个的数组就存在问题
************************************/
structInfo & getStructInfo(string structName){
    string::iterator it = structName.begin();
    while (it != structName.end()) {
        if ((*it >= '0') && (*it <= '9')) {
            it = structName.erase(it);
        } else { // 必须加else
            it++;
        }
    }
    for(int i = 0; i < structinfo.size(); i++){
        if (structinfo[i].structType == structName){
            return structinfo[i];
        }
    } //空怎么办
}

template<class T, class M>
void setValueByAddress(string type_str, T &info, int vc, M data, int Size = 0){
    if(type_str == "char"){
        *((char *)((void *)&info + vc)) = data;
    }
    if(type_str == "int"){
        *((int *)((char *)&info + vc)) = data;
    }
    if(type_str == "unsigned_int"){
        *((unsigned int *)((char *)&info + vc)) = data;
    }
    if(type_str == "short_int"){
        *((short int *)((char *)&info + vc)) = data;
    }
    if(type_str == "unsigned_short_int"){
        *((unsigned short int *)((char *)&info + vc)) = data;
    }
    if(type_str == "long_int"){
        *((long int *)((char *)&info + vc)) = data;
    }
    if(type_str == "unsigned_long_int"){
        *((unsigned long int *)((char *)&info + vc)) = data;
    }
    if(type_str == "float"){
        *((float *)((char *)&info + vc)) = data;
    }
    if(type_str == "double"){
        *((double *)((char *)&info + vc)) = data;
    }
    if(type_str == "long_double"){
        *((long double *)((char *)&info + vc)) = data;
    }
}

template<class T, class M>
void setValueByAddress_S(string type_str, T &info, int vc, M data){
    if(type_str == "char_ptr"){
        *((char **)((void *)&info + vc)) = new char[strlen(data)];
        strcpy(*((char **)((void *)&info + vc)), data);
    }
}

/***********************************
*   解析基础类型
************************************/
template<class T>
void JsonToBase(T & struct_, metaInfo & metainfostruct, string json_){
            if(metainfostruct.memberType == "char_ptr"){
                auto value = json_.c_str();
                setValueByAddress_S(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "char"){
                auto value = atoi(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "bool"){
                bool value;
                if(json_ == "true"){
                    value = true;
                }else{
                    value = false;
                }
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "int"){
                auto value = atoi(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "unsigned_int"){
                auto value = atoi(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "short_int"){
                auto value = atoi(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "unsigned_short_int"){
                auto value = atoi(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "long_int"){
                auto value = atol(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "unsigned_long_int"){
                auto value = atol(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "float"){
                auto value = atof(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "double"){
                auto value = atof(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            } 
            else if (metainfostruct.memberType == "long_double"){
                auto value = atof(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
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
    if (metainfostruct.memberType == "headmaster"){
        FdogJsonToStruct(*((decltype(getheadmaster()) *)((void *)&struct_ + metainfostruct.memberOffset)), json_);
    }
    if (metainfostruct.memberType == "school"){
        FdogJsonToStruct(*((decltype(getschool()) *)((void *)&struct_ + metainfostruct.memberOffset)), json_);
    }
    if (metainfostruct.memberType == "student"){
        FdogJsonToStruct(*((decltype(fdog_student) *)((void *)&struct_ + metainfostruct.memberOffset)), json_);
    }
    if (metainfostruct.memberType == "teather"){
        FdogJsonToStruct(*((decltype(fdog_teacher) *)((void *)&struct_ + metainfostruct.memberOffset)), json_);
    }
}
/***********************************
*   解析数组
************************************/
template<class T>
void JsonToArray(T & struct_, metaInfo & metainfostruct, string json_){
    RemoveFirstComma(json_);
    RemoveLastComma(json_);

    map<string, string>::iterator iter;
    iter = BaseArrayValueType.begin();
    bool isBaseArray = false;
    while(iter != BaseArrayValueType.end()) {
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
                    setValueByAddress_S(metainfostruct.memberType.substr(0, subscriptValue), struct_, metainfostruct.memberOffset + (j * 8), pos->str().data());
                }else{
                    setValueByAddress(metainfostruct.memberType.substr(0, subscriptValue), struct_, metainfostruct.memberOffset + (j * 4), atoi(pos->str().data()));
                }

                j++;
            }
    }else{
        for(int i = 0; i < metainfostruct.memberArraySize; i++){
            cout << "数组类型=" << metainfostruct.memberType << "=== 数组大小=" << metainfostruct.memberArraySize << endl;
            //考虑如何分割数组json {},{}
            if (metainfostruct.memberType == "headmaster_array"){
                FdogJsonToStruct(*((decltype(getheadmaster()) *)((void *)&struct_ + metainfostruct.memberOffset + (sizeof(fdog_headmaster) * i))), json_);
            }
            if (metainfostruct.memberType == "school_array"){
                FdogJsonToStruct(*((decltype(getschool()) *)((void *)&struct_ + metainfostruct.memberOffset + (sizeof(fdog_headmaster) * i))), json_);
            }
            if (metainfostruct.memberType == "student_array"){
                FdogJsonToStruct(*((decltype(fdog_student) *)((void *)&struct_ + metainfostruct.memberOffset + (sizeof(fdog_headmaster) * i))), json_);
            }
            if (metainfostruct.memberType == "teather_array"){
                FdogJsonToStruct(*((decltype(fdog_teacher) *)((void *)&struct_ + metainfostruct.memberOffset + (sizeof(fdog_headmaster) * i))), json_);
            }
        }
    }
}


template<class T>
string getValueByAddress_S(string type_str, T &info, int vc){
    if(type_str == "char_ptr"){
        auto value = *((const char **)((void *)&info + vc));
        string str_value = value;
        return "\"" + str_value  + "\"";
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
        //string char_value(1,value);
        return to_string((int)value);
    }
    if(type_str == "unsigned_char"){
        auto value = *((char *)((void *)&info + vc));
        //string char_value(1,value);
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
        //浮点数默认小数点后6位，这里可以截断后面的0
        return to_string(value);
    }
    if(type_str == "double"){
        auto value = *((double *)((char *)&info + vc));
        return to_string(value);
    }
    if(type_str == "long_double"){
        auto value = *((long double *)((char *)&info + vc));
        return to_string(value);
    }
    return "";
}

template<class T>
void BaseToJson(string & json_, metaInfo & metainfostruct, T & struct_){
    cout << "----------BaseToJson---------" << endl;
    string quotationMark = "\"";
    string value;
        if (metainfostruct.memberType == "char_ptr"){
            value = getValueByAddress_S(metainfostruct.memberType, struct_, metainfostruct.memberOffset);
        } else{
            value = getValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset);
        }
        json_ = json_ + quotationMark + metainfostruct.memberName + quotationMark + ":" + value + ",";
}

template<class T>
void ObjectToJson(string & json_, metaInfo & metainfostruct, T & struct_){
    cout << "----------ObjectToJson---------" << metainfostruct.memberType << endl;
    //先搁置
    if (metainfostruct.memberType == "headmaster"){
        FdogStructToJson(json_, *((decltype(getheadmaster()) *)((void *)&struct_ + metainfostruct.memberOffset)));
    }
    if (metainfostruct.memberType == "school"){
        FdogStructToJson(json_, *((decltype(getschool()) *)((void *)&struct_ + metainfostruct.memberOffset)));
    }
    if (metainfostruct.memberType == "student"){
        FdogStructToJson(json_, *((decltype(fdog_student) *)((void *)&struct_ + metainfostruct.memberOffset)));
    }
}

template<class T>
void ArrayToJson(string & json_, metaInfo & metainfostruct, T & struct_){
    string curlyBracketL = "{";
    string curlyBracketR = "}";
    string squareBracketsL = "[";
    string squareBracketsR = "]";
    map<string, string>::iterator iter;
    iter = BaseArrayValueType.begin();
    bool isBaseArray = false;
    while(iter != BaseArrayValueType.end()) {
        if(metainfostruct.memberType == iter->second){
            isBaseArray = true;
            break;
        }
        iter++;
    }
    if (isBaseArray){
        string quotationMark = "\"";
        string json_2;
        json_2 =  json_2 + quotationMark + metainfostruct.memberName + quotationMark + ":" + squareBracketsL;
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
        RemoveLastComma(json_2);
    json_ = json_ + json_2 + squareBracketsR + ",";

    }else{
        json_ = json_ + "\"" + metainfostruct.memberName + "\"" + ":[";
        for(int i = 0; i < metainfostruct.memberArraySize; i++){
            if (metainfostruct.memberType == "headmaster_array"){
                json_ = json_ + "{"; 
                FdogStructToJson(json_, *((decltype(getheadmaster()) *)((void *)&struct_ + metainfostruct.memberOffset + (sizeof(fdog_headmaster) * i))));
                RemoveLastComma(json_);
                json_ = json_ + "},";
            }
            if (metainfostruct.memberType == "school_array"){
                json_ = json_ + "{"; 
                FdogStructToJson(json_, *((decltype(getschool()) *)((void *)&struct_ + metainfostruct.memberOffset + (sizeof(fdog_school) * i))));
                RemoveLastComma(json_);
                json_ = json_ + "},";
            }
            if (metainfostruct.memberType == "student_array"){
                json_ = json_ + "{"; 
                FdogStructToJson(json_, *((decltype(fdog_student) *)((void *)&struct_ + metainfostruct.memberOffset + (sizeof(fdog_student) * i))));
                RemoveLastComma(json_);
                json_ = json_ + "},";
            }
        }
        RemoveLastComma(json_);
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
    iter = BaseValueType.begin();
    bool isObject = true;
    while(iter != BaseValueType.end()) {
        if(type_str == iter->second){
            isObject = false;
            break;
        }
        iter++;
    }
    iter = BasePointerValueType.begin();
    while(iter != BasePointerValueType.end()) {
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
        smatch result_1;
        string regex_key_1 = "(\"" + a.metainfoStruct[i].memberName +"\")";
        string regex_value_1 = BaseRegex[a.metainfoStruct[i].memberType];
        if(regex_value_1 == ""){
            if(Vtype == 2){
                regex_value_1 = "(\\{(.*?)\\})";//OtherRegex[a.metainfoStruct[i].memberType];
            }else{
                regex_value_1 = "(\\[(.*?)\\])";//OtherRegex[a.metainfoStruct[i].memberType];
            }
            
        }
        regex pattern_1(regex_key_1 + ":" +regex_value_1);
        if(regex_search(json_, result_1, pattern_1)){
        auto value = result_1.str(2).c_str();
            switch(Vtype){
                case FDOGBASE:
                    cout << "基础类型" << a.metainfoStruct[i].memberType << "=" << value << endl;
                    JsonToBase(struct_, a.metainfoStruct[i], value); //基础类型 提供数据
                    break;
                case FDOGOBJECT:
                    cout << "对象类型" << a.metainfoStruct[i].memberType << "=" << result_1.str(2).c_str() << endl;
                    JsonToObject(struct_, a.metainfoStruct[i], result_1.str(2).c_str()); 
                    break;
                case FDOGARRAY:
                    cout << "数组类型" << a.metainfoStruct[i].memberType << "=" << result_1.str(2).c_str() << endl;
                    JsonToArray(struct_, a.metainfoStruct[i], result_1.str(2).c_str()); //数组类型 
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
        smatch result_1;
        string regex_key_1 = "(\"" + info.metainfoStruct[i].memberName +"\")";
        string regex_value_1 = BaseRegex[info.metainfoStruct[i].memberType];
        if(regex_value_1 == ""){
            if(Vtype == 2){
                regex_value_1 = "(\\{(.*?)\\})";
            }else{
                regex_value_1 = "(\\[(.*?)\\])";
            }
        }
            switch(Vtype){
                case FDOGBASE:
                    cout << "基础类型" << info.metainfoStruct[i].memberType << endl;
                    BaseToJson(json_, info.metainfoStruct[i], struct_);
                    break;
                case FDOGOBJECT:
                    cout << "对象类型" << info.metainfoStruct[i].memberType << endl;
                    json_ = json_ + "\"" + info.metainfoStruct[i].memberName + "\"" + ":{";
                    ObjectToJson(json_, info.metainfoStruct[i], struct_); 
                    RemoveLastComma(json_);
                    json_ = json_ + "},";
                    break;
                case FDOGARRAY:
                    cout << "数组类型" << info.metainfoStruct[i].memberType << endl;
                    ArrayToJson(json_, info.metainfoStruct[i], struct_); //数组类型
                    break;
            }
    }
    //RemoveLastComma(json_);
    //json_ = curlyBracketL + json_ + curlyBracketR;
}
//序列化 struct=>json
template<typename T>
void FdogSerialize(string & json_, T & struct_){
    FdogStructToJson(json_, struct_);
    RemoveLastComma(json_);
    json_ = "{" + json_ +"}";
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

//获取原始名
#define NAME(x) #x
//获取别名
#define ALIASNAME(x) #x
//自定义变量名生成


//#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)  //获取偏移值

#define MEMBERTYPE(TYPE, MEMBER) getValueTyle(typeid(((TYPE *)0)->MEMBER).name())//获取类型  //可以直接使用函数 去掉MEMBERTYPE

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
        valueTyle res = MEMBERTYPE(TYPE, arg);\
        metainfo_one.memberType = res.valueType;\
        metainfo_one.memberTypeSize = sizeof(TYPE);\
        metainfo_one.memberArraySize = res.ArraySize;\
        metainfoStruct.push_back(metainfo_one);\
    }while(0);


}
#endif
//cout << metainfo_one.memberName << "----pianyizhi:" << metainfo_one.memberOffset << endl;

// template<class Type, class ...Args>
// void register_member(Args... rest){
//     structInfo structinfo_one;
//     structinfo_one.structType = NAME(Type);
//     //递归执行
//     register_member_s()
//     structinfo_one.push_back(structinfo_one);
// }

// template<class Type, class Place, class MetainfoStruct, class Size, class Arg, class ...Args>
// void register_member_s(){
//     metaInfo metainfo_one;
//     metainfo_one.memberName = NAME(Arg);
//     metainfo_one.memberAliasName = ALIASNAME(Arg);
//     metainfo_one.memberOffset = offsetof(Type, Arg);
//     valueTyle res = getValueTyle(typeid(((Type *)0)->Arg).name());
//     metainfo_one.memberType = res.valueType;
//     metainfo_one.memberTypeSize = sizeof(Type);
//     metainfo_one.memberArraySize = res.ArraySize;
//     metainfoStruct.push_back(metainfo_one);
//     //递归本函数
//     register_member_s(type, place, metainfoStruct, size, args...);
// }