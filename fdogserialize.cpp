#include "fdogserialize.h"

FdogSerialize * FdogSerialize::fdogserialize = nullptr;
mutex * FdogSerialize::mutex_serialize = new(mutex);

FdogSerialize::FdogSerialize(){
    ObjectInfo * objectinfo = new ObjectInfo();
    objectinfo->objectType = "NULL";
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
    removeNumbers(objectName);
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
memberAttribute FdogSerialize::getMemberAttribute(string key){
    memberAttribute resReturn;
    if(baseType.count(key) != 0){
        resReturn.valueType = baseType[key];  
        resReturn.ArraySize = 0;
        return resReturn;
    } else if (basePointerType.count(key) != 0){
        resReturn.valueType = basePointerType[key];
        resReturn.ArraySize = 0;
    } else {
        smatch result;
        regex pattern(patternArray);
        if(regex_search(key, result, pattern)){
            string key_ = result.str(2) + result.str(4);
            if(baseArrayType.count(key_) != 0){
                resReturn.valueType = baseArrayType[key_];
            } else {
                objectArrayType[key_] = result.str(4) + "_array";
                resReturn.valueType = objectArrayType[key_];
            }
            resReturn.ArraySize = atoi(result.str(3).data());
            return resReturn;
        } else {
            smatch result;
            regex pattern(patternObject);
            if(regex_search(key, result, pattern)){
                string str = result.str(2) + " " + result.str(3);
                resReturn.valueType = result.str(3);
                resReturn.ArraySize = atoi(result.str(3).data());
                return resReturn;
            }
        }
    }
    return resReturn;
}

int FdogSerialize::getObjectType(string objectName){
    //cout << objectName << "--getObjectType类型--：" << &(getObjectInfo(objectName)) << endl;
    return getObjectInfo(objectName).objectTypeInt;
}

void * getInstance(){
    return FdogSerialize::Instance();
}