/*****************************
*Author:花狗Fdog(张旭)
*Email:2506897252@qq.com
*Time:2021.11.20
******************************/
#ifndef FDOGSTRUCTJSON_H
#define FDOGSTRUCTJSON_H
#include<iostream>
#include<vector>
#include<typeinfo>
#include<regex>
#include<map>
using namespace std;

namespace FdogStruct2Json{

/***********************************
*   用于存储基础类型
************************************/
map<string, string> BaseValueType = {
            {"c", "char"}, 
            {"h", "char"}, 
            {"a", "char"},
            {"i", "int"}, 
            {"j", "unsigned_int"}, 
            {"s", "short_int"}, 
            {"t", "unsigned_short_int"}, 
            {"l", "long"}, 
            {"m", "unsigned_long"}, 
            {"f", "float"},
            {"d", "double"}, 
            {"e", "long_double"},
};

/***********************************
*   用于存储基础数组类型
************************************/
map<string, string> BaseArrayValueType = {
            {"Ai", "int_array"},
            {"Aj", "unsigned_int_array"},
            {"As", "short_int_array"},
            {"At", "unsigned_short_int_array"},
            {"Al", "long_array"},
            {"Am", "unsigned_long_array"},
            {"Af", "float_array"},
            {"Ad", "double_array"},
            {"Ae", "long_double_array"},
};

/***********************************
*   用于存储基础指针类型
************************************/
map<string, string> BasePointerValueType = {
            {"Pc", "char_ptr"},
            {"Ph", "char_ptr"}, 
            {"Pa", "char_ptr"},
            {"Pi", "int_ptr"},
            {"Pj", "unsigned_int_ptr"},
            {"Ps", "short_int_ptr"},
            {"Pt", "unsigned_short_int_ptr"},
            {"Pl", "long_ptr"},
            {"Pm", "unsigned_long_ptr"},
            {"Pf", "float_ptr"}, 
            {"Pd", "double_ptr"},
            {"Pe", "long_double_ptr"},
};

/***********************************
*   用于存储自定义类型(结构体/对象)
*   自定义类型需要在运行中动态增加，任何类型都会转成基础类型,无需手动添加
************************************/
map<string, string> ObjectValueType;

/***********************************
*   用于存储自定义数组类型
*   自定义类型需要在运行中动态增加，任何类型都会转成基础类型,无需手动添加
************************************/
map<string, string> ObjectArrayValueType;

/***********************************
*   用于存储自定义指针类型
*   自定义类型需要在运行中动态增加，任何类型都会转成基础类型,无需手动添加
************************************/
map<string, string> ObjectPointerValueType;

/***********************************
*   用于提取基础类型数据的正则表达式
************************************/
map<string, string> BaseRegex = {
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
            {"int_array", "(\\[(.*?)\\])"},
};

/***********************************
*   用于提取除基础类型数据的正则表达式
************************************/
map<string, string> OtherRegex = {
            {"headmaster", "(\\{(.*?)\\})"},
            //{"teacher", "(\\{(.*?)\\})"},
            //{"student", "(\\{(.*?)\\})"},
            {"Student", "(\\{(.*?)\\})"},
            {"char_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"char_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"char_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"int_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"unsigned_int_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"short_int_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"unsigned_short_int_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"long_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"unsigned_long_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"float_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"double_ptr", "\"(.*?)\""}, {"int_array", "\\[(.*?)\\]"},
            {"long_double_ptr", "\"(.*?)\""},{"int_array", "\\[(.*?)\\]"},
            
};

/***********************************
*   存储结构体元信息(成员名， 成员别名（默认空，若有内容则使用别名）， 成员所在偏移量， 成员类型， 成员类型位数， 数组大小（默认0，若是数组，则存储对应大小）)
************************************/
typedef struct metaInfo{
    string memberName;
    string memberAliasName;
    size_t memberOffset;
    string memberType;
    size_t memberTypeSize;                                                         
    size_t memberArraySize;                                                                                                                                                                                           
}metaInfo;

/***********************************
*   存储结构体结构体类型，以及元信息
************************************/
typedef struct structInfo{
    string structType;  //结构体类型
    vector<metaInfo> metainfoStruct;  //结构体元信息 
}structInfo;

static vector<structInfo> structinfo; //所有结构体信息存储于此

/***********************************
*   存储成员类型，数组大小
************************************/
struct ValueTyle {
    string valueType;
    int valueTypeSize;
    int ArraySize;
};

/***********************************
*   返回对应的成员类型(包括基本类型和自定义类型)，数组大小
************************************/
ValueTyle getValueTyle(string key){
    ValueTyle res;
    if(BaseValueType.count(key) != 0){
        res.valueType = BaseValueType[key];
        res.valueTypeSize = 0;
        res.ArraySize = 0;
        return res;
    } else if (BasePointerValueType.count(key) != 0){
        res.valueType = BasePointerValueType[key];
        res.valueTypeSize = 0;
        res.ArraySize = 0;
    } else {
        smatch result;
        regex pattern("((A)(\\d+)_\\d?(\\D+))");
        if(regex_search(key, result, pattern)){
            string str = result.str(2) + result.str(4);
            if(BaseArrayValueType.count(str) != 0){
                res.valueType = BaseArrayValueType[str];
                res.valueTypeSize = 0;
            } else {
                ObjectArrayValueType[str] = result.str(4) + "_array";
                res.valueType = ObjectArrayValueType[str];
                res.valueTypeSize = 0;
            }
            res.ArraySize = atoi(result.str(3).data());
            return res;
        } else {
            smatch result;
            regex pattern("((\\d+)(\\D+))");
            if(regex_search(key, result, pattern)){
                string str = result.str(2) + " " + result.str(3);
                res.valueType = result.str(3);
                res.valueTypeSize = 0;
                res.ArraySize = atoi(result.str(3).data());
                return res;
            }
        }
    }
    return res;
}
/***********************************
*   删除字符串最后一个逗号
************************************/
void RemoveLastComma(string & return_){
    return_.pop_back();
}

/***********************************
*   查询对应的结构体信息
************************************/
structInfo & getStructInfo(string name){
    name = name.substr(1);
    for(int i = 0; i < structinfo.size(); i++){
        if (structinfo[i].structType == name){
            return structinfo[i];
        }
    }
}

template<class T, class M>
void setValueByAddress(string type_str, T &info, int vc, M data, int Size = 0){
    if(type_str == "char"){
        *((char *)((void *)&info + vc)) = data;
    }
    if(type_str == "int"){
        *((int *)((char *)&info + vc)) = data;
    }
    if(type_str == "unsigned_int"){
        *((unsigned int *)((char *)&info + vc)) = data;
    }
    if(type_str == "short_int"){
        *((short int *)((char *)&info + vc)) = data;
    }
    if(type_str == "unsigned_short_int"){
        *((unsigned short int *)((char *)&info + vc)) = data;
    }
    if(type_str == "long_int"){
        *((long int *)((char *)&info + vc)) = data;
    }
    if(type_str == "unsigned_long_int"){
        *((unsigned long int *)((char *)&info + vc)) = data;
    }
    if(type_str == "float"){
        *((float *)((char *)&info + vc)) = data;
    }
    if(type_str == "double"){
        *((double *)((char *)&info + vc)) = data;
    }
    if(type_str == "long_double"){
        *((long double *)((char *)&info + vc)) = data;
    }
}

template<class T, class M>
void setValueByAddress_S(string type_str, T &info, int vc, M data){
    if(type_str == "char_ptr"){
        char * value =  new char[strlen(data)];
        //要记得释放这个内存
        strcpy(value, data);
        *((const char **)((void *)&info + vc)) = value;
    }
}

/***********************************
*   解析基础类型
************************************/
template<class T>
void JsonToBase(T & struct_, metaInfo & metainfostruct, string json_){
            if(metainfostruct.memberType == "char_ptr"){
                auto value = json_.c_str();
                setValueByAddress_S(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "char"){
                auto value = atoi(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "int"){
                auto value = atoi(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "unsigned_int"){
                auto value = atoi(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "short_int"){
                auto value = atoi(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "unsigned_short_int"){
                auto value = atoi(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "long_int"){
                auto value = atol(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "unsigned_long_int"){
                auto value = atol(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "float"){
                auto value = atof(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
            else if(metainfostruct.memberType == "double"){
                auto value = atof(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            } 
            else if (metainfostruct.memberType == "long_double"){
                auto value = atof(json_.c_str());
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset, value);
            }
}

/***********************************
*   解析对象
************************************/
template<class T>
void FdogJsonToStruct(T & struct_, string json_);

template<class T>
void JsonToObject(T & struct_, string json_){
    //这里应该传子对象变量
    
    FdogJsonToStruct(struct_, json_);
}

/***********************************
*   解析数组
************************************/
template<class T>
void JsonToArray(T & struct_, metaInfo & metainfostruct, string json_){
    //只考虑是不是数组，分两种，基础数组直接调用，对象数组分批调用
    map<string, string>::iterator iter;
    iter = BaseArrayValueType.begin();
    bool isBaseArray = false;
    while(iter != BaseArrayValueType.end()) {
        if(metainfostruct.memberType == iter->second){
            isBaseArray = true;
            break;
        }
        iter++;
    }
    if (isBaseArray){
        string value = "asd";
        for(int i = 0; i < metainfostruct.memberArraySize; i++){
            std::regex reg(",");
            std::sregex_token_iterator pos(value.begin(), value.end(), reg, -1);
            decltype(pos) end;
            int j = 0;
            for (; pos != end; ++pos){
                //这里需要判断是不是字符串数组
                setValueByAddress(metainfostruct.memberType, struct_, metainfostruct.memberOffset + (j * metainfostruct.memberTypeSize), atoi(pos->str().data()));
                j++;
            }
        }
    }else{
        for(int i = 0; i < metainfostruct.memberArraySize; i++){
            //还需要对json进行截取
            FdogJsonToStruct(struct_, json_);
        }
    }
}

// template<class T>
// void ObjectToJson(string json_, T &info){

// }
// template<class T>
// void ArrayToJson(string json_, int size, T &info){

// }
// template<class T>
// void BaseToJson(string json_, T &info){

// }

template<class T>
string getValueByAddress_S(string type_str, T &info, int vc){
    if(type_str == "char_ptr"){
        auto value = *((const char **)((void *)&info + vc));
        string str_value = value;
        return "\"" + str_value  + "\"";
    }
}

template<class T>
string getValueByAddress_N(string type_str, T &info, int vc){
    // if(type_str == "char"){
    //     auto value = *((char *)((void *)&info + vc));
    //     string char_value(1,value);
    //     return char_value;
    // }
    // if(type_str == "char"){
    //     auto value = *((char *)((void *)&info + vc));
    //     return value;
    // }

    if(type_str == "int"){
        auto value = *((int *)((char *)&info + vc));
        return to_string(value);
    }
    //浮点数默认小数点后6位，这里可以截断后面的0
    if(type_str == "float"){
        auto value = *((float *)((char *)&info + vc));
        return to_string(value);
    }
    if(type_str == "double"){
        auto value = *((double *)((char *)&info + vc));
        return to_string(value);
    }
    return "";
}

enum MetainfoType {
    FDOGBASE = 1,
    FDOGOBJECT,
    FDOGARRAY
};

/***********************************
*   获取成员类型 1：基础类型 2：自定义类型 3：数组
************************************/
int getMemberVType(string type_str, int Size){
    if(Size > 0){
        return FDOGARRAY;
    }
    map<string, string>::iterator iter;
    iter = BaseValueType.begin();
    bool isObject = true;
    while(iter != BaseValueType.end()) {
        if(type_str == iter->second){
            isObject = false;
            break;
        }
        iter++;
    }
    iter = BasePointerValueType.begin();
    while(iter != BasePointerValueType.end()) {
        if(type_str == iter->second){
            isObject = false;
            break;
        }
        iter++;
    }
    if(isObject){
        return FDOGOBJECT;
    }
    return FDOGBASE;
}

#define TURN_2(x) x

#define TURN(t, x) t.x

/***********************************
*   Json转Struct接口
************************************/
template<typename T>
void FdogJsonToStruct(T & struct_, string json_){

    structInfo a = getStructInfo(typeid(T).name());
    for(int i = 0; i < a.metainfoStruct.size(); i++){
        int Vtype = getMemberVType(a.metainfoStruct[i].memberType, a.metainfoStruct[i].memberArraySize);
        smatch result_1;
        string regex_key_1 = "(\"" + a.metainfoStruct[i].memberName +"\")";
        string regex_value_1 = BaseRegex[a.metainfoStruct[i].memberType];
        if(regex_value_1 == ""){
            if(Vtype == 2){
                regex_value_1 = "(\\{(.*?)\\})";//OtherRegex[a.metainfoStruct[i].memberType];
            }else{
                regex_value_1 = "(\\[(.*?)\\])";//OtherRegex[a.metainfoStruct[i].memberType];
            }
            
        }
        //cout << "regex_key_1=" << regex_key_1;
        //cout << "  regex_value_1=" << a.metainfoStruct[i].memberType << " - - "<< regex_value_1 << endl;
        regex pattern_1(regex_key_1 + ":" +regex_value_1);
        if(regex_search(json_, result_1, pattern_1)){
            //cout << "-----" << result_1.str(2).c_str() << endl;
        auto value = result_1.str(2).c_str();
            switch(Vtype){
                case FDOGBASE:
                    cout << "基础类型" << a.metainfoStruct[i].memberType << "=" << value << endl;
                    //规定如果是基础类型，只需要提供值即可
                    JsonToBase(struct_, a.metainfoStruct[i], value); //基础类型 提供数据
                    break;
                case FDOGOBJECT:
                /*    这里不知道什么原因，value接受的是空值      */
                    cout << "对象类型" << a.metainfoStruct[i].memberType << "=" << result_1.str(2).c_str() << endl;
                    JsonToObject(struct_, value); //object类型 本质还是结构体，需要继续调用FdogJsonToStruct
                    //cout << TURN(struct_, master) << endl;
                    break;
                case FDOGARRAY:
                    cout << "数组类型" << a.metainfoStruct[i].memberType << "=" << result_1.str(2).c_str() << endl;
                    JsonToArray(struct_, a.metainfoStruct[i], value); //数组类型 
                    break;
            }
        }
    }
}

/***********************************
*   Struct转Json接口
************************************/
template<typename T>
void FdogStructToJson(string & json_, T & struct_){
    //获取传进来结构体对应的元信息
    structInfo info = getStructInfo(typeid(T).name());
    string curlyBracketL = "{";
    string curlyBracketR = "}";
    string quotationMark = "\"";
    string json = "";
    for(int i = 0; i < info.metainfoStruct.size(); i++){
        if (info.metainfoStruct[i].memberType == "char_ptr"){
            string s6 = getValueByAddress_S(info.metainfoStruct[i].memberType, struct_, info.metainfoStruct[i].memberOffset);
            json = json + quotationMark + info.metainfoStruct[i].memberName + quotationMark + ":" + s6 + ",";
            continue;
        }
        string s6 = getValueByAddress_N(info.metainfoStruct[i].memberType, struct_, info.metainfoStruct[i].memberOffset);
        json = json + quotationMark + info.metainfoStruct[i].memberName + quotationMark + ":" + s6 + ",";
    }
    RemoveLastComma(json);
    json_ = curlyBracketL + json + curlyBracketR;

    //这里也分三种类型
    //基础类型
    //数组类型
    //自定义类型
    // for(int i = 0; i < a.metainfoStruct.size(); i++){
    //     int Vtype = getMemberVType(a.metainfoStruct[i].memberType, a.metainfoStruct[i].memberArraySize);
    //     smatch result_1;
    //     string regex_key_1 = "(\"" + a.metainfoStruct[i].memberName +"\")";
    //     string regex_value_1 = BaseRegex[a.metainfoStruct[i].memberType];
    //     if(regex_value_1 == ""){
    //         regex_value_1 = OtherRegex[a.metainfoStruct[i].memberType];
    //     }
    //     cout << "regex_value_1" << regex_value_1 << endl;
    //     regex pattern_1(regex_key_1 + ":" +regex_value_1);
    //     if(regex_search(json_, result_1, pattern_1)){
    //     auto value = result_1.str(2).c_str();
    //         switch(Vtype){
    //             case FDOGBASE:
    //                 cout << "基础类型" << a.metainfoStruct[i].memberType << "=" << value << endl;
    //                 //规定如果是基础类型，只需要提供值即可
    //                 //JsonToBase(struct_, a.metainfoStruct[i], value); //基础类型 提供数据
    //                 break;
    //             case FDOGOBJECT:
    //                 cout << "对象类型" << a.metainfoStruct[i].memberType << "=" << value << endl;
    //                 //JsonToObject(struct_, value); //object类型 本质还是结构体，需要继续调用FdogJsonToStruct
    //                 break;
    //             case FDOGARRAY:
    //                 cout << "数组类型" << a.metainfoStruct[i].memberType << "=" << value << endl;
    //                 //JsonToArray(struct_, a.metainfoStruct[i], value); //数组类型 
    //                 break;
    //         }
    //     }
    // }
}

#define ARG_N(...) \
    ARG_N_(0, ##__VA_ARGS__, ARG_N_RESQ()) 

#define ARG_N_(...) \
    ARG_N_M(__VA_ARGS__)

#define ARG_N_M(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, N,...) N

#define ARG_N_RESQ() 10,9,8,7,6,5,4,3,2,1,0

//获取原始名
#define NAME(x) #x
//获取别名
#define ALIASNAME(x) #x

//#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)  //获取偏移值

#define MEMBERTYPE(TYPE, MEMBER) getValueTyle(typeid(((TYPE *)0)->MEMBER).name())//获取类型

#define PLACEHOLDER(placeholder, ...) placeholder

#define REGISTEREDMEMBER(TYPE, ...) \
do{ \
    structInfo structinfo_one;\
    structinfo_one.structType = NAME(TYPE);\
    REGISTEREDMEMBER_s_1(TYPE, PLACEHOLDER(__VA_ARGS__), structinfo_one.metainfoStruct, ARG_N(__VA_ARGS__) - 1, ##__VA_ARGS__, PLACEHOLDER(__VA_ARGS__));\
    structinfo.push_back(structinfo_one);\
}while(0);

#define REGISTEREDMEMBER_s_1(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_2(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_2(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_3(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_3(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_4(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_4(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_5(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_5(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_6(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_6(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_7(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_7(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_8(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_8(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_9(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_9(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1); if (size > 0) REGISTEREDMEMBER_s_10(TYPE, PLACE, metainfoStruct, size-1, ##__VA_ARGS__, PLACE);

#define REGISTEREDMEMBER_s_10(TYPE, PLACE, metainfoStruct, size, arg1, ...) \
REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg1);

#define REGISTEREDMEMBER_s(TYPE, metainfoStruct, arg) \
    do{\
        metaInfo metainfo_one;\
        metainfo_one.memberName = NAME(arg);\
        metainfo_one.memberAliasName = ALIASNAME(arg);\
        metainfo_one.memberOffset = offsetof(TYPE, arg);\
        ValueTyle res = MEMBERTYPE(TYPE, arg);\
        metainfo_one.memberType = res.valueType;\
        cout << "获取到的类型" << res.valueType << endl;\
        metainfo_one.memberTypeSize = res.valueTypeSize;\
        metainfo_one.memberArraySize = res.ArraySize;\
        metainfoStruct.push_back(metainfo_one);\
    }while(0);

}

#endif

            // else if(a.metainfoStruct[i].memberType == "int_array"){
            //     string value = result.str(2).c_str();
            //     cout << "获取到的值："<< value << " 数组长度：" << a.metainfoStruct[i].memberArraySize << endl;
            //     // 数字已经找出来，然后从字符串中找到数字
    //     //for(int j = 0; j < a.metainfoStruct[i].memberArraySize; j++){
            //         //cout << "进入循环" << endl;
            //         std::regex reg(",");
            //         std::sregex_token_iterator pos(value.begin(), value.end(), reg, -1);
            //         decltype(pos) end;
            //         int j = 0;
            //         for (; pos != end; ++pos)
            //         {
            //             if(j < a.metainfoStruct[i].memberArraySize){
//         setValueByAddress_N(a.metainfoStruct[i].memberType, struct_, a.metainfoStruct[i].memberOffset + (j*4), atoi(pos->str().data()));
//             }
//             j++;
//             //std::cout << "====" << pos->str() << std::endl;//            }
                
   //     //}
            // }
        //}