/*
该项目签署了Apache-2.0 License，详情请参见LICENSE
根据 Apache 许可，版本 2.0（“许可”）获得许可
除非遵守许可，否则您不得使用此文件。

Copyright 2021-2022 花狗Fdog(张旭)
*/

#include "FStruct.h"

FdogSerializerBase * FdogSerializerBase::FdogSerializerbase = nullptr;
mutex * FdogSerializerBase::mutex_base = new(mutex);

FdogSerializerBase * FdogSerializerBase::Instance(){
    mutex_base->lock();
    if(FdogSerializerbase == nullptr){
        FdogSerializerbase = new FdogSerializerBase();
    }
    mutex_base->unlock();
    return FdogSerializerbase;
}


FdogSerializer * FdogSerializer::FdogSerializer = nullptr;
mutex * FdogSerializer::mutex_serialize = new(mutex);

FdogSerializer::FdogSerializer(){
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
    metainfo->memberType = "unsigned char";
    metainfo->memberTypeSize = sizeof(unsigned char);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "int";
    metainfo->memberTypeSize = sizeof(int);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "unsigned int";
    metainfo->memberTypeSize = sizeof(unsigned int);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "short";
    metainfo->memberTypeSize = sizeof(short);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "unsigned short";
    metainfo->memberTypeSize = sizeof(unsigned short);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "long";
    metainfo->memberTypeSize = sizeof(long);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "unsigned long";
    metainfo->memberTypeSize = sizeof(unsigned long);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "long long";
    metainfo->memberTypeSize = sizeof(long long);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "unsigned long long";
    metainfo->memberTypeSize = sizeof(unsigned long long);
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

    metainfo = new MetaInfo();
    metainfo->memberType = "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >";
    metainfo->memberTypeSize = sizeof(string);
    this->baseInfoList.push_back(metainfo);

    metainfo = new MetaInfo();
    metainfo->memberType = "char*";
    metainfo->memberTypeSize = sizeof(char*);
    this->baseInfoList.push_back(metainfo);

}

FdogSerializer::~FdogSerializer(){
    //释放内存
    delete(FdogSerializer::mutex_serialize);
    delete(FdogSerializer::FdogSerializer);
}

FdogSerializer * FdogSerializer::Instance(){
    mutex_serialize->lock();
    if(FdogSerializer == nullptr){
        FdogSerializer = new FdogSerializer();
    }
    mutex_serialize->unlock();
    return FdogSerializer;
}

void FdogSerializer::addObjectInfo(ObjectInfo * objectinfo){
    this->objectInfoList.push_back(objectinfo);
}

ObjectInfo & FdogSerializer::getObjectInfo(string objectName){
    for(auto objectinfo : FdogSerializer::Instance()->objectInfoList){
        if(objectinfo->objectType == objectName){
            return *objectinfo;
        }
    }
    return *(FdogSerializer::Instance()->objectInfoList[0]);
}

MetaInfo * FdogSerializer::getMetaInfo(string TypeName){
    //cout << "getMetaInfo - TypeName = "  << TypeName << endl;
    for(auto metainfo : FdogSerializer::Instance()->baseInfoList){
        if(metainfo->memberType == TypeName){
            return metainfo;
        }
    }
    return nullptr;
}

void FdogSerializer::setAliasName(string type, string memberName, string aliasName){
    ObjectInfo & objectinfo = this->getObjectInfo(type);
    for(auto metainfoObject : objectinfo.metaInfoObjectList){
        if(metainfoObject->memberName == memberName){
            metainfoObject->memberAliasName = aliasName;
            break;
        }
    }
}

void FdogSerializer::setIgnoreField(string Type, string memberName){
    ObjectInfo & objectinfo = this->getObjectInfo(Type);
    for(auto metainfoObject : objectinfo.metaInfoObjectList){
        if(metainfoObject->memberName == memberName){
            metainfoObject->memberIsIgnore = true;
            break;
        }
    }
}

void FdogSerializer::setIgnoreLU(string Type, string memberName){
    ObjectInfo & objectinfo = this->getObjectInfo(Type);
    for(auto metainfoObject : objectinfo.metaInfoObjectList){
        if(metainfoObject->memberName == memberName){
            metainfoObject->memberIsIgnoreLU = true;
            break;
        }
    }
}

void FdogSerializer::setFuzzy(string Type) {

}

