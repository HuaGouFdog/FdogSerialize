#include<iostream>
#include<vector>
#include<typeinfo>
#include<regex>
#include "fdogstructjson.h"
using namespace std;
using namespace FdogStruct2Json;

struct Info{
    int age;
    int sex;
    int mail;
};

int main(){

    Info info;
    REGISTEREDMEMBER(Info, age, sex, mail);

    FdogStructToJson(info,"{\"age\":21,\"sex\":32,\"mail\":42}");

    cout << "年龄："<<info.age <<" 性别：" << info.sex << " 邮箱：" << info.mail << endl;

    Info info2;
    info2.age = 11;
    info2.sex = 12;
    info2.mail = 13;

    string jsondata;

    FdogJsonToStruct(jsondata, info2);

    cout << jsondata << endl;

    return 0;
}
