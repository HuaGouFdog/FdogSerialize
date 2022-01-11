#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#define TYPE_NAME(name) #name

#define Serialize_type_judgment(TYPE)\
    if(metainfoObject.memberType == TYPE_NAME(TYPE)){\
        Serialize(json_, *((TYPE *)((void *)&object_ + metainfoObject.memberOffset)));\
    }

#define DesSerialize_type_judgment(TYPE)\
    if(metainfoObject.memberType == TYPE_NAME(TYPE)){\
        DesSerialize(*((TYPE *)((void *)&object_ + metainfoObject.memberOffset), json_));\
    }

#define Serialize_type_judgment_all\
    //Serialize_type_judgment()\
    //Serialize_type_judgment()\
    //Serialize_type_judgment()\

#define DesSerialize_type_judgment_all\
    //DesSerialize_type_judgment()\
    //DesSerialize_type_judgment()\
    //DesSerialize_type_judgment()\

#endif