**中文**|English



# :fire:FStruct:fire:

<img src="https://img-blog.csdnimg.cn/b9712c0943a14a00a96a7793831ae2bc.png" style="zoom: 15%;"/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;FStruct is an open source library for C++ serialization, using a non-invasive way, no need to modify the original structure of the project, support the basic type, structure, STL container and other complex data types of serialization, support direct output object content, support to determine whether the JSON format is correct, support to obtain a JSON field value. Support custom alias, ignore fields, ignore case and other features, support JSON and XML (to be implemented) data format, at least two lines of code to complete conversion.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;FStruct是一个用于C++序列化的开源库，采用非入侵方式，无需在项目原有结构体上进行修改，支持基础类型，结构体，STL容器等复杂数据类型的序列化，支持直接输出对象内容，支持判断JSON格式是否正确，支持获取JSON某个字段值，支持自定义别名，忽略字段，忽略大小写等特性，支持JSON和XML（待实现）两种数据格式，最少两行代码即可完成转换。



&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;欢迎体验，如果对您有帮助，不妨给我一个 :star:，同时本人能力有限，若您有更好的解决方案，欢迎给我留言。:blush::blush::blush:

---

### 目录

- [:christmas_tree:起源](#christmas_tree起源)
- [:christmas_tree:类型支持](#christmas_tree类型支持)
- [:christmas_tree:接口支持](#christmas_tree接口支持)
- [:christmas_tree:测试文档(即使用说明)](#christmas_tree测试文档即使用说明)
- [:christmas_tree:可能的疑问](#christmas_tree可能的疑问)
- [:christmas_tree:代码编译/分支说明](#christmas_tree代码编译分支说明)
- [:christmas_tree:作者](#christmas_tree作者)
- [:christmas_tree:许可](#christmas_tree许可)
- [:christmas_tree:其他](#christmas_tree其他)

---

### :christmas_tree:起源

​		使用java或者go这样的语言是很容易将结构体和json之间进行转换的，然而对于C++而言，这是困难的，C++不支持反射，不会保存对象元信息，在实际的项目操作中避免不了需要使用到JSON，在没有库的情况下，在需要转换的场景中，每产生一个新类型的对象，都将产生大量代码，这将浪费大量的开发时间用于对象和JSON的转换的代码编写。

​		我在github寻找了一些用于C++的序列化与反序列的库，看看有没有符合我心意的库，很明显，没有找到符合我心意的库，要不然也不会有FStruct了，其中有一些不太和我心意的设计：

1. 使用者需要添加过多代码:cactus:
2. 只支持基础类型组成的结构体转换:cactus:

3. 在注册的时候需要一个一个指定类型，较繁琐:cactus:

4. 不支持别名（由于go的特性，在go中经常被使用），不支持忽略字段，不支持设置大小写敏感等等::cactus:

5. 不支持指针类型（如果原项目中存在指针类型便需要指针类型）:cactus:

6. 除了转换，没有其他能力，比如JSON的其他操作，验证JSON字符串格式是否正确，，某个字段是否存在等等。:cactus:



长痛不如短痛， 自己动手写一个吧。（一开始想的简单了，后面还是有难度的）

============================================================================================================================

​	第一阶段（已完成）：

1. 支持包含基础类型的结构体与Json互转:cactus:
2. 支持包含基础类型数组的结构体与json互转:cactus:

============================================================================================================================

​	第二阶段（已完成）：

1. 支持包含结构体嵌套的结构体（如果成员包括STL容器，则STL容器只支持基本类型）与Json互转:cactus:

2. 支持包含STL容器（STL的数据类型为基础类型）的结构体与json互转:cactus:

3. 对第一阶段所使用的接口进行优化，将六个接口整合为两个接口:cactus:

============================================================================================================================

​	第三阶段（已完成）：

1. STL部分容器支持自定义类型:cactus:

2. 支持对json字符串进行格式正确判断:cactus:

3. 支持获取某个字段是否存在:cactus:

4. 支持获取某个字段的值，而无须先进行序列化:cactus:
5. 支持直接输出对象值:cactus:

============================================================================================================================

​	第四阶段（未完成）：

1. 支持必选字段和可选字段，当必选字段无值时，进行报错(定义为指针类型即为可选字段):clock3:
1. 默认支持模糊匹配（可能出现争议）:clock3:
1. 判断JSON格式是否正确，如果不正确，输出出错位置:clock3:

============================================================================================================================

​	第五阶段（未完成）：

1. 部分函数无法跨平台使用，适配不同平台:clock3:
1. 对typeid().name()进行封装处理，不同平台输出唯一值:clock3:
1. 对部分接口进行重新命名:clock3:
1. 别名，字段忽略，大小写敏感没有恢复接口，需要增加恢复接口:clock3:
1. 支持XML数据格式的转换:clock3:

============================================================================================================================

​	杂项支持（已完成）：

1. 支持别名:cactus:
1. 支持字段忽略:cactus:
1. 支持忽略大小写:cactus:

============================================================================================================================

如果您有更好或者更有趣的建议，麻烦告诉我！



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

| 接口支持                                                     | 说明                           |
| ------------------------------------------------------------ | ------------------------------ |
| FJson(string & json_, T & object_)                           | object转json                   |
| FObject( T & object_, string & json_, string name = "")      | json转object                   |
| FJsonToString(T & object_)                                   | 输出对象                       |
| setAliasName(string Type, string memberName, string AliasName) | 使用别名（单个变量设置）       |
| setAliasNameS(string Type, string memberName, string AliasName, Args... args) | 使用别名（支持多个同时设置）   |
| setIgnoreField(string Type, string memberName)               | 忽略字段（单个变量设置）       |
| setIgnoreFieldS(string Type, string memberName, Args... args) | 忽略字段（支持多个同时设置）   |
| setIgnoreLU(string Type, string memberName)                  | 忽略大小写（单个变量设置）     |
| setIgnoreLUS()(string Type, string memberName, Args... args) | 忽略大小写（支持多个同时设置） |
| JsonValids()                                                 | json格式是否正确               |
| Exist()                                                      | 查找字段是否存在               |
| *GetIntValue*                                                | *获取int类型的值*              |
| GetDoubleValue                                               | 获取double类型的值             |
| GetStringValue                                               | 获取string类型的值             |
| GetBoolValue                                                 | 获取bool类型的值               |

未完成：

| 接口支持                | 说明                               |
| ----------------------- | ---------------------------------- |
| setFuzzy(string Type)() | 模糊转换（接口改为默认，无须调用） |
| FXml()                  | object转xml                        |
| FObjectX()              | xml转object                        |
| XmlValid()              | xml格式是否正确                    |



考虑对外删除setAliasName，setIgnoreField，setIgnoreLU并保留其S版本。

同时增加对应功能恢复函数，使其更灵活。



如果您还希望提供什么好的接口，请联系我。

---



### :christmas_tree:测试文档(即使用说明)

[中文](https://github.com/HuaGouFdog/FdogSerialize/blob/master/example/example.md) | [**English**](https://github.com/HuaGouFdog/FdogSerialize/blob/master/example/example_EN.md)

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

struct class_base_array{
    int numbers[5];
};

struct class_base_vector{
    vector<int> numbers;
};

struct class_object_array{
    student stus[2];
};

struct class_object_vector{
    vector<student> stus;
};

struct class_map
{
    map<string, int> grade;
};


//将需要定义结构体的头文件添加在definition.h头文件中
//definition.h 添加结构体，定义结构体
#include "example/testType.h"

//并非需要同时添加，如果你确定只需要单向转换，那么可以只定义一方即可。

//1结构体中包含自定义结构体在此添加
//用于序列化
#define Serialize_type_judgment_all\
    Serialize_type_judgment(student)\
    Serialize_type_judgment(teacher)\
//用于反序列化
#define Deserialize_type_judgment_all\
    Deserialize_type_judgment(student)\
    Deserialize_type_judgment(teacher)\

//2结构体中包含自定义类型的数组在此提添加
//用于序列化
#define Serialize_arraytype_judgment_all\
    Serialize_arraytype_judgment(student)\
//用于反序列化
#define Deserialize_arraytype_judgment_all\
    Deserialize_arraytype_judgment(student)\

//3结构体中包含自定义类型STL容器在此添加

//用于序列化
#define Serialize_vector_type_judgment_all\
    Serialize_vector_type_judgment(student)\

#define Serialize_list_type_judgment_all\
    Serialize_list_type_judgment(student)\

#define Serialize_deque_type_judgment_all\
    Serialize_deque_type_judgment(student)\

#define Serialize_set_type_judgment_all\
    Serialize_set_type_judgment(student)\

#define Serialize_map_type_judgment_all\
    //Serialize_map_type_judgment(string,student) 目前map还没完成

//用于反序列化
#define Deserialize_vector_type_judgment_all\
    Deserialize_vector_type_judgment(student)\

#define Deserialize_list_type_judgment_all\
    Deserialize_list_type_judgment(student)\

#define Deserialize_deque_type_judgment_all\
    Deserialize_deque_type_judgment(student)\

#define Deserialize_set_type_judgment_all\
    Deserialize_set_type_judgment(student)\

#define Deserialize_map_type_judgment_all\
    //Deserialize_map_type_judgment(string,student) 目前map还没完成


//测试文件
//example.cpp
#include "../FStruct.h" //添加序列化所需头文件

int main(){
    
    cout << "\n1：结构体只包含基础类型------------------------------------\n";
    //1.结构体只包含基础类型(int,char,char*,string,以及由基础类型构成的数组,或者是STL容器(map暂不支持全类型))，则只需要注册成员即可。
    REGISTEREDMEMBER(student, name, age);  //注册student成员
    student stu;
    stu.name = "yujing";
    stu.age = 21;
    string stu_json = "";
    //结构体转json
    Fdog::FJson(stu_json, stu);  
    //结果 输出stu_json为： {"name":"yujing","age":21}
    cout << stu_json << endl;
    student stu2;
    string stu2_json = "{\"name\":\"zhangxv\",\"age\":21}"; //引号要做处理
    //json转结构体
    Fdog::FObject(stu2, stu2_json);  
    //结果  stu2.name = zhangxv   stu2.age = 21
    cout << "name = " << stu2.name << " age = " << stu2.age << endl;

    cout << "\n2：结构体中除了基础类型，还包括自定义结构体------------------------------------\n";

    //2.结构体中除了基础类型，还包括自定义结构体
    REGISTEREDMEMBER(teacher, name, age);  //注册teacher成员
    REGISTEREDMEMBER(school, stu, tea);     //注册school成员
    school sch;
    sch.stu.name = "liuliu";
    sch.stu.age = 18;
    sch.tea.name = "wufang";
    sch.tea.age = 48;
    string sch_json = "";
    //结构体转json
    Fdog::FJson(sch_json, sch); 
    //结果 输出sch_json为：{"stu":{"name":"liuliu","age":18},"tea":{"name":"wufang","age":48}}
  	cout << sch_json << endl;
    //json转结构体
    school sch2;
    string sch2_json = "{\"stu\":{\"name\":\"liuliu\",\"age\":18},\"tea\":{\"name\":\"wufang\",\"age\":48}}";
    Fdog::FObject(sch2, sch2_json);
    //结果  
    cout << "name = " << sch.stu.name << " age = " << sch.stu.age << endl;
    cout << "name = " << sch.tea.name << " age = " << sch.tea.age << endl;

    cout << "\n3：结构体中使用STL容器map------------------------------------\n";

    //3.结构体中使用STL容器map
    REGISTEREDMEMBER(class_map, grade);
    class_map cm;
    cm.grade["zhangxu"] = 88;
    cm.grade["yujing"] = 99;
    string cm_json;
    Fdog::FJson(cm_json, cm);
    cout << cm_json << endl;
    class_map cm2;
    string cm2_json = "{\"grade\":{\"lisi\":33,\"angwu\":11}}";
    Fdog::FObject(cm2, cm2_json);
    for(auto j : cm2.grade){
        cout << "name = " << j.first << " age = " << j.second << endl;
    }

    cout << "\n4：结构体成员存在数组，且数组存储的基础类型数据------------------------------------\n";

    //4.结构体成员存在数组，且数组存储的基础类型数据
    REGISTEREDMEMBER(class_base_array, numbers)
    class_base_array cba;
    cba.numbers[0] = 0;
    cba.numbers[1] = 1;
    cba.numbers[2] = 2;
    cba.numbers[3] = 3;
    cba.numbers[4] = 4;
    string cba_json = "";
    //结构体转json
    Fdog::FJson(cba_json, cba);
    cout << cba_json << endl;
    //json转结构体
    class_base_array cba2;
    string cba2_json = "{\"cba2_json\":{\"numbers\":[0,1,2,3,4]}}";
    Fdog::FObject(cba2, cba2_json);
    cout << "numbers is " << cba2.numbers[0] << " " << cba2.numbers[1] << " " << cba2.numbers[2] << " " << cba2.numbers[3] << " " << cba2.numbers[4] << endl;

    cout << "\n5：结构体成员存在STL容器，且容器为基础类型------------------------------------\n";

    //5.结构体成员存在STL容器，且容器为基础类型
    REGISTEREDMEMBER(class_base_vector, numbers)
    class_base_vector cbv; //要注意刚声明的变量 vector长度为0，使用下角标赋值会出错
    cbv.numbers.push_back(0);
    cbv.numbers.push_back(1);
    cbv.numbers.push_back(2);
    cbv.numbers.push_back(3);
    cbv.numbers.push_back(4);
    string cbv_json = "";
    //结构体转json
    Fdog::FJson(cbv_json, cbv);
    cout << cbv_json << endl;
    //json转结构体
    class_base_vector cbv2;
    string cbv2_json = "{\"cbv2_json\":{\"numbers\":[0,1,2,3,4]}}";
    Fdog::FObject(cbv2, cbv2_json);
    cout << "numbers is " << cbv2.numbers[0] << " " << cbv2.numbers[1] << " " << cbv2.numbers[2] << " " << cbv2.numbers[3] << " " << cbv2.numbers[4] << endl;

    cout << "\n6：结构体成员存在自定义类型的数组------------------------------------\n";

    //6.结构体成员存在自定义类型的数组
    REGISTEREDMEMBER(class_object_array, stus)
    class_object_array coa;
    coa.stus[0].name = "zhangxu";
    coa.stus[0].age = 21;
    coa.stus[1].name = "yujing";
    coa.stus[1].age = 22;
    string coa_json = "";
    //结构体转json
    Fdog::FJson(coa_json, coa);
    cout << coa_json << endl;
    //json转结构体
    class_object_array coa2;
    string coa2_json = "{\"stus\":[{\"name\":\"jinqi\",\"age\":25},{\"name\":\"ruiming\",\"age\":12}]}";
    Fdog::FObject(coa2, coa2_json);
    cout << " name = " << coa2.stus[0].name << " age = " << coa2.stus[0].age << endl;
    cout << " name = " << coa2.stus[1].name << " age = " << coa2.stus[1].age << endl;

    cout << "\n7：结构体成员存在自定义类型STL容器------------------------------------\n";

    //7.结构体成员存在自定义类型STL容器
    REGISTEREDMEMBER(class_object_vector, stus)
    class_object_vector cov;
    student stu3;
    stu3.name = "zhangxu";
    stu3.age = 21;
    student stu4;
    stu4.name = "yujing";
    stu4.age = 21;
    cov.stus.push_back(stu3);
    cov.stus.push_back(stu4);
    string cov_json = "";
    //结构体转json
    Fdog::FJson(cov_json, cov);
    cout << cov_json << endl;
    //json转结构体
    class_object_vector cov2; //记得初始化长度 这个后期要改成自动
    cov2.stus.resize(2);
    string cov2_json = "{\"stus\":[{\"name\":\"jinqi\",\"age\":25},{\"name\":\"ruiming\",\"age\":12}]}";
    Fdog::FObject(cov2, cov2_json);
    cout << " name = " << cov2.stus[0].name << " age = " << cov2.stus[0].age << endl;
    cout << " name = " << cov2.stus[1].name << " age = " << cov2.stus[1].age << endl;

    cout << "\n8：支持别名------------------------------------\n";

    //8.支持别名
    //Fdog::setAliasName("student", "name", "Aliasname"); //第一个参数为类型，第二参数为原名，第三个参数为别名 单字段设置
    Fdog::setAliasNameS("student", "name", "Aliasname", "age", "Aliasage"); //支持多个字段设置
    stu_json = "";
    Fdog::FJson(stu_json, stu);  
    //结果 输出stu_json为： {"Aliasname":"yujing","age":21}
    cout << stu_json << endl;

    cout << "\n9：支持字段忽略------------------------------------\n";

    //9.支持字段忽略
    //Fdog::setIgnoreField("student", "name");  //第一个参数为类型，第二参数为需要忽略的字段 单字段设置
    Fdog::setIgnoreFieldS("student", "name", "age"); //支持多个字段
    stu_json = "";
    Fdog::FJson(stu_json, stu);
    //结果 输出stu_json为： {"age":21}  //name字段的数据将被忽略
    cout <<  stu_json << endl;

    cout << "\n10：支持忽略字段大小写------------------------------------\n";

    //10.支持忽略字段大小写，当将json转为对象时，如json中的键值与对象中的成员名存在大小写不同，可以设定忽略大小写。
    //Fdog::setIgnoreLU("student", "name"); 单字段设置
    //Fdog::setIgnoreLU("student", "age");

    Fdog::setIgnoreLUS("student", "name", "age"); //支持多字段设置
    stu_json = "{\"Name\":\"yujing\", \"AGE\":21}";
    Fdog::FObject(stu, stu_json);  //将Name对应name，AGE对应age
    cout << "name = " << stu.name << " age = " << stu.age << endl;

    cout << "\n11：针对7，8，9接口增加对应的一次性接口------------------------------------\n";
    //11.针对7，8，9接口增加对应的一次性接口，避免有多个字段需要设置，从而多次调用接口
    cout << "暂无" << endl;
    
    cout << "\n12：默认支持模糊匹配------------------------------------\n";
    //12.默认支持模糊匹配
    //马上支持，当不小心写错字段名时，程序将自动进行模糊匹配，最大可能完成转换。
    cout << "暂无" << endl;
    
    cout << "\n13：检测Json格式是否正确------------------------------------\n";
    //13.检测Json格式是否正确
    string json_v = "{\"stus\":[{\"name\":\"jinqi\",\"age\":25},{\"name\":\"ruiming\",\"age\":12}]}";
    cout << "字符串:" << json_v << endl;
    auto res = Fdog::JsonValidS(json_v);
    cout << "字符串是否正确：" << res.code << endl;
    cout << "字符串错误提示：" << res.message << endl;
    
    cout << "\n14：查找json中某个字段是否存在------------------------------------\n";
    //14.查找json中某个字段是否存在
    if(Fdog::Exist(json_v, "age")) {
        cout << "存在" << endl; 
    } else {
      cout << "不存在" << endl;
    }
    
    cout << "\n15：支持获取某个字段的值------------------------------------\n";
    //15.支持获取某个字段的值(返回类型支持int, double, string, bool)
    //建议配合Exist函数使用
    if(Fdog::Exist(json_v, "age")) {
        cout << "age的值:" << Fdog::GetStringValue(json_v, "age") << endl;
    } else {
      cout << "不存在" << endl;
    }
    //除GetStringValue之外，还提供下面四个用于返回不同类型
    // GetIntValue
    // GetDoubleValue
    // GetLongValue
    // GetBoolValue

    //json还存在一些问题，并不能100%检验，还需要时间来完善

    cout << "\n16：支持直接输出对象------------------------------------\n";
    //16. 支持输出对象
    school sch_2;
    sch_2.stu.name = "liuliu";
    sch_2.stu.age = 18;
    sch_2.tea.name = "wufang";
    sch_2.tea.age = 48;
    cout << aaaa << endl;
    cout << "对象值：" << Fdog::FJsonToString(sch_2) << endl;
    
    return 0;
}
```



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

​	该库包括define.h，defintion.h，FStruct.h，FStruct.cpp，四个文件。

​	FStruct.h，FStruct.cpp提供的所有可用接口，已在<接口支持>中列出，您可按需调用。

​	defintion.h头文件用于宏替换某些必要的代码，如果一个参与序列化与反序列化的结构体中存在某个成员也是结构体，那么您应该在defintion.h对应的宏里面添加相应的定义。

​	example.md提供了足够详细的测试示例(如果您觉得还有缺陷，可与我联系)。

​	对应的example.cpp提供了上述所提供示例的代码。







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







