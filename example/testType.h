#ifndef TEST_H
#define TEST_H
#include <iostream>
#include "../FStruct.h"
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <unordered_map>
//#include <stdint.h>
using namespace std;

struct TestType
{
    uint8_t age1;
    uint16_t age2;
    uint32_t age3;
    uint64_t age4;
    int      age5;
    float    age6;
    double   age7;
    std::wstring age8;
};

class student{
    public:
    string name;
    int age;
    REGISTEREDMEMBER(student, name, age);
    //REGISTEREDMEMBER_ARRAY(student, name, age);
};
// student::init();
// teacher::init();
// school::init();

class teacher{
    public:
    string name;
    int age;
    REGISTEREDMEMBER(teacher, name, age);
    //REGISTEREDMEMBER_ARRAY(teacher, name, age);
};

//假设学校只有两个人
class school{
    public:
    student stu;
    teacher tea;
    vector<student> stuList;
    //list<teacher> teaList;
    REGISTEREDMEMBER(school, stu, tea, stuList);
    //REGISTEREDMEMBER_ARRAY(school, stu, tea);
};TAGDISPATCH_LIST(student)


class classtest{
public:
    void sum(){
        cout << "test";
    }
public:
    string name;
    int age;
};

struct class_base_array{
    int numbers[5];
};

struct class_base_vector{
    vector<int> numbers;
};

struct class_object_array{
    student stus[2];
};

struct class_object_vector{
    vector<student> stus;
};

struct class_map
{
    map<string, int> grade;
};

struct class_unordered_map
{
    unordered_map<string, int> grade;
    REGISTEREDMEMBER(class_unordered_map, grade);
};


struct test_map{
    map<int, int> grade1;
};

// template<> struct TagDispatchTrait_zx<student> {
//     using Tag = BaseTag_zx;
// };

// template<> struct TagDispatchTrait_zx<vector<student>> {
//     using Tag = BaseTag_zx;
// };


// template<> struct TagDispatchTrait_zx<vector<student>> {
//     using Tag = ArrayTag;
// };
// template<> struct TagDispatchTrait_zx<map<student>> {
//     using Tag = MapTag;
// };
// template<> struct TagDispatchTrait_zx<unordered_map<student>> {
//     using Tag = MapTag;
// };

// template<> struct TagDispatchTrait_zx<vector<teacher>> {
//     using Tag = ArrayTag;
// };
// template<> struct TagDispatchTrait_zx<map<teacher>> {
//     using Tag = MapTag;
// };
// template<> struct TagDispatchTrait_zx<unordered_map<teacher>> {
//     using Tag = MapTag;
// };

// template<> struct TagDispatchTrait_zx<vector<school>> {
//     using Tag = ArrayTag;
// };
// template<> struct TagDispatchTrait_zx<map<school>> {
//     using Tag = MapTag;
// };
// template<> struct TagDispatchTrait_zx<unordered_map<school>> {
//     using Tag = MapTag;
// };

#endif