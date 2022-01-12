#ifndef FDOGSERIALIZE_H
#define FDOGSERIALIZE_H

#include "fdogserializebase.h"
#include "behavior.h"
#include <map>
#include <vector>
#include <regex>
using namespace fsj;

// static vector<string> baseType = {
//         "int", "char*"
//         }

/***********************************
*         基础存储基础类型            *
************************************/
static map<string, string> baseType = {
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

/***********************************
*         存储基础指针类型            *
************************************/
static map<string, string> basePointerType = {
            {"Pb", "bool_ptr"},
            {"Pc", "char_ptr"},
            {"Ph", "signed_char_ptr"}, 
            {"Pa", "unsigned_char_ptr"},
            {"Pi", "int_ptr"},
            {"Pi", "signed_int_ptr"},
            {"Pj", "unsigned_int_ptr"},
            {"Ps", "short_int_ptr"},
            {"Ps", "signed_short_int_ptr"},
            {"Pt", "unsigned_short_int_ptr"},
            {"Pl", "long_int_ptr"},
            {"Pl", "signed_long_int_ptr"},
            {"Pm", "unsigned_long_int_ptr"},
            {"Px", "long_long_int_ptr"}, 
            {"Px", "signed_long_long_int_ptr"},
            {"Py", "unsigned_long_long_ptr"}, 
            {"Pf", "float_ptr"}, 
            {"Pd", "double_ptr"},
            {"Pe", "long_double_ptr"},
};


/***********************************
*         存储基础数组类型
************************************/
static map<string, string> baseArrayType = {
            {"Ab", "bool_array"},
            {"Ac", "char_array"},
            {"Ah", "char_array"},
            {"Aa", "char_array"},
            {"Ai", "int_array"},
            {"Ai", "signed_int_array"},
            {"Aj", "unsigned_int_array"},
            {"As", "short_int_array"},
            {"As", "signed_short_int_array"},
            {"At", "unsigned_short_int_array"},
            {"Al", "long_int_array"},
            {"Al", "signed_long_int_array"},
            {"Am", "unsigned_long_int_array"},
            {"Ax", "long_long_int_array"}, 
            {"Ax", "signed_long_long_int_array"},
            {"Ay", "unsigned_long_long_int_array"}, 
            {"Af", "float_array"},
            {"Ad", "double_array"},
            {"Ae", "long_double_array"},
            {"APc", "char_ptr_array"},
};

/***********************************
*   用于存储自定义类型(结构体/对象)
*   自定义类型需要在运行中动态增加，任何类型都会转成基础类型,无需手动添加
************************************/
static map<string, string> objectType;

/***********************************
*   用于存储自定义数组类型
*   自定义类型需要在运行中动态增加，任何类型都会转成基础类型,无需手动添加
************************************/
static map<string, string> objectArrayType;

/***********************************
*   用于存储自定义指针类型
*   自定义类型需要在运行中动态增加，任何类型都会转成基础类型,无需手动添加
************************************/
static map<string, string> objectPointerType;

/***********************************
*   用于提取基础类型数据的正则表达式
************************************/
static map<string, string> baseRegex = {
            {"char", "(\\d+)"}, 
            {"char", "(\\d+)"}, 
            {"char", "(\\d+)"},
            {"int", "(\\d+)"}, 
            {"unsigned_int", "(\\d+)"},
            {"short_int", "(\\d+)"}, 
            {"unsigned_short_int", "(\\d+)"}, 
            {"long", "(\\d+)"}, 
            {"unsigned_long", "(\\d+)"}, 
            {"float", "(\\d+.\\d+)"}, 
            {"double", "(\\d+.\\d+)"},
            {"long_double", "(\\d+.\\d+)"},
            {"char_ptr", "\"(.*?)\""},
            //{"int_array", "(\\[(.*?)\\])"},
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
};
enum MemberType{
    MEMBER_BASE = 1,
    MEMBER_STRUCT,
    MEMBER_CLASS,
    MEMBER_ARRAY,
    MEMBER_VECTOR,
    MEMBER_MAP,
    MEMBER_LIST,
};

/***********************************
*   存储结构体结构体类型，以及元信息
************************************/
typedef struct ObjectInfo{
    string objectType;  //结构体类型 字符串表示
    int objectTypeInt;  //结构体类型 数值表示
    vector<MetaInfo *> metaInfoObjectList;  //结构体元信息 
}ObjectInfo;

/***********************************
*   存储成员类型，数组大小
************************************/
struct memberAttribute {
    string valueType;
    int valueTypeInt; //类型 数值表示
    int ArraySize;
};

class FdogSerialize {

    private:
    static mutex * mutex_serialize;
    static FdogSerialize * fdogserialize;
    vector<ObjectInfo *> objectInfoList;

    FdogSerialize();
    ~FdogSerialize();

    public:
    //获取实例
    static FdogSerialize * Instance();

    //添加objectinfo
    void addObjectInfo(ObjectInfo * objectinfo);

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
        ObjectInfo & objectinfo = getObjectInfo(abi::__cxa_demangle(typeid(T).name(),0,0,0));
        int objectType = getObjectType(abi::__cxa_demangle(typeid(T).name(),0,0,0));
        for(auto metainfoObject : objectinfo.metaInfoObjectList){
            string json_s;
            if(metainfoObject->memberTypeInt == MEMBER_BASE){
                FdogSerializeBase::Instance()->BaseToJson(json_, metainfoObject, object_);
            }
            Serialize_type_judgment_all;
            if(metainfoObject->memberTypeInt == MEMBER_STRUCT){
                removeLastComma(json_s);
                json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "{" + json_s + "}";
                json_s = "";
            }
            if(metainfoObject->memberTypeInt == MEMBER_ARRAY){
                removeLastComma(json_s);
                json_ = json_ + "\"" + metainfoObject->memberName + "\"" + ":" + "[" + json_s + "]";
                json_s = "";
            }
        }
    }

    template<typename T>
    void FSerialize(string & json_, T & object_){
        Serialize(json_, object_);
        json_ = "{" + json_ + "}";
        //这里需要判断类型
    }

    //反序列化
    template<typename T>
    void DesSerialize(T & object_, string & json_){
        //cout << "地址：--" << &object_ << endl;
        ObjectInfo & objectinfo = getObjectInfo(abi::__cxa_demangle(typeid(T).name(),0,0,0));
        int objectType = getObjectType(abi::__cxa_demangle(typeid(T).name(),0,0,0));
        for(auto metainfoObject : objectinfo.metaInfoObjectList){
            //通过正则表达式获取对应的json
            //cout << "总类型：" << objectType << "--当前成员：" << metainfoObject->memberName << " 类型为：" << metainfoObject->memberType << endl;
            smatch result;
            string regex_key = "(\"" + metainfoObject->memberName +"\")";
            string regex_value = baseRegex[metainfoObject->memberType];
            if(regex_value == ""){
                if(objectType == 2){
                    regex_value = objectRegex;
                }
                if(objectType == 3){
                    regex_value = arrayRegex;
                }
                if(objectType == 4){
                    regex_value = arrayRegex;
                }
            }
            regex pattern(regex_key + ":" +regex_value);
            //cout << "metainfoObject->memberType=" << metainfoObject->memberType << " regex_key=" << regex_key << "regex_value" << regex_value << endl;
            //这里就要截取对应的json
            if(regex_search(json_, result, pattern)){
                string value = result.str(2).c_str();
                //cout << "value=" << value << " type=" <<metainfoObject->memberTypeInt << "--"<<metainfoObject->memberType << endl;
                //----------------------------------

                if(metainfoObject->memberTypeInt == MEMBER_BASE){
                    FdogSerializeBase::Instance()->JsonToBase(object_, metainfoObject, value);
                }
                DesSerialize_type_judgment_all;
            }
        }
    }

    template<typename T>
    void FDesSerialize(T & object_, string & json_){
        DesSerialize(object_, json_);
    }

};

void * getInstance();

#endif