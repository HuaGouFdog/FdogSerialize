#ifndef FDOGSERIALIZEBASE_H
#define FDOGSERIALIZEBASE_H
#include "utils.h"
#include <mutex>
#include <map>
#include <vector>
#include <string.h>

using namespace std;

namespace fsj{

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
    int memberTypeInt;
    bool memberIsIgnore;                                                                                                                                                                                           
}MetaInfo;

//声明序列化base类
class FdogSerializeBase {
    private:
    static mutex * mutex_base;
    static FdogSerializeBase * fdogserializebase;

    public:
    static FdogSerializeBase * Instance();

    template<class T>
    string getValueByAddress(string valueType, T & object, int offsetValue){
        if(valueType == "char*"){
            auto value = *((const char **)((void *)&object + offsetValue));
            string str_value = value;
            return aQuotationMark + str_value  + aQuotationMark;
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
        //好像需要清理缓存
        if(valueType == "bool"){
            ss >> *((bool *)((void *)&object + offsetValue));
        }
        if(valueType == "char"){
            //可能溢出
            ss >> *((char *)((void *)&object + offsetValue));
        }
        if(valueType == "unsigned char"){
            //可能溢出
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
    // //json转基础类型
    template<class T>
    void JsonToBase(T & object_, MetaInfo * metainfoobject, string json_){
        //cout << "进入JsonToBase" << "json_:" << json_ << "metainfoobject->memberName:" << metainfoobject->memberName << endl;
        setValueByAddress(metainfoobject->memberType, object_, metainfoobject->memberOffset, json_);
    }
};

//序列化Array类
// class FdogSerializeArray : public FdogSerializeBase {
//     public:
//     void ArrayToJson();
//     void jsonToArray();
// };

//序列化struct类
// class FdogSerializeStruct{
    
//     private:
//     static mutex * mutex_struct;
//     static FdogSerializeStruct * fdogserializestruct;

//     //FdogObjectInfo();
//     //~FdogObjectInfo();

//     public:
    
//     static FdogSerializeStruct * Instance();

//     void StructToJson();
//     void jsonToStruct();
// };

//序列化class类
// class FdogSerializeClass : public FdogSerializeBase {
//     public:
//     void ClassToJson();
//     void jsonToClass();
// };

//序列化vector类
// class FdogSerializeVector : public FdogSerializeBase {
//     public:
//     void vectorToJson();
//     void jsonToVector();
// };

//序列化list类
// class FdogSerializeList : public FdogSerializeBase {
//     public:
//     void ListToJson();
//     void jsonToList();
// };

//序列化map类
// class FdogSerializeMap : public FdogSerializeBase {
//     public:
//     void MapToJson();
//     void jsonToMap();
// };

}

#endif