#ifndef FDOGSERIALIZE_H
#define FDOGSERIALIZE_H

#include "definition.h"
//在此添加用到的类型头文件
#include "example/testType.h"
#include <map>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <regex>
#include <algorithm>
#include <mutex>
#include <sstream>
#include <string>
#include <cxxabi.h>
#include <cstring>

using namespace std;


static vector<string> baseType = {
        "bool", "bool*"
        "char", "unsigned char", "char*", "unsigned char*",
        "int", "unsigned int", "int*", "unsigned int*",
        "short", "unsigned short", "short*", "unsigned short*",
        "long", "unsigned long int", "long*", "unsigned long*",
        "long long", "unsigned long long", "long long*", "unsigned long long*",
        "float", "double", "long double", "float*", "double*", "long double*"
};

//有符号类型应该拥有正负号，正号忽视 ^(-|+)? 匹配负号
static map<string, string> baseRegex = {
        {"bool", "(\\d+)"},
        {"float", "(\\d+.\\d+)"}, 
        {"double", "(\\d+.\\d+)"},
        {"long double", "(\\d+.\\d+)"},
        {"char*", "\"(.*?)\""},
        {"char", "(\\d+)"}, {"unsigned char", "(\\d+)"}, 
        {"int", "(\\d+)"}, {"unsigned int", "(\\d+)"},
        {"short", "(\\d+)"}, {"unsigned short", "(\\d+)"}, 
        {"long", "(\\d+)"}, {"unsigned long", "(\\d+)"},
        {"long long", "(\\d+)"}, {"unsigned long long", "(\\d+)"}, 
};

static map<int, string> complexRegex = {
    {5, "std::vector<(.*?),"},
    {6, "std::map<(.*?), (.*?),"},
    {7, "std::__cxx11::list<(.*?),"},
    {8, "匹配set"},
};


//用于数组整体提取
const string arrayRegex = "(\\[(.*?)\\])";
//匹配数组
const string patternArray = "((A)(\\d+)_\\d?(\\D+))";
//匹配别名
const string patterAlias = "(\\(A:(.*?)\\))";
//用于结构体整体提取
const string objectRegex = "(\\{(.*?)\\})";
//匹配结构体
const string patternObject = "((\\d+)(\\D+))";

enum ObjectType{
    OBJECT_BASE = 1,
    OBJECT_STRUCT,
    OBJECT_CLASS,
    OBJECT_ARRAY,
    OBJECT_VECTOR,
    OBJECT_MAP,
    OBJECT_LIST,
    OBJECT_SET,
    OBJECT_DEQUE,
};


/***********************************
*   存储结构体元信息
************************************/
typedef struct MetaInfo{
    string memberName;  
    string memberAliasName;
    string memberType;
    size_t memberOffset;
    size_t memberTypeSize;                                                         
    size_t memberArraySize;
    int memberTypeInt;   
    string first;  //如果是map类型 first表示key的类型，如果是其他类型，表示value类型
    string second; //如果是map类型，second表示value类型
    bool memberIsIgnore = false;
    bool memberIsIgnoreLU = false;                                                                                                                                                                                 
}MetaInfo;

/***********************************
*   存储结构体结构体类型，以及元信息
************************************/
typedef struct ObjectInfo{
    string objectType;  //结构体类型 字符串表示
    int objectTypeInt;  //结构体类型 数值表示
    int objectSize;     //结构体大小
    vector<MetaInfo *> metaInfoObjectList;  //结构体元信息 
}ObjectInfo;


typedef struct FdogMap{
    string first;
    string second;
};

/***********************************
*   存储成员类型，数组大小
************************************/
struct memberAttribute {
    string valueType;
    int valueTypeInt; //类型 数值表示
    string first;  //如果是map类型 first表示key的类型，如果是其他类型，表示value类型
    string second; //如果是map类型，second表示value类型
    int ArraySize;
};

//声明序列化base类
class FdogSerializeBase {
    private:
    static mutex * mutex_base;
    static FdogSerializeBase * fdogserializebase;

    public:
    static FdogSerializeBase * Instance();

