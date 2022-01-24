#include <iostream>
#include "macrodefinition.h"
//#include "fdogserialize.h"
#include <typeinfo>
#include <iostream>
#include <list>
using namespace std;

struct cool {
    int age;
    school fdog;
};
void FdogSerializeInit(){
    REGISTEREDMEMBER(student, name, age);
    REGISTEREDMEMBER(school, name, age, stu);
    REGISTEREDMEMBER(cool, age, fdog);
}



int main()
{
    FdogSerializeInit();
    cool sch;
    list<int> a = {1,2,3,4,5,6};

    list<student> stu {{"zhangxv",21},{"yujing",22},{"huruim",23}};

    map<string, int> mapstu{
        {"zhangxv",21},
        {"zhangd",32},
    };
    string json_ = "{\"age\":31,\"fdog\":{\"name\":\"张旭\",\"age\":23,\"stu\":{\"name\":\"yujing\",\"age\":21}}}";
    string json_2 ="";
    //cout << "地址：" << &(sch.stu) << "--"<< &(sch.stu.name) << "--" << &(sch.stu.age) << endl;
    // FdogSerialize::Instance()->FDesSerialize(sch, json_);
    // cout << json_ << endl;
    // cout << sch.age << "--" << sch.fdog.age << "--" << sch.fdog.name << "--"  << sch.fdog.stu.age << "--"  << sch.fdog.stu.name << endl;
    int a2 = 10;
    //cout << *(int *)( + (sizeof(int) * 0));

    //FdogSerialize::Instance()->FSerialize(json_2, a, &a[0], a.size(), "a");
    //FdogSerialize::Instance()->FSerialize(json_2, a2);
    FdogSerialize::Instance()->FSerializeM(json_2, mapstu, "mapstu");
    //FdogSerialize::Instance()->FSerialize(json_2, *(int *)((void *)&a[0] + (sizeof(int) * 0)));
    //vector list
    //map
    cout << json_2 << endl;
    //cout << &a << "---" << &a[0] << "---" << &a[1] << "---" << &a[2] << "---" << &a[3] << "---" << &a[4] << endl;
    // vector<int> a;
    // cout << sizeof(a)<< endl;
    //cout << FdogSerialize::Instance()->isStructType("school", abi::__cxa_demangle(typeid(sch).name(),0,0,0)) << endl;
    return 0;
}
//先找vrctor map list三个关键字
//再找是不是包含[数字]


//g++ -std=c++11 utils.cpp fdogserializebase.cpp fdogserialize.cpp macrodefinition.h main.cpp -o main -w