#### 一. 介绍

FdogStructJson是一个开源的C++序列化和反序列化快速互转库，支持C++结构体和JSON快速互转等等，简单的非入侵方式，最少三行代码完成转换，使它足够方便。其他功能正在逐步加入，我相信它会变强大。



#### 二. 说明

##### 1. 支持类型：

| 序号 |      基础类型      |          说明          | 序号 |     基础类型      |       说明       |
| :--: | :----------------: | :--------------------: | :--: | :---------------: | :--------------: |
|  1   |        bool        |        布尔类型        |  10  |    signed int     |   有符号短整型   |
|  2   |        char        |      无符号字符型      |  11  |   unsigned int    |   无符号短整型   |
|  3   |    signed char     |      有符号字符型      |  12  |     long int      |      长整型      |
|  4   |   unsigned char    |      无符号字符型      |  13  |  signed long int  |   有符号短整型   |
|  5   |       char *       | 字符指针（字符串类型） |  14  | unsigned long int |   无符号短整型   |
|  6   |     short int      |         短整型         |  15  |       float       |  单精度浮点类型  |
|  7   |  signed short int  |      有符号短整型      |  16  |      double       |  双进度浮点类型  |
|  8   | unsigned short int |      无符号短整型      |  17  |    long double    | 长双进度浮点类型 |
|  9   |        int         |          整型          |      |                   |                  |

基于C++的基础类型，FdogSturctJson定义了如上的17种类型，只要你的结构体中最基本类型在这17种范围之内（事实上大多数结构体的最基本类型都在这里了），便可完成转换，那么如何解释最基本类型呢？

例如：

```
struct headmaster{
	char * name;
	int age;
};

struct student{
	char * name;
	int age;
};

struct teacher{
	char * name;
	int age;
};

struct school{
	char * schoolName;
	headmaster master;
	student stu[1];
	teacher tea[1];
}
```

虽然school这个结构体包含了headmaster，student[]，teacher[]三种自定义类型，但是追述自定义类型，例如headmaster，它包含的最基本类型是char *和int，而student[]是由多个student组成，student是由最基本类型char *和int构成，所以school这个结构体完全可以被正常解析。



#### 三. 使用说明

##### 1. Struct与Json互转

```
//注册结构体
REGISTEREDMEMBER(headmaster, name, age);
REGISTEREDMEMBER(student, name, age);
REGISTEREDMEMBER(teacher, name, age);
REGISTEREDMEMBER(school, schoolName, master, stu, tea);

//完成json转struct
school sch;
std::string json = "{\"schoolName\":\"wyai\",\"master\":\"花狗Fdog\",\"stu\":[{\"name\":\"于静\",\"age\":21},{\"name\":\"二狗\",\"age\":21}],\"tea\":[{\"name\":\"李四\",\"age\":21},{\"name\":\"王五\",\"age\":21}]}";
FdogJsonToStruct(sch, json);

//完成struct转json
school sch_2;
sch_2.schoolName = "wyai";
sch_2.master = "花狗Fdog";
sch_2.stu = {{"于静", 21}, {"二狗", 21}};
sch_2.tea = {{"李四", 21}, {"王五", 21}};
std::string json_2
FdogStructToJson(json_2, sch_2);
```

