/*
该项目签署了Apache-2.0 License，详情请参见LICENSE
根据 Apache 许可，版本 2.0（“许可”）获得许可
除非遵守许可，否则您不得使用此文件。

Copyright 2021-2022 花狗Fdog(张旭)
*/

#ifndef BEHAVIOR_H
#define BEHAVIOR_H

/******在此添加结构体所包含的头文件****/
#include "example/testType.h"
/*********************************/

#define TYPE_NAME(name) #name

#define Serialize_type_judgment(TYPE)\
    if(metainfoObject->memberType == TYPE_NAME(TYPE) && metainfoObject->memberIsIgnore != true){\
        Serialize(json_s, *(TYPE *)((void *)&object_ + metainfoObject->memberOffset));\
        json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "{" + json_s + "}" + ",";\
    }

#define Deserialize_type_judgment(TYPE)\
    if(metainfoObject->memberType == TYPE_NAME(TYPE) && metainfoObject->memberIsIgnore != true){\
        Deserialize(*((TYPE *)((void *)&object_ + metainfoObject->memberOffset)), value);\
    }

//上面不要动


//自定义机构体在此添加
#define Serialize_type_judgment_all\
    Serialize_type_judgment(student)\
    Serialize_type_judgment(teacher)\

#define Deserialize_type_judgment_all\
    Deserialize_type_judgment(student)\
    Deserialize_type_judgment(teacher)\

#endif