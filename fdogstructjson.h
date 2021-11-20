#ifndef FDOGSTRUCTJSON_H
#define FDOGSTRUCTJSON_H
#include<iostream>
#include<vector>
#include<typeinfo>
#include<regex>
using namespace std;

namespace FdogStruct2Json{

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
    if(type_str == "Pc"){
        *((char *)((void *)&info + vc)) = ((char*)data)[0];
    }
    if(type_str == "PPc"){
        *((const char **)((void *)&info + vc)) = (char *)data;
    }
    if(type_str == "Ph"){
        *((char *)((void *)&info + vc)) = ((char*)data)[0];
    }
    if(type_str == "Pa"){
        *((char *)((void *)&info + vc)) = ((char*)data)[0];
    }
    if(type_str == "Pi"){
        *((int *)((char *)&info + vc)) = (*(int *)&data);
    }
    if(type_str == "Pj"){
        *((unsigned int *)((char *)&info + vc)) = *(unsigned int *)&data;
    }
    if(type_str == "Ps"){
        *((short *)((char *)&info + vc)) = *(short *)&data;
    }
    if(type_str == "Pt"){
        *((unsigned short *)((char *)&info + vc)) = *(unsigned short *)&data;
    }
    if(type_str == "Pl"){
        *((long *)((char *)&info + vc)) = *(long *)&data;
    }
    if(type_str == "Pm"){
        *((unsigned long *)((char *)&info + vc)) = *(unsigned long *)&data;
    }
    if(type_str == "Pf"){
        *((float *)((char *)&info + vc)) = *(float *)&data;
    }
    if(type_str == "Pd"){
        *((double *)((char *)&info + vc)) = *(double *)&data;
    }
    if(type_str == "Pe"){
        *((long double *)((char *)&info + vc)) = *(long double *)&data;
    }
}

template<class T>
string getValueByAddress(string type_str, T &info, int vc){
    int value;
    if(type_str == "Pi"){
        value = *((int *)((char *)&info + vc));
    }
    return to_string(value);
}

/*struct转json*/
template<typename T>
void FdogStructToJson(T & struct_, string json_){
//获取传进来结构体对应的元信息
structInfo a = getStructInfo(typeid(T).name());

    for(int i = 0; i < a.metainfoStruct.size(); i++){

        smatch result;
        string regex1 = "(\"";
        string regex_key = regex1 + a.metainfoStruct[i].memberName +"\")";
        string regex_value = "(\\d+)";
        regex pattern(regex_key + ":" +regex_value);

        if(regex_search(json_, result, pattern)){
            int value = atoi(result.str(2).data());
            setValueByAddress(a.metainfoStruct[i].memberType, struct_, a.metainfoStruct[i].memberOffset, value);
        }
    }
}

/*json转struct*/
template<typename T>
void FdogJsonToStruct(string & json_, T & struct_){
    //获取传进来结构体对应的元信息
    structInfo a = getStructInfo(typeid(T).name());
    string s1 = "{";
    string s2 = "}";
    string s3 = "\"";
    string s4 = "\"";
    string s5 = "";
    for(int i = 0; i < a.metainfoStruct.size(); i++){
        // cout << a.metainfoStruct[i].memberName << endl;
        string s6 = getValueByAddress(a.metainfoStruct[i].memberType, struct_, a.metainfoStruct[i].memberOffset);
        s5 = s5 + s3 + a.metainfoStruct[i].memberName + s4 + ":" + s6 + ",";
    }
    RemoveLastComma(s5);
    json_ = s1 +s5 + s2;
}

#define NAME(x) #x

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)  //获取偏移值

#define ARG_N(...) \
    ARG_N_(0, ##__VA_ARGS__, ARG_N_RESQ()) 

#define ARG_N_(...) \
    ARG_N_M(__VA_ARGS__)

#define ARG_N_M( \
          _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N

#define ARG_N_RESQ() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0


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

#define MEMBERTYPE(TYPE, MEMBER) typeid(&((TYPE *)0)->MEMBER).name()//获取类型

}

#endif