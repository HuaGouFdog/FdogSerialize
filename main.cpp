#include <iostream>
#include "macrodefinition.h"
//#include "fdogserialize.h"
#include <typeinfo>
#include <iostream>
using namespace std;

struct school{
    char * name;
    int age;
};

int main()
{
    REGISTEREDMEMBER(school, name, age);
    school sch;
    string json_ = "{\"name\":\"张旭\",\"age\":23}";
    FdogSerialize::Instance()->DesSerialize(sch, json_);
    cout << json_ << endl;
    cout << sch.name << "--" << sch.age << endl;
    string json_2 ="";
    FdogSerialize::Instance()->Serialize(json_2, sch);
    cout << json_2 << endl;
    return 0;
}

//g++ -std=c++11 utils.cpp fdogserializebase.cpp fdogserialize.cpp macrodefinition.h main.cpp -o main -w