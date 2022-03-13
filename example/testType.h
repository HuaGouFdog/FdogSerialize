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
    char * name;
    int age;
};

struct school{
    int id;
    student stu;
};

struct zhang {
    int age;
    vector<int> a;
    map<int,int> b;
    list<int> c;
    deque<int> d;
    set<int> e;
};


#endif