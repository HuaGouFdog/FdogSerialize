#ifndef TEST_H
#define TEST_H
#include <iostream>
#ifdef __GNUC__
#include "../FStruct.h"
#elif _MSC_VER
#include "../FStruct.h"
#endif
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

// class student;
// template<> struct TagDispatchTrait<student> {
// 	using Tag = BaseTag;
// };

F_CLASS(Yujin2) {
public:
	int age;
	Yujin2() {
		Yujin2::InitFdogSerialize();
	}
	REGISTERED_MEMBER_C(Yujin2, age);
};

F_CLASS(Yujin) {
public:
	float * age;
	vector<Yujin2> ages;
	Yujin2 yujin2;
	Yujin(): age(NULL) {
		Yujin::InitFdogSerialize();
	}
	REGISTERED_MEMBER_C(Yujin, age, ages, yujin2);
};

F_CLASS(student) {
public:
	string name;
	int age;
	student(){
		student::InitFdogSerialize();
	}
	REGISTERED_MEMBER_C(student, name, age);
};

// class teacher;
F_CLASS(teacher) {
public:
	string name;
	int age;
	REGISTERED_MEMBER_S(teacher, name, age);
};


//假设学校只有两个人
F_CLASS(school) {
public:
	student stu;
	teacher tea;
	vector<student> stuList;
	vector<int>  intTesst;
	vector<long>  intTesst2;
	map<int, student> intMap;
	map<int, int> intMap2;
	REGISTERED_MEMBER_S(school, stu, tea, stuList, intTesst, intTesst2, intMap,intMap2);
};
// template<> struct TagDispatchTrait<vector<student>> {using Tag = ArrayTag;};
// template<> struct TagSTLType<vector<student>> {using Tag = InitStructTag;};


//map容器中包含自定义类型需要添加REGISTERE_CONTAINER_M宏
//
REGISTERE_CONTAINER_M(int,student)


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
	REGISTEREDMEMBER(class_unordered_map, grade);
};


struct test_map {
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