/***********************************
*   返回对应的成员类型(包括基本类型和自定义类型)，数组大小
************************************/
memberAttribute FdogSerializer::getMemberAttribute(string typeName){
    //cout << "getMemberAttribute = " <<typeName << endl;
    memberAttribute resReturn;
    smatch result;
    if(FdogSerializer::isBaseType(typeName)){
        resReturn.valueType = typeName;
        resReturn.ArraySize = 0;
        resReturn.valueTypeInt = OBJECT_BASE;
    }else if(FdogSerializer::isVectorType("", typeName)){
        //cout << "=========>>1  typeName ============= = " << typeName << endl;
        resReturn.valueType = typeName;
        regex pattern(complexRegex[5]);
        if(regex_search(typeName, result, pattern)){
            string value = result.str(1).c_str();
            //cout << "=========>>1  isVectorType 暂时有问题 ============= = " << value << endl;
            //处理好
            if (value == "std::__cxx11::basic_string<char"){
                resReturn.first = "string";
            } else{
                resReturn.first = value;
            }
        }
        resReturn.valueTypeInt = OBJECT_VECTOR;
    }else if(FdogSerializer::isMapType("", typeName)){
        resReturn.valueType = typeName;
        regex pattern(complexRegex[6]);
        if(regex_search(typeName, result, pattern)){
            string value = result.str(1).c_str();
            string value2 = result.str(2).c_str();
            //cout << "=========>>1  isMapType " << value << endl;
            //cout << "=========>>2  isMapType " << value2 << endl;
            resReturn.first = value;
            resReturn.second = value2;
        }
        resReturn.valueTypeInt = OBJECT_MAP;
    }else if(FdogSerializer::isListType("", typeName)){
        resReturn.valueType = typeName;
        regex pattern(complexRegex[7]);
        if(regex_search(typeName, result, pattern)){
            string value = result.str(1).c_str();
            //cout << "=========>>1  isListType " << value << endl;
            resReturn.first = value;
        }
        resReturn.valueTypeInt = OBJECT_LIST;
        //getTypeOfList
    }else if(FdogSerializer::isSetType("", typeName)){
        resReturn.valueType = typeName;
        regex pattern(complexRegex[8]);
        if(regex_search(typeName, result, pattern)){
            string value = result.str(1).c_str();
            //cout << "=========>>1  isSetType " << value << endl;
            resReturn.first = value;
        }
        resReturn.valueTypeInt = OBJECT_SET;
        //getTypeOfList
    }else if(FdogSerializer::isDequeType("", typeName)){
        resReturn.valueType = typeName;
        regex pattern(complexRegex[9]);
        if(regex_search(typeName, result, pattern)){
            string value = result.str(1).c_str();
            //cout << "=========>>1  isDequeType " << value << endl;
            resReturn.first = value;
        }
        resReturn.valueTypeInt = OBJECT_DEQUE;
        //getTypeOfList
    }else if(FdogSerializer::isArrayType("", typeName)){
        resReturn.valueType = typeName;
        // ObjectInfo objectinfo = getObjectInfoByType(typeName, OBJECT_ARRAY);  
        // resReturn.valueType = objectinfo.objectType;
        regex pattern(complexRegex[4]);
        if(regex_search(typeName, result, pattern)){
            string value = result.str(1).c_str();
            //cout << "数组类型:" << value << " ";
            string value2 = result.str(3).c_str();
            //cout << "数组大小:" << value2;
            resReturn.first = value;
            resReturn.ArraySize = atoi(value2.data());
        }
        resReturn.valueTypeInt = OBJECT_ARRAY;
    }else{
        resReturn.valueType = typeName;
        resReturn.ArraySize = 0;
        resReturn.valueTypeInt = OBJECT_STRUCT;
    }
    return resReturn;
}

int FdogSerializer::getObjectTypeInt(string objectName, string typeName){
    //cout << "进入getObjectTypeInt" << endl;
    if(FdogSerializer::Instance()->isBaseType(typeName)){
        return OBJECT_BASE;
    }
    if(FdogSerializer::Instance()->isVectorType(objectName, typeName)){
        return OBJECT_VECTOR;
    }
    if(FdogSerializer::Instance()->isMapType(objectName, typeName)){
        return OBJECT_MAP;
    }
    if(FdogSerializer::Instance()->isListType(objectName, typeName)){
        return OBJECT_LIST;
    }
    if(FdogSerializer::Instance()->isStructType(objectName, typeName)){
        //这里也存在问题 判断
        return OBJECT_STRUCT;    
    }
    if(FdogSerializer::Instance()->isArrayType(objectName, typeName)){
        return OBJECT_ARRAY;
    }
}

ObjectInfo FdogSerializer::getObjectInfoByType(string typeName, int objectTypeInt){
    smatch result;
    regex pattern(complexRegex[objectTypeInt]);
    switch (objectTypeInt)
    {
    case OBJECT_VECTOR:
        //截取值
        if(regex_search(typeName, result, pattern)){
            string value = result.str(1).c_str();
            //cout << "-----" << value << endl;
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
            //cout << "-----" << value << endl;
            return getObjectInfo(value);
        }
        break;
    case OBJECT_ARRAY:
        if(regex_search(typeName, result, pattern)){
            string value = result.str(1).c_str();
            //cout << "-----" << value << endl;
            return getObjectInfo(value);
        }
        break;
    default:
        break;
    }
}

