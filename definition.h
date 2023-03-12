/*
该项目签署了Apache-2.0 License，详情请参见LICENSE
根据 Apache 许可，版本 2.0（“许可”）获得许可
除非遵守许可，否则您不得使用此文件。
Copyright 2021-2022 花狗Fdog(张旭)
*/

#ifndef BEHAVIOR_H
#define BEHAVIOR_H
#include "define.h"
#include "testType.h"
/******在此添加结构体所包含的头文件****/
//#include "example/testType.h"
/*********************************/


//结构体中包含自定义结构体在此添加
//用于序列化
#define Serialize_type_judgment_all\
    Serialize_type_judgment(student)\
    Serialize_type_judgment(teacher)\
    Serialize_type_judgment(classtest)\
//用于反序列化
#define Deserialize_type_judgment_all\
    Deserialize_type_judgment(student)\
    Deserialize_type_judgment(teacher)\
    Deserialize_type_judgment(classtest)\


//结构体中包含自定义类型的数组在此提添加
//用于序列化
#define Serialize_arraytype_judgment_all\
    Serialize_arraytype_judgment(student)\
//用于反序列化
#define Deserialize_arraytype_judgment_all\
    Deserialize_arraytype_judgment(student)\



//结构体中包含自定义类型STL容器在此添加

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
    //Serialize_map_type_judgment(string,student)

//用于反序列化
#define Deserialize_vector_type_judgment_all\
    Deserialize_vector_type_judgment(student)\

#define Deserialize_list_type_judgment_all\
    Deserialize_list_type_judgment(student)\

#define Deserialize_deque_type_judgment_all\
    Deserialize_deque_type_judgment(student)\

#define Deserialize_set_type_judgment_all Deserialize_set_type_judgment(student)\

#define Deserialize_map_type_judgment_all //Deserialize_map_type_judgment(string,student)

#endif