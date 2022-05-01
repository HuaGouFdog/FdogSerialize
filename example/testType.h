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
using namespace std;

struct student{
    string name;
    int age;
};

struct teacher{
    string name;
    int age;
};

//假设学校只有两个人
struct school{
    student stu;
    teacher tea;
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

// struct zhang {
//     int age;
//     vector<char *> a;
//     map<int,int> b;
//     list<char *> c;
//     deque<char *> d;
//     set<char *> e;
//     int f[2];
// };

// struct text {
//     string a[2];
//     //string b[2];
// };

#endif