int FdogSerializer::getObjectTypeByObjectInfo(string objectName){
    return getObjectInfo(objectName).objectTypeInt;
}

bool FdogSerializer::isBaseType(string typeName){
    //cout << "isBaseType = " << typeName << endl;
    vector<string>::iterator result = find(baseType.begin(), baseType.end(), typeName);
    if(result != baseType.end()){
        return true;
    } 
    return false;
}
//这里存在问题，当类型是基本类型是不会错，但是当stl包含char* string，就会出问题

bool FdogSerializer::isBaseTypeByMap(string typeName){
    smatch result;
    regex pattern(complexRegex[10]);
    if(regex_search(typeName, result, pattern)){
        string value = result.str(2).c_str();
        vector<string>::iterator result = find(baseType.begin(), baseType.end(), value);
        if(result != baseType.end()){
            return true;
        }
        // return result.str(1).c_str();
        //         result.str(2).c_str();
    }
    return false;
}

//判断是否为vector类型
bool FdogSerializer::isVectorType(string objectName, string typeName){
    auto x = typeName.find("std::vector<");
    if(x != string::npos && x == 0){
        return true;
    }
    return false;
}

//判断是否为map类型
bool FdogSerializer::isMapType(string objectName, string typeName){
    auto x = typeName.find("std::map<");
    if(x != string::npos && x == 0){
        return true;
    }
    return false;
}

//判断是否是list类型
bool FdogSerializer::isListType(string objectName, string typeName){
    auto x = typeName.find("std::__cxx11::list");
    if(x != string::npos && x == 0){
        return true;
    }
    return false;
}

bool FdogSerializer::isSetType(string objectName, string typeName){
    auto x = typeName.find("std::set<");
    if(x != string::npos && x == 0){
        return true;
    }
    return false;
}

bool FdogSerializer::isDequeType(string objectName, string typeName){
    auto x = typeName.find("std::deque<");
    if(x != string::npos && x == 0){
        return true;
    }
    return false;
}


bool FdogSerializer::isStructType(string objectName, string typeName){
    if(objectName == typeName){
        return true;
    }
    return false;
}

//是数组返回对于值，不是数组返回0 
bool FdogSerializer::isArrayType(string objectName, string typeName){
    memberAttribute resReturn;
    smatch result;
    resReturn.valueType = typeName;
    regex pattern(complexRegex[4]);
    if(regex_search(typeName, result, pattern)){
        string value = result.str(2).c_str();
        string value2 = result.str(4).c_str();
        //cout << "value = " << value << endl;
        //cout << "value2 = " << value2 << endl;
        if((value + value2) == "[]"){
            return true;
        }
    }
    return false;
}

void FdogSerializer::removeFirstComma(string & return_){
    return_ = return_.substr(1);
}

void FdogSerializer::removeLastComma(string & return_){
    return_.pop_back();
}

void FdogSerializer::removeNumbers(string & return_){
    string::iterator it = return_.begin();
    while (it != return_.end()) {
        if ((*it >= '0') && (*it <= '9')) {
            it = return_.erase(it);
        } else {
            it++;
        }
    }
}

vector<string> FdogSerializer::CuttingArray(string data){
	int sum = 0;
	int first = 0;
	int end = 0;
	vector<string> StrArray;
    int len = data.length();
	for (int i = 0; i <= len; i++) {
		if (data[i] == '{'){
			sum++;
			if (sum == 1) {
				first = i;
			}
		}
		if (data[i] == '}'){
			sum--;
			if (sum == 0) {
				end = i;
				string da = data.substr(first, end - first + 1);
				StrArray.push_back(da);
			}
		}
	}
	return StrArray;
}

vector<string> FdogSerializer::split(string str, string pattern){
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

string FdogSerializer::getTypeOfVector(string objectName, string typeName){
    smatch result;
    regex pattern(complexRegex[5]);
    if(regex_search(typeName, result, pattern)){
        return result.str(1).c_str();
    }
    return "";
}

FdogMap FdogSerializer::getTypeOfMap(string objectName, string typeName){
    smatch result;
    regex pattern(complexRegex[5]);
    FdogMap fdogMap;
    if(regex_search(typeName, result, pattern)){
        fdogMap.first = result.str(1).c_str();
        fdogMap.second;
        return fdogMap;
    }
    return fdogMap;
}

string FdogSerializer::getTypeOfList(string objectName, string typeName){
    smatch result;
    regex pattern(complexRegex[5]);
    if(regex_search(typeName, result, pattern)){
        return result.str(1).c_str();
    }
    return "";
}