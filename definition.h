/*
该项目签署了Apache-2.0 License，详情请参见LICENSE
根据 Apache 许可，版本 2.0（“许可”）获得许可
除非遵守许可，否则您不得使用此文件。

Copyright 2021-2022 花狗Fdog(张旭)
*/

#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#define TYPE_NAME(name) #name

#define Serialize_type_judgment(TYPE)\
    if(metainfoObject->memberType == TYPE_NAME(TYPE) && metainfoObject->memberIsIgnore != true){\
        Serialize(json_s, *(TYPE *)((void *)&object_ + metainfoObject->memberOffset));\
        json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "{" + json_s + "}" + ",";\
    }

#define DesSerialize_type_judgment(TYPE)\
    if(metainfoObject->memberType == TYPE_NAME(TYPE) && metainfoObject->memberIsIgnore != true){\
        DesSerialize(*((TYPE *)((void *)&object_ + metainfoObject->memberOffset)), value);\
    }

//上面不要动

//自定义类型在此添加

#define Serialize_type_judgment_all\
    Serialize_type_judgment(student)\

#define DesSerialize_type_judgment_all\
    DesSerialize_type_judgment(student)\

#endif