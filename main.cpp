#include <iostream>
#include "fdogserialize.h"
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
    //cool sch;
    //list<int> a = {1,2,3,4,5,6};

    //list<student> stu {{"zhangxv",21},{"yujing",22},{"huruim",23}};

    // map<string, int> mapstu{
    //     {"zhangxv",21},
    //     {"zhangd",32},
    // };
    // string json_ = "{\"age\":31,\"fdog\":{\"name\":\"张旭\",\"age\":23,\"stu\":{\"name\":\"yujing\",\"age\":21}}}";
    student stu;
    stu.name = "花狗Fdog";
    stu.age = 22;
    string json_2 ="";
    FdogSerialize::Instance()->setIgnoreField("student", "name");
    FdogSerialize::Instance()->setIgnoreField("student", "age");
    FdogSerialize::Instance()->FSerialize(json_2, stu);
    cout << json_2 << endl;

    // time_t begin,end;
    // double ret;
    // begin=clock();
    // for(int i = 0; i< 100000; i++){
    // }
    // end=clock();
    // ret=double(end-begin)/CLOCKS_PER_SEC;
    // cout<<"runtime:   "<<ret<<endl;
    return 0;
}
//先找vrctor map list三个关键字
//再找是不是包含[数字]


//g++ -std=c++11 utils.cpp fdogserializebase.cpp fdogserialize.cpp macrodefinition.h main.cpp -o main -w