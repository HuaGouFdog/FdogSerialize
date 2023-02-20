/*
该项目签署了Apache-2.0 License，详情请参见LICENSE
根据 Apache 许可，版本 2.0（“许可”）获得许可
除非遵守许可，否则您不得使用此文件。

Copyright 2021-2022 花狗Fdog(张旭)
*/
#ifndef DEFINE_H
#define DEFINE_H

#define TYPE_NAME(name) #name

#define Serialize_type_judgment(TYPE)\
    if(metainfoObject->memberType == TYPE_NAME(TYPE) && metainfoObject->memberIsIgnore != true){\
        Serialize(json_s, *(TYPE *)((char *)&object_ + metainfoObject->memberOffset));\
        json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "{" + json_s + "}" + ",";\
    }

#define Deserialize_type_judgment(TYPE)\
    if(metainfoObject->memberType == TYPE_NAME(TYPE) && metainfoObject->memberIsIgnore != true){\
        Deserialize(*((TYPE *)((char *)&object_ + metainfoObject->memberOffset)), value);\
    }


#define Serialize_arraytype_judgment(TYPE)\
    if(metainfoObject->first == TYPE_NAME(TYPE)){\
        for(int i = 0; i < metainfoObject->memberArraySize; i++){\
            string json_z = "";\
            Serialize(json_z, *(TYPE *)((char *)&object_ + metainfoObject->memberOffset + (i * sizeof(TYPE))));\
            json_s = json_s + "{" + json_z + "}" + ",";\
        }\
    }

#define Deserialize_arraytype_judgment(TYPE)\
    if(metainfoObject->first == TYPE_NAME(TYPE)){\
        for(int i = 0; i < metainfoObject->memberArraySize; i++){\
            Deserialize(*(TYPE *)((char *)&object_ + metainfoObject->memberOffset + (i * sizeof(TYPE))), json_array[j++]);\
        }\
    }


#define Serialize_vector_type_judgment(TYPE)\
    if(metainfoObject->first == TYPE_NAME(TYPE)){\
        FSerialize(json_s, *(vector<TYPE> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});\
    }

#define Deserialize_vector_type_judgment(TYPE)\
    if(metainfoObject->first == TYPE_NAME(TYPE)){\
        FDeserialize(*(vector<TYPE> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<vector<int>>::Tag{});\
    }


#define Serialize_list_type_judgment(TYPE)\
    if(metainfoObject->first == TYPE_NAME(TYPE)){\
        FSerialize(json_s, *(vector<TYPE> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});\
    }

#define Deserialize_list_type_judgment(TYPE)\
    if(metainfoObject->first == TYPE_NAME(TYPE)){\
        FDeserialize(*(vector<TYPE> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<list<int>>::Tag{});\
    }

#define Serialize_deque_type_judgment(TYPE)\
    if(metainfoObject->first == TYPE_NAME(TYPE)){\
        FSerialize(json_s, *(vector<TYPE> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});\
    }

#define Deserialize_deque_type_judgment(TYPE)\
    if(metainfoObject->first == TYPE_NAME(TYPE)){\
        FDeserialize(*(vector<TYPE> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<deque<int>>::Tag{});\
    }

#define Serialize_set_type_judgment(TYPE)\
    if(metainfoObject->first == TYPE_NAME(TYPE)){\
        FSerialize(json_s, *(vector<TYPE> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});\
    }

#define Deserialize_set_type_judgment(TYPE)\
    if(metainfoObject->first == TYPE_NAME(TYPE)){\
        FDeserialize(*(vector<TYPE> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<set<int>>::Tag{});\
    }

#define Serialize_map_type_judgment(TYPE1, TYPE2)\
    if(metainfoObject->first == TYPE_NAME(TYPE1) && metainfoObject->second == TYPE_NAME(TYPE2)){\
        FSerialize(json_s, *(vector<TYPE1,TYPE2> *)((char *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<map<int,int>>::Tag{});\
    }

#define Deserialize_map_type_judgment(TYPE1, TYPE2)\
    if(metainfoObject->first == TYPE_NAME(TYPE1) && metainfoObject->second == TYPE_NAME(TYPE2)){\
        FDeserialize(*(vector<TYPE,TYPE2> *)((char *)&object_ + metainfoObject->memberOffset), value, TagDispatchTrait<map<int,int>>::Tag{});\
    }

//上面不要动
#endif