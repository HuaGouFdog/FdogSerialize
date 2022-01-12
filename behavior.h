#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#define TYPE_NAME(name) #name

#define Serialize_type_judgment(TYPE)\
    if(metainfoObject->memberType == TYPE_NAME(TYPE)){\
        Serialize(json_s, *(TYPE *)((void *)&object_ + metainfoObject->memberOffset));\
    }

#define DesSerialize_type_judgment(TYPE)\
    if(metainfoObject->memberType == TYPE_NAME(TYPE)){\
        DesSerialize(*((TYPE *)((void *)&object_ + metainfoObject->memberOffset)), value);\
    }

#define Serialize_type_judgment_all\
    Serialize_type_judgment(student)

#define DesSerialize_type_judgment_all\
    DesSerialize_type_judgment(student)

#endif