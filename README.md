# C/C++序列化与反序列化库

**FdogStructJson**是一个用于C/C++序列化和反序列化快速互转开源库，**支持结构体和JSON快速互转**，其他等等，采用**非入侵方式**，无需在原有结构体上进行修改，**支持别名**，**三行代码**即可完成转换使得它足够方便。其他功能正在逐步加入，我相信它会变强大。

---



### 起源

C/C++和java/go一样，拥有结构体类型，经常使用java/go的人知道，这些语言进行序列化和反序列化是很容易的，而偏偏就是伟大的C/C++，他不支持序列化与反序列化，最根本的原因是C/C++不支持反射，但不得不承认有时候能够进行序列化和反序列化确实会方便许多。

记得我在大二时用C++写的一个小项目，自己规定了传输的数据格式，第一个字段应该是什么，第二个字段应该是什么，我每次都需要使用非常繁琐的代码去拼出一个可以传递的字符串，是的，这样确实可以完成我想要的功能，但是我自己定的数据格式只能是自己用，后来我接触到了JSON和XML，大家也大多使用这两种数据格式来保存对象，如果我的项目想要使用这种大众化的数据格式，我又将重构我的代码，于是，我试着寻找，有没有什么办法可以帮助我快速把对象转变成JSON，于是我从网上找到了我想要的库。

##### 他们是这样的：

1. 有的过于庞大。

2. 有的采用入侵方式，需要改变原有的结构体。
3. 有的不需要入侵，但是在注册的时候需要一个一个指定类型。
4. 有的只支持基础类型组成的结构体转换，不支持结构体中包含对象。
5. 有的库不支持别名(在go中经常被使用)



有没有一个库可以把这几个都解决掉？没有的话不如自己写一个吧？说动手就动手，说实话这其实并不简单。。。。。



---



### 支持类型

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

基于C/C++的基础类型，FdogSturctJson定义了如上的十七种类型，只要你的结构体中**最基本类型**在这17种范围之内，便可完成转换，那么如何解释最基本类型呢？

例如：

```cpp
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
};
```

虽然school这个结构体包含了headmaster，student，teacher三种自定义类型，但追寻本质，他们的底层都属于**最基本类型**。

headmaster，它包含的最基本类型是char *和int。

student[]是由多个student组成，而student是由最基本类型char *和int构成。

所以school这个结构体完全可以被正常解析，事实上大多数结构体的最基本类型都在这里了。



你可能会有如下疑问：

1. 为什么char * 类型算作基础类型

​		因为json中除了数值型数据，剩下的便是字符串类型，考虑到字符串使用的频率，决定把他作为最基础类型。

2. char 类型怎么传递？'a' ？

   由于JSON并不支持单引号，所以将使用数值传递，例如：

   ```cpp
   char ch = 'A'; //ASCII码十进制为98
   
   //你可以像这样去传递char类型
   std::string json = "{\"ch\":98}";//注意C/C++的中的转义
   
   //又或者你不知道字符'A'的ASCII码是多少，那么可以使用如下方式：
   std::string json = "{\"ch\":char('A')}";//库会自动将'A'转为98
   
   //相对的，如果一个包含char的json想转为struct，库也会根据struct中结构体成员信息选择对于的存储格式。
   ```

   

---



### 使用说明

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



---



### 作者

花狗Fdog(张旭)

vx：sui2506897252

Mail：2506897252@qq.com



---



### 许可

该项目签署了xxxxx许可，详情请参见xxxxxxxxx



---

### 其他

如果对您有帮助的话，请帮我点个star！

如果觉得它好用的话，可以推广给你的朋友们！



---
