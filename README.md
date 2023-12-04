**中文**|English



# :christmas_tree:FStruct:christmas_tree:

<img src="https://img-blog.csdnimg.cn/b9712c0943a14a00a96a7793831ae2bc.png" style="zoom: 15%;"/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;FStruct is an open source library for C++ serialization, using a non-invasive way, no need to modify the original structure of the project, support the basic type, structure, STL container and other complex data types of serialization, support direct output object content, support to determine whether the JSON format is correct, support to obtain a JSON field value. Support custom alias, ignore fields, ignore case and other features, at least two lines of code to complete conversion.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;FStruct是一个用于C++序列化的开源库，采用非入侵方式，无需在项目原有结构体上进行修改，支持基础类型，结构体，STL容器等复杂数据类型的序列化，支持直接输出对象内容，支持判断JSON格式是否正确，支持获取JSON某个字段值，支持自定义别名，忽略字段，忽略大小写等特性，最少两行代码即可完成转换。



&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;欢迎体验，如果对您有帮助，不妨给我一个 :star:，同时本人能力有限，若您有更好的解决方案，欢迎给我留言。:blush::blush::blush:

---

### 

### :christmas_tree:使用方式

在需要序列化类/结构体的文件中包含FStruct.h头文件

```cpp
#include "FStruct.h"

//一个正常的结构体定义
struct student{
  	string name;
	int age;  
};

//修改为如下形式，即可支持序列化，非常简单
F_CLASS(student) {
	string name;
	int age;
	REGISTERED_MEMBER_S(student, name, age);
};

//一个正常的类定义
class teacher {
public:
	string name;
	int age;
};

//修改为如下形式，即可支持序列化，非常简单
F_CLASS(teacher) {
public:
	string name;
	int age;
	REGISTERED_MEMBER_S(teacher, name, age);
};

//其中REGISTERED_MEMBER_S宏会自动添加构造函数，并在其中添加InitFdogSerialize接口来完成序列化初始化的工作
//如果您有在构造函数中添加其他内容的需求，请使用REGISTERED_MEMBER_C，而非REGISTERED_MEMBER_S

//您只需要在显式的构造函数中添加InitFdogSerialize()的接口即可，您无需关心其实现。
F_CLASS(student) {
public:
	string name;
	int age;
	student(){
		student::InitFdogSerialize();
	}
	REGISTERED_MEMBER_C(student, name, age);
};

```

需要在序列化/反序列化的地方调用相应的接口即可

```cpp
int main() {
	student stu;
    stu.name = "yujing";
    stu.age = 23;
    string stu_json = "";
    //结构体转json
    Fdog::FJson(stu_json, stu);  
    //结果 输出stu_json为： {"name":"yujing","age":23}
    cout << stu_json << endl;
    student stu2;
    string stu2_json = "{\"name\":\"zhangxv\",\"age\":23}";
    //json转结构体
    Fdog::FObject(stu2, stu2_json); 
    //结果  stu2.name = zhangxv   stu2.age = 23
    return 0;
}
```

相对于其他序列化方式，它真的很简单。

---

### :christmas_tree:类型支持

| 序号  |    基础类型    |            说明            | 序号 |      基础类型      |       说明       |
| :---: | :------------: | :------------------------: | :--: | :----------------: | :--------------: |
|   1   |      bool      |          布尔类型          |  9   |    unsigned int    |    无符号整型    |
|   2   |      char      |        无符号字符型        |  10  |        long        |      长整型      |
|   3   | unsigned char  |        无符号字符型        |  11  |   unsigned long    |   无符号长整型   |
| **4** |   **char ***   | **字符指针（字符串类型）** |  12  |     long long      |     超长整型     |
|   5   |     short      |           短整型           |  13  | unsigned long long |  无符号超长整型  |
|   6   | unsigned short |        无符号短整型        |  14  |       float        |  单精度浮点类型  |
|   7   |      int       |            整型            |  15  |       double       |  双进度浮点类型  |
|   8   |     string     |         字符串类型         |  16  |    long double     | 长双进度浮点类型 |

基于C++的基础类型，FStruct支持如上的十六种类型，并支持其对应的**指针类型**，包括vector，list，map等容器，只要你的结构体中**最基本类型**在这十六种范围之内，便可完成转换。

---

### :christmas_tree:特性支持

此外，FStruct还支持使用别名，忽略字段，忽略大小写等，以及获取相关对应的类型值。

