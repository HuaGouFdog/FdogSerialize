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

#define REGISTERED(TYPE) extern TYPE NAMESPLIT(TYPE);

REGISTERED(headmaster);
REGISTERED(school);

static vector<structInfo> structinfo; //所有结构体信息存储于此

/***********************************
*   返回对应的成员类型(包括基本类型和自定义类型)，数组大小
************************************/
valueTyle getValueTyle(string key){
    valueTyle res;
    if(BaseValueType.count(key) != 0){
        res.valueType = BaseValueType[key];
        res.ArraySize = 0;
        return res;
    } else if (BasePointerValueType.count(key) != 0){
        res.valueType = BasePointerValueType[key];
        res.ArraySize = 0;
    } else {
        smatch result;
        regex pattern("((A)(\\d+)_\\d?(\\D+))");
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
            regex pattern("((\\d+)(\\D+))");
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
void RemoveFirstComma(string & return_){
    return_ = return_.substr(1);
}
/***********************************
*   删除字符串最后一个逗号
************************************/
void RemoveLastComma(string & return_){
    return_.pop_back();
}

void RemoveLastZero(string & return_){

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
    }
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
void JsonToObject(T & struct_, string json_){
    //这里struct代表的是最大的对象，需要解析最小的
    //解析对象先搁置
    cout << "进入JsonToObject" << "T=" << typeid(struct_).name() << " json=" << json_ << endl;
    struct_.name = "321";
    struct_.age = 21;
    FdogJsonToStruct(struct_, json_);
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
            //解析对象数组先搁置
            FdogJsonToStruct(struct_, json_);
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
    // if(type_str == "char"){
    //     auto value = *((char *)((void *)&info + vc));
    //     string char_value(1,value);
    //     return char_value;
    // }
    // if(type_str == "char"){
    //     auto value = *((char *)((void *)&info + vc));
    //     return value;
    // }

    if(type_str == "int"){
        auto value = *((int *)((char *)&info + vc));
        return to_string(value);
    }
    //浮点数默认小数点后6位，这里可以截断后面的0
    if(type_str == "float"){
        auto value = *((float *)((char *)&info + vc));
        return to_string(value);
    }
    if(type_str == "double"){
        auto value = *((double *)((char *)&info + vc));
        return to_string(value);
    }
    return "";
}

template<class T>
void BaseToJson(string & json_, metaInfo & metainfostruct, T & struct_){
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
    //先搁置
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
        for(int i = 0; i < metainfostruct.memberArraySize; i++){
            //解析对象数组先搁置
            //FdogJsonToStruct(struct_, json_);
        }
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
        //cout << "regex_key_1=" << regex_key_1;
        //cout << "  regex_value_1=" << a.metainfoStruct[i].memberType << " - - "<< regex_value_1 << endl;
        regex pattern_1(regex_key_1 + ":" +regex_value_1);
        if(regex_search(json_, result_1, pattern_1)){
            //cout << "-----" << result_1.str(2).c_str() << endl;
        auto value = result_1.str(2).c_str();
            switch(Vtype){
                case FDOGBASE:
                    //cout << "基础类型" << a.metainfoStruct[i].memberType << "=" << value << endl;
                    //规定如果是基础类型，只需要提供值即可
                    JsonToBase(struct_, a.metainfoStruct[i], value); //基础类型 提供数据
                    break;
                case FDOGOBJECT:
                /*    这里不知道什么原因，value接受的是空值      */
                    {cout << "对象类型" << a.metainfoStruct[i].memberType << "=" << result_1.str(2).c_str() << endl;
                    cout << __FILE__ << "----" << __LINE__ << "----" << __FUNCTION__ << endl;
                    
                    decltype(fdog_headmaster) & cc = *((decltype(fdog_headmaster) *)((void *)&struct_ + a.metainfoStruct[i].memberOffset));
                    cout << "headmaster地址：" << &cc << "--" << typeid(cc).name() << endl;
                    cout << "偏移值" << offsetof(decltype(fdog_school), master) << endl;
                    JsonToObject(cc, result_1.str(2).c_str()); //object类型 本质还是结构体，需要继续调用FdogJsonToStruct
                    break;}
                case FDOGARRAY:
                    //cout << "数组类型" << a.metainfoStruct[i].memberType << "=" << result_1.str(2).c_str() << endl;
                    JsonToArray(struct_, a.metainfoStruct[i], result_1.str(2).c_str()); //数组类型 
                    break;
            }
        }
    }
}

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
        //cout << "类型值：" << Vtype << endl;
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
        //cout << "regex_value_1" << regex_value_1 << endl;
        regex pattern_1(regex_key_1 + ":" +regex_value_1);
        auto value = "dsa";//result_1.str(2).c_str();
            switch(Vtype){
                case FDOGBASE:
                    //cout << "基础类型" << info.metainfoStruct[i].memberType << "=" << value << endl;
                    BaseToJson(json_, info.metainfoStruct[i], struct_);
                    //cout << "json_------ --- -- -- " << json_ << endl;
                    break;
                case FDOGOBJECT:
                    //cout << "对象类型" << info.metainfoStruct[i].memberType << "=" << value << endl;
                    //ObjectToJson(json_, info.metainfoStruct[i], struct_); 
                    break;
                case FDOGARRAY:
                    //cout << "数组类型" << info.metainfoStruct[i].memberType << "=" << value << endl;
                    ArrayToJson(json_, info.metainfoStruct[i], struct_); //数组类型 
                    break;
            }
        //}
    }
    RemoveLastComma(json_);
    json_ = curlyBracketL + json_ + curlyBracketR;
}

#define ARG_N(...) \
    ARG_N_(0, ##__VA_ARGS__, ARG_N_RESQ()) 

#define ARG_N_(...) \
    ARG_N_M(__VA_ARGS__)

#define ARG_N_M(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, N,...) N

#define ARG_N_RESQ() 10,9,8,7,6,5,4,3,2,1,0

//获取原始名
#define NAME(x) #x
//获取别名
#define ALIASNAME(x) #x
//自定义变量名生成


//#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)  //获取偏移值

#define MEMBERTYPE(TYPE, MEMBER) getValueTyle(typeid(((TYPE *)0)->MEMBER).name())//获取类型

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
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1);

#define REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg) \
    do{\
        metaInfo metainfo_one;\
        metainfo_one.memberName = NAME(arg);\
        metainfo_one.memberAliasName = ALIASNAME(arg);\
        metainfo_one.memberOffset = offsetof(TYPE, arg);\
        cout << metainfo_one.memberName << "----pianyizhi:" << metainfo_one.memberOffset << endl;\
        valueTyle res = MEMBERTYPE(TYPE, arg);\
        metainfo_one.memberType = res.valueType;\
        metainfo_one.memberTypeSize = 1;\
        metainfo_one.memberArraySize = res.ArraySize;\
        metainfoStruct.push_back(metainfo_one);\
    }while(0);


}
#endif
