#ifndef TEST_H
#define TEST_H
#include <iostream>
#include "FStruct.h"
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
	uint8_t		age1;
	uint16_t	age2;
	uint32_t	age3;
	uint64_t	age4;
	int			age5;
	float		age6;
	double		age7;
	std::wstring age8;
};


struct student {
	string name;
	int age;
};

struct teacher {
	string name;
	int age;
};

//假设学校只有两个人
struct school {
	student stu;
	teacher tea;
};

class classtest {
public:
	void sum() {
		cout << "test";
	}
public:
	string name;
	int age;
};

struct class_base_array {
	int numbers[5];
};

struct class_base_vector {
	vector<int> numbers;
};

struct class_object_array {
	student stus[2];
};

struct class_object_vector {
	vector<student> stus;
};

struct class_map
{
	map<string, int> grade;
};

struct class_unordered_map
{
	unordered_map<string, int> grade;
};


struct test_map {
	map<int, int> grade1;
};

#endif