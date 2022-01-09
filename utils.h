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

/***********************************
*   删除字符串第一个字符
************************************/
void removeFirstComma(string & return_){
    return_ = return_.substr(1);
}

// /***********************************
// *   删除字符串最后一个字符
// ************************************/
void removeLastComma(string & return_){
    return_.pop_back();
}

// /***********************************
// *   删除浮点数后面多余的0
// ************************************/
template<class T>
string removeLastZero(T & return_){
    std::ostringstream oss; 
    oss << return_;
    return oss.str();
}

// /***********************************
// *   删除字符串的数字 这个有一些问题，名字中不能包含数字，要改
// ************************************/
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

//根据参数生成数据


#endif