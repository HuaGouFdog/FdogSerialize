#ifndef FDOGSERIALIZE_H
#define FDOGSERIALIZE_H

#include "fdogserializebase.h"

using namespace fsj;

/***********************************
*         基础存储基础类型            *
************************************/
map<string, string> baseType = {
            {"b", "bool"},
            {"c", "char"},
            {"h", "signed_char"},
            {"a", "unsigned_char"},
            {"i", "int"},
            {"i", "signed_int"},
            {"j", "unsigned_int"},
            {"s", "short_int"},
            {"s", "signed_short_int"},
            {"t", "unsigned_short_int"}, 
            {"l", "long_int"}, 
            {"l", "signed_long_int"},
            {"m", "unsigned_long_int"}, 
            {"x", "long_long_int"}, 
            {"x", "signed_long_long_int"},
            {"y", "unsigned_long_long_int"}, 
            {"f", "float"},
            {"d", "double"}, 
            {"e", "long_double"},
};


enum ObjectType{
    BASE_ = 1,
    STRUCT_,
    CLASS_,
    ARRAY_,
    VECTOR_,
    MAP_,
    LIST_,
};

/***********************************
*   存储结构体元信息
*   成员名， 别名， 成员类型, 成员所在偏移量，成员类型位数， 数组大小
************************************/
typedef struct MetaInfo{
    string memberName;
    string memberAliasName;
    string memberType;
    size_t memberOffset;
    size_t memberTypeSize;                                                         
    size_t memberArraySize;
    bool memberIsIgnore;                                                                                                                                                                                           
}MetaInfo;

/***********************************
*   存储结构体结构体类型，以及元信息
************************************/
typedef struct ObjectInfo{
    string objectType;  //结构体类型 字符串表示
    int objectTypeInt;  //结构体类型 数值表示
    vector<MetaInfo> metainfoObjectList;  //结构体元信息 
}ObjectInfo;

/***********************************
*   存储成员类型，数组大小
************************************/
struct memberAttribute {
    string valueType;
    int ArraySize;
};

class FdogSerialize {

    private:
    static mutex * mutex_serialize;
    static FdogSerialize * fdogserialize;
    vector<ObjectInfo> objectInfoList;

    FdogSerialize();
    ~FdogSerialize();

    public:
    //获取实例
    static FdogSerialize * Instance();

    public:

    //获取对应Info
    ObjectInfo & getObjectInfo(string objectName);

    //设置别名
    void setAliasName(string Type, string Name, string AliasName);

    //设置是否忽略该字段序列化
    void setIgnoreField(string Type, string Name);

    //一次性设置多个别名
    template<class T, class ...Args>
    void setAliasNameAll();

    //一次性设置忽略多个字段序列化
    template<class T, class ...Args>
    void setIgnoreField();

    //获取成员属性
    memberAttribute getMemberAttribute(string key);

    //获取object类型
    int getObjectType(string objectName);

    //序列化
    template<typename T>
    void Serialize(string & json_, T & object_){
        ObjectInfo & objectinfo = getObjectInfo(typeid(T).name());
        int objectType = getObjectType(typeid(T).name());
        for(auto metainfoObject : objectinfo.metainfoObjectList){
            //通过正则表达式获取对应的json
            smatch result;
            string regex_key = "(\"" + metainfoObject.memberName +"\")";
            string regex_value = baseRegex[metainfoObject.memberType];
            if(regex_value == ""){
                if(Vtype == 2){
                    regex_value = objectRegex;
                }else{
                    regex_value = arrayRegex;
                }
            }
            regex pattern(regex_key + ":" +regex_value);
            if(regex_search(json_, result, pattern)){
                string value = result.str(2).c_str();
                //只需要获取下一个类型的值进行进行递归即可
                if( xxx == "schoolName"){
                    
                }
                if(ObjectType == BASE_){
                    FdogSerializeBase::Instance()->BaseToJson(object, json_);
                }else{
                    Serialize(json, object_);
                }
            }
        }
        //removeLastComma(json_);
        //json_ = curlyBracketL + json_ + curlyBracketR;
    }

    //反序列化
    template<typename T>
    void DesSerialize(T & object_, string & json_){
        ObjectInfo & objectinfo = getObjectInfo(typeid(T).name());
        int objectType = getObjectType(typeid(T).name());
        for(auto metainfoObject : objectinfo.metainfoObjectList){
            //只需要获取下一个类型的值递归即可
        }
    }

};

#endif