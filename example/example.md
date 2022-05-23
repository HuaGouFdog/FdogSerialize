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

    //16.支持其他类型指针(指针类型将拥有可选字段属性，对于指针变量，在转换时，将先判断指针地址是否为空，若为空，将不进行转换，类似于忽略字段)
    //下个版本
    
    //17.支持xml序列化
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