### [中文](https://github.com/HuaGouFdog/FdogSerialize/blob/master/example/example.md) | [**English**](https://github.com/HuaGouFdog/FdogSerialize/blob/master/example/example_EN.md)



## :christmas_tree:测试文档(即使用示例)

```cpp
//测试所用结构体在example/testType.h中定义
//textType.h
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

//将需要定义结构体的头文件添加在definition.h头文件中
//definition.h 添加结构体，定义结构体
#include "example/testType.h"

//用于示例2, 并非需要同时添加，如果你确定只需要单向转换，那么可以只定义一方即可。
//这个用于结构体转json
#define Serialize_type_judgment_all\
    Serialize_type_judgment(student)\
    Serialize_type_judgment(teacher)\
//这个用于json转结构体
#define DesSerialize_type_judgment_all\
    DesSerialize_type_judgment(student)\
    DesSerialize_type_judgment(teacher)\


//测试文件
//example.cpp
#include "../FStruct.h" //添加序列化所需头文件

int main(){
    
    //1.结构体只包含基础类型(int,char,char*,string,以及由基础类型构成的数组,或者是STL容器(map暂不支持全类型))，则只需要注册成员即可。
    REGISTEREDMEMBER(student, name, age);  //注册student成员
    student stu;
    stu.name = "yujing";
    stu.age = 21;
    string stu_json = "";
    //结构体转json
    Fdog::FJson(stu_json, stu);  //结果 输出stu_json为： {"name":"yujing","age":21}
    student stu2;
    string stu2_json = "{\"name\":\"zhangxv\",\"age\":21}"; //引号要做处理
    //json转结构体
    Fdog::FObject(stu2, stu2_json);  //结果  stu2.name = zhangxv   stu2.age = 21
    
    
    //2.结构体中除了基础类型，还包括自定义结构体
    REGISTEREDMEMBER(teacher, name, age);  //注册teacher成员
    REGISTEREDMEMBER(school, stu, tea);  //注册school成员
    school sch;
    sch.stu.name = "liuliu";
    sch.stu.age = 18;
    sch.tea.name = "wufang";
    sch.tea.age = 48;
    string sch_json = "";
    //结构体转json
    Fdog::FJson(sch_json, sch); //结果 输出sch_json为：{"stu":{"name":"liuliu","age":18},"tea":{"name":"wufang","age":48}}
  	//json转结构体
    school sch2;
    string sch2_json = "{\"stu\":{\"name\":\"liuliu\",\"age\":18},\"tea\":{\"name\":\"wufang\",\"age\":48}}";
    Fdog::FObject(sch2, sch2_json);
    
    //3.结构体成员存在自定义类型的数组
    //马上支持
    
    
    //4.结构体成员存在自定义类型STL容器
    //马上支持
    
    
    //5.支持别名(这个接口调用太复杂，后期会优化)
    FdogSerialize::Instance()->setAliasName("student", "name", "Aliasname"); //第一个参数为类型，第二参数为原名，第三个参数为别名
    Fdog::FJson(stu_json, stu);  //结果 输出stu_json为： {"Aliasname":"yujing","age":21}
    
    
    //6.支持字段忽略(这个接口调用太复杂，后期会优化)
    FdogSerialize::Instance()->setIgnoreField("student", "name");  //第一个参数为类型，第二参数为需要忽略的字段
    Fdog::FJson(stu_json, stu);  //结果 输出stu_json为： {"age":21}  //name字段的数据将被忽略
    
    
    //7.支持忽略字段大小写(这个接口调用太复杂，后期会优化)
    //当将json转为对象时，如json中的键值与对象中的成员名存在大小写不同，可以设定忽略大小写。
    FdogSerialize::Instance()->setIgnoreLU("student", "name");
    FdogSerialize::Instance()->setIgnoreLU("student", "age");
    string stu_json = "{\"Name\":\"yujing\", \"AGE\":21}";
    Fdog::FObject(stu, stu_json);  //将Name对应name，AGE对应age
    
    //8.针对5，6，7接口增加对应的一次性接口，避免有多个字段需要设置，从而多次调用接口
    //下个版本支持
    
    //9.默认支持模糊匹配
    //马上支持，当不小心写错字段名时，程序将自动进行模糊匹配，最大可能完成转换。
    
    
    //10.检测Json格式是否正确
    //马上支持
    
    
    //11.查找json中某个字段是否存在
    //马上支持
    
    //12.支持获取某个字段的值(返回类型支持int, double, string, bool)
    //马上支持
    
    //13.支持其他类型指针(指针类型将拥有可选字段属性，对于指针变量，在转换时，将先判断指针地址是否为空，若为空，将不进行转换，类似于忽略字段)
    //下个版本
    
    //14.支持xml序列化
    //下下版本～
    
    return 0;
}
```



### :christmas_tree:可能的疑问

---



1. :point_right:为什么char * 类型算作基础类型

​	因为json中除了数值型数据，剩下的便是字符串类型，考虑到字符串使用的频率，再加上C++对于C兼容的原因，决定把char *作为最基础类型，除了char * 可以存储字符串，也可使用string存储字符串。

2. :point_right:char 类型怎么传递 

   由于JSON并不支持单引号，所以将使用数值传递并还原，例如：

   ```cpp
   char ch = 'A'; //A的ASCII码十进制为98
   
   //如果一个包含char的结构体转Json，检测到ch的类型为char将自动转为字符，//注意C++的中的转义
   std::string json = "{\"ch\":98}";
   
   //相对的，如果一个包含char的json想转为struct
   std::string json = "{\"ch\":98}";
   
   //又或者你不知道字符'A'的ASCII码是多少，那么可以使用如下方式，一般情况下不会遇到自己写json
   std::string json = "{\"ch\":\"char('A')\"}";//库会根据ch的类型，若ch为char类型自动将\"char('A')\"转为98
   ```

   

3. :point_right:目前支持20个成员的结构体，也可自行添加。

   ```cpp
   //添加方法：在FStruct.h底部，找到
   #define REGISTEREDMEMBER_s_20(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
   REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1);
   
   //当前支持20，若您有更大的需求，可以根据已有格式自行添加，当然如果您觉得20个太多，也可以自行删除。
   ```

   

4. :point_right:关于指针类型的支持

​		对于指针类型的支持，内部在处理指针时，将判断指针是否为nullpr，如果为nullpr则不参与序列化，如果不为nullpr则参与序列化，指针类型在这里有可选字段的属性，您可赋值，也可不赋值，当遇到非必传字段时，建议您使用指针类型。



5. :point_right:vector<bool>的问题

​	vector<bool>的问题不是一个STL容器，出于**空间优化的原因**，C++ 标准（最早可追溯到 C++98）明确地将 vector<bool> 称为特殊的标准容器，其中每个 bool 仅使用一位空间而不是像普通 bool 那样使用一个字节（ 实现一种“动态位集”）。

​	标准库提供了两个替代品：deque<bool>，bitset



6. :point_right:文件说明

​	该库包括defintion.h，FStruct.h，FStruct.cpp，三个文件。

​	FStruct.h，FStruct.cpp提供的所有可用接口，已在<接口支持>中列出，您可按需调用。

​	defintion.h头文件用于宏替换某些必要的代码，如果一个参与序列化与反序列化的结构体中存在某个成员也是结构体，那么您应该在defintion.h对应的宏里面添	加相应的定义。

​	example.md提供了足够详细的测试示例(如果您觉得还有缺陷，可与我联系)。

​	对应的example.cpp提供了example.md所提供示例的代码。