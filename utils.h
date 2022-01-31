#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

const string comma = ",";
const string arrowL = "<";
const string arrpwR = ">";
const string curlyBracketL = "{";
const string curlyBracketR = "}";
const string squareBracketsL = "[";
const string squareBracketsR = "]";
const string aQuotationMark = "\"";

struct student{
    char * name;
    int age;
};
struct school{
    char * name;
    int age;
    student stu;
};

//测试数据

//基础类型

int baseInt = 10;

//基础数组类型
int baseArray[5] = { 1,2,3,4,5 };

//结构体

struct baseObject {
	char * name;
	int age;
};

baseObject bo = {"张旭"}














#endif