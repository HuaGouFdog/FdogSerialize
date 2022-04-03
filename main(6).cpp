#include <iostream>
#include <string>
#include <regex>
#include <vector>
using namespace std;


//判断花括号是否成对
bool IsCurlyBraces(string json_){
    char start = json_[0];
    char end = json_[json_.length()-1];
    if(start != '{' || end != '}'){
        return false;
    }
    return true;
}

//判断方括号是否成对
bool IsSquareBracket(string json_){
    char start = json_[0];
    char end = json_[json_.length()-1];
    if(start != '[' || end != ']'){
        return false;
    }  
    return true;
}



int getStatusType(string json_){
    int resp = -1;
    string regexStr1 = "\"\\w+\":(\\{(.*?)\\})";
    string regexStr2 = "\"\\w+\":(\\[(.*?)\\])";
    string regexStr3 = "\"\\w+\":((\"\\w+\")|([+-]?([0-9]*\\.?[0-9]+|[0-9]+\\.?[0-9]*)([eE][+-]?[0-9]+)?))";
    string regexStr4 = "\"\\w+\":((true|false))";
    smatch result;
    regex pattern1(regexStr1);
    if(regex_search(json_, result, pattern1)){
        string value = result[0];
        if (value != ""){
            resp = 1;
            return resp;
        }
    }
    regex pattern2(regexStr2);
    if(regex_search(json_, result, pattern2)){
        string value = result[0];
        if (value != ""){
            resp = 2;
            return resp;
        }
    }
    regex pattern3(regexStr3);
    if(regex_search(json_, result, pattern3)){
        string value = result[0];
        if (value != ""){
            resp = 3;
            return resp;
        }
    }
    regex pattern4(regexStr4);
    if(regex_search(json_, result, pattern4)){
        string value = result[0];
        if (value != ""){
            resp = 4;
            return resp;
        }
    }
    return resp;
}

// 1 对象  2 数组   3 普通   默认是对象
// //json是否正确  用递归搜索

//结构体用于返回信息
struct result {
    int code;   //1.正确 0.错误
    string message;
};

//1.只返回正确或者错误
//2.返回正确或错误的同时，返回错误位置以及可能的错误原因

// bool JsonValid(string json_){
//     result res = JsonValidS(json_);
//     if(res.code == 0){
//         return false;
//     }
//     return true;
// }

void removeFirstComma(string & return_){
    return_ = return_.substr(1);
}

void removeLastComma(string & return_){
    return_.pop_back();
}

/*
1.查看:后面有没有{} 有就匹配
2.查看:后面有没有[]，有就匹配
2.没有{}和[]就是普通值

*/

