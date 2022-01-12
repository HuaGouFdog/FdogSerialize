#include <iostream>
#include "macrodefinition.h"
//#include "fdogserialize.h"
#include <typeinfo>
#include <iostream>
#include <list>
using namespace std;

void FdogSerializeInit(){
    REGISTEREDMEMBER(student, name, age);
    REGISTEREDMEMBER(school, name, age, stu);
}

int main()
{
    FdogSerializeInit();
    school sch;
    string json_ = "{\"name\":\"张旭\",\"age\":23,\"stu\":{\"name\":\"yujing\",\"age\":21}}";
    string json_2 ="";
    //cout << "地址：" << &(sch.stu) << "--"<< &(sch.stu.name) << "--" << &(sch.stu.age) << endl;
    FdogSerialize::Instance()->FDesSerialize(sch, json_);
    cout << json_ << endl;
    //cout << sch.name << "--" << sch.age << "--" << sch.stu.name << "--"  << sch.stu.age << endl;
    FdogSerialize::Instance()->FSerialize(json_2, sch);
    cout << json_2 << endl;
    return 0;
}

//g++ -std=c++11 utils.cpp fdogserializebase.cpp fdogserialize.cpp macrodefinition.h main.cpp -o main -w