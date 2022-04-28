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


FdogSerializer * FdogSerializer::fdogSerializer = nullptr;
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
    delete(FdogSerializer::fdogSerializer);
}

FdogSerializer * FdogSerializer::Instance(){
    mutex_serialize->lock();
    if(fdogSerializer == nullptr){
        fdogSerializer = new FdogSerializer();
    }
    mutex_serialize->unlock();
    return fdogSerializer;
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

void FdogSerializer::__setAliasName(string type, string memberName, string aliasName){
    ObjectInfo & objectinfo = this->getObjectInfo(type);
    for(auto metainfoObject : objectinfo.metaInfoObjectList){
        if(metainfoObject->memberName == memberName){
            metainfoObject->memberAliasName = aliasName;
            break;
        }
    }
}

void FdogSerializer::__setIgnoreField(string Type, string memberName){
    ObjectInfo & objectinfo = this->getObjectInfo(Type);
    for(auto metainfoObject : objectinfo.metaInfoObjectList){
        if(metainfoObject->memberName == memberName){
            metainfoObject->memberIsIgnore = true;
            break;
        }
    }
}

void FdogSerializer::__setIgnoreLU(string Type, string memberName){
    ObjectInfo & objectinfo = this->getObjectInfo(Type);
    for(auto metainfoObject : objectinfo.metaInfoObjectList){
        if(metainfoObject->memberName == memberName){
            metainfoObject->memberIsIgnoreLU = true;
            break;
        }
    }
}

void FdogSerializer::__setFuzzy(string Type) {

}

void Fdog::setAliasName(string Type, string memberName, string AliasName){
    FdogSerializer::Instance()->__setAliasName(Type, memberName, AliasName);
}

void Fdog::setIgnoreField(string Type, string memberName){
    FdogSerializer::Instance()->__setIgnoreField(Type, memberName);
}

void Fdog::setIgnoreLU(string Type, string memberName){
    FdogSerializer::Instance()->__setIgnoreLU(Type, memberName);
}

void Fdog::setFuzzy(string Type){
    FdogSerializer::Instance()->__setFuzzy(Type);
}

//获取key值
string FdogSerializer::getKey(string json){
    smatch result;
    regex pattern("\"(.*)\":(.*)");
        if(regex_search(json, result, pattern)){
            string key = result.str(1).c_str();
            return key;
        }
    return "";
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
        regex pattern(complexRegex[62]);
        if(regex_search(typeName, result, pattern)){
            string value = result.str(1).c_str();
            if(value == "std::__cxx11::basic_string<char"){
                string value2 = result.str(4).c_str();
                resReturn.second = value2;
            } else {
                string value2 = result.str(2).c_str();
                resReturn.second = value2;                
            }
            //cout << "=========>>2  isMapType " << value2 << endl;
            resReturn.first = value;
        } else {
            regex pattern2(complexRegex[6]);
            if(regex_search(typeName, result, pattern)){
                string value = result.str(1).c_str();
                string value2 = result.str(2).c_str();               
                resReturn.first = value;
                resReturn.second = value2; 
            }
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

//判断json格式是否正确
vector<string> FdogSerializer::CuttingJson(string json_){
    vector<string> json_array;
    //这个函数可以完成对json消息的分割，只分割第一层，如果其他消息体有多层，依次分析
    int status = -1; //0 表示下一个类型任意  1 表示基本类型(不包含嵌套) 2 表示对象  3 表示数组
    int first = 0;  //字符串初始位置
    int end = 0;    //字符串结束位置
    int len = json_.length();
    int sum = 0;
    int xiabiao = 0; //记录匹配下标
    bool isadd = false;
    for(int i = 0; i < len; i++){
        //cout << "i = " << i << endl;
        if(json_[i] == '"'){
			if(sum == 0 && status == -1){
                status = 0;
                first = i;
                //cout << "i = " << i << endl;
            }
        }
        if(status == 3 && json_[i] == '{' && isadd == true){
            sum++;
        }

        if(status == 4 && json_[i] == '[' && isadd == true){
            sum++;
        }

        if(json_[i] == ',' && json_[i-1] == '"' && status == 2){
            cout << ",后面是\" 且 status = 2 出错" << endl;
        }

        if(json_[i] == '"' && json_[i+1] == ':' && json_[i+2] == '{'){
            if(status == 0){
                //cout << "status = 3" << endl;
                status = 3;
                //sum++;
                isadd = true;
            }
        } else if(json_[i] == '"' && json_[i+1] == ':' && json_[i+2] == '['){
            if(status == 0){
                status = 4;
                //sum++;
                isadd = true;
            }
        } else if(json_[i] == '"' && json_[i+1] == ':' && json_[i+2] == '"'){
            if(status == 0){
                status = 1;
                sum++;
                //cout << "sum+1 =" << sum << endl;
                xiabiao = i + 2;
                isadd = true;
            }
        } else if(json_[i] == '"' && json_[i+1] == ':'){
            if(status == 0){
                status = 2;
                sum++;
                isadd = true;
            }
        }
        if(json_[i] == '"'){
            if(json_[i+1] == ',' && status == 1){
                sum--;
                if(sum == 0){
                    end = i;
                    string da = json_.substr(first, end - first + 1);
                    //cout << "获取到的值1：" << da << endl;
                    status = -1;
                    json_array.push_back(da);
                }
            }else if(json_[i+1] == '}' && status == 1){
                sum--;
                //cout << "sum-1 =" << sum << endl;
                if(sum == 0){
                    end = i;
                    string da = json_.substr(first, end - first + 1);
                    //cout << "获取到的值1：" << da << endl;
                    status = -1;
                    json_array.push_back(da);
                }            
            }else if(json_[i+1] == ']' && status == 1){
                sum--;
                if(sum == 0){
                    end = i;
                    string da = json_.substr(first, end - first + 1);
                    //cout << "获取到的值1：" << da << endl;
                    status = -1;
                    json_array.push_back(da);
                }
            }else{
                if(sum - 1 == 0 && xiabiao < i){
                    //cout << "字符串找不到匹配值" << endl;
                }
            }
            if(json_[i+1] == ']' && status == 4){
                sum--;
                if(sum == 0){
                    end = i + 1;
                    string da = json_.substr(first, end - first + 1);
                    //cout << "获取到的值3：" << da << endl;
                    status = -1;
                    json_array.push_back(da);
                }
            }
        }
        if((status == 2 || status == 3 || status == 4) && sum != 0){
            if(status == 2 && json_[i-1] != '"' && json_[i] == ',' && json_[i+1] == '"'){
                sum--;
                if(sum == 0){
                    end = i;
                    string da = json_.substr(first, end - first);
                        if(da.npos != da.find(",")){
                            cout << "失败" << endl;
                        }
                    //cout << "获取到的值4：" << da << endl;
                    status = -1;
                    json_array.push_back(da);
                }
                continue;
            }
            if(status == 3 && json_[i] == '}' && json_[i+1] == ','){
                sum--;
                if(sum == 0){
                    end = i;
                    string da = json_.substr(first, end - first + 1);
                    //cout << "获取到的值5：" << da << endl;
                    status = -1;
                    json_array.push_back(da);
                }
                continue;
            }
            //3 和 6可以区分是不是对象数组
            if(status == 4 && json_[i] == ']' && json_[i+1] == ','){
                sum--;
                if(sum == 0){
                    end = i;
                    string da = json_.substr(first, end - first + 1);
                    //cout << "获取到的值6：" << da << endl;
                    status = -1;
                    json_array.push_back(da);
                }
                continue;
            }
            if((status == 3 || status == 2) && json_[i] == '}'){
                if(isadd == true){
                    sum--;
                }
                if(sum == 0){
                    end = i;
                    if (status == 2 && json_[i-1] != '"'){
                        string da = json_.substr(first, end - first);
                        if(da.npos != da.find(",")){
                            cout << "失败" << endl;
                        }
                        //cout << "获取到的值71：" << da << endl;
                        status = -1;
                        json_array.push_back(da);
                    }else{
                        string da = json_.substr(first, end - first + 1);
                        //cout << "获取到的值72：" << da << endl;
                        status = -1;
                        json_array.push_back(da);
                    }
                }
                continue;
            }
            if(status == 4 && json_[i] == ']'){
                sum--;
                if(sum == 0){
                    end = i;
                    string da = json_.substr(first, end - first + 1);
                    //cout << "获取到的值8：" << da << endl;
                    status = -1;
                    json_array.push_back(da);
                }
                continue;
            }
        }
    }
    if(status != -1){
        cout << "匹配失败在字符串<";
        int a = first;
        for(int i = 0; i < (json_.length()-a > 20 ? 20 : json_.length() - a); i++){
            cout << json_[a++];
        }
        cout << ">附近"<< endl;
    } else {
        int count = 0;
        for(int i = 0; i < json_array.size(); i++){
            count = count + json_array[i].length() + 1;
        }
        if(count + 1 != json_.length()){
            cout << "长度出错 :" << json_ << "---原字符串长度：" << json_.length() << "---现在长度:" << count + 1 << endl;
        }
    }
    if(1){
        string json_a = "{";
        for(int i = 0; i < json_array.size(); i++){
            if(i + 1 == json_array.size()){
                json_a = json_a + json_array[i];
                break;
            }
            json_a = json_a + json_array[i] + ",";
        }
        json_a = json_a + "}";
        if(json_ != json_a){
            cout << "加逗号还原，不匹配" << json_a << endl;
        }
    }
    return json_array;
}

//判断方括号是否匹配
bool FdogSerializer::IsSquareBracket(string json_){
    char start = json_[0];
    char end = json_[json_.length()-1];
    if(start != '[' || end != ']'){
        return false;
    }  
    return true;
}

//判断花括号是否匹配
bool FdogSerializer::IsCurlyBraces(string json_){
    char start = json_[0];
    char end = json_[json_.length()-1];
    if(start != '{' || end != '}'){
        return false;
    }
    return true;
}

//判断总符号数是否匹配
bool FdogSerializer::isMatch(string json_){
    int h_sum = 0;
    int f_sum = 0;
    for(int i = 0; i < json_.length(); i++){
		if (json_[i] == '{'){
			h_sum++;
		}
		if (json_[i] == '}'){
			h_sum--;
		}    
		if (json_[i] == '['){
			f_sum++;
		}
		if (json_[i] == ']'){
			f_sum--;
		}
    }
    if( h_sum == f_sum && f_sum == 0){
        return true;
    }else{
        return false;
    }
    return false;
}

//判断json正确性
result FdogSerializer::__JsonValidS(string json_){
    result res;
    res.code = 1;
    //检查左右括号
    if(!IsCurlyBraces(json_)){
        res.code = 0;
        res.message = "缺少花括号";
        return res;
    }
    //检查总符号数是否匹配([]{})
    if(!isMatch(json_)){
        res.code = 0;
        res.message = "括号不匹配";
        return res;
    }
    vector<string> array_json = CuttingJson(json_);
    if(array_json.size() == 0){
        res.code = 0;
        return res;
    }
    return res;
}

//判断字段是否存在
bool FdogSerializer::__Exist(string json_, string key){
   auto num = key.find(".");
    if(num != key.npos){
        if(json_.find(key.substr(0, num)) != json_.npos){
            //这里需要在找到的里面找对应字符串
            string resp = "";
            string jsonNew = "\"" + key.substr(0, num) + "\":";
            cout << "n = " << num << endl;
            auto num2 = json_.find(jsonNew);
            if(num2 != json_.npos){
                if(json_[num2 + jsonNew.length()] == '{'){
                    string json_2 = json_.substr(num2 + jsonNew.length());
                    int len = json_2.length();
                    int sum = 0;
                    int first = 0;
                    int end = 0;
                    for (int i = 0; i <= len; i++) {
                        if (json_2[i] == '{'){
                            sum++;
                            if (sum == 1) {
                                first = i;
                            }
                        }
                        if (json_2[i] == '}'){
                            sum--;
                            if (sum == 0) {
                                end = i;
                            }
                        }
                    }
                    return __Exist(json_2.substr(first, end - first + 1), key.substr(num + 1));
                }else if(json_[num2 + jsonNew.length()] == '['){
                    string json_2 = json_.substr(num2 + jsonNew.length());
                    int len = json_2.length();
                    int sum = 0;
                    int first = 0;
                    int end = 0;
                    for (int i = 0; i <= len; i++) {
                        if (json_2[i] == '['){
                            sum++;
                            if (sum == 1) {
                                first = i;
                            }
                        }
                        if (json_2[i] == ']'){
                            sum--;
                            if (sum == 0) {
                                end = i;
                            }
                        }
                    }
                    return __Exist(json_2.substr(first, end - first + 1), key.substr(num + 1));
                }else {
                    return false;
                }
            }
        } else {
            return false;
        }
    } else {
        int x = json_.find(key);
        if(x != json_.npos){
            if(json_[x - 1] == '"' && json_[x + key.length()] == '"'){
                return true;
            } else {
                return false;
            }
            
        } else {
            return false;
        }
    }
    return false;
}

//获取字段的值
string FdogSerializer::__GetStringValue(string json_, string key){
    auto num = key.find(".");
    if(num != key.npos){
        if(json_.find(key.substr(0, num)) != json_.npos){
            //这里需要在找到的里面找对应字符串
            string resp = "";
            string jsonNew = "\"" + key.substr(0, num) + "\":";
            cout << "n = " << num << endl;
            auto num2 = json_.find(jsonNew);
            if(num2 != json_.npos){
                if(json_[num2 + jsonNew.length()] == '{'){
                    string json_2 = json_.substr(num2 + jsonNew.length());
                    int len = json_2.length();
                    int sum = 0;
                    int first = 0;
                    int end = 0;
                    for (int i = 0; i <= len; i++) {
                        if (json_2[i] == '{'){
                            sum++;
                            if (sum == 1) {
                                first = i;
                            }
                        }
                        if (json_2[i] == '}'){
                            sum--;
                            if (sum == 0) {
                                end = i;
                            }
                        }
                    }
                    return __GetStringValue(json_2.substr(first, end - first + 1), key.substr(num + 1));
                }else if(json_[num2 + jsonNew.length()] == '['){
                    string json_2 = json_.substr(num2 + jsonNew.length());
                    int len = json_2.length();
                    int sum = 0;
                    int first = 0;
                    int end = 0;
                    for (int i = 0; i <= len; i++) {
                        if (json_2[i] == '['){
                            sum++;
                            if (sum == 1) {
                                first = i;
                            }
                        }
                        if (json_2[i] == ']'){
                            sum--;
                            if (sum == 0) {
                                end = i;
                            }
                        }
                    }
                    return __GetStringValue(json_2.substr(first, end - first + 1), key.substr(num + 1));
                }else {
                    return "";
                }
            }
        } else {
            return "";
        }
    } else {
        int x = json_.find(key);
        if(x != json_.npos){
            if(json_[x - 1] == '"' && json_[x + key.length()] == '"'){
                // //基础类型可以使用正则表达式获取
                string res = "(\"" + key + "\":)((\"(.*?)\")|([+-]?([0-9]*\\.?[0-9]+|[0-9]+\\.?[0-9]*)([eE][+-]?[0-9]+)?))";
                string res2 = "(\"" + key + "\":)((true)|(false))";
                smatch result;
                regex pattern(res);
                if(regex_search(json_, result, pattern)){
                    return result.str(2);
                }
                regex pattern2(res2);
                if(regex_search(json_, result, pattern2)){
                    return result.str(2);
                }
                return "";
            } else {
                return "";
            }
            
        } else {
            return "";
        }
    }
    return "";
}

//获取字段的值
int FdogSerializer::__GetIntValue(string json_, string key){
    string value =  __GetStringValue(json_, key);
    if (value == "") {
        return -1;
    } else {
        return atoi(value.c_str());
    }
}

//获取字段的值
double FdogSerializer::__GetDoubleValue(string json_, string key){
    string value =  __GetStringValue(json_, key);
    if (value == "") {
        return -1.0;
    } else {
        return atof(value.c_str());
    }
}

//获取字段的值
long FdogSerializer::__GetLongValue(string json_, string key){
    string value =  __GetStringValue(json_, key);
    if (value == "") {
        return -1.0l;
    } else {
        return atol(value.c_str());
    }
}

//获取字段的值
bool FdogSerializer::__GetBoolValue(string json_, string key){
    string value = __GetStringValue(json_, key);
    if(value == "true"){
        return 1;
    } else {
        return 0;
    }
    return 0;
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

//判断json正确性
result Fdog::JsonValidS(string json_){
    return FdogSerializer::Instance()->__JsonValidS(json_);
}

//判断字段是否存在
bool Fdog::Exist(string json_, string key){
    return FdogSerializer::Instance()->__Exist(json_, key);
}

//获取字段的值
string Fdog::GetStringValue(string json_, string key){
    return FdogSerializer::Instance()->__GetStringValue(json_, key);
}

//获取字段的值
int Fdog::GetIntValue(string json_, string key){
    return FdogSerializer::Instance()->__GetIntValue(json_, key);
}

//获取字段的值
double Fdog::GetDoubleValue(string json_, string key){
    return FdogSerializer::Instance()->__GetDoubleValue(json_, key);
}

//获取字段的值
long Fdog::GetLongValue(string json_, string key){
    return FdogSerializer::Instance()->__GetLongValue(json_, key);
}

//获取字段的值
bool Fdog::GetBoolValue(string json_, string key){
    return FdogSerializer::Instance()->__GetBoolValue(json_, key);
}