#### [中文](https://github.com/HuaGouFdog/FdogSerialize/blob/master/example/example.md) | [**English**](https://github.com/HuaGouFdog/FdogSerialize/blob/master/example/example_EN.md)



## :christmas_tree:Test documentation (use examples)

```cpp
//The test constructs are defined in example/testtype.h
//textType.h
struct student{
  	string name;
    int age;
};

struct teacher{
    string name;
    int age;
};

//Let's say there are only two people in school
struct school{
    student stu;
    teacher tea;
};

//Add the header file that needs to define the structure to the definition.h header file
//definition.h Add struct, define struct
#include "example/testType.h"

//For example 2, it is not necessary to add both; if you are sure that only one-way conversions are required, you can define only one side.
//This is used for structure-to-JSON
#define Serialize_type_judgment_all\
    Serialize_type_judgment(student)\
    Serialize_type_judgment(teacher)\
//This is used for JSON-to-structure
#define DesSerialize_type_judgment_all\
    DesSerialize_type_judgment(student)\
    DesSerialize_type_judgment(teacher)\


//The test file
//example.cpp
#include "../FStruct.h" //Add the FStruct header file

int main(){
    
    //1.Structs contain only base types (int,char,char*,string, and arrays of base types, or STL containers (map does not currently support full types), so you only need to register members.
    REGISTEREDMEMBER(student, name, age);  //Registered student Member
    student stu;
    stu.name = "yujing";
    stu.age = 21;
    string stu_json = "";
    //structure-to-JSON
    Fdog::FJson(stu_json, stu);  //The output stu_json is ： {"name":"yujing","age":21}
    student stu2;
    string stu2_json = "{\"name\":\"zhangxv\",\"age\":21}"; //The quotes have to be handled
    //JSON-to-structure
    Fdog::FObject(stu2, stu2_json);  //The output  stu2.name = zhangxv   stu2.age = 21
    
    
    //2.In addition to base types, custom structures are also included in the structure
    REGISTEREDMEMBER(teacher, name, age);  //Registered teacher Member
    REGISTEREDMEMBER(school, stu, tea);  //Registered school Member
    school sch;
    sch.stu.name = "liuliu";
    sch.stu.age = 18;
    sch.tea.name = "wufang";
    sch.tea.age = 48;
    string sch_json = "";
    //structure-to-JSON
    Fdog::FJson(sch_json, sch); //The output sch_json is：{"stu":{"name":"liuliu","age":18},"tea":{"name":"wufang","age":48}}
  	//JSON-to-structure
    school sch2;
    string sch2_json = "{\"stu\":{\"name\":\"liuliu\",\"age\":18},\"tea\":{\"name\":\"wufang\",\"age\":48}}";
    Fdog::FObject(sch2, sch2_json);
    
    //3.A structure member has an array of custom types
    //Wait a moment
    
    
    //4.A structure member has a custom type STL container
    //Wait a moment
    
    
    //5.Support alias(This interface call is too complex and will be optimized later)
    FdogSerialize::Instance()->setAliasName("student", "name", "Aliasname"); //The first parameter is type, the second parameter is original name, and the third parameter is alias.
    Fdog::FJson(stu_json, stu);  //The output sch_json is： {"Aliasname":"yujing","age":21}
    
    
    //6.Field ignore support(This interface call is too complex and will be optimized later)
    FdogSerialize::Instance()->setIgnoreField("student", "name");  //The first argument is the type, and the second argument is the field to ignore
    Fdog::FJson(stu_json, stu);  //The output sch_json is： {"age":21}  //The data in the name field is ignored
    
    
    //7.Ignoring field case is supported(This interface call is too complex and will be optimized later)
    //When converting JSON to an object, if case is different between the key value in the JSON and the member name in the object, you can set case to be ignored.
    FdogSerialize::Instance()->setIgnoreLU("student", "name");
    FdogSerialize::Instance()->setIgnoreLU("student", "age");
    string stu_json = "{\"Name\":\"yujing\", \"AGE\":21}";
    Fdog::FObject(stu, stu_json);  //Name corresponds to name and AGE corresponds to age
    
    //8.Add one-time interfaces for interfaces 5, 6, and 7 to avoid multiple calls to interfaces because multiple fields need to be set
    //Support for the next version
    
    //9.Fuzzy matching is supported by default
    //Wait a moment
    //When you accidentally write the wrong field name, the program will automatically fuzzy match, as much as possible to complete the conversion.
    
    
    //10.Check whether the Json format is correct
    //Wait a moment
    
    
    //11.Find if a field in json exists
    //Wait a moment
    
    //12.The value of a field can be obtained(The return type can be int, double, string, or bool)
    //Wait a moment
    
    //13.Support for other types of Pointers (pointer types will have optional field attributes. In the case of pointer variables, it will determine whether the pointer address is empty during conversion. If it is empty, it will not be converted, similar to ignoring fields)
    //Support for the next version
    
    //14.XML serialization support
    //Support for the next version
    
    return 0;
}
```



### :christmas_tree:Possible doubt

---



1. :point_right:Why does the char * type count as a base type

​		Because the only type in JSON except numeric data is string, we decided to use char * as the most basic type because of the frequency of string usage and C++ compatibility. Char * can store strings as well as strings.



2. :point_right:How do I pass a char

   Since JSON does not support single quotes, numeric values are passed and restored, for example:

   ```cpp
   char ch = 'A'; //The ASCII code of A is 98 in decimal notation
   
   //If a structure containing char is converted to Json, the type ch detected as char is automatically converted to a character, // note the escape in C++
   std::string json = "{\"ch\":98}";
   
   //In contrast, if a JSON containing a char wants to be converted to a struct
   std::string json = "{\"ch\":98}";
   
   //Or if you don't know what the ASCII code for the character 'A' is, you can use the following method, and you won't normally have to write json yourself
   std::string json = "{\"ch\":\"char('A')\"}";
   //The library automatically converts \"char('A')\" to 98 depending on the type of ch
   ```

   

3. :point_right:Constructs that currently support 20 members can also be added.

   ```cpp
   //Add method: at the bottom of FStruct.h
   #define REGISTEREDMEMBER_s_20(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
   REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1);
   
   //Currently, 20 is supported. If you have larger requirements, you can add them according to the existing format. Of course, if you think 20 is too much, you can also delete them.
   ```

   

4. :point_right:Support for pointer types

​		If nulLPR is nullPR, it will not participate in serialization; if nulLPR is not nullPR, it will participate in serialization. Pointer type has the attribute of optional fields here, which can be assigned or not assigned. When you encounter non-mandatory fields, it is recommended that you use pointer type.



5. :point_right:The problem of the vector < bool >

​		The problem with vector<bool> is that it is not an STL container. For spatial optimization reasons, the C++ standard (dating back to C++98) explicitly calls vector<bool> a special standard container.</bool></bool> Where each bool uses only one bit of space rather than one byte as a normal bool (to implement a "dynamic bit set").

​		The standard library provides two alternatives: deque<bool> and bitset</bool>



6. :point_right:Document describing

​		This library includes defintion.h, FStruct.h, FStruct.cpp, three files.

​		All available interfaces provided by FStruct.h, FStruct.cpp, are listed in < Interface Support > and can be invoked on demand.

​		The defintion.h header is used to replace some code necessary for macros. If a structure participating in serialization and deserialization has a member that is also a structure, you should add the corresponding definition to the macros corresponding to defintion.h.

​		Example.md provides a sufficiently detailed test sample (contact me if you feel there are still defects).

​		The corresponding example.cpp provides the code for the example provided by example.md.