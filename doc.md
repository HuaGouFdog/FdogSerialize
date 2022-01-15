
- [使用说明](#%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E)
  - [1. 不包含结构体嵌套的转换](#1-%E4%B8%8D%E5%8C%85%E5%90%AB%E7%BB%93%E6%9E%84%E4%BD%93%E5%B5%8C%E5%A5%97%E7%9A%84%E8%BD%AC%E6%8D%A2)
  - [2. 包含结构体嵌套的转换](#2-%E5%8C%85%E5%90%AB%E7%BB%93%E6%9E%84%E4%BD%93%E5%B5%8C%E5%A5%97%E7%9A%84%E8%BD%AC%E6%8D%A2)
  - [3. 使用别名](#3-%E4%BD%BF%E7%94%A8%E5%88%AB%E5%90%8D)
### 使用说明


#### 1. 不包含结构体嵌套的转换

```cpp
//该结构体包含基础类型
struct baseTest{
    bool bool_b;
    char * char_str;
    char char_c;
    signed char char_s;
    unsigned char char_u;
    int int_i;
    signed int int_s;
    unsigned int int_u;
    short int short_i;
    signed short int short_s;
    unsigned short int short_u;
    long int long_i;
    signed long int long_s;
    unsigned long int long_u;
    long long int long_li;
    signed long long int long_ls;
    unsigned long long int long_lu;
    float float_f;
    double double_d;
    long double double_l;
};

//注册
REGISTEREDMEMBER(baseTest, bool_b, char_str, char_c, char_s, char_u, int_i, int_s, int_u, short_i, short_s, short_u, long_i, long_s, long_u, long_li, long_ls, long_lu, float_f, double_d, double_l);

//调用接口
baseTest test = {true, "花狗", 'A', 'A', 'A', -123, -123, 123, -123, -123, 123, -123, -123, 123, -123, -123, 123, 158.132f, 132.45, 11654.32131};
string json_1;
FdogSerialize(json_1, test);
```

输出：

```JSON
{
    "bool_b":true,
    "char_str":"花狗",
    "char_c":65,
    "char_s":65,
    "char_u":65,
    "int_i":-123,
    "int_s":-123,
    "int_u":123,
    "short_i":-123,
    "short_s":-123,
    "short_u":123,
    "long_i":-123,
    "long_s":-123,
    "long_u":123,
    "long_li":-123,
    "long_ls":-123,
    "long_lu":123,
    "float_f":158.132004,
    "double_d":132.45,
    "double_l":11654.32131
}
```



---

#### 2. 包含结构体嵌套的转换

相当于普通结构体的转换，包含结构体嵌套的结构体需要多做一步

提供了下面四个宏定义，默认是空。

- REGISTERED_OBJECT_SERIALIZE_ALL ：如若存在嵌套结构体需要序列化应在此添加
- REGISTERED_OBJECT_ARRAY_SERIALIZE_ALL：如若存在嵌套结构体数组需要序列化应在此添加
- REGISTERED_OBJECT_DESSERIALIZE_ALL：如若存在嵌套结构体需要反序列化应在此添加
- REGISTERED_OBJECT_ARRAY_DESSERIALIZE_ALL：如若存在嵌套结构体数组需要反序列化应在此添加

按需填写即可

```cpp
#define REGISTERED_OBJECT_SERIALIZE_ALL\
    REGISTERED_OBJECT_SERIALIZE(headmaster)\
    REGISTERED_OBJECT_SERIALIZE(school)\
    REGISTERED_OBJECT_SERIALIZE(teacher)\
    REGISTERED_OBJECT_SERIALIZE(student)\

#define REGISTERED_OBJECT_ARRAY_SERIALIZE_ALL\
    REGISTERED_OBJECT_ARRAY_SERIALIZE(headmaster)\
    REGISTERED_OBJECT_ARRAY_SERIALIZE(school)\
    REGISTERED_OBJECT_ARRAY_SERIALIZE(teacher)\
    REGISTERED_OBJECT_ARRAY_SERIALIZE(student)\

#define REGISTERED_OBJECT_DESSERIALIZE_ALL\
    REGISTERED_OBJECT_DESSERIALIZE(headmaster)\
    REGISTERED_OBJECT_DESSERIALIZE(school)\
    REGISTERED_OBJECT_DESSERIALIZE(teacher)\
    REGISTERED_OBJECT_DESSERIALIZE(student)\

#define REGISTERED_OBJECT_ARRAY_DESSERIALIZE_ALL\
    REGISTERED_OBJECT_ARRAY_DESSERIALIZE(headmaster)\
    REGISTERED_OBJECT_ARRAY_DESSERIALIZE(school)\
    REGISTERED_OBJECT_ARRAY_DESSERIALIZE(teacher)\
    REGISTERED_OBJECT_ARRAY_DESSERIALIZE(student)\
```



```cpp
struct headmaster{
	char * name;
	int age;
};

struct student{
	char * name;
	int age;
};

struct school{
	char * schoolName;
	headmaster master;
    int number [3];
	student stu[2];
};

//注册结构体
REGISTEREDMEMBER(headmaster, name, age);
REGISTEREDMEMBER(student, name, age);
REGISTEREDMEMBER(school, schoolName, master, stu, tea);

//调用接口
string json_ = "{\"schoolName\":\"wyai\",\"master\":{\"name\":\"花狗Fdog\",\"age\":21},\"number\":[1,2,3],\"stu\":[{\"name\":\"于静\",\"age\":21},{\"name\":\"二狗\",\"age\":21}]}";
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
string json_2 = "";
FdogSerialize(json_2, sch);
cout << "json=" << json_2 << endl;
```

输出：

```cpp
schoolName=scjxy
master.name=花狗Fdog
master.age=21
number[0]=1
number[1]=2
number[2]=3
stu[0]=于静,21
stu[1]=二狗,21
json={"schoolName":"scjxy","master":{"name":"花狗Fdog","age":21},"number":[1,2,3],"stu":[{"name":"于静","age":21},{"name":"二狗","age":21}]}
```



---

#### 3. 使用别名

```cpp
set_alias_name("school", "schoolName", "schName"); //将类型为school的结构体中成员schoolName的名字替换为schName
```

