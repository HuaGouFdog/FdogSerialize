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

#define Serialize_type_judgment_all\
    Serialize_type_judgment(student)\
    Serialize_type_judgment(school)
    

#define DesSerialize_type_judgment_all\
    DesSerialize_type_judgment(student)\
    DesSerialize_type_judgment(school)

#endif