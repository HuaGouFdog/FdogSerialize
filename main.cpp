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

    // REGISTEREDMEMBER(baseTest, bool_b, char_str, char_c, char_s, char_u, int_i, int_s, int_u, short_i, short_s, short_u, long_i, long_s, long_u, long_li, long_ls, long_lu, float_f, double_d, double_l);
    // baseTest test = {true, "花狗", 'A', 'A', 'A', -123, -123, 123, -123, -123, 123, -123, -123, 123, -123, -123, 123, 158.132f, 132.45, 11654.32131};
    // string json_2;
    // FdogSerialize(json_2, test);

    // baseTest test2;
    // FdogDesSerialize(test2, json_2);


    // cout << json_2 << endl;
    // cout << "------" << endl;
    // cout << test.bool_b << endl;
    // cout << test.char_str << endl;
    // cout << test.char_c << endl;
    // cout << test.char_s << endl;
    // cout << test.char_u << endl;
    // cout << test.int_i << endl;
    // cout << test.int_s << endl;
    // cout << test.int_u << endl;
    // cout << test.short_i << endl;
    // cout << test.short_s << endl;
    // cout << test.short_u << endl;
    // cout << test.long_i << endl;
    // cout << test.long_s << endl;
    // cout << test.long_u << endl;
    // cout << test.long_li << endl;
    // cout << test.long_ls << endl;
    // cout << test.long_lu << endl;
    // cout << test.float_f << endl;
    // cout << test.double_d << endl;
    // cout << test.double_l << endl;

    REGISTEREDMEMBER(headmaster, name, age);
    REGISTEREDMEMBER(student, name, age);
    REGISTEREDMEMBER(teacher, name, age);
    REGISTEREDMEMBER(school, schoolName, master, number, stu);
    string json_ = "{\"schoolName\":\"scjxy\",\"master\":{\"name\":\"花狗Fdog\",\"age\":21},\"number\":[1,2,3],\"stu\":[{\"name\":\"于静\",\"age\":21},{\"name\":\"二狗\",\"age\":21}]}";
    school sch;
    FdogDesSerialize(sch, json_);
    cout << "schoolName=" << sch.schoolName << endl;
    cout << "master.name=" << sch.master.name << endl;
    cout << "master.age=" << sch.master.age << endl;
    cout << "number[0]=" << sch.number[0] << endl;
    cout << "number[1]=" << sch.number[1] << endl;
    cout << "number[2]=" << sch.number[2] << endl;
    cout << "stu[0]=" << sch.stu[0].name << "," << sch.stu[0].age << endl;
    cout << "stu[1]=" << sch.stu[1].name << "," << sch.stu[1].age << endl;
    // cout << "tea[0]=" << sch.tea[0].name << "," << sch.tea[0].age << endl;
    // cout << "tea[1]=" << sch.tea[1].name << "," << sch.tea[1].age << endl;
    string json_2 = "";
    FdogSerialize(json_2, sch);
    cout << "json=" << json_2 << endl;
    return 0;
}

//g++ -std=c++11 fdogtype.h fdogstructjson.h main.cpp -o main -w