    template<class T>
    string removeLastZero(T & return_){
        std::ostringstream oss; 
        oss << return_;
        return oss.str();
    }

    template<class T>
    string getValueByAddress(string valueType, T & object, int offsetValue){
        if(valueType == "char*"){
            auto value = *((const char **)((void *)&object + offsetValue));
            string str_value = value;
            return "\"" + str_value  + "\"";
        }
        if(valueType == "bool"){
            auto value = *((bool *)((char *)&object + offsetValue));
            if(value){
                return "true";
            }else{
                return "false";
            }
        }
        if(valueType == "char"){
            auto value = *((char *)((void *)&object + offsetValue));
            return to_string((int)value);
        }
        if(valueType == "unsigned char"){
            auto value = *((char *)((void *)&object + offsetValue));
            return to_string((unsigned int)value);
        }
        if(valueType == "int"){
            auto value = *((int *)((char *)&object + offsetValue));
            return to_string(value);
        }
        if(valueType == "unsigned int"){
            auto value = *((unsigned int *)((char *)&object + offsetValue));
            return to_string(value);
        }
        if(valueType == "short"){
            auto value = *((short int *)((char *)&object + offsetValue));
            return to_string(value);
        }
        if(valueType == "unsigned short"){
            auto value = *((unsigned short int *)((char *)&object + offsetValue));
            return to_string(value);
        }
        if(valueType == "long"){
            auto value = *((long int *)((char *)&object + offsetValue));
            return to_string(value);
        }
        if(valueType == "unsigned long"){
            auto value = *((unsigned long int *)((char *)&object + offsetValue));
            return to_string(value);
        }
        if(valueType == "long long"){
            auto value = *((long long int *)((char *)&object + offsetValue));
            return to_string(value);
        }
        if(valueType == "unsigned long long"){
            auto value = *((unsigned long long int *)((char *)&object + offsetValue));
            return to_string(value);
        }        
        if(valueType == "float"){
            auto value = *((float *)((char *)&object + offsetValue));
            return removeLastZero(value);
        }
        if(valueType == "double"){
            auto value = *((double *)((char *)&object + offsetValue));
            return removeLastZero(value);
        }
        if(valueType == "long double"){
            auto value = *((long double *)((char *)&object + offsetValue));
            return removeLastZero(value);
        }
        return "";
    }

    template<class T>
    void setValueByAddress(string valueType, T &object, int offsetValue, string value){
        if(valueType == "char*"){
            *((char **)((void *)&object + offsetValue)) = new char[strlen(value.c_str())];
            strcpy(*((char **)((void *)&object + offsetValue)), value.c_str());
        }
        std::stringstream ss;
        ss.str(value);
        //只是用一次不用clear
        if(valueType == "bool"){
            ss >> *((bool *)((void *)&object + offsetValue));
        }
        if(valueType == "char"){
            ss >> *((char *)((void *)&object + offsetValue));
        }
        if(valueType == "unsigned char"){
            ss >> *((unsigned char *)((void *)&object + offsetValue));
        }
        if(valueType == "int"){
            ss >> *((int *)((char *)&object + offsetValue));
        }
        if(valueType == "unsigned int"){
            ss >> *((unsigned int *)((char *)&object + offsetValue));
        }
        if(valueType == "short"){
            ss >> *((short int *)((char *)&object + offsetValue));
        }
        if(valueType == "unsigned short"){
            ss >> *((unsigned short int *)((char *)&object + offsetValue));
        }
        if(valueType == "long"){
            ss >> *((long int *)((char *)&object + offsetValue));
        }
        if(valueType == "unsigned long"){
            ss >> *((unsigned long int *)((char *)&object + offsetValue));
        }
        if(valueType == "long long"){
            ss >> *((long long int *)((char *)&object + offsetValue));
        }
        if(valueType == "unsigned long long"){
            ss >> *((unsigned long long  int *)((char *)&object + offsetValue));
        }  
        if(valueType == "float"){
            ss >> *((float *)((char *)&object + offsetValue));
        }
        if(valueType == "double"){
            ss >> *((double *)((char *)&object + offsetValue));
        }
        if(valueType == "long double"){
            ss >> *((long double *)((char *)&object + offsetValue));
        }
    }

