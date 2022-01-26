

# :fire:FdogSerialize:fire:

​		FdogSerialize是一个用于C++序列化的开源库，采用非入侵方式，无需在原有结构体上进行修改，目前支持基础类型，基础类型数组，结构体，以及vector，list，map等数据类型的序列化，支持JSON和XML两种数据格式，支持别名，支持忽略字段，最少三行代码即可完成转换。

​			欢迎体验，如果对您有帮助，不妨给我一个 :star:，同时本人能力有限，若您有更好的解决方案，欢迎给我留言。:blush::blush::blush:

---

### 目录

- [FdogSerialize](#FdogSerialize)
    - [目录](#%E7%9B%AE%E5%BD%95)
    - [背景](#%E8%83%8C%E6%99%AF)
    - [类型支持](#%E7%B1%BB%E5%9E%8B%E6%94%AF%E6%8C%81)
    - [使用说明](#%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E)
    - [作者](#%E4%BD%9C%E8%80%85)
    - [许可](#%E8%AE%B8%E5%8F%AF)
    - [其他](#%E5%85%B6%E4%BB%96)

---

### 起源

​		经常使用java或者go的人应该知道这些语言在进行序列化和反序列化是很容易的，但是对于C++而言，这是困难的，根本原因是C++不支持反射，基于c++的语言哲学，C++宁死也要坚守的zero overhead，不为用不到的特性付出任何代价，不管这个代价有多小，也不管是怎样的语言特性，都不会妥协。

​		虽然C++不支持反射，但是我们依旧可以通过自己的方式实现序列化与反序列化，记得我在大二时用C++写的一个client-server小项目，自己规定了传输的数据格式（当时觉得自己解析Json很麻烦），第一个字段应该是什么，第二个字段应该是什么，正是因为没有方便的函数进行转换，我每次都需要使用非常繁琐的代码去拼出一个可以传递的字符串，是的，这样确实可以完成我想要的功能，但是我自己定的数据格式只适合自己用，这种方式长期必然行不通，而大多数人使用JSON和XML这两种数据格式来保存对象，如果我的项目想要使用这种大众化的数据格式，我又将重构我的代码。

​		我试着在github寻找一些用于C++的序列化与反序列的库，看看有没有什么办法可以帮助我快速把对象转变成JSON，我找到了一些类似的库，但是存在一些问题

:point_right:不太和我心意的设计：

使用者需要添加过多代码:x:

采用入侵方式，需要改变原有的结构体:x:

不需要入侵，但是在注册的时候需要一个一个指定类型:x:

只支持基础类型组成的结构体转换:x:

不支持别名（由于go的特性，在go中经常被使用）:x:

不支持忽略字段:x:

不支持指针类型（如果原项目中存在指针类型便需要指针类型）:x:



既然心高气傲都不满意，那就自己动手写一个吧，一开始想的简单了，说实话后面还是有些难度的，虽然说C++中应该尽可能的少用宏，但就目前来说C++没有反射机制，编译后不保存结构体的元信息来说，不得不使用大量的宏。

:point_right:暂时将这个库分为下面几个阶段：

1. 支持由基础类型和Json互转:white_check_mark:

2. 支持由基础类型组成的数组和json互转:white_check_mark:

3. 支持由基础类型组合成结构体类型和Json互转:white_check_mark:

4. 支持由结构体包含结构体的类型和Json互转:white_check_mark:

5. 支持vector类型和json互转:white_check_mark:

6. 支持list类型和json互转:white_check_mark:

7. 支持map类型和json互转:white_check_mark:

8. 支持set类型和json互转:white_check_mark:

9. 支持STL和自定义类型多层嵌套

10. 支持XML数据格式

    

:point_right:杂项支持：

1. 支持别名:white_check_mark:
1. 支持字段忽略:white_check_mark:
1. 支持忽略大小写
1. 支持模糊转换



暂时就想到怎么多，欢迎补充！



---

### 类型支持

| 序号  |    基础类型    |            说明            | 序号 |      基础类型      |       说明       |
| :---: | :------------: | :------------------------: | :--: | :----------------: | :--------------: |
|   1   |      bool      |          布尔类型          |  9   |    unsigned int    |    无符号整型    |
|   2   |      char      |        无符号字符型        |  10  |        long        |      长整型      |
|   3   | unsigned char  |        无符号字符型        |  11  |   unsigned long    |   无符号长整型   |
| **4** |   **char ***   | **字符指针（字符串类型）** |  12  |     long long      |     超长整型     |
|   5   |     short      |           短整型           |  13  | unsigned long long |  无符号超长整型  |
|   6   | unsigned short |        无符号短整型        |  14  |       float        |  单精度浮点类型  |
|   7   |      int       |            整型            |  15  |       double       |  双进度浮点类型  |
|   8   |  unsigned int  |         无符号整型         |  16  |    long double     | 长双进度浮点类型 |

基于C++的基础类型，FdogSerialize支持如上的十六种类型，只要你的结构体中**最基本类型**在这十六种范围之内，便可完成转换。

:point_right:最基本类型说明：

```cpp
struct headmaster{
	char * name;
	int age;
};

struct school{
	char * schoolName;
	headmaster info;
};
```

虽然school这个结构体包含了一种最基础类型和一种自定义类型，但追寻本质，自定义类型的底层都属于**最基本类型**。

因此school这个结构体完全可以被正常解析，事实上是任意类型都可以解析。



---



### 示例说明

序列化和反序列化各提供三个函数

| 序列化调用函数                                             | 说明                     |
| ---------------------------------------------------------- | ------------------------ |
| FSerialize(string & json_, T & object_, string name = "")  | 基础类型及结构体类型调用 |
| FSerializeA(string & json_, T & object_, string name = "") | 数组，vector，list调用   |
| FSerializeS(string & json_, T & object_, string name = "") | set，map类型调用         |

| 反序列化调用函数                                            | 说明                       |
| ----------------------------------------------------------- | -------------------------- |
| FSerialize( T & object_, string & json_, string name = "")  | 基础类型及结构体类型调用   |
| FSerializeA( T & object_, string & json_, string name = "") | 数组，vector，list类型调用 |
| FSerializeS( T & object_, string & json_, string name = "") | set，map类型调用           |

#### 1. 基础类型序列化

```cpp
#include "fdogserialize.h" //添加序列化所需头文件

int main()
{
	int value = 10;
    string json_;
    //将value转为json格式数据，建议使用第二种类型
    FdogSerialize::Instance()->FSerialize(json_, value); 			//json值为 "{10}"
    FdogSerialize::Instance()->FSerialize(json_, value, "value");   //json值为"{"value":10}"

    //将json格式数据转为value 需保证json_为正确格式字符串
    FdogSerialize::Instance()->FDesSerialize(value, json_);
    return 0;
}
```



---

#### 2. 基础类型数组序列化

```cpp
#include "fdogserialize.h" //添加序列化所需头文件

int main()
{
    int valueArray[5] = {1,2,3,4,5};
    string json_;
    //将valueArray转为json格式数据，建议使用第二种类型
    FdogSerialize::Instance()->FSerialize(json_, value); 			//json值为 "{[1,2,3,4,5]}"
    FdogSerialize::Instance()->FSerialize(json_, value, "value");   //json值为"{"valueArray":[1,2,3,4,5]}"

    //将json格式数据转为value 需保证json_为正确格式字符串
    FdogSerialize::Instance()->FDesSerialize(value, json_);
}
```



---

#### 3. 基础类型组成的结构体序列化

```cpp
#include "fdogserialize.h" //添加序列化所需头文件

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
    FdogSerialize::Instance()->FSerialize(json_, value, "stu");   //json值为"{"stu":{"name":"花狗Fdog","age":22}}"

    //将json格式数据转为value 需保证json_为正确格式字符串
    FdogSerialize::Instance()->FDesSerialize(value, json_);
}
```



---

#### 4. 包含结构体类型的结构体序列化

```cpp
#include "fdogserialize.h" //添加序列化所需头文件

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
    FdogSerialize::Instance()->FSerialize(json_, value, "sch");   
    //json值为"{"sch":{"schoolName":"fzyz","stu":{"name":"花狗Fdog","age":21}}}"

    //将json格式数据转为value 需保证json_为正确格式字符串
    FdogSerialize::Instance()->FDesSerialize(value, json_);
}
```



---

#### 5. vector类型的序列化

```cpp
#include "fdogserialize.h" //添加序列化所需头文件

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
    FdogSerialize::Instance()->FSerializeA(json_, stu, "stu");   
    //json值为"{"stu":[{"name":"花狗Fdog","age":22},{"name":"黑狗Fdog","age":23}]}"

    //将json格式数据转为value 需保证json_为正确格式字符串
    FdogSerialize::Instance()->FDesSerializeA(value, json_);
}
```



---

#### 6. list类型的序列化

```Cpp
#include "fdogserialize.h" //添加序列化所需头文件

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
    FdogSerialize::Instance()->FSerializeA(json_, stu, "stu");   
    //json值为"{"stu":[{"name":"花狗Fdog","age":22},{"name":"黑狗Fdog","age":23}]}"

    //将json格式数据转为value 需保证json_为正确格式字符串
    FdogSerialize::Instance()->FDesSerializeA(value, json_);
}
```



---

#### 7. map类型的序列化

```cpp
#include "fdogserialize.h" //添加序列化所需头文件

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
    FdogSerialize::Instance()->FSerializeA(json_, stu, "stu");   
    //json值为"{"stu":[{"name":"花狗Fdog","age":22},{"name":"黑狗Fdog","age":23}]}"

    //将json格式数据转为value 需保证json_为正确格式字符串
    FdogSerialize::Instance()->FDesSerializeA(value, json_);
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



### 杂项支持

| 杂项函数支持                                                 | 说明       |
| :----------------------------------------------------------- | ---------- |
| setAliasName(string Type, string memberName, string AliasName) | 使用别名   |
| setIgnoreField(string Type, string memberName)               | 忽略字段   |
| setIgnoreLU(string Type, string memberName)                  | 忽略大小写 |



#### 1. 支持别名

```cpp
#include "fdogserialize.h" //添加序列化所需头文件

//自定义基础类型结构体
struct student{
    char * name;
    int age;
};

int main()
{
    REGISTEREDMEMBER(student, name, age); //需要注册自定义类型，第一个参数为自定义结构体名，后面参数依次为成员名
    setAliasName("student", "name", "Aliasname");  //第一个参数为类型，第二参数为原名，第三个参数为别名
    //除此之外，也可以使用setAliasNameAll设置多个参数的别名
    struct stu;
    stu.name = "花狗Fdog";
    stu.age = 22;
    string json_;
    FdogSerialize::Instance()->FSerialize(json_, value, "stu");   //json值为"{{"Aliasname":"花狗Fdog","age":22}}"
}
```



----

#### 2. 支持字段忽略

```cpp
#include "fdogserialize.h" //添加序列化所需头文件

//自定义基础类型结构体
struct student{
    char * name;
    int age;
};

int main()
{
    REGISTEREDMEMBER(student, name, age); //需要注册自定义类型，第一个参数为自定义结构体名，后面参数依次为成员名
    setIgnoreField("student", "name");  //第一个参数为类型，第二参数为需要忽略的字段
    //除此之外，也可以使用setIgnoreFieldAll设置多个忽略的字段
    struct stu;
    stu.name = "花狗Fdog";
    stu.age = 22;
    string json_;
    FdogSerialize::Instance()->FSerialize(json_, value, "stu");   //json值为"{{"age":22}}"
}
```



---

#### 3. 支持忽略大小写

当将json转为对象时，如json中的键值与对象中的成员名存在大小写不同，可以设定忽略大小写。



#### 4. 支持模糊转换

若json和对象中的键值不相同，开启后将根据模糊等级进行匹配



---



### 一些可能的疑问

---

你可能会有如下疑问：

1. 为什么char * 类型算作基础类型

​		因为json中除了数值型数据，剩下的便是字符串类型，考虑到字符串使用的频率，再加上C++对于C兼容的原因，决定把他作为最基础类型。

2. char 类型怎么传递 

   由于JSON并不支持单引号，所以将使用数值传递并还原，例如：

   ```cpp
   char ch = 'A'; //ASCII码十进制为98
   
   //如果一个包含char的结构体转Json，检测到ch的类型为char将自动转为字符，//注意C++的中的转义
   std::string json = "{\"ch\":98}";
   
   //相对的，如果一个包含char的json想转为struct
   std::string json = "{\"ch\":98}"; //建议使用这种方式
   
   //又或者你不知道字符'A'的ASCII码是多少，那么可以使用如下方式，一般情况下不会遇到自己写json
   std::string json = "{\"ch\":\"char('A')\"}";//库会根据ch的类型，若ch为char类型自动将\"char('A')\"转为98
   
   ```

3. 目前支持20个成员的结构体，也可自行添加。

4. 关于指针类型的支持，后续会添加，思路是先获取指针类型的值再进行解析。



---

### 作者

:boy: Author：花狗Fdog(张旭)

:iphone:Wechat：sui2506897252

:email:Email：2506897252@qq.com

---

### 许可

该项目签署了Apache-2.0 License，详情请参见LICENSE

根据 Apache 许可，版本 2.0（“许可”）获得许可
除非遵守许可，否则您不得使用此文件。

---

### 其他

如果对您有帮助的话，请帮我点个star！

如果觉得它好用的话，可以推广给你的朋友们！

---











