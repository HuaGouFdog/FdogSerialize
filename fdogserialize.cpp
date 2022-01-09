#include "fdogserialize.h"

FdogObjectInfo * FdogSerialize::fdogserialize = nullptr;
mutex * FdogSerialize::mutex_serialize = new(mutex);

FdogSerialize::FdogSerialize(){
    ObjectInfo objectinfo;
    objectinfo.objectType = "NULL";
    this->objectInfoList.push_back(objectinfo);
}

~FdogSerialize(){
    //释放内存
    delete(FdogObjectInfo::mutex_object);
    delete(FdogObjectInfo::fdogobjectinfo);
}

static FdogSerialize * FdogSerialize::Instance(){
    mutex_serialize->lock();
    if(fdogserialize == nullptr){
        fdogserialize = new FdogSerialize();
    }
    mutex_serialize->unlock();
    return fdogserialize;
}

ObjectInfo & FdogSerialize::getObjectInfo(string objectName){
    for(auto objectinfo : this->objectInfoList){
        if(objectinfo.objectType == objectName){
            return objectinfo;
        }
    }
    return this->objectInfoList[0];
}

void FdogSerialize::setAliasName(string type, string name, string aliasName){
    ObjectInfo & objectinfo = this->getObjectInfo(type);
    for(auto metainfoObject : objectinfo.metainfoObjectList){
        if(metainfoObject.memberName == name){
            metainfoObject.memberAliasName = aliasName;
            break;
        }
    }
}

void FdogSerialize::setIgnoreField(string Type, string Name){
    ObjectInfo & objectinfo = this->getObjectInfo(type);
    for(auto metainfoObject : objectinfo.metainfoObjectList){
        if(metainfoObject.memberName == Name){
            metainfoObject.memberIsIgnore = true;
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
    return getObjectInfo(objectName).objectType;
}