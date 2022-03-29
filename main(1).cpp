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
    bool isadd = false;
    for(int i = 0; i < len; i++){
        if(json_[i] == '"'){
			if(sum == 0 && status == -1){
                status = 0;
                first = i;
            }
        }
        if(status == 3 && json_[i] == '{' && isadd == true){
            sum++;
        }

        if(status == 4 && json_[i] == '[' && isadd == true){
            sum++;
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
            if(status == 2 && json_[i] == ',' && json_[i+1] == '"'){
                sum--;
                if(sum == 0){
                    end = i;
                    string da = json_.substr(first, end - first);
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
            if(status == 3 && json_[i] == '}'){
                if(isadd == true){
                    sum--;
                }
                if(sum == 0){
                    end = i;
                    string da = json_.substr(first, end - first + 1);
                    cout << "获取到的值7：" << da << endl;
                    status = -1;
                    json_array.push_back(da);
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

result JsonValidS(string json_) {
    result res;
    res.code = 1;
    if (!IsCurlyBraces(json_)){
        res.code = 0;
        res.message = "缺少花括号";
        return res;
    }
    string regexStr = "\"\\w+\":((\\{(.*?)\\})|(\\[(.*?)\\])|(\"(.*?)\")|([+-]?([0-9]*\\.?[0-9]+|[0-9]+\\.?[0-9]*)([eE][+-]?[0-9]+)?)|(true|false))";
    smatch result;
    regex pattern(regexStr);

    string::const_iterator iterStart = json_.begin();
    string::const_iterator iterEnd = json_.end();
    string temp;
    while (regex_search(iterStart, iterEnd, result, pattern))
    {
        temp = result[0];
        cout << "类型：" << temp << endl;
        //JsonValidS_s(res, temp);
        // if (statusType == 3 || statusType == 4){

        // }
        iterStart = result[0].second;	//更新搜索起始位置,搜索剩下的字符串
    }
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



int main(){
    //{"stu":{"name":"liuliu","age":18},"tea":{"name":"wufang","age":48}}
    string js = "{\"name\":\"liuliu\",\"age\":18}";
    string js2 = "{\"name\":\"liuliu\",\"age\":18,\"stu\":{\"name\":\"liuliu\",\"age\":18},\"tea\":{\"name\":\"wufang\",\"age\":48}}";
    //cout << js2 << endl;



    string json_ = "{\"stu\":{\"name\":\"liuliu\",\"age\":18},\"tea\":{\"name\":\"wufang\",\"age\":48},\"stu\":[\"zhan\",\"xv\",\"321%90^$%\"],\"tex\":\"dsa\",\"wxc\":-312,\"dsa\":456,\"oew\":-412.3123,\"dae\":true}";

    string json_2 = "{\"class\":423,\"name\":\"张三\",\"age\":21,\"stu\":true,\"student\":{\"name\":\"李四\",\"age\":13},\"school\":[\"2506892\",\"186598\",\"12344687\"],\"addr\":[{\"name\":\"王五\",\"age\":22},{\"name\":\"于静\",\"age\":23},{\"name\":\"张旭\",\"age\":33}],\"all\":{\"name\":\"胡瑞敏\",\"age\":32,\"stu\":true,\"student\":{\"name\":\"zhangxv\",\"age\":1343},\"school\":[\"3f4312\",\"321089\",\"756896\"],\"addr\":[{\"name\":\"dasdas\",\"age\":13},{\"name\":\"ytrhfg\",\"age\":13}],\"addr\":[{\"name\":\"dasdas\",\"age\":13},{\"name\":\"ytrhfg\",\"age\":13},{\"name\":\"hgftre\",\"age\":13}]}}}}";
    cout << "原值：" << json_2 << endl;

    string json_3 = "{\"all\":{\"name\":\"胡瑞敏\",\"age\":32,\"stu\":true,\"student\":{\"name\":\"zhangxv\",\"age\":1343},\"school\":[\"3f4312\",\"321089\",\"756896\"],\"addr\":[{\"name\":\"dasdas\",\"age\":13},{\"name\":\"ytrhfg\",\"age\":13}],\"addr\":[{\"name\":\"dasdas\",\"age\":13},{\"name\":\"ytrhfg\",\"age\":13},{\"name\":\"hgftre\",\"age\":13}]}}";
    //cout << json_3 << endl;
    //func(json_2);
    CuttingJson(json_2);
    //cout << json_2 << endl;
    //result res = JsonValidS(json_2);
    //cout << "返回值：" << res.code << endl;
    return 0;
}
