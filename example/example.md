- [1. 基础类型序列化](#1-%E5%9F%BA%E7%A1%80%E7%B1%BB%E5%9E%8B%E5%BA%8F%E5%88%97%E5%8C%96)
- [2. 基础类型数组序列化](#2-%E5%9F%BA%E7%A1%80%E7%B1%BB%E5%9E%8B%E6%95%B0%E7%BB%84%E5%BA%8F%E5%88%97%E5%8C%96)
- [3. 基础类型组成的结构体序列化](#3-%E5%9F%BA%E7%A1%80%E7%B1%BB%E5%9E%8B%E7%BB%84%E6%88%90%E7%9A%84%E7%BB%93%E6%9E%84%E4%BD%93%E5%BA%8F%E5%88%97%E5%8C%96)
- [4. 包含结构体类型的结构体序列化](#4-%E5%8C%85%E5%90%AB%E7%BB%93%E6%9E%84%E4%BD%93%E7%B1%BB%E5%9E%8B%E7%9A%84%E7%BB%93%E6%9E%84%E4%BD%93%E5%BA%8F%E5%88%97%E5%8C%96)
- [5. vector类型的序列化](#5-vector%E7%B1%BB%E5%9E%8B%E7%9A%84%E5%BA%8F%E5%88%97%E5%8C%96)
- [6. list类型的序列化](#6-list%E7%B1%BB%E5%9E%8B%E7%9A%84%E5%BA%8F%E5%88%97%E5%8C%96)
- [7. map类型的序列化](#7-map%E7%B1%BB%E5%9E%8B%E7%9A%84%E5%BA%8F%E5%88%97%E5%8C%96)
- [8. 必要说明](#8-%E5%BF%85%E8%A6%81%E8%AF%B4%E6%98%8E)
- [1. 支持别名](#1-%E6%94%AF%E6%8C%81%E5%88%AB%E5%90%8D)
- [2. 支持字段忽略](#2-%E6%94%AF%E6%8C%81%E5%AD%97%E6%AE%B5%E5%BF%BD%E7%95%A5)
- [3. 支持忽略大小写](#3-%E6%94%AF%E6%8C%81%E5%BF%BD%E7%95%A5%E5%A4%A7%E5%B0%8F%E5%86%99)
- [4. 支持模糊转换](#4-%E6%94%AF%E6%8C%81%E6%A8%A1%E7%B3%8A%E8%BD%AC%E6%8D%A2)

#### 1. 基础类型序列化

```cpp
#include "../FStruct.h" //添加序列化所需头文件

int main()
{
	int value = 10;
    string json_;
    //将value转为json格式数据，建议使用第二种类型
    Fdog::FJson(json_, value); 			//json值为 "{10}"
    Fdog::FJson(json_, value, "value");   //json值为"{"value":10}"

    //将json格式数据转为value 需保证json_为正确格式字符串
    Fdog::FObject(value, json_);Fdog::FJson
    return 0;
}
```



---

#### 2. 基础类型数组序列化

```cpp
#include "../FStruct.h" //添加序列化所需头文件

int main()
{
    int valueArray[5] = {1,2,3,4,5};
    string json_;
    //将valueArray转为json格式数据，建议使用第二种类型
    Fdog::FJson(json_, value); 			//json值为 "{[1,2,3,4,5]}"
    Fdog::FJson(json_, value, "value");   //json值为"{"valueArray":[1,2,3,4,5]}"

    //将json格式数据转为value 需保证json_为正确格式字符串
    Fdog::FObject(value, json_);
}
```



---

#### 3. 基础类型组成的结构体序列化

```cpp
#include "../FStruct.h" //添加序列化所需头文件

//自定义基础类型结构体
struct student{
    char * name;
    int age;
};

int main()
{
    REGISTEREDMEMBER(student, name, age); //需要注册自定义类型，第一个参数为自定义结构体名，后面参数依次为成员名
    struct stu;
    stu.name = "花狗Fdog";
    stu.age = 22;
    string json_;
    //将value转为json格式数据
    Fdog::FJson(json_, value, "stu");   //json值为"{"stu":{"name":"花狗Fdog","age":22}}"

    //将json格式数据转为value 需保证json_为正确格式字符串
    Fdog::FObject(value, json_);
}
```



---

#### 4. 包含结构体类型的结构体序列化

```cpp
#include "../FStruct.h" //添加序列化所需头文件

//自定义基础类型结构体
struct student{
    char * name;
    int age;
};

struct school{
  char * schoolName;
  student stu;
};

//需要在宏Serialize_type_judgment_all定义下添加嵌套结构体
#define Serialize_type_judgment_all\
    Serialize_type_judgment(student)
	//依次添加
    
//需要在宏Serialize_type_judgment_all定义下添加嵌套结构体
#define DesSerialize_type_judgment_all\
    DesSerialize_type_judgment(student)
	//依次添加

int main()
{
    REGISTEREDMEMBER(student, name, age); //需要注册自定义类型
    REGISTEREDMEMBER(school, school, stu); //需要注册自定义类型
   
    school sch;
    sch.schoolName = "fzyz";
    sch.stu.name = "花狗Fdog";
    sch.stu.age = 22;
    string json_;
    //将value转为json格式数据
    Fdog::FJson(json_, value, "sch");   
    //json值为"{"sch":{"schoolName":"fzyz","stu":{"name":"花狗Fdog","age":21}}}"

    //将json格式数据转为value 需保证json_为正确格式字符串
    Fdog::FObject(value, json_);
}
```



---

#### 5. vector类型的序列化

```cpp
#include "../FStruct.h" //添加序列化所需头文件

//自定义基础类型结构体
struct student{
    char * name;
    int age;
};

int main()
{
    REGISTEREDMEMBER(student, name, age); //需要注册自定义类型，第一个参数为自定义结构体名，后面参数依次为成员名
    vector<student> stu;
    struct stu_1;
    stu_1.name = "花狗Fdog";
    stu_1.age = 22;
    
    struct stu_2;
    stu_2.name = "黑狗Fdog";
    stu_2.age = 23;
    
    stu.push_back(stu_1);
    stu.push_back(stu_2);
    
    string json_;
    //将value转为json格式数据
    Fdog::FJson(json_, stu, "stu");   
    //json值为"{"stu":[{"name":"花狗Fdog","age":22},{"name":"黑狗Fdog","age":23}]}"

    //将json格式数据转为value 需保证json_为正确格式字符串
    Fdog::FObject(value, json_);
}
```



---

#### 6. list类型的序列化

```Cpp
#include "../FStruct.h" //添加序列化所需头文件

//自定义基础类型结构体
struct student{
    char * name;
    int age;
};

int main()
{
    REGISTEREDMEMBER(student, name, age); //需要注册自定义类型，第一个参数为自定义结构体名，后面参数依次为成员名
    list<student> stu;
    struct stu_1;
    stu_1.name = "花狗Fdog";
    stu_1.age = 22;
    
    struct stu_2;
    stu_2.name = "黑狗Fdog";
    stu_2.age = 23;
    
    stu.push_back(stu_1);
    stu.push_back(stu_2);
    
    string json_;
    //将value转为json格式数据
    Fdog::FJson(json_, stu, "stu");   
    //json值为"{"stu":[{"name":"花狗Fdog","age":22},{"name":"黑狗Fdog","age":23}]}"

    //将json格式数据转为value 需保证json_为正确格式字符串
    Fdog::FObject(value, json_);
}
```



---

#### 7. map类型的序列化

```cpp
#include "../FStruct.h" //添加序列化所需头文件

//自定义基础类型结构体
struct student{
    char * name;
    int age;
};

int main()
{
    REGISTEREDMEMBER(student, name, age); //需要注册自定义类型，第一个参数为自定义结构体名，后面参数依次为成员名
    vector<student> stu;
    struct stu_1;
    stu_1.name = "花狗Fdog";
    stu_1.age = 22;
    
    struct stu_2;
    stu_2.name = "黑狗Fdog";
    stu_2.age = 23;
    
    stu.push_back(stu_1);
    stu.push_back(stu_2);
    
    string json_;
    //将value转为json格式数据
    Fdog::FJson(json_, stu, "stu");   
    //json值为"{"stu":[{"name":"花狗Fdog","age":22},{"name":"黑狗Fdog","age":23}]}"

    //将json格式数据转为value 需保证json_为正确格式字符串
    Fdog::FObject(value, json_);
}
```

#### 8. 必要说明

```cpp
//需要在宏Serialize_type_judgment_all定义下添加嵌套结构体
#define Serialize_type_judgment_all\
    Serialize_type_judgment(student)
	//依次添加
    
//需要在宏Serialize_type_judgment_all定义下添加嵌套结构体
#define DesSerialize_type_judgment_all\
    DesSerialize_type_judgment(student)
	//依次添加

//两个宏的定义准则：使用该自定义类型做为其他自定义类型的成员
```



---



#### 1. 支持别名

```cpp
#include "../FStruct.h" //添加序列化所需头文件

//自定义基础类型结构体
struct student{
    char * name;
    int age;
};

int main()
{
    REGISTEREDMEMBER(student, name, age); //需要注册自定义类型，第一个参数为自定义结构体名，后面参数依次为成员名
    FdogSerialize::Instance()->setAliasName("student", "name", "Aliasname"); 
    //第一个参数为类型，第二参数为原名，第三个参数为别名
    //除此之外，也可以使用setAliasNameAll设置多个参数的别名
    struct stu;
    stu.name = "花狗Fdog";
    stu.age = 22;
    string json_;
    Fdog::FJson(json_, value);   //json值为"{{"Aliasname":"花狗Fdog","age":22}}"
}
```



----

#### 2. 支持字段忽略

```cpp
#include "../FStruct.h" //添加序列化所需头文件

//自定义基础类型结构体
struct student{
    char * name;
    int age;
};

int main()
{
    REGISTEREDMEMBER(student, name, age); //需要注册自定义类型，第一个参数为自定义结构体名，后面参数依次为成员名
    FdogSerialize::Instance()->setIgnoreField("student", "name");  
    //第一个参数为类型，第二参数为需要忽略的字段
    //除此之外，也可以使用setIgnoreFieldAll设置多个忽略的字段
    struct stu;
    stu.name = "花狗Fdog";
    stu.age = 22;
    string json_;
    Fdog::FJson(json_, value);   //json值为"{{"age":22}}"
}
```



---

#### 3. 支持忽略大小写

当将json转为对象时，如json中的键值与对象中的成员名存在大小写不同，可以设定忽略大小写。

```cpp
#include "../FStruct.h" //添加序列化所需头文件

//自定义基础类型结构体
struct student{
    char * name;
    int age;
};

int main()
{
    REGISTEREDMEMBER(student, name, age); 
    struct stu;
    FdogSerialize::Instance()->setIgnoreLU("student", "name"); 
    string stu_json = "{\"Name\":\"yujing\", \"AGE\":21}";
    Fdog::FObject(json_, value);
}
```



#### 4. 支持模糊转换

若json和对象中的键值不相同，开启后将根据模糊等级进行匹配

暂无