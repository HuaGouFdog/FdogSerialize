#include<iostream>
#include<vector>
#include<typeinfo>
#include<regex>
#include "fdogstructjson.h"
using namespace std;
using namespace FdogStruct2Json;

struct Info{
    char * name;
    int    age;
    float  sex;
    double mail;
    int    number[5];
};

int main(){

    Info info;
    REGISTEREDMEMBER(Info, name, age, sex, mail, number);
    FdogJsonToStruct(info,"{\"name\":\"花狗\",\"age\":12,\"sex\":32.6,\"mail\":42.6,\"number\":[1,2,3,4,5]}");
    cout << "名字：" << info.name <<  " 年龄："<< info.age <<" 性别：" << info.sex << " 邮箱：" << info.mail << endl;
    //cout <<"2地址"  << &info.name << " 值：" << info.name << endl;
    Info info2;
    info2.name = "张旭";
    info2.age = 12;
    info2.sex = 12.6;
    info2.mail = 13.0;
    string jsondata;
    FdogStructToJson(jsondata, info2);
    cout << jsondata << endl;

    int a[10] = {0};
    int c;
     cout << typeid(info.number).name();
    return 0;
}
