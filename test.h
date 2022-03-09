#ifndef TEST_H
#define TEST_H
#include <iostream>
#include "FStruct.h"
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

// const string comma = ",";
// const string arrowL = "<";
// const string arrpwR = ">";
// const string curlyBracketL = "{";
// const string curlyBracketR = "}";
// const string squareBracketsL = "[";
// const string squareBracketsR = "]";
// const string aQuotationMark = "\"";

struct student{
    char * name;
    int age;
};

struct school{
    int id;
    student stu;
};

struct zhang {
    int age;
    vector<int> a;
    map<int,int> b;
};

#endif