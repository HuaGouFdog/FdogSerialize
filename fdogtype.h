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
#ifndef FDOGTYPE_H
#define FDOGTYPE_H
#include<iostream>
#include<vector>
#include<typeinfo>
#include<regex>
#include<map>
using namespace std;

/***********************************
*            存储基础类型            *
************************************/
map<string, string> BaseValueType = {
            {"b", "bool"},//1111111
            {"c", "char"}, 
            {"h", "char"}, 
            {"a", "char"},
            {"i", "int"}, 
            {"j", "signed_int"}, //1111
            {"j", "unsigned_int"}, 
            {"s", "short_int"},
            {"t", "signed_short_int"}, //1111
            {"t", "unsigned_short_int"}, 
            {"l", "long_int"}, 
            {"m", "signed_long_int"}, //101111
            {"m", "unsigned_long_int"}, 
            {"f", "float"},
            {"d", "double"}, 
            {"e", "long_double"},
};

/***********************************
*         存储基础指针类型
************************************/
map<string, string> BasePointerValueType = {
            {"Pb", "bool_ptr"},//111111
            {"Pc", "char_ptr"},
            {"Ph", "char_ptr"}, 
            {"Pa", "char_ptr"},
            {"Pi", "int_ptr"},
            {"Pj", "signed_int_ptr"},//11111
            {"Pj", "unsigned_int_ptr"},
            {"Ps", "short_int_ptr"},
            {"Pt", "signed_short_int_ptr"}, //111111
            {"Pt", "unsigned_short_int_ptr"},
            {"Pl", "long_int_ptr"},
            {"Pm", "signed_long_int_ptr"},//11111111
            {"Pm", "unsigned_long_int_ptr"},
            {"Pf", "float_ptr"}, 
            {"Pd", "double_ptr"},
            {"Pe", "long_double_ptr"},
};


/***********************************
*         存储基础数组类型
************************************/
map<string, string> BaseArrayValueType = {
            {"Ab", "bool_array"},//111111
            {"Ac", "char_array"},
            {"Ah", "char_array"},
            {"Aa", "char_array"},
            {"Ai", "int_array"},
            {"Aj", "signed_int_array"}, //11111
            {"Aj", "unsigned_int_array"},
            {"As", "short_int_array"},
            {"At", "signed_short_int_array"}, //111111
            {"At", "unsigned_short_int_array"},
            {"Al", "long_int_array"},
            {"Am", "signed_long_int_array"},//11111111
            {"Am", "unsigned_long_int_array"},
            {"Af", "float_array"},
            {"Ad", "double_array"},
            {"Ae", "long_double_array"},
            {"APc", "char_ptr_array"},
};

/***********************************
*   用于存储自定义类型(结构体/对象)
*   自定义类型需要在运行中动态增加，任何类型都会转成基础类型,无需手动添加
************************************/
map<string, string> ObjectValueType;

/***********************************
*   用于存储自定义数组类型
*   自定义类型需要在运行中动态增加，任何类型都会转成基础类型,无需手动添加
************************************/
map<string, string> ObjectArrayValueType;

/***********************************
*   用于存储自定义指针类型
*   自定义类型需要在运行中动态增加，任何类型都会转成基础类型,无需手动添加
************************************/
map<string, string> ObjectPointerValueType;

/***********************************
*   用于提取基础类型数据的正则表达式
************************************/
map<string, string> BaseRegex = {
            {"char", "(\\d+)"}, 
            {"char", "(\\d+)"}, 
            {"char", "(\\d+)"},
            {"int", "(\\d+)"}, 
            {"unsigned_int", "(\\d+)"},
            {"short_int", "(\\d+)"}, 
            {"unsigned_short_int", "(\\d+)"}, 
            {"long", "(\\d+)"}, 
            {"unsigned_long", "(\\d+)"}, 
            {"float", "(\\d+.\\d+)"}, 
            {"double", "(\\d+.\\d+)"},
            {"long_double", "(\\d+.\\d+)"},
            {"char_ptr", "\"(.*?)\""},
            {"int_array", "(\\[(.*?)\\])"},
};

/***********************************
*   用于提取除基础类型数据的正则表达式
************************************/
map<string, string> OtherRegex = {
            {"char_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"char_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"char_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"int_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"unsigned_int_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"short_int_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"unsigned_short_int_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"long_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"unsigned_long_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"float_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"double_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"long_double_ptr", "\"(.*?)\""},{"int_array", "\\[(.*?)\\]"},
            
};

//增加编译时获取对应平台的值
map<string, int> TypeSize;

/***********************************
*   存储结构体元信息(成员名， 成员别名（默认空，若有内容则使用别名）， 成员所在偏移量， 
*   成员类型， 成员类型位数， 数组大小（默认0，若是数组，则存储对应大小）)
************************************/
typedef struct metaInfo{
    string memberName;
    string memberAliasName;
    size_t memberOffset;
    string memberType;
    size_t memberTypeSize;                                                         
    size_t memberArraySize;                                                                                                                                                                                           
}metaInfo;

/***********************************
*   存储结构体结构体类型，以及元信息
************************************/
typedef struct structInfo{
    string structType;  //结构体类型
    vector<metaInfo> metainfoStruct;  //结构体元信息 
}structInfo;

/***********************************
*   存储成员类型，数组大小
************************************/
struct valueTyle {
    string valueType;
    int valueTypeSize;
    int ArraySize;
};

#endif