| 接口支持                                                     | 说明                           |
| ------------------------------------------------------------ | ------------------------------ |
| setAliasNameS(string Type, string memberName, string AliasName, Args... args) | 使用别名（支持多个同时设置）   |
| setIgnoreFieldS(string Type, string memberName, Args... args) | 忽略字段（支持多个同时设置）   |
| setIgnoreLUS()(string Type, string memberName, Args... args) | 忽略大小写（支持多个同时设置） |
| JsonValids()                                                 | json格式是否正确               |
| Exist()                                                      | 查找字段是否存在               |
| *GetIntValue*                                                | *获取int类型的值*              |
| GetDoubleValue                                               | 获取double类型的值             |
| GetStringValue                                               | 获取string类型的值             |
| GetBoolValue                                                 | 获取bool类型的值               |



### :christmas_tree:可能的疑问

---

0. :point_right:支不支持类，成员函数和成员变量位置有没有要求

​	支持类，但是目前只支持public的成员变量，暂时不支持private成员变量。

​	成员函数和成员变量的位置没有要求。



1. :point_right:为什么char * 类型算作基础类型

​	因为json中除了数值型数据，剩下的便是字符串类型，考虑到字符串使用的频率，再加上C++对于C兼容的原因，决定把char *作为最基础类型，除了char * 可以存储字符串，也可使用string存储字符串。



2. :point_right:char * 如果不作为字符串的场景该如何转换

​	之前没有考虑到这一点，单纯的默认了char * 就是字符串，下一步将加入判断，以’\0’结尾判断是否是字符串。



3. :point_right:char 类型怎么传递 

​	由于JSON并不支持单引号，所以将使用数值传递并还原，例如：

```cpp
char ch = 'A'; //A的ASCII码十进制为98

//如果一个包含char的结构体转Json，检测到ch的类型为char将自动转为字符，//注意C++的中的转义
std::string json = "{\"ch\":98}";

//相对的，如果一个包含char的json想转为struct
std::string json = "{\"ch\":98}";

//又或者你不知道字符'A'的ASCII码是多少，那么可以使用如下方式，一般情况下不会遇到自己写json
std::string json = "{\"ch\":\"char('A')\"}";//库会根据ch的类型，若ch为char类型自动将\"char('A')\"转为98
```



4. :point_right:目前支持20个成员的结构体，也可自行添加。

```cpp
//添加方法：在FStruct.h底部，找到
#define REGISTEREDMEMBER_s_20(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1);

//当前支持20，若您有更大的需求，可以根据已有格式自行添加，当然如果您觉得20个太多，也可以自行删除。
```



5. :point_right:关于指针类型的支持

​		对于指针类型的支持，内部在处理指针时，将判断指针是否为nullpr，如果为nullpr则不参与序列化，如果不为nullpr则参与序列化，指针类型在这里有可选字段的属性，您可赋值，也可不赋值，当遇到非必传字段时，建议您使用指针类型。



6. :point_right:vector<bool>的问题

​	vector<bool>的问题不是一个STL容器，出于**空间优化的原因**，C++ 标准（最早可追溯到 C++98）明确地将 vector<bool> 称为特殊的标准容器，其中每个 bool 仅使用一位空间而不是像普通 bool 那样使用一个字节（ 实现一种“动态位集”）。

​	标准库提供了两个替代品：deque<bool>，bitset



7. :point_right:文件说明

​	该库包括FStruct.h，FStruct.cpp两个文件。

​	您只需要在有需要的位置包含FStruct.h即可



---



### :christmas_tree:代码编译/分支说明

> 使用git命令将代码拉取到本地：git clone https://github.com/HuaGouFdog/FdogSerialize.git
>
> 目前仅保证linux环境正常，代码中有使用到C++11特性，使用到了正则表达式，linux编译，需保证gcc版本在4.9(4.8不支持正则表达式)
>
> windows暂未适配，敬请期待。
>
> 
>
> linux编译方式1，使用g++编译：
>
> 进入example目录执行
>
> g++ -std=c++11 ../FStruct.cpp example.cpp -o main -w
>
> 
>
> linux编译方式2，使用cmake/make编译：
>
> 根目录执行
>
> mkdir build  && cd build
>
> cmake ../ 
>
> make -j8
>
> 
>
> 分支说明：
>
> master为不稳定分支，请勿使用
>
> dev分支为本人开发测试分支，请勿使用
>
> release分支为稳定分支，版本号越高，支持越多，请使用最新的release分支



---

### :christmas_tree:作者

:boy: Author：花狗Fdog(张旭)

:iphone:Wechat：sui2506897252

:email:Email：2506897252@qq.com

---

### :christmas_tree:许可

该项目签署了Apache-2.0 License，详情请参见LICENSE

根据 Apache 许可，版本 2.0（“许可”）获得许可

除非遵守许可，否则您不得使用此文件。

---

### :christmas_tree:其他

如果FStruct对您有帮助的话，请给FStruct点个star！

如果觉得FStruct好用的话，可以推广给你的朋友们！

如果您有更好的建议，欢迎您告诉我！

---







