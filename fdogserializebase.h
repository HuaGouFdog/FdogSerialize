#ifndef FDOGSERIALIZEBASE_H
#define FDOGSERIALIZEBASE_H
#include "utils.h"
#include <mutex>
#include <map>
#include <vector>
#include <string.h>

using namespace std;

namespace fsj{

//声明序列化base类
class FdogSerializeBase {
    private:
    mutex * mutex_base;
    FdogSerializeBase * fdogserializebase;

    public:
    static FdogSerializeBase * Instance();

    template<class T>
    string getValueByAddress(string valueType, T & object, int offsetValue){
        if(valueType == "char_ptr"){
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
        if(valueType == "char" || valueType == "signed_char"){
            auto value = *((char *)((void *)&object + offsetValue));
            return to_string((int)value);
        }
        if(valueType == "unsigned_char"){
            auto value = *((char *)((void *)&object + offsetValue));
            return to_string((unsigned int)value);
        }
        if(valueType == "int" || valueType == "signed_int"){
            auto value = *((int *)((char *)&object + offsetValue));
            return to_string(value);
        }
        if(valueType == "unsigned_int"){
            auto value = *((unsigned int *)((char *)&object + offsetValue));
            return to_string(value);
        }
        if(valueType == "short_int" || valueType == "signed_short_int"){
            auto value = *((short int *)((char *)&object + offsetValue));
            return to_string(value);
        }
        if(valueType == "unsigned_short_int"){
            auto value = *((unsigned short int *)((char *)&object + offsetValue));
            return to_string(value);
        }
        if(valueType == "long_int" || valueType == "signed_long_int"){
            auto value = *((long int *)((char *)&object + offsetValue));
            return to_string(value);
        }
        if(valueType == "unsigned_long_int"){
            auto value = *((unsigned long int *)((char *)&object + offsetValue));
            return to_string(value);
        }
        if(valueType == "long_long_int" || valueType == "signed_long_long_int"){
            auto value = *((long long int *)((char *)&object + offsetValue));
            return to_string(value);
        }
        if(valueType == "unsigned_long_long_int"){
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
        if(valueType == "long_double"){
            auto value = *((long double *)((char *)&object + offsetValue));
            return removeLastZero(value);
        }
        return "";
    }

    template<class T>
    void setValueByAddress(string valueType, T &object, int offsetValue, string value){
        if(valueType == "char_ptr"){
            *((char **)((void *)&object + offsetValue)) = new char[strlen(value.c_str())];
            strcpy(*((char **)((void *)&object + offsetValue)), value.c_str());
        }
        std::stringstream ss;
        ss.str(value);
        //好像需要清理缓存
        if(valueType == "bool"){
            ss >> *((bool *)((void *)&object + offsetValue));
        }
        if(valueType == "char" || valueType == "signed char"){
            //可能溢出
            ss >> *((char *)((void *)&object + offsetValue));
        }
        if(valueType == "unsigned char"){
            //可能溢出
            ss >> *((unsigned char *)((void *)&object + offsetValue));
        }
        if(valueType == "int" || valueType == "signed int"){
            ss >> *((int *)((char *)&object + offsetValue));
        }
        if(valueType == "unsigned_int"){
            ss >> *((unsigned int *)((char *)&object + offsetValue));
        }
        if(valueType == "short_int" || valueType == "signed_short_int"){
            ss >> *((short int *)((char *)&object + offsetValue));
        }
        if(valueType == "unsigned_short_int"){
            ss >> *((unsigned short int *)((char *)&object + offsetValue));
        }
        if(valueType == "long_int" || valueType == "signed_long_int"){
            ss >> *((long int *)((char *)&object + offsetValue));
        }
        if(valueType == "unsigned_long_int"){
            ss >> *((unsigned long int *)((char *)&object + offsetValue));
        }
        if(valueType == "long_long_int" || valueType == "signed_long_long_int"){
            ss >> *((long long int *)((char *)&object + offsetValue));
        }
        if(valueType == "unsigned_long_int"){
            ss >> *((unsigned long long  int *)((char *)&object + offsetValue));
        }  
        if(valueType == "float"){
            ss >> *((float *)((char *)&object + offsetValue));
        }
        if(valueType == "double"){
            ss >> *((double *)((char *)&object + offsetValue));
        }
        if(valueType == "long_double"){
            ss >> *((long double *)((char *)&object + offsetValue));
        }
    }

    // //基础类型转json
    template<class T>
    void BaseToJson(string & json_, MetaInfo & metainfoobject, T & object_){
        string value = getValueByAddress(metainfoobject.memberType, object_, metainfoobject.memberOffset);
        if(metainfoobject.memberAliasName != ""){
            json_ = json_ + "\"" + metainfoobject.memberAliasName + "\"" + ":" + value + ",";
        }else{
            json_ = json_ + "\"" + metainfoobject.memberName + "\"" + ":" + value + ",";
        }
    }
    // //json转基础类型
    template<class T>
    void JsonToBase(T & object_, MetaInfo & metainfoobject, string json_){
        setValueByAddress(metainfoobject.memberType, object_, metainfoobject.memberOffset, json_);
    }
};

//序列化Array类
// class FdogSerializeArray : public FdogSerializeBase {
//     public:
//     void ArrayToJson();
//     void jsonToArray();
// };

//序列化struct类
class FdogSerializeStruct{
    
    private:
    static mutex * mutex_struct;
    static FdogSerializeStruct * fdogserializestruct;

    //FdogObjectInfo();
    //~FdogObjectInfo();

    public:
    
    static FdogSerializeStruct * Instance();

    void StructToJson();
    void jsonToStruct();
};

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