vector<string> CuttingJson(string json_){
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
                    cout << "获取到的值1：" << da << endl;
                    status = -1;
                    json_array.push_back(da);
                }
            }else if(json_[i+1] == '}' && status == 1){
                sum--;
                //cout << "sum-1 =" << sum << endl;
                if(sum == 0){
                    end = i;
                    string da = json_.substr(first, end - first + 1);
                    cout << "获取到的值1：" << da << endl;
                    status = -1;
                    json_array.push_back(da);
                }            
            }else if(json_[i+1] == ']' && status == 1){
                sum--;
                if(sum == 0){
                    end = i;
                    string da = json_.substr(first, end - first + 1);
                    cout << "获取到的值1：" << da << endl;
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
                    cout << "获取到的值3：" << da << endl;
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
                    cout << "获取到的值4：" << da << endl;
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
                    cout << "获取到的值5：" << da << endl;
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
                    cout << "获取到的值6：" << da << endl;
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
                        cout << "获取到的值71：" << da << endl;
                        status = -1;
                        json_array.push_back(da);
                    }else{
                        string da = json_.substr(first, end - first + 1);
                        cout << "获取到的值72：" << da << endl;
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
                    cout << "获取到的值8：" << da << endl;
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


vector<string> CuttingObject(string data){
	int sum = 0;
	int first = 0;
	int end = 0;
	vector<string> StrArray;
    int len = data.length();
	for (int i = 0; i <= len; i++) {
		if (data[i-1] == ':' && data[i] == '{'){
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

vector<string> CuttingArray(string data){
	int sum = 0;
	int first = 0;
	int end = 0;
	vector<string> StrArray;
    int len = data.length();
	for (int i = 0; i <= len; i++) {
		if (data[i-1] == ':' && data[i] == '['){
			sum++;
			if (sum == 1) {
				first = i;
			}
		}
		if (data[i] == ']'){
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

void JsonValidS_s(result & res, string & json_){
    int istrue = 0;
    cout << "--------------------- " << json_ << endl;
    string temp;
    string regexStr1 = "(\\{(.*?)\\})";
    string regexStr2 = "(\\[(.*?)\\])";
    string regexStr3 = "\"\\w+\":((\"(.*?)\")|([+-]?([0-9]*\\.?[0-9]+|[0-9]+\\.?[0-9]*)([eE][+-]?[0-9]+)?))";
    string regexStr4 = "\"\\w+\":((true|false))";
    string regexStr5 = "((\"\\w+\")|([+-]?([0-9]*\\.?[0-9]+|[0-9]+\\.?[0-9]*)([eE][+-]?[0-9]+)?))";
    string regexStr6 = "((true|false))";
    string::const_iterator iterStart = json_.begin();
    string::const_iterator iterEnd = json_.end();
    smatch result;
    regex pattern1(regexStr1); //对象
    while (regex_search(iterStart, iterEnd, result, pattern1))
    {
        string value = result[0];
        if (value != ""){
            string regex_ = "(\\{(.*?)\\})";
            smatch result_;
            regex pattern_(regex_);
            if(regex_search(value, result_, pattern_)){
                string v = result_[0];
                removeFirstComma(v);
                removeLastComma(v);
                //这里需要获取具体类型
                JsonValidS_s(res, v);
            }
            //return;
        }
        iterStart = result[0].second;	//更新搜索起始位置,搜索剩下的字符串
    }
    if (istrue)return;
    regex pattern2(regexStr2); //数组
    while (regex_search(iterStart, iterEnd, result, pattern2))
    {
        string value = result[0];
        if (value != ""){
            string regex_ = "(\\[(.*?)\\])";
            smatch result_;
            regex pattern_(regex_);
            if(regex_search(value, result_, pattern_)){
                string v = result_[0];
                removeFirstComma(v);
                removeLastComma(v);
                JsonValidS_s(res, v);
            }
            //return;
        }
        iterStart = result[0].second;	//更新搜索起始位置,搜索剩下的字符串
    }
    regex pattern3(regexStr3);//key+ 数值
    while (regex_search(iterStart, iterEnd, result, pattern3))
    {
        temp = result[0];
        iterStart = result[0].second;	//更新搜索起始位置,搜索剩下的字符串
    }
    if (istrue)return;
    regex pattern4(regexStr4);//key+ bool
    while (regex_search(iterStart, iterEnd, result, pattern4))
    {
        temp = result[0];
        iterStart = result[0].second;	//更新搜索起始位置,搜索剩下的字符串
    }
    if (istrue)return;
    regex pattern5(regexStr5);//数值
    while (regex_search(iterStart, iterEnd, result, pattern5))
    {
        temp = result[0];
        iterStart = result[0].second;	//更新搜索起始位置,搜索剩下的字符串
    }
    if (istrue)return;
    regex pattern6(regexStr6);//bool
    while (regex_search(iterStart, iterEnd, result, pattern6))
    {
        temp = result[0];
        iterStart = result[0].second;	//更新搜索起始位置,搜索剩下的字符串
    }
    return;
}

bool isMatch(string json_){
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

result JsonValidS(string json_) {
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

    // string regexStr = "\"\\w+\":((\\{(.*?)\\})|(\\[(.*?)\\])|(\"(.*?)\")|([+-]?([0-9]*\\.?[0-9]+|[0-9]+\\.?[0-9]*)([eE][+-]?[0-9]+)?)|(true|false))";
    // smatch result;
    // regex pattern(regexStr);

    // string::const_iterator iterStart = json_.begin();
    // string::const_iterator iterEnd = json_.end();
    // string temp;
    // while (regex_search(iterStart, iterEnd, result, pattern))
    // {
    //     temp = result[0];
    //     cout << "类型：" << temp << endl;
    //     //JsonValidS_s(res, temp);
    //     // if (statusType == 3 || statusType == 4){

    //     // }
    //     iterStart = result[0].second;	//更新搜索起始位置,搜索剩下的字符串
    // }
    return res;
}

void func(string json_2){
    removeFirstComma(json_2);
    removeLastComma(json_2);
    auto a = CuttingObject(json_2);
    for(auto & aa : a){
        cout << "值：" << aa << endl;
        func(aa);
    }
    auto b = CuttingArray(json_2);
    for(auto & aa : b){
        cout << "值：" << aa << endl;
        func(aa);
    }
    return;
}


vector<string> split(string str, string pattern){
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
//可以做一个缓存
//直接查不缓存 有一些浪费，先这样，后期可以考虑
bool Exist(string json_, string key){
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
                    return Exist(json_2.substr(first, end - first + 1), key.substr(num + 1));
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
                    return Exist(json_2.substr(first, end - first + 1), key.substr(num + 1));
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
// C++11 中增加了返回类型后置（trailing-return-type，又称跟踪返回类型）语法 可以用到
string GetStringValue(string json_, string key){
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
                    return GetStringValue(json_2.substr(first, end - first + 1), key.substr(num + 1));
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
                    return GetStringValue(json_2.substr(first, end - first + 1), key.substr(num + 1));
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

int GetIntValue(string json_, string key) {
    string value =  GetStringValue(json_, key);
    if (value == "") {
        return -1;
    } else {
        return atoi(value.c_str());
    }
}

double GetDoubleValue(string json_, string key) {
    string value =  GetStringValue(json_, key);
    if (value == "") {
        return -1.0;
    } else {
        return atof(value.c_str());
    }
}

long GetLongValue(string json_, string key) {
    string value =  GetStringValue(json_, key);
    if (value == "") {
        return -1.0l;
    } else {
        return atol(value.c_str());
    }
}

bool GetBoolValue(string json_, string key) {
    string value = GetStringValue(json_, key);
    if(value == "true"){
        return 1;
    } else {
        return 0;
    }
    return 0;
}

int main(){
    //{"stu":{"name":"liuliu","age":18},"tea":{"name":"wufang","age":48}}
    string js = "{\"red\":\"liuliu\",\"class\":18,\"stu\":{\"name\":\"liuliu\",\"age\":18}}";
    cout << "js = " << js << endl;
    //vector<string> valueList = CuttingJson(js);
    // data.name.ckdsa
    string key = "stu.age";
    //.表示层级将按照层级去查值，如果只有name 则返回遇到的第一个
    cout << "是否存在：" << Exist(js, key) << endl;
    cout << "值：" << GetStringValue(js, key) << endl;

    //明天任务
    //1. 解决杂项问题的接口
    //2. 一次性接口，支持多个参数
    //3. 解决掉map的问题 可以先支持几个
    //4. string,string   string,int   int,string  string,bool   int,bool
    //5. STL支持自定义类型考虑写法


    string js2 = "{\"name\":\"liuliu\",\"age\":18,\"stu\":{\"name\":\"liuliu\",\"age\":18},\"tea\":{\"name\":\"wufang\",\"age\":48}}";
    string js3 = "{\"name\":\"liuliu\",\"age\":18,\"addr\":321}";
    //cout << js2 << endl;



    string json_ = "{\"stu\":{\"name\":\"liuliu\",\"age\":18},\"tea\":{\"name\":\"wufang\",\"age\":48},\"stu\":[\"zhan\",\"xv\",\"321%90^$%\"],\"tex\":\"dsa\",\"wxc\":-312,\"dsa\":456,\"oew\":-412.3123,\"dae\":true}";

    string json_2 = "{\"class\":423,\"name\":\"张三\",\"age\":21,\"stu\":true,\"student\":{\"name\":\"李四\",\"age\":13},\"school\":[\"2506892\",\"186598\",\"12344687\"],\"addr\":[{\"name\":\"王五\",\"age\":22},{\"name\":\"于静\",\"age\":23},{\"name\":\"张旭\",\"age\":33}],\"all\":{\"name\":\"胡瑞敏\",\"age\":32,\"stu\":true,\"student\":{\"name\":\"zhangxv\",\"age\":1343},\"school\":[\"3f4312\",\"321089\",\"756896\"],\"addr\":[{\"name\":\"dasdas\",\"age\":13},{\"name\":\"ytrhfg\",\"age\":13}],\"addr\":[{\"name\":\"dasdas\",\"age\":13},{\"name\":\"ytrhfg\",\"age\":13},{\"name\":\"hgftre\",\"age\":113}]}}";
    
    
    // cout << "原值：" << js << endl;

    // string json_3 = "{\"all\":{\"name\":\"胡瑞敏\",\"age\":32,\"stu\":true,\"student\":{\"name\":\"zhangxv\",\"age\":1343},\"school\":[\"3f4312\",\"321089\",\"756896\"],\"addr\":[{\"name\":\"dasdas\",\"age\":13},{\"name\":\"ytrhfg\",\"age\":13}],\"addr\":[{\"name\":\"dasdas\",\"age\":13},{\"name\":\"ytrhfg\",\"age\":13},{\"name\":\"hgftre\",\"age\":13}]}}";
    // //cout << json_3 << endl;
    // //func(json_2);
    // string dsa = "{\"name\":\"321\"\"age\":\"321}";
    // cout << endl;
    // cout << json_2 << endl;
    // CuttingJson(json_2);
    // //cout << json_2 << endl;
    // //result res = JsonValidS(json_2);
    // //cout << "返回值：" << res.code << endl;
    // string d1 = "{\"name\":\"321\"\"age\":\"321}}";
    // string d2 = "{[\"name\":\"321\"\"age\":\"321]}";
    // string d3 = "{[\"name\":\"321\"\"age\":\"321}";
    // string d4 = "{\"name\":\"321\"\"age\":\"321}[]{";
    // result r1 =  JsonValidS(d1);
    // result r2 =  JsonValidS(d2);
    // result r3 =  JsonValidS(d3);
    // result r4 =  JsonValidS(d4);
    // cout << r1.code << endl;
    // cout << r2.code << endl;
    // cout << r3.code << endl;
    // cout << r3.code << endl;
    // string text1 = "{\"name\":\"liuliu\"\"age\":18}";
    // CuttingJson(text1);
    // cout << endl;
    // string text2 = "{\"name\":\"liuliu\",\"age\":18,}";
    // CuttingJson(text2);
    // cout << endl;
    // string text3 = "{\"name\":\"liuliu}";
    // CuttingJson(text3);
    // cout << endl;
    // string text4 = "{\"name\":\"liuliu\",age\":18}";
    // CuttingJson(text4);
    // cout << endl;
    // string text5 = "{\"name\":liuliu\",\"age\":18}";
    // CuttingJson(text5);
    // cout << endl;
    // string text6 = "{name\":\"liuliu\",age\":18}";
    // CuttingJson(text6);
    // cout << endl;
    // string text7 = "{\"name\":\"liuliu\",\"age:18}";
    // CuttingJson(text7);
    // cout << endl;
    // string text8 = "{\"name\":\"liuliu\",,\"age\":18}";
    // CuttingJson(text8);
    // cout << endl;
    // string text9 = "{,\"name\":\"liuliu\",\"age:18}";
    // CuttingJson(text9);
    // cout << endl;
    // string text10 = "{\"name\":\"liuliu\",\"age:18}";
    // CuttingJson(text10);
    // cout << endl;
    // string text11 = "{\"name\":\"liuliu\",age:18}";
    // CuttingJson(text11);
    // cout << endl;
    // string text12 = "{\"name\":\"liuliu\",\"age\":18,\"stu\":\"name\":\"liuliu\",\"age\":18},\"tea\":{\"name\":\"wufang\",\"age\":48}}";
    // CuttingJson(text12);
    // cout << endl;
    // string text13 = "{\"name\":\"liuliu\",\"age\":18,\"stu\":{\"name\":\"liuliu\",\"age\":18,\"tea\":{\"name\":\"wufang\",\"age\":48}}";
    // CuttingJson(text13);
    // cout << endl;
    // string text14 = "{\"name\":\"liuliu\",\"age\":18,\"stu\":{\"name\":\"liuliu\",\"age\":18}\"tea\":{\"name\":\"wufang\",\"age\":48}}";
    // CuttingJson(text14);
    // cout << endl;
    // string text15 = "{\"name\":\"liuliu\",\"age\":18,\"stu\":{\"name\":\"liuliu\",\"age\":18},\"tea\":\"name\":\"wufang\",\"age\":48}}";
    // CuttingJson(text15);
    // cout << endl;
    // string text16 = "{\"name\":\"liuliu\",\"age\":18,\"stu\":{\"name\":\"liuliu\",\"age\":18},\"tea\"{\"name\":\"wufang\",\"age\":48}}";
    // CuttingJson(text16);
    // cout << endl;
    // string text17 = "{\"name\":\"liuliu\",\"age\"18,\"stu\":{\"name\":\"liuliu\",\"age\":18},\"tea\":{\"name\":\"wufang\",\"age\":48}}";
    // CuttingJson(text17);
    // cout << endl;
    // string text18 = "{\"name\":\"liuliu\",\"age\":18,\"stu\":{\"name\":\"liuliu\",\"age\":18},\"tea\":{\"name\":\"wufang\",\"age\":48";
    // CuttingJson(text18);
    // cout << endl;
    // string text19 = "{\"name\":\"liuliu\",\"age\":18,\"stu\":\"name\":\"liuliu\",\"age\":18},\"tea\":\"name\":\"wufang\",\"age\":48}}";
    // CuttingJson(text19);
    // cout << endl;
    // string text20 = "{\"name\":\"liuliu\",\"age\":18,\"stu\"::\"liuliu\",\"age\":18},\"tea\":{\"name\":\"wufang\",\"age\":48}}";
    // CuttingJson(text20);
    // cout << endl;
    return 0;
}
