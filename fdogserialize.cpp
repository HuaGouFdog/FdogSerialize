#include "fdogserialize.h"

FdogSerialize * FdogSerialize::fdogserialize = nullptr;
mutex * FdogSerialize::mutex_serialize = new(mutex);

FdogSerialize::FdogSerialize(){
    ObjectInfo * objectinfo = new ObjectInfo();
    objectinfo->objectType = "NULL";
    objectinfo->objectTypeInt = -1;
    this->objectInfoList.push_back(objectinfo);
}

FdogSerialize::~FdogSerialize(){
    //释放内存
    delete(FdogSerialize::mutex_serialize);
    delete(FdogSerialize::fdogserialize);
}

FdogSerialize * FdogSerialize::Instance(){
    mutex_serialize->lock();
    if(fdogserialize == nullptr){
        fdogserialize = new FdogSerialize();
    }
    mutex_serialize->unlock();
    return fdogserialize;
}

void FdogSerialize::addObjectInfo(ObjectInfo * objectinfo){
    //cout << "addObjectInfo-----"  << &objectinfo << endl;
    this->objectInfoList.push_back(objectinfo);
}

ObjectInfo & FdogSerialize::getObjectInfo(string objectName){
    //removeNumbers(objectName);
    for(auto objectinfo : this->objectInfoList){
        //cout << "getObjectInfo:" << objectName << "---" << &objectinfo << "--" << objectinfo->objectType<< endl;
        if(objectinfo->objectType == objectName){
            //cout << "找到" << endl;
            return *objectinfo;
        }
    }
    return *(this->objectInfoList[0]);
}

void FdogSerialize::setAliasName(string type, string name, string aliasName){
    ObjectInfo & objectinfo = this->getObjectInfo(type);
    for(auto metainfoObject : objectinfo.metaInfoObjectList){
        if(metainfoObject->memberName == name){
            metainfoObject->memberAliasName = aliasName;
            break;
        }
    }
}

void FdogSerialize::setIgnoreField(string Type, string Name){
    ObjectInfo & objectinfo = this->getObjectInfo(Type);
    for(auto metainfoObject : objectinfo.metaInfoObjectList){
        if(metainfoObject->memberName == Name){
            metainfoObject->memberIsIgnore = true;
            break;
        }
    }
}

/***********************************
*   返回对应的成员类型(包括基本类型和自定义类型)，数组大小
************************************/
memberAttribute FdogSerialize::getMemberAttribute(string typeName){
    cout << "getMemberAttribute----------"<<typeName <<endl;
    memberAttribute resReturn;

    if(FdogSerialize::isBaseType(typeName)){
        resReturn.valueType = typeName;
        resReturn.ArraySize = 0;
        resReturn.valueTypeInt = MEMBER_BASE;
    }
    else if(FdogSerialize::isVectorType("", typeName)){
        ObjectInfo objectinfo = getObjectInfoByType(typeName, MEMBER_VECTOR);  
        resReturn.valueType = objectinfo.objectType;
        resReturn.ArraySize = 0;
        resReturn.valueTypeInt = MEMBER_VECTOR;
    }
    else if(FdogSerialize::isListType("", typeName)){
        ObjectInfo objectinfo = getObjectInfoByType(typeName, MEMBER_LIST);  
        resReturn.valueType = objectinfo.objectType;
        resReturn.ArraySize = 0;
        resReturn.valueTypeInt = MEMBER_LIST;
    }
    else if(FdogSerialize::isMapType("", typeName)){
        ObjectInfo objectinfo = getObjectInfoByType(typeName, MEMBER_MAP);  
        resReturn.valueType = objectinfo.objectType;
        resReturn.ArraySize = 0;
        resReturn.valueTypeInt = MEMBER_MAP;
    }
    else if(FdogSerialize::isArrayType("", typeName)){
        ObjectInfo objectinfo = getObjectInfoByType(typeName, MEMBER_ARRAY);  
        resReturn.valueType = objectinfo.objectType;
        resReturn.ArraySize = 0;
        resReturn.valueTypeInt = MEMBER_ARRAY;
    }else{
        resReturn.valueType = typeName;
        resReturn.ArraySize = 0;
        resReturn.valueTypeInt = MEMBER_STRUCT;
    }
    // if(baseType.count(typeName) != 0){
    //     resReturn.valueType = baseType[typeName];  
    //     resReturn.ArraySize = 0;
    //     resReturn.valueTypeInt = MEMBER_BASE;
    //     return resReturn;
    // } else if (basePointerType.count(typeName) != 0){
    //     resReturn.valueType = basePointerType[typeName];
    //     resReturn.ArraySize = 0;
    //     resReturn.valueTypeInt = MEMBER_BASE;
    // } else {
    //     smatch result;
    //     regex pattern(patternArray);
    //     if(regex_search(typeName, result, pattern)){
    //         string typeName_ = result.str(2) + result.str(4);
    //         if(baseArrayType.count(typeName_) != 0){
    //             resReturn.valueType = baseArrayType[typeName_];
    //         } else {
    //             objectArrayType[typeName_] = result.str(4) + "_array";
    //             resReturn.valueType = objectArrayType[typeName_];
    //         }
    //         resReturn.ArraySize = atoi(result.str(3).data());
    //         resReturn.valueTypeInt = MEMBER_ARRAY;
    //         return resReturn;
    //     } else {
    //         smatch result;
    //         regex pattern(patternObject);
    //         if(regex_search(typeName, result, pattern)){
    //             string str = result.str(2) + " " + result.str(3);
    //             resReturn.valueType = result.str(3);
    //             resReturn.ArraySize = atoi(result.str(3).data());
    //             resReturn.valueTypeInt = MEMBER_STRUCT;
    //             return resReturn;
    //         }
    //     }
    // }
    return resReturn;
}