    // //基础类型转json
    template<class T>
    void BaseToJson(string & json_, MetaInfo * metainfoobject, T & object_){
        string value = getValueByAddress(metainfoobject->memberType, object_, metainfoobject->memberOffset);
        if(metainfoobject->memberAliasName != ""){
            json_ = json_ + "\"" + metainfoobject->memberAliasName + "\"" + ":" + value + ",";
        }else{
            json_ = json_ + "\"" + metainfoobject->memberName + "\"" + ":" + value + ",";
        }
    }

    // //基础类型转json
    template<class T>
    void BaseToJsonA(string & json_, MetaInfo * metainfoobject, T & object_){
        string value = getValueByAddress(metainfoobject->memberType, object_, metainfoobject->memberOffset);
        json_ = json_ + value + ",";
    }    

    // //json转基础类型
    template<class T>
    void JsonToBase(T & object_, MetaInfo * metainfoobject, string json_){
        setValueByAddress(metainfoobject->memberType, object_, metainfoobject->memberOffset, json_);
    }
};

struct BaseTag {};
struct ArrayTag {};
struct MapTag {};

template<typename T> struct TagDispatchTrait {
    using Tag = BaseTag;
};

template<> struct TagDispatchTrait<vector<int>> {
    using Tag = ArrayTag;
};

template<> struct TagDispatchTrait<list<int>> {
    using Tag = ArrayTag;
};

template<> struct TagDispatchTrait<set<int>> {
    using Tag = ArrayTag;
};

template<> struct TagDispatchTrait<deque<int>> {
    using Tag = ArrayTag;
};

template<> struct TagDispatchTrait<map<int,int>> {
    using Tag = MapTag;
};

class FdogSerialize {

    private:
    static mutex * mutex_serialize;
    static FdogSerialize * fdogserialize;
    vector<ObjectInfo *> objectInfoList;
    vector<MetaInfo *> baseInfoList;

    FdogSerialize();
    ~FdogSerialize();

    public:
    //获取实例
    static FdogSerialize * Instance();

    //添加objectinfo
    void addObjectInfo(ObjectInfo * objectinfo);

    //获取对应Info
    ObjectInfo & getObjectInfo(string objectName);

    //获取对于Info 针对基础类型获取
    MetaInfo * getMetaInfo(string TypeName);

    //设置别名
    void setAliasName(string Type, string memberName, string AliasName);

    //设置是否忽略该字段序列化
    void setIgnoreField(string Type, string memberName);

	//设置是否忽略大小写
    void setIgnoreLU(string Type, string memberName);

	//设置进行模糊转换 结构体转json不存在这个问题主要是针对json转结构体的问题，如果存在分歧，可以尝试进行模糊转换
	void setFuzzy(string Type);


    //一次性设置多个别名
    template<class T, class ...Args>
    void setAliasNameAll();

    //一次性设置忽略多个字段序列化
    template<class T, class ...Args>
    void setIgnoreField();

    void removeFirstComma(string & return_);

    void removeLastComma(string & return_);

    void removeNumbers(string & return_);

    template<class T, class ...Args>
    void setIgnoreLU();

    //获取成员属性
    memberAttribute getMemberAttribute(string key);

    //获取object类型
    int getObjectTypeInt(string objectName, string typeName);

    //获取基础类型 只有base和struct两种
    ObjectInfo getObjectInfoByType(string typeName, int objectTypeInt);

    //通过宏定义加载的信息获取
    int getObjectTypeByObjectInfo(string objectName);

    //判断是否是基础类型
    bool isBaseType(string typeName);

    //判断是否为vector类型
    bool isVectorType(string objectName, string typeName);
    
    //获取vector中的类型
    string getTypeOfVector(string objectName, string typeName);

    //判断是否为map类型
    bool isMapType(string objectName, string typeName);

    //获取map中的key，value类型
    FdogMap getTypeOfMap(string objectName, string typeName);

    //判断是否是list类型
    bool isListType(string objectName, string typeName);

    //获取list中的类型
    string getTypeOfList(string objectName, string typeName);

    //判断是否是结构体类型
    bool isStructType(string objectName, string typeName);

    //判断是否是数组
    bool isArrayType(string objectName, string typeName);

    //解析数组
    vector<string> CuttingArray(string data);

    //切割
    vector<string> split(string str, string pattern);

    //序列化
    template<typename T>
    void Serialize(string & json_, T & object_, string name = ""){
        //通过传进来的T判断是什么复合类型，ObjectInfo只保存结构体,如果是NULL可以确定传进来的不是struct类型
        ObjectInfo objectinfo = FdogSerialize::Instance()->getObjectInfo(abi::__cxa_demangle(typeid(T).name(),0,0,0));
        //获取的只能是结构体的信息，无法知道是什么复合类型，尝试解析类型 objectType其实是一个结构体类型名称
        int objectType = getObjectTypeInt(objectinfo.objectType, abi::__cxa_demangle(typeid(T).name(),0,0,0));
        if(objectinfo.objectType == "NULL" && objectType != OBJECT_BASE && objectType != OBJECT_STRUCT){
            //说明不是struct类型和base类型尝试，尝试解析类型
            objectinfo = getObjectInfoByType(abi::__cxa_demangle(typeid(T).name(),0,0,0), objectType);
            objectType = getObjectTypeInt(objectinfo.objectType, abi::__cxa_demangle(typeid(T).name(),0,0,0));
            //这里这个objectinfo应该还是空 所以拿objecttype的数值判断
        }
        int sum = objectinfo.metaInfoObjectList.size();
        cout << "sum：" << sum << endl;
        int i = 1;
        //获取到的objectType才是真正的类型，根据这个类型进行操作
        switch(objectType){
            case OBJECT_BASE:
            {
                MetaInfo * metainfo1 = getMetaInfo(abi::__cxa_demangle(typeid(object_).name(),0,0,0));
                FdogSerializeBase::Instance()->BaseToJsonA(json_, metainfo1, object_);
            }
            break;
            case OBJECT_STRUCT:
            {
                for(auto metainfoObject : objectinfo.metaInfoObjectList){
                    string json_s;
                    cout <<"成员类型：" << metainfoObject->memberType << "--" << metainfoObject->memberTypeInt << "--"<< metainfoObject->first << endl;
                    if(metainfoObject->memberTypeInt == OBJECT_BASE && metainfoObject->memberIsIgnore != true){
                        FdogSerializeBase::Instance()->BaseToJson(json_s, metainfoObject, object_);
                        json_ = json_ + json_s;
                    }
                    if(metainfoObject->memberTypeInt == OBJECT_VECTOR && metainfoObject->memberIsIgnore != true){
                        if(metainfoObject->first == "int"){
                            FSerialize(json_s, *(vector<int> *)((void *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<vector<int>>::Tag{});
                        }
                        json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]" + ",";
                    }
                    if(metainfoObject->memberTypeInt == OBJECT_LIST && metainfoObject->memberIsIgnore != true){
                        if(metainfoObject->first == "int"){
                            FSerialize(json_s, *(list<int> *)((void *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<list<int>>::Tag{});
                        }
                        json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]" + ",";
                    }
                    if(metainfoObject->memberTypeInt == OBJECT_DEQUE && metainfoObject->memberIsIgnore != true){ 
                        FSerialize(json_s, *(deque<int> *)((void *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<deque<int>>::Tag{});
                        json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]" + ",";
                    }
                    if(metainfoObject->memberTypeInt == OBJECT_SET && metainfoObject->memberIsIgnore != true){ 
                        FSerialize(json_s, *(set<int> *)((void *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<set<int>>::Tag{});
                        json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]" + ",";                       
                    }
                    if(metainfoObject->memberTypeInt == OBJECT_MAP && metainfoObject->memberIsIgnore != true){
                        if(metainfoObject->first == "int" && metainfoObject->second == "int"){
                            FSerialize(json_s, *(map<int, int> *)((void *)&object_ + metainfoObject->memberOffset), TagDispatchTrait<map<int,int>>::Tag{});
                        }
                        json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "{" + json_s + "}" + ",";
                    }
                    Serialize_type_judgment_all;
                    if(i == sum){
                        if(json_.length() > 0){
                            removeLastComma(json_);
                        }
                    }
                    json_s = "";
                    i++;
                }
            }
            break;
        }
    }

    template<typename T>
    void FSerialize(string & json_, T & object_, BaseTag, string name = ""){
        Serialize(json_, object_, name);
        //这里需要判断类型 如果是基础类型直接使用name 不是基础类型，可以使用
        if (isBaseType(abi::__cxa_demangle(typeid(T).name(),0,0,0))) {
            removeLastComma(json_);
            json_ = "{\"" + name + "\":" + json_ + "}";
            return ;
        }
        json_ = "{" + json_ + "}";
    }

    template<typename T>
    void FSerialize(string & json_, T & object_, ArrayTag, string name = ""){
        for(auto & object_one : object_){
            int objectType = isBaseType(abi::__cxa_demangle(typeid(object_one).name(),0,0,0));
            if (!objectType){
                json_ = json_ + "{";
            }
            Serialize(json_, object_one);
            if (!objectType){
                json_ = json_ + "},";
            }
        }
        if(json_.length() > 0){
            removeLastComma(json_);
        }
        //json_ = "{\"" + name + "\":[" + json_ + "]}";
        //如果转换对象直接就是数组，可以再额外提供一个，或者说其他
    }

    template<typename T>
    void FSerialize(string & json_, T & object_, MapTag, string name = ""){
        int i = 0;
        int len = object_.size();
        for(auto & object_one : object_){
            //看情况，如果是结构体，需要花括号，基本类型不需要
            json_ = json_ + "\"" + to_string(object_one.first) + "\"" + ":";
            Serialize(json_, object_one.second);
            removeLastComma(json_);
            json_ = json_ + ",";
            i++;
        }
        removeLastComma(json_);
        //json_ = "{" + json_ + "}";
    }

    //反序列化
    template<typename T>
    void DesSerialize(T & object_, string & json_, string name = ""){
        ObjectInfo & objectinfo = FdogSerialize::Instance()->getObjectInfo(abi::__cxa_demangle(typeid(T).name(),0,0,0));
        int objectType = getObjectTypeInt(objectinfo.objectType, abi::__cxa_demangle(typeid(T).name(),0,0,0));
        if(objectinfo.objectType == "NULL" && objectType != OBJECT_BASE && objectType != OBJECT_STRUCT){
            //说明不是struct类型和base类型尝试，尝试解析类型
            objectinfo = getObjectInfoByType(abi::__cxa_demangle(typeid(T).name(),0,0,0), objectType);
            objectType = getObjectTypeInt(objectinfo.objectType, abi::__cxa_demangle(typeid(T).name(),0,0,0));
        }

        if (OBJECT_BASE == objectType) {
            MetaInfo * metainfo1 = getMetaInfo(abi::__cxa_demangle(typeid(object_).name(),0,0,0));
            smatch result;
            string regex_key;
            string regex_value = baseRegex[metainfo1->memberType];
            regex * pattern;
            if (name == "") {
                pattern = new regex(regex_value);
            } else {
                pattern = new regex(regex_key + ":" +regex_value);
                // if (metainfoObject->memberIsIgnoreLU == false){
                //     pattern = new regex(regex_key + ":" +regex_value);
                // }else{
                //     pattern = new regex(regex_key + ":" +regex_value,regex::icase);//icase用于忽略大小写
                // }
            }
            if(regex_search(json_, result, *pattern)){
                string value = result.str(2).c_str();
                if (value == ""){
                    value = result.str(1).c_str();
                }
                FdogSerializeBase::Instance()->JsonToBase(object_, metainfo1, value);
            }
        }

        if (OBJECT_STRUCT == objectType) {
            for(auto metainfoObject : objectinfo.metaInfoObjectList){
                //通过正则表达式获取对应的json
                smatch result;
                string regex_key = "(\"" + metainfoObject->memberName +"\")";
                string regex_value = baseRegex[metainfoObject->memberType];
                if(regex_value == ""){
                    if(objectType == OBJECT_STRUCT){
                        regex_value = objectRegex;
                    }
                    if(objectType == OBJECT_ARRAY){
                        regex_value = arrayRegex;
                    }
                }
                //根据大小写判断
                regex * pattern = nullptr;
                if (metainfoObject->memberIsIgnoreLU == false){
                    pattern = new regex(regex_key + ":" +regex_value);
                }else{
                    pattern = new regex(regex_key + ":" +regex_value,regex::icase);//icase用于忽略大小写
                }
                if(regex_search(json_, result, *pattern)){
                    string value = result.str(2).c_str();
                    if(metainfoObject->memberTypeInt == OBJECT_BASE){
                        FdogSerializeBase::Instance()->JsonToBase(object_, metainfoObject, value);
                    }
                    DesSerialize_type_judgment_all;
                }
            }
        }
        if(OBJECT_ARRAY == objectType) {
            //FDesSerializeA(object_, json_, name);
        }
    }

    //基础类型和结构体
    template<typename T>
    void FDesSerialize(T & object_, string & json_, BaseTag, string name = ""){
        DesSerialize(object_, json_, name);
    }
    //用于数组，vector，list，set
    template<typename T>
    void FDesSerialize(T & object_, string & json_, ArrayTag, string name = ""){
        int objectType;
        for(auto & object_one : object_){
            objectType = isBaseType(abi::__cxa_demangle(typeid(object_one).name(),0,0,0));
            break;
        }
        vector<string> json_array;
        if (objectType){
                smatch result;
                regex pattern(arrayRegex);
                if(regex_search(json_, result, pattern)){
                    string value = result.str(2).c_str();
                    json_array = split(value, ",");
                }
        }else{
            removeFirstComma(json_);
            removeLastComma(json_);
            json_array = FdogSerialize::Instance()->CuttingArray(json_);
        }
        int i = 0;
        for(auto & object_one : object_){
            DesSerialize(object_one, json_array[i]);
            i++;
        }
    }
    //用于map
    template<typename T>
    void FDesSerialize(T & object_, string & json_, MapTag){
        removeFirstComma(json_);
        removeLastComma(json_);
        vector<string> json_array = FdogSerialize::Instance()->CuttingArray(json_);
        int i = 0;
        int len = json_array.size();
        //这里有个问题，就是可能key的顺序不匹配
        for(auto & object_one : object_){
            DesSerialize(object_one.second, json_array[i]);
            i++;
        }
    }
};
namespace Fdog{
    
template<typename T>
void FJson(string & json_, T & object_, string name = "") {
    FdogSerialize::Instance()->FSerialize(json_, object_, typename TagDispatchTrait<T>::Tag{}, name);
}

template<typename T>
void FObject(T & object_, string & json_, string name = ""){
  FdogSerialize::Instance()->FDesSerialize(object_, json_, typename TagDispatchTrait<T>::Tag{}, name);
}

// template<typename T>
// void FJson(string & json_, T & object_, string name = "") {
//     FdogSerialize::Instance()->FSerialize(json_, object_, name);
// }

// template<typename T>
// void FJsonA(string & json_, T & object_, string name = "") {
//     FdogSerialize::Instance()->FSerializeA(json_, object_, name);
// }

// template<typename T>
// void FJsonM(string & json_, T & object_, string name = "") {
//     FdogSerialize::Instance()->FSerializeM(json_, object_, name);
// }

// template<typename T>
// void FObjectA(T & object_, string & json_, string name = ""){
//   FdogSerialize::Instance()->FDesSerializeA(object_, json_, name);
// }

// template<typename T>
// void FObjectM(T & object_, string & json_, string name = ""){
//   FdogSerialize::Instance()->FDesSerializeM(object_, json_);
// }


// //设置别名
// void setAliasName1(string Type, string memberName, string AliasName){
//   FdogSerialize::Instance()->setAliasName(Type, memberName, AliasName);
// }

// //设置是否忽略该字段序列化
// void setIgnoreField1(string Type, string memberName){
//   FdogSerialize::Instance()->setIgnoreField(Type, memberName);
// }

// //设置是否忽略大小写
// void setIgnoreLU(string Type, string memberName){
//   FdogSerialize::Instance()->setIgnoreLU(Type, memberName);
// }

/*
{
    "test1": "wx9fdb8ble7ce3c68f",
    "test2": "123456789",
    "testData1": {   
        "testdatason1": "97895455"
        "testdatason2":3,
    }
}

*/

/*
    "key" : "dsasadsadsadsa"
    "key" : 32131
    "key" : true
*/

// //json是否正确
// bool Valid(string json_) {
//     //1. 两个花括号是否存在
//     //2. 将key:value 转化为 *：*  判断是否有不存在的情况
//     //3. 判断
//     return true;
// }

// //所查找字段是否存在
// bool Exists(string field_) {
//     return true;
// }

// //获取int类型的值
// int GetIntValue(string json_, string field_) {
//     return 11;
// }

// //获取double类型的值
// double GetDoubleValue(string json_, string field_) {
//     return 11.3;
// }

// //获取string类型的值
// string GetStringValue(string json_, string field_) {
//     return "xxx";
// }

// //获取bool类型的值
// bool GetBoolValue(string json_, string field_) {
//     return true;
// }
}

#define NAME(x) #x

#define ARG_N(...) \
    ARG_N_(0, ##__VA_ARGS__, ARG_N_RESQ()) 

#define ARG_N_(...) \
    ARG_N_M(__VA_ARGS__)

#define ARG_N_M(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, _11, _12, _13, _14, _15, _16, _17, _18, _19,_20, N,...) N

#define ARG_N_RESQ() 20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0

#define MEMBERTYPE(TYPE, MEMBER) FdogSerialize::Instance()->getMemberAttribute(abi::__cxa_demangle(typeid(((TYPE *)0)->MEMBER).name(),0,0,0))

#define PLACEHOLDER(placeholder, ...) placeholder

#define REGISTEREDMEMBER(TYPE, ...) \
do{ \
    ObjectInfo * objectinfo_one = new ObjectInfo();\
    objectinfo_one->objectType = NAME(TYPE);\
    objectinfo_one->objectTypeInt = FdogSerialize::Instance()->getObjectTypeInt(objectinfo_one->objectType, abi::__cxa_demangle(typeid(TYPE).name(),0,0,0));\
    objectinfo_one->objectSize = sizeof(TYPE);\
    FdogSerialize::Instance()->addObjectInfo(objectinfo_one);\
    REGISTEREDMEMBER_s_1(TYPE, PLACEHOLDER(__VA_ARGS__), objectinfo_one->metaInfoObjectList, ARG_N(__VA_ARGS__) - 1, ##__VA_ARGS__, PLACEHOLDER(__VA_ARGS__));\
}while(0);

#define REGISTEREDMEMBER_s_1(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_2(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_2(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_3(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_3(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_4(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_4(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_5(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_5(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_6(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_6(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_7(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_7(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_8(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_8(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_9(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_9(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_10(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_10(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_11(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_11(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_12(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_12(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_13(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_13(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_14(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_14(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_15(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_15(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_16(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_16(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_17(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_17(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_18(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_18(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_19(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_19(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1); if (size > 0) REGISTEREDMEMBER_s_20(TYPE, PLACE, metaInfoObjectList, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_20(TYPE, PLACE, metaInfoObjectList, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg1);

#define REGISTEREDMEMBER_s(TYPE, metaInfoObjectList, arg) \
    do{\
        MetaInfo * metainfo_one = new MetaInfo();\
        metainfo_one->memberName = NAME(arg);\
        metainfo_one->memberAliasName = "";\
        metainfo_one->memberOffset = offsetof(TYPE, arg);\
        memberAttribute resReturn = MEMBERTYPE(TYPE, arg);\
        metainfo_one->memberType = resReturn.valueType;\
        metainfo_one->memberTypeSize = sizeof(TYPE);\
        metainfo_one->memberArraySize = resReturn.ArraySize;\
        metainfo_one->memberTypeInt = resReturn.valueTypeInt;\
        metainfo_one->first = resReturn.first;\
        metainfo_one->second = resReturn.second;\
        metainfo_one->memberIsIgnore = false;\
        metainfo_one->memberIsIgnoreLU = false;\
        metaInfoObjectList.push_back(metainfo_one);\
    }while(0);

#endif