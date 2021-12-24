#include<iostream>
#include<vector>
#include<typeinfo>
#include<regex>
#include "fdogstructjson.h"
#include "fdogtype.h"
using namespace std;
using namespace fsj;

// struct app {
//     char * a;
//     int b;
// };

// struct cpp {
//     app c[2];
// };

// template<class T, class arg>
// void func(){
//     cout << typeid(T).name() << endl;
// }

int main(){

    REGISTEREDMEMBER(baseTest, bool_b, char_str, char_c, char_s, char_u, int_i, int_s, int_u, short_i, short_s, short_u, long_i, long_s, long_u, long_li, long_ls, long_lu, float_f, double_d, double_l);
    baseTest test = {true, "花狗", 'A', 'A', 'A', -123, -123, 123, -123, -123, 123, -123, -123, 123, -123, -123, 123, 158.132f, 132.45, 11654.32131};
    string json_2;
    FdogSerialize(json_2, test);
    cout << json_2 << endl;


    // REGISTEREDMEMBER(headmaster, name, age);
    // REGISTEREDMEMBER(student, name, age);
    // REGISTEREDMEMBER(teacher, name, age);
    // REGISTEREDMEMBER(school, schoolName, master, number, stu);
    //string json_ = "{\"schoolName\":\"wyai\",\"master\":{\"name\":\"花狗Fdog\",\"age\":21},\"number\":[1,2,3],\"stu\":[{\"name\":\"于静\",\"age\":21},{\"name\":\"二狗\",\"age\":21}]}";
    //,\"tea\":[{\"name\":\"李四\",\"age\":21},{\"name\":\"王五\",\"age\":21}]}";
    // school sch;
    // FdogJsonToStruct(sch, json_);
    // cout << "schoolName=" << sch.schoolName << endl;
    // cout << "master.name=" << sch.master.name << endl;
    // cout << "master.age=" << sch.master.age << endl;
    // cout << "number[0]=" << sch.number[0] << endl;
    // out << "number[1]=" << sch.number[1] << endl;
    // cout << "number[2]=" << sch.number[2] << endl;
    // cout << "stu[0]=" << sch.stu[0].name << "," << sch.stu[0].age << endl;
    // cout << "stu[1]=" << sch.stu[1].name << "," << sch.stu[1].age << endl;
    // cout << "tea[0]=" << sch.tea[0].name << "," << sch.tea[0].age << endl;
    // cout << "tea[1]=" << sch.tea[1].name << "," << sch.tea[1].age << endl;
    // string json_2 = "";
    // FdogStructToJson(json_2, sch);
    // cout << "json=" << json_ << endl;
    // RemoveLastComma(json_2);
    // json_2 = "{" + json_2 + "}";
    // cout << "json=" << json_2 << endl;c

    // cpp cP;
    // cP.c[0] = {"于静", 21};
    // cout << cP.c[0].a << endl;
    // cout << typeid(bool *).name() << endl;
    // cout << typeid(char).name() << endl;
    // cout << typeid(signed char).name() << endl;
    // cout << typeid(unsigned char).name() << endl;
    // cout << typeid(int).name() << endl;
    // cout << typeid(signed int).name() << endl;
    // cout << typeid(unsigned int).name() << endl;
    // cout << typeid(short int).name() << endl;
    // cout << typeid(signed short int).name() << endl;
    // cout << typeid(unsigned  short int).name() << endl;
    // cout << typeid(long int).name() << endl;
    // cout << typeid(signed long int).name() << endl;
    // cout << typeid(unsigned long int).name() << endl;
    // cout << typeid(long long int).name() << endl;
    // cout << typeid(signed long long int).name() << endl;
    // cout << typeid(unsigned long long int).name() << endl;
    return 0;
}

//g++ -std=c++11 fdogtype.h fdogstructjson.h main.cpp -o main -w

