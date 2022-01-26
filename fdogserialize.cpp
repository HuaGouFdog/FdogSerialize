#include "fdogserialize.h"

FdogSerializeBase * FdogSerializeBase::fdogserializebase = nullptr;
mutex * FdogSerializeBase::mutex_base = new(mutex);

FdogSerializeBase * FdogSerializeBase::Instance(){
    mutex_base->lock();
    if(fdogserializebase == nullptr){
        fdogserializebase = new FdogSerializeBase();
    }
    mutex_base->unlock();
    return fdogserializebase;
}


FdogSerialize * FdogSerialize::fdogserialize = nullptr;
mutex * FdogSerialize::mutex_serialize = new(mutex);

FdogSerialize::FdogSerialize(){
    ObjectInfo * objectinfo = new ObjectInfo();
    objectinfo->objectType = "NULL";
    objectinfo->objectTypeInt = -1;
    this->objectInfoList.push_back(objectinfo);

    //对基础类型做初始化
    MetaInfo * metainfo = nullptr;
    metainfo = new MetaInfo();
    metainfo->memberType = "bool";
    metainfo->memberTypeSize = sizeof(bool);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "char";
    metainfo->memberTypeSize = sizeof(char);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "int";
    metainfo->memberTypeSize = sizeof(int);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "short";
    metainfo->memberTypeSize = sizeof(short);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "long";
    metainfo->memberTypeSize = sizeof(long);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "long long";
    metainfo->memberTypeSize = sizeof(long long);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "float";
    metainfo->memberTypeSize = sizeof(float);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "double";
    metainfo->memberTypeSize = sizeof(double);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "long double";
    metainfo->memberTypeSize = sizeof(long double);
    this->baseInfoList.push_back(metainfo);

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
    this->objectInfoList.push_back(objectinfo);
}

ObjectInfo & FdogSerialize::getObjectInfo(string objectName){
    for(auto objectinfo : FdogSerialize::Instance()->objectInfoList){
        if(objectinfo->objectType == objectName){
            return *objectinfo;
        }
    }
    return *(FdogSerialize::Instance()->objectInfoList[0]);
}

MetaInfo * FdogSerialize::getMetaInfo(string TypeName){
    for(auto metainfo : FdogSerialize::Instance()->baseInfoList){
        if(metainfo->memberType == TypeName){
            return metainfo;
        }
    }
}

void FdogSerialize::setAliasName(string type, string memberName, string aliasName){
    ObjectInfo & objectinfo = this->getObjectInfo(type);
    for(auto metainfoObject : objectinfo.metaInfoObjectList){
        if(metainfoObject->memberName == memberName){
            metainfoObject->memberAliasName = aliasName;
            break;
        }
    }
}

void FdogSerialize::setIgnoreField(string Type, string memberName){
    ObjectInfo & objectinfo = this->getObjectInfo(Type);
    for(auto metainfoObject : objectinfo.metaInfoObjectList){
        if(metainfoObject->memberName == memberName){
            metainfoObject->memberIsIgnore = true;
            break;
        }
    }
}

void FdogSerialize::setIgnoreLU(string Type, string memberName){
    ObjectInfo & objectinfo = this->getObjectInfo(Type);
    for(auto metainfoObject : objectinfo.metaInfoObjectList){
        if(metainfoObject->memberName == memberName){
            metainfoObject->memberIsIgnoreLU = true;
            break;
        }
    }
}

/***********************************
*   返回对应的成员类型(包括基本类型和自定义类型)，数组大小
************************************/
memberAttribute FdogSerialize::getMemberAttribute(string typeName){
    memberAttribute resReturn;

    if(FdogSerialize::isBaseType(typeName)){
        resReturn.valueType = typeName;
        resReturn.ArraySize = 0;
        resReturn.valueTypeInt = OBJECT_BASE;
    }
    else if(FdogSerialize::isVectorType("", typeName)){
        ObjectInfo objectinfo = getObjectInfoByType(typeName, OBJECT_VECTOR);  
        resReturn.valueType = objectinfo.objectType;
        resReturn.ArraySize = 0;
        resReturn.valueTypeInt = OBJECT_VECTOR;
    }
    else if(FdogSerialize::isListType("", typeName)){
        ObjectInfo objectinfo = getObjectInfoByType(typeName, OBJECT_LIST);  
        resReturn.valueType = objectinfo.objectType;
        resReturn.ArraySize = 0;
        resReturn.valueTypeInt = OBJECT_LIST;
    }
    else if(FdogSerialize::isMapType("", typeName)){
        ObjectInfo objectinfo = getObjectInfoByType(typeName, OBJECT_MAP);  
        resReturn.valueType = objectinfo.objectType;
        resReturn.ArraySize = 0;
        resReturn.valueTypeInt = OBJECT_MAP;
    }
    else if(FdogSerialize::isArrayType("", typeName)){
        ObjectInfo objectinfo = getObjectInfoByType(typeName, OBJECT_ARRAY);  
        resReturn.valueType = objectinfo.objectType;
        resReturn.ArraySize = 0;
        resReturn.valueTypeInt = OBJECT_ARRAY;
    }else{
        resReturn.valueType = typeName;
        resReturn.ArraySize = 0;
        resReturn.valueTypeInt = OBJECT_STRUCT;
    }
    return resReturn;
}

int FdogSerialize::getObjectTypeInt(string objectName, string typeName){
    if(FdogSerialize::Instance()->isBaseType(typeName)){
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
            //除了复杂类型，还应该有基础类型
        }
        break;
    case OBJECT_LIST:
        if(regex_search(typeName, result, pattern)){
            string value = result.str(1).c_str();
            return getObjectInfo(value);
        }
        break;        
    case OBJECT_MAP:
        if(regex_search(typeName, result, pattern)){
            string value = result.str(1).c_str();
            return getObjectInfo(value);
        }
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

vector<string> FdogSerialize::CuttingArray(string data){
    //[{{}},{},{}]
    //第一步：查找第一个{     并计数
    //再找第二个，如果期间有{ 继续计数
    //直到知道}并且计数要对应 作为第一组
    // int sumL = 0; //记录左
    // int sumR = 0; //记录右
    // int sum = 0;
    // while(1){
    //     int tag = data.find("{", sum);
    //     if(tag != data.npos){
    //         sumL = sumL + 1；
    //         sum = tag;

    //     }
    //     break;
    // }
    vector<string> a;
    return a;
}