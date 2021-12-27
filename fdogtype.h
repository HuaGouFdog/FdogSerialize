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
*         基础存储基础类型            *
************************************/
map<string, string> baseType = {
            {"b", "bool"},
            {"c", "char"},
            {"h", "signed_char"},
            {"a", "unsigned_char"},
            {"i", "int"},
            {"i", "signed_int"},
            {"j", "unsigned_int"},
            {"s", "short_int"},
            {"s", "signed_short_int"},
            {"t", "unsigned_short_int"}, 
            {"l", "long_int"}, 
            {"l", "signed_long_int"},
            {"m", "unsigned_long_int"}, 
            {"x", "long_long_int"}, 
            {"x", "signed_long_long_int"},
            {"y", "unsigned_long_long_int"}, 
            {"f", "float"},
            {"d", "double"}, 
            {"e", "long_double"},
};

/***********************************
*         存储基础指针类型            *
************************************/
map<string, string> basePointerType = {
            {"Pb", "bool_ptr"},
            {"Pc", "char_ptr"},
            {"Ph", "signed_char_ptr"}, 
            {"Pa", "unsigned_char_ptr"},
            {"Pi", "int_ptr"},
            {"Pi", "signed_int_ptr"},
            {"Pj", "unsigned_int_ptr"},
            {"Ps", "short_int_ptr"},
            {"Ps", "signed_short_int_ptr"},
            {"Pt", "unsigned_short_int_ptr"},
            {"Pl", "long_int_ptr"},
            {"Pl", "signed_long_int_ptr"},
            {"Pm", "unsigned_long_int_ptr"},
            {"Px", "long_long_int_ptr"}, 
            {"Px", "signed_long_long_int_ptr"},
            {"Py", "unsigned_long_long_ptr"}, 
            {"Pf", "float_ptr"}, 
            {"Pd", "double_ptr"},
            {"Pe", "long_double_ptr"},
};


/***********************************
*         存储基础数组类型
************************************/
map<string, string> baseArrayType = {
            {"Ab", "bool_array"},
            {"Ac", "char_array"},
            {"Ah", "char_array"},
            {"Aa", "char_array"},
            {"Ai", "int_array"},
            {"Ai", "signed_int_array"},
            {"Aj", "unsigned_int_array"},
            {"As", "short_int_array"},
            {"As", "signed_short_int_array"},
            {"At", "unsigned_short_int_array"},
            {"Al", "long_int_array"},
            {"Al", "signed_long_int_array"},
            {"Am", "unsigned_long_int_array"},
            {"Ax", "long_long_int_array"}, 
            {"Ax", "signed_long_long_int_array"},
            {"Ay", "unsigned_long_long_int_array"}, 
            {"Af", "float_array"},
            {"Ad", "double_array"},
            {"Ae", "long_double_array"},
            {"APc", "char_ptr_array"},
};

/***********************************
*   用于存储自定义类型(结构体/对象)
*   自定义类型需要在运行中动态增加，任何类型都会转成基础类型,无需手动添加
************************************/
map<string, string> objectType;

/***********************************
*   用于存储自定义数组类型
*   自定义类型需要在运行中动态增加，任何类型都会转成基础类型,无需手动添加
************************************/
map<string, string> objectArrayType;

/***********************************
*   用于存储自定义指针类型
*   自定义类型需要在运行中动态增加，任何类型都会转成基础类型,无需手动添加
************************************/
map<string, string> objectPointerType;

/***********************************
*   用于提取基础类型数据的正则表达式
************************************/
map<string, string> baseRegex = {
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
            //{"int_array", "(\\[(.*?)\\])"},
};

//用于数组整体提取
const string arrayRegex = "(\\[(.*?)\\])";
//匹配数组
const string patternArray = "((A)(\\d+)_\\d?(\\D+))";

//匹配别名
const string patterAlias = "(\\(A:(.*?)\\))";

//用于结构体整体提取
const string objectRegex = "(\\{(.*?)\\})";
//匹配结构体
const string patternObject = "((\\d+)(\\D+))";

const string curlyBracketL = "{";
const string curlyBracketR = "}";
const string squareBracketsL = "[";
const string squareBracketsR = "]";
const string aQuotationMark = "\"";

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
struct memberAttribute {
    string valueType;
    int ArraySize;
};

struct Student{
    int name;
    int mail;
};

struct Bxy{
    int dd;
};

struct Info{
    char * name;
    char zimu;
    int    age;
    float  sex;
    double mail;
    int    number[5];
    Student stu;
    int    addr;
};

struct headmaster{
	char * name;
	int age;
};

struct student{
	char * name;
	int age;
};

struct teacher{
	char * name;
	int age;
};

struct school{
	char * schoolName;
	headmaster master;
    int number [3];
	student stu[2];
	//teacher tea[1];
};

//基础类型测试
struct baseTest{
    bool bool_b;
    char * char_str;
    char char_c;
    signed char char_s;
    unsigned char char_u;
    int int_i;
    signed int int_s;
    unsigned int int_u;
    short int short_i;
    signed short int short_s;
    unsigned short int short_u;
    long int long_i;
    signed long int long_s;
    unsigned long int long_u;
    long long int long_li;
    signed long long int long_ls;
    unsigned long long int long_lu;
    float float_f;
    double double_d;
    long double double_l;
};

#endif