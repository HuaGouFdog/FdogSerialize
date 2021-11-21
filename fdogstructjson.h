#ifndef FDOGSTRUCTJSON_H
#define FDOGSTRUCTJSON_H
#include<iostream>
#include<vector>
#include<typeinfo>
#include<regex>
#include<map>
using namespace std;

namespace FdogStruct2Json{

//这个map是起到一个别名作用
map<string, string> valueType = {
            {"Pc", "char"}, {"PPc", "char_ptr"}, 
            {"Ph", "char"}, {"PPh", "char_ptr"}, 
            {"Pa", "char"}, {"PPa", "char_ptr"}, 
            {"Pi", "int"}, {"PPi", "int_ptr"}, 
            {"Pj", "unsigned_int"}, {"PPj", "unsigned_int_ptr"}, 
            {"Ps", "short_int"}, {"PPs", "short_int_ptr"},
            {"Pt", "unsigned_short_int"}, {"PPt", "unsigned_short_int_ptr"},
            {"Pl", "long"}, {"PPl", "long_ptr"},
            {"Pm", "unsigned_long"}, {"PPm", "unsigned_long_ptr"},
            {"Pf", "float"}, {"PPf", "float_ptr"},
            {"Pd", "double"}, {"PPd", "double_ptr"},
            {"Pe", "long_double"}, {"PPe", "long_double_ptr"}
};

//这个map是根据类型来使用不同正则表达式的
map<string, string> typeRegex = {
            {"char", "1"}, {"char_ptr", "1"}, 
            {"char", "1"}, {"char_ptr", "1"}, 
            {"char", "1"}, {"char_ptr", "1"}, 
            {"int", "(\\d+)"}, {"int_ptr", "1"}, 
            {"unsigned_int", "(\\d+)"}, {"unsigned_int_ptr", "1"}, 
            {"short_int", "(\\d+)"}, {"short_int_ptr", "1"},
            {"unsigned_short_int", "(\\d+)"}, {"unsigned_short_int_ptr", "1"},
            {"long", "(\\d+)"}, {"long_ptr", "1"},
            {"unsigned_long", "(\\d+)"}, {"unsigned_long_ptr", "1"},
            {"float", "(\\d+.\\d+)"}, {"float_ptr", "1"},
            {"double", "(\\d+.\\d+)"}, {"double_ptr", "1"},
            {"long_double", "(\\d+.\\d+)"}, {"long_double_ptr", "1"}
};

typedef struct metaInfo{
    string memberName;      //成员名名称
    size_t memberOffset;    //成员所在偏移量
    string memberType;      //成员类型
}metaInfo;

typedef struct structInfo{
    string structType;  //结构体类型
    vector<metaInfo> metainfoStruct;  //结构体元信息 
}structInfo;

static vector<structInfo> structinfo;

void RemoveLastComma(string & return_){
    return_.pop_back();
}

structInfo & getStructInfo(string name){
    name = name.substr(1);
    for(int i = 0; i < structinfo.size(); i++){
        if (structinfo[i].structType == name){
            return structinfo[i];
        }
    }
}

template<class T, class M>
void setValueByAddress(string type_str, T &info, int vc, M data){

    //如果不是基础类型，就必须继续递归。
    // if(type_str == "Pc"){
    //     *((char *)((void *)&info + vc)) = ((char*)data)[0];
    // }
    // if(type_str == "PPc"){
    //     *((const char **)((void *)&info + vc)) = (char *)data;
    // }
    // if(type_str == "Ph"){
    //     *((char *)((void *)&info + vc)) = ((char*)data)[0];
    // }
    // if(type_str == "Pa"){
    //     *((char *)((void *)&info + vc)) = ((char*)data)[0];
    // }
    if(type_str == "int"){
        *((int *)((char *)&info + vc)) = data;
    }
    // if(type_str == "Pj"){
    //     *((unsigned int *)((char *)&info + vc)) = data;
    // }
    // if(type_str == "Ps"){
    //     *((short *)((char *)&info + vc)) = data;
    // }
    // if(type_str == "Pt"){
    //     *((unsigned short *)((char *)&info + vc)) = data;
    // }
    // if(type_str == "Pl"){
    //     *((long *)((char *)&info + vc)) = data;
    // }
    // if(type_str == "Pm"){
    //     *((unsigned long *)((char *)&info + vc)) = data;
    // }
    if(type_str == "float"){
        *((float *)((char *)&info + vc)) = data;
    }
    if(type_str == "double"){
        *((double *)((char *)&info + vc)) = data;
    }
    // if(type_str == "Pe"){
    //     *((long double *)((char *)&info + vc)) = data;
    // }
}

template<class T>
string getValueByAddress(string type_str, T &info, int vc){
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

/*struct转json*/
template<typename T>
void FdogStructToJson(T & struct_, string json_){
//获取传进来结构体对应的元信息
structInfo a = getStructInfo(typeid(T).name());

    for(int i = 0; i < a.metainfoStruct.size(); i++){

        smatch result;
        string regex_key = "(\"" + a.metainfoStruct[i].memberName +"\")";
        string regex_value = typeRegex[a.metainfoStruct[i].memberType];
        //这个正则表达式应该根据类型进行调整
        regex pattern(regex_key + ":" +regex_value);

        if(regex_search(json_, result, pattern)){
            //需要做类型判断
            if(a.metainfoStruct[i].memberType == "int"){
                auto value = atoi(result.str(2).data());
                setValueByAddress(a.metainfoStruct[i].memberType, struct_, a.metainfoStruct[i].memberOffset, value);
            }
            if(a.metainfoStruct[i].memberType == "float"){
                auto value = atof(result.str(2).data());
                setValueByAddress(a.metainfoStruct[i].memberType, struct_, a.metainfoStruct[i].memberOffset, value);
            }
            if(a.metainfoStruct[i].memberType == "double"){
                auto value = atof(result.str(2).data());
                setValueByAddress(a.metainfoStruct[i].memberType, struct_, a.metainfoStruct[i].memberOffset, value);
            }  
        }
    }
}

/*json转struct*/
template<typename T>
void FdogJsonToStruct(string & json_, T & struct_){
    //获取传进来结构体对应的元信息
    structInfo info = getStructInfo(typeid(T).name());
    string curlyBracketL = "{";
    string curlyBracketR = "}";
    string quotationMark = "\"";
    string json = "";
    for(int i = 0; i < info.metainfoStruct.size(); i++){
        string s6 = getValueByAddress(info.metainfoStruct[i].memberType, struct_, info.metainfoStruct[i].memberOffset);
        json = json + quotationMark + info.metainfoStruct[i].memberName + quotationMark + ":" + s6 + ",";
    }
    RemoveLastComma(json);
    json_ = curlyBracketL + json + curlyBracketR;
}

#define NAME(x) #x

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)  //获取偏移值

#define ARG_N(...) \
    ARG_N_(0, ##__VA_ARGS__, ARG_N_RESQ()) 

#define ARG_N_(...) \
    ARG_N_M(__VA_ARGS__)

#define ARG_N_M(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, N,...) N

#define ARG_N_RESQ() 10,9,8,7,6,5,4,3,2,1,0


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
        metainfo_one.memberOffset = offsetof(TYPE, arg);\
        metainfo_one.memberType = MEMBERTYPE(TYPE, arg);\
        metainfoStruct.push_back(metainfo_one);\
    }while(0);

#define MEMBERTYPE(TYPE, MEMBER) valueType[typeid(&((TYPE *)0)->MEMBER).name()]//获取类型

}

#endif