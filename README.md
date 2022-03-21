**中文**|English



# :fire:FStruct:fire:

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;FStruct是一个用于C++序列化的开源库，采用非入侵方式，无需在原有结构体上进行修改，目前支持基础类型，结构体，以及vector，list，deque，set，map等复杂数据类型的序列化，支持JSON和XML两种数据格式，支持别名，支持忽略字段，等其他特性，最少两行代码即可完成转换。



> 使用git命令将代码拉取到本地：git clone https://github.com/HuaGouFdog/FdogSerialize.git
>
> linux编译：g++ -std=c++11 ../FStruct.cpp main.cpp -o main -w
>
> 代码中有使用到C++11特性，使用到了正则表达式，若是linux编译，需保证gcc版本在4.9(4.8不支持正则表达式)。

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;欢迎体验，如果对您有帮助，不妨给我一个 :star:，同时本人能力有限，若您有更好的解决方案，欢迎给我留言。:blush::blush::blush:



---

### 目录

- [:fire:FStruct:fire:](#fireFStructfire)
    - [目录](#%E7%9B%AE%E5%BD%95)
    - [起源](#%E8%B5%B7%E6%BA%90)
    - [类型支持](#%E7%B1%BB%E5%9E%8B%E6%94%AF%E6%8C%81)
    - [接口支持](#%E6%8E%A5%E5%8F%A3%E6%94%AF%E6%8C%81)
    - [测试文档(即使用说明)](#%E6%B5%8B%E8%AF%95%E6%96%87%E6%A1%A3%E5%8D%B3%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E)
    - [可能的疑问](#%E5%8F%AF%E8%83%BD%E7%9A%84%E7%96%91%E9%97%AE)
    - [作者](#%E4%BD%9C%E8%80%85)
    - [许可](#%E8%AE%B8%E5%8F%AF)
    - [其他](#%E5%85%B6%E4%BB%96)

---

### :christmas_tree:起源

​		使用过java或者go的人知道这些语言在进行序列化和反序列化是很容易的，对于C++而言，这是困难的，根本原因是C++不支持反射，虽然C++不支持反射，但是我们依旧可以通过自己的方式来保存对象元信息来实现序列化与反序列化，记得我在大二时用C++写的一个client-server小项目，自己规定了传输的数据格式（当时觉得自己解析Json很麻烦），第一个字段应该是什么，第二个字段应该是什么，正是因为没有方便的函数进行转换，我每次都需要使用非常繁琐的代码去拼出一个可以传递的字符串，是的，这样确实可以完成我想要的功能，但是我自己定的数据格式只适合自己用，这种方式长期必然行不通，而大多数人使用JSON和XML这两种数据格式来保存数据，如果我的项目想要使用这种大众化的数据格式，我又将重构我的代码。

​		我试着在github寻找一些用于C++的序列化与反序列的库，看看有没有什么办法可以帮助我快速把对象转变成JSON，我找到了一些类似的库，但是获得或多或少存在一些问题。

:point_right:不太和我心意的设计：

使用者需要添加过多代码:x:

采用入侵方式，需要改变原有的结构体:x:

不需要入侵，但是在注册的时候需要一个一个指定类型:x:

只支持基础类型组成的结构体转换:x:

不支持别名（由于go的特性，在go中经常被使用）:x:

不支持忽略字段:x:

不支持指针类型（如果原项目中存在指针类型便需要指针类型）:x:



长痛不如短痛， 自己动手写一个吧。（一开始想的简单了，说实话后面还是有些难度的，但是没关系，一项一项攻破。）

:point_right:第一阶段分为下面几个部分：

1. 支持由基础类型和Json互转:heavy_check_mark:
2. 支持由基础类型组成的数组和json互转:heavy_check_mark:
3. 支持由基础类型组合成结构体类型和Json互转:heavy_check_mark:
4. 支持由结构体包含结构体的类型和Json互转:heavy_check_mark:
5. 支持vector类型和json互转:heavy_check_mark:
6. 支持list类型和json互转:heavy_check_mark:
7. 支持map类型和json互转:heavy_check_mark:
8. 支持set类型和json互转:heavy_check_mark:
8. 支持deque类型和json相转:heavy_check_mark:



:point_right:第二阶段分为下面几个部分：

1. 结构体多层嵌套(如果成员包括STL容器，则STL容器支持基本类型):heavy_check_mark:
2. 对第一阶段所使用的接口进行优化，将六个接口整合为两个：FJson/FObject，方面调用:heavy_check_mark:



:point_right:第三阶段分为下面几个部分：

1. 支持对json字符串进行格式正确判断:clock3:
2. 支持获取某个字段是否存在:clock3:
3. 支持获取某个字段的值，而无须先进行序列化:clock3:
3. 结构体多层嵌套(如果成员包括STL容器，则STL容器支持基本类型):clock3:



:point_right:第四阶段分为下面几个部分：

1. 支持必选字段和可选字段，当必选字段无值时，进行报错(定义为指针类型即为可选字段):clock3:
1. 支持XML数据格式的转换:clock3:



:point_right:杂项支持：

1. 支持别名:heavy_check_mark:
1. 支持字段忽略:heavy_check_mark:
1. 支持忽略大小写:heavy_check_mark:
1. 支持字段为空，则不进行序列化:clock3:
1. 支持模糊转换:clock3:



暂时就想到怎么多，打钩的是已经实现的，如果您有更好或者更有趣的建议，麻烦告诉我！



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

基于C++的基础类型，FStruct支持如上的十六种类型，只要你的结构体中**最基本类型**在这十六种范围之内，便可完成转换。

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



### :christmas_tree:接口支持

FStructm目前期望提供的接口支持：

已完成：

| 接口支持                                                     | 说明                       |
| ------------------------------------------------------------ | -------------------------- |
| FJson(string & json_, T & object_)                           | object转json               |
| FObject( T & object_, string & json_, string name = "")      | json转object               |
| setAliasName(string Type, string memberName, string AliasName) | 使用别名（单个变量设置）   |
| setIgnoreField(string Type, string memberName)               | 忽略字段（单个变量设置）   |
| setIgnoreLU(string Type, string memberName)                  | 忽略大小写（单个变量设置） |

未完成：

| 接口支持                | 说明                                                         |
| ----------------------- | ------------------------------------------------------------ |
| FXml()                  | object转xml                                                  |
| FObjectX()              | xml转object                                                  |
| setAliasNameAll()       | 使用别名（支持多个同时设置）                                 |
| setIgnoreFieldAll()     | 忽略字段（支持多个同时设置）                                 |
| setIgnoreLUAll()        | 忽略大小写（支持多个同时设置）                               |
| setFuzzy(string Type)() | 模糊转换（设置后，对于字段名设置不正确，将尝试模糊匹配，考虑加入到默认） |
| JsonValid()             | json格式是否正确                                             |
| XmlValid()              | xml格式是否正确                                              |
| Exist()                 | 查找字段是否存在                                             |
| *GetIntValue*           | *获取int类型的值*                                            |
| GetDoubleValue          | 获取double类型的值                                           |
| GetStringValue          | 获取string类型的值                                           |
| GetBoolValue            | 获取bool类型的值                                             |
|                         |                                                              |

如果您还希望提供什么好的接口，请联系我。



---



### :christmas_tree:测试文档(即使用说明)



[example.md](https://github.com/HuaGouFdog/FdogSerialize/blob/master/example/example_EN.md)



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