int FdogSerialize::getObjectTypeInt(string objectName, string typeName){
    if(FdogSerialize::Instance()->isBaseType(objectName)){
        return OBJECT_BASE;
    }
    if(FdogSerialize::Instance()->isVectorType(objectName, typeName)){
        return OBJECT_VECTOR;
    }
    if(FdogSerialize::Instance()->isMapType(objectName, typeName)){
        return OBJECT_MAP;
    }
    if(FdogSerialize::Instance()->isListType(objectName, typeName)){
        return OBJECT_LIST;
    }
    if(FdogSerialize::Instance()->isStructType(objectName, typeName)){
        return OBJECT_STRUCT;    
    }
    if(FdogSerialize::Instance()->isArrayType(objectName, typeName)){
        return OBJECT_ARRAY;
    }
}

ObjectInfo FdogSerialize::getObjectInfoByType(string typeName, int objectTypeInt){
    smatch result;
    regex pattern(complexRegex[objectTypeInt]);
    switch (objectTypeInt)
    {
    case OBJECT_VECTOR:
        //截取值
        if(regex_search(typeName, result, pattern)){
            string value = result.str(1).c_str();
            return getObjectInfo(value);
        }
        break;
    case OBJECT_LIST:
        if(regex_search(typeName, result, pattern)){
            string value = result.str(1).c_str();
            return getObjectInfo(value);
        }
        break;        
    case OBJECT_MAP:
        break;
    case OBJECT_ARRAY:
        break;
    default:
        break;
    }
}

void * getInstance(){
    return FdogSerialize::Instance();
}

int FdogSerialize::getObjectTypeByObjectInfo(string objectName){
    return getObjectInfo(objectName).objectTypeInt;
}

bool FdogSerialize::isBaseType(string typeName){
    vector<string>::iterator result = find(baseType1.begin(), baseType1.end(), typeName);
    if(result != baseType1.end()){
        return true;
    } 
    return false;
}

//判断是否为vector类型
bool FdogSerialize::isVectorType(string objectName, string typeName){
    auto x = typeName.find("std::vector<");
    if(x != string::npos && x == 0){
        return true;
    }
    return false;
}

//判断是否为map类型
bool FdogSerialize::isMapType(string objectName, string typeName){
    auto x = typeName.find("std::map<");
    if(x != string::npos && x == 0){
        return true;
    }
    return false;
}

//判断是否是list类型
bool FdogSerialize::isListType(string objectName, string typeName){
    auto x = typeName.find("std::__cxx11::list");
    if(x != string::npos && x == 0){
        return true;
    }
    return false;
}

bool FdogSerialize::isStructType(string objectName, string typeName){
    if(objectName == typeName){
        return true;
    }
    return false;
}

//是数组返回对于值，不是数组返回0
bool FdogSerialize::isArrayType(string objectName, string typeName){
    
    return false;
}
