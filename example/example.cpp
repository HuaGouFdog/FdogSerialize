#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
#include <typeinfo>
#include <iostream>
#include <list>
#include <functional>
#include <time.h>//clock()需要的头文件
#include <unordered_map>
#include <locale.h>

//g++ -std=c++11 ../FStruct.cpp example.cpp -o main -w
#include "testType.h"
#ifdef __GNUC__
#include <dirent.h>
#include<sys/types.h>
#include <unistd.h>
#elif _MSC_VER
#endif

#include <regex>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

// void initFdog() {
// 	addFun(student::init);
// 	addFun(teacher::init);
// 	addFun(school::init);
// 	return;
// }

// Fdog::AddSerialize{
// 	student::init, 
// 	teacher::init, 
// 	school::init
// }

//Fdog::InitFdogSerialize();



string to_string(const wstring& str, const locale& loc = locale("chs"))
{
	vector<char>buf(str.size());
	use_facet<ctype<wchar_t>>(loc).narrow(str.data(), str.data() + str.size(), '*', buf.data());
	return string(buf.data(), buf.size());
}

wstring to_wstring(const string& str, const locale& loc = locale("chs"))
{
	vector<wchar_t>buf(str.size());
	use_facet<ctype<wchar_t>>(loc).widen(str.data(), str.data() + str.size(), buf.data());
	return wstring(buf.data(), buf.size());
}
//g++ -std=c++11 ../FStruct.cpp example.cpp -o main -w

// std::vector<std::string> get_all_files(std::string path, std::string suffix)
// {
//     std::vector<std::string> files;
// //    files.clear();
//     regex reg_obj(suffix, regex::icase);

//     std::vector<std::string> paths;
//     paths.push_back(path);

//     for(int i = 0; i < paths.size(); i++)
//     {
//         string curr_path = paths[i];
//         DIR *dp;
//         struct dirent *dirp;
//         if((dp = opendir(curr_path.c_str())) == NULL)
//         {
//             cerr << "can not open this file." << endl;
//             continue;
//         }
//         while((dirp = readdir(dp)) != NULL)
//         {
//             if(dirp->d_type == 4)
//             {
//                 if((dirp->d_name)[0] == '.') // 这里很奇怪，每个文件夹下都会有两个文件： '.'  和   '..'
//                     continue;
// //                cout << dirp->d_name << " ";
//                 string tmp_path = curr_path + "/" + dirp->d_name;
// //                cout << tmp_path << " ";
//                 paths.push_back(tmp_path);
// //                cout << paths[paths.size() - 1] << endl;
//             }
//             else if(dirp->d_type == 8)
//             {
//                 if(regex_match(dirp->d_name, reg_obj))
//                 {
//                     string full_path = curr_path + "/" + dirp->d_name;
//                     files.push_back(full_path);
//                 }
//             }
//         }
//         closedir(dp);
//     }
//     return files;
// }

int main() {
	//获取应用程序目录
	// char sourceFilePash[128];
	// set<string> param;
	// string path = getcwd(sourceFilePash, sizeof(sourceFilePash) - 1);
	// cout << " 路径 : " << path << endl;
	// string re = ".*.cpp|.*.h|.*.hpp";
	// auto a = get_all_files(path, re);
	// cout << "长度 ="  << a.size() << endl;
	// smatch result;
	// for(auto j : a) {
	//     cout << "name = " << j << endl;
	//     ifstream infile;
	//     infile.open(j.c_str()); 
	//     if(!infile){ 
	//       cout << "出错" << endl;
	//     }
	//     while (infile.good()) {
	//       string text;
	//       getline(infile,text,'\n');//以回车为分隔符
	//       regex pattern("REGISTEREDMEMBER\\((\\w+)", regex::icase);
	//       if(regex_search(text, result, pattern)){
	//           string value = result.str(1).c_str();
	//           cout << "参数：" << value << endl;
	//           param.insert(value);
	//       } 
	//     }
	// }

	// ofstream out;
	// out.open("defType123.h", ios::out|ios::trunc);
	// if (out.is_open()) {
	//   out << "#ifndef BEHAVIOR_H\n";
	//   out << "#define BEHAVIOR_H\n";
	//   out << "#include \"define.h\"\n\n";

	//     out << "#define Serialize_type_judgment_all\\\n";
	//     for(auto j : param) {
	//       out << "    Serialize_type_judgment(" << j << ")\\\n";
	//     }
	//     out << "\n";

	//     out << "#define Deserialize_type_judgment_all\\\n";
	//     for(auto j : param) {
	//       out << "    Deserialize_type_judgment(" << j << ")\\\n";
	//     }
	//     out << "\n";


	//     out << "#define Serialize_arraytype_judgment_all\\\n";
	//     for(auto j : param) {
	//       out << "    Serialize_arraytype_judgment(" << j << ")\\\n";
	//     }
	//     out << "\n";


	//     out << "#define Deserialize_arraytype_judgment_all\\\n";
	//     for(auto j : param) {
	//       out << "    Deserialize_arraytype_judgment(" << j << ")\\\n";
	//     }
	//     out << "\n";


	//     out << "#define Serialize_vector_type_judgment_all\\\n";
	//     for(auto j : param) {
	//       out << "    Serialize_vector_type_judgment(" << j << ")\\\n";
	//     }
	//     out << "\n";


	//     out << "#define Deserialize_vector_type_judgment_all\\\n";
	//     for(auto j : param) {
	//       out << "    Deserialize_vector_type_judgment(" << j << ")\\\n";
	//     }
	//     out << "\n";


	//     out << "#define Serialize_list_type_judgment_all\\\n";
	//     for(auto j : param) {
	//       out << "    Serialize_list_type_judgment(" << j << ")\\\n";
	//     }
	//     out << "\n";


	//     out << "#define Deserialize_list_type_judgment_all\\\n";
	//     for(auto j : param) {
	//       out << "    Deserialize_list_type_judgment(" << j << ")\\\n";
	//     }
	//     out << "\n";


	//     out << "#define Serialize_deque_type_judgment_all\\\n";
	//     for(auto j : param) {
	//       out << "    Serialize_deque_type_judgment(" << j << ")\\\n";
	//     }
	//     out << "\n";


	//     out << "#define Deserialize_deque_type_judgment_all\\\n";
	//     for(auto j : param) {
	//       out << "    Deserialize_deque_type_judgment(" << j << ")\\\n";
	//     }
	//     out << "\n";


	//     out << "#define Serialize_set_type_judgment_all\\\n";
	//     for(auto j : param) {
	//       out << "    Serialize_set_type_judgment(" << j << ")\\\n";
	//     }
	//     out << "\n";


	//     out << "#define Deserialize_set_type_judgment_all\\\n";
	//     for(auto j : param) {
	//       out << "    Deserialize_set_type_judgment(" << j << ")\\\n";
	//     }
	//     out << "\n";

	//     out << "#define Serialize_map_type_judgment_all\\\n";
	//     for(auto j : param) {
	//       out << "    Serialize_map_type_judgment(" << j << ")\\\n";
	//     }
	//     out << "\n";


	//     out << "#define Deserialize_map_type_judgment_all\\\n";
	//     for(auto j : param) {
	//       out << "    Deserialize_map_type_judgment(" << j << ")\\\n";
	//     }
	//     out << "\n";



	//   out << "#endif\n";
	// }

	//REGISTEREDMEMBER(class_unordered_map, grade);
	//   cout << "===============================================================" << endl;
	//   wstring aaa = L"你好";
	//   wcout << "aaa = " << aaa << endl;

	//   std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
	//   cout << "aaa2 = " << strCnv.to_bytes(aaa) << endl;

	//   cout << "===============================================================" << endl;
	//   cout << "\n1：结构体包含unordered_map------------------------------------\n";
	//   //std::wcout << L"用户首选区域设置为" << std::locale("").name().c_str() << std::endl;
	//   //std::wcout << L"用户首选区域设置为" << std::locale("").name().c_str() << std::endl;
	// // 以用户偏好的本地环境替换 C++ 本地环境和 C 本地环境
	//   //std::locale::global(std::locale(""));
	//   // 将来的宽字符输出使用新的全局本地环境
	//   //setlocale(LC_ALL, "");
	//   //std::wcout << L"中文" << std::endl;
	//   REGISTEREDMEMBER(class_unordered_map, grade);
	// class_unordered_map ucm;
	// ucm.grade["zhangxu"] = 88;
	// ucm.grade["yujing"] = 99;
	// string ucm_json;
	// Fdog::FJson(ucm_json, ucm);
	// cout << "ucm_json =" << ucm_json << endl;
	// class_unordered_map ucm2;
	// string ucm2_json = "{\"grade\":{\"lisi\":33,\"angwu\":33}}";
	// Fdog::FObject(ucm2, ucm2_json);
	// for(auto j : ucm2.grade){
	//     cout << "name = " << j.first << " age = " << j.second << endl;
	// }

	cout << "===============================================================" << endl;
	// string sch1 = "";
	// string sch2 = "";
	// student s;
	// s.name = "aaa";
	// s.age = 12;
	// teacher t;
	// t.name = "333";
	// t.age = 12;
	// Fdog::FJson(sch1, s); 
	// cout << sch1 << endl;
	// Fdog::FJson(sch2, t);
	// cout << sch2 << endl;
	//initFdog();
	double dur;
	clock_t start, end;
	start = clock();
	school sch;
	sch.stu.name = "liuliu";
	sch.stu.age = 18;
	sch.tea.name = "wufang";
	sch.tea.age = 48;

	student stu3;
	stu3.name = "zhangxu";
	stu3.age = 21;
	student stu4;
	stu4.name = "yujing";
	stu4.age = 21;
	sch.stuList.push_back(stu3);
	sch.stuList.push_back(stu4);
	for (int i = 0; i < 1; i++) {

		string sch_json = "";
		//结构体转json
		Fdog::FJson(sch_json, sch);
		//结果 输出sch_json为：{"stu":{"name":"liuliu","age":18},"tea":{"name":"wufang","age":48}}
		cout << sch_json << endl;
		//json转结构体
		school sch2;
		//sch2.stuList.resize(2);
		string sch2_json = "{\"stu\":{\"name\":\"liuliu\",\"age\":18},\"tea\":{\"name\":\"wufang\",\"age\":48},\"stuList\":[{\"name\":\"zhangxu\",\"age\":21},{\"name\":\"yujing\",\"age\":21}]}";
		Fdog::FObject(sch2, sch2_json);
		// //结果  
		cout << "name = " << sch2.stu.name << " age = " << sch2.stu.age << endl;
		cout << "name = " << sch2.tea.name << " age = " << sch2.tea.age << endl;
		for (int i = 0; i < sch2.stuList.size(); i++) {
			cout << "stuList name = " << sch2.stuList[i].name << " age = " << sch2.stuList[i].age << endl;
		}
	}

	end = clock();
	dur = (double)(end - start);
	printf("Use Time:%f\n", (dur / CLOCKS_PER_SEC));
	//   // REGISTEREDMEMBER(test_map, grade1);
	//   // test_map tm;
	//   // tm.grade1[11] = 88;
	//   // tm.grade1[22] = 99;
	//   // string tm_json;
	//   // Fdog::FJson(tm_json, tm);
	//   // cout << "tm =" << tm_json << endl;

	//   // map<string,int> a;
	//   // unordered_map<string,int> b;
	//   // unordered_map<string,string> c;
	//   //std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > >
	//   //std::unordered_map<int, int, std::hash<int>, std::equal_to<int>, std::allocator<std::pair<int const, int> > >
	//   //cout << "a = " << abi::__cxa_demangle(typeid(a).name(),0,0,0) << endl;
	//   //cout << "b = " << abi::__cxa_demangle(typeid(b).name(),0,0,0) << endl;
	//   //cout << "c = " << abi::__cxa_demangle(typeid(c).name(),0,0,0) << endl;
	//   cout << "===============================================================" << endl;
	//   cout << "\n1：结构体只包含基础类型 uint8_t，uint16_t，uint32_t，uint64_t，int，float，double，wstring------------------------------------\n";
	//   REGISTEREDMEMBER(TestType, age1, age2, age3, age4, age5, age6, age7, age8);
	//   TestType tt;
	//   tt.age1 = 1;
	//   tt.age2 = 2;
	//   tt.age3 = 3;
	//   tt.age4 = 123456;
	//   tt.age5 = 123456;
	//   tt.age6 = 123.456f;
	//   tt.age7 = 1234.5678;
	//   tt.age8 = L"你好";
	//   string tt_json = "";
	//   Fdog::FJson(tt_json, tt);  
	//   //结果 输出stu_json为： {"name":"yujing","age":21}
	//   cout << tt_json << endl;
	//   string tt2_json = "{\"age1\":1,\"age2\":2,\"age3\":3,\"age4\":123456,\"age5\":123456,\"age6\":123.456,\"age7\":1234.57,\"age8\":\"哈哈\"}";
	//   TestType tt2;
	//   Fdog::FObject(tt2, tt2_json);
	//   cout << "age1" << tt2.age1 << "  age2"<< tt2.age2 << "  age3"<< tt2.age3
	//         << "  age4"<< tt2.age4 << "  age5"<< tt2.age5 << "  age6"<< tt2.age6 << "  age7"<< tt2.age7 << endl;
	//   wcout << "age8 = " << tt2.age8 << endl;
	//   std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv1;
	//   cout << "age8_8 = " << strCnv1.to_bytes(tt2.age8) << endl;

	//   cout << "===============================================================" << endl;
	//   cout << "\n1：结构体只包含基础类型------------------------------------\n";
	//   //1.结构体只包含基础类型(int,char,char*,string,以及由基础类型构成的数组,或者是STL容器(map暂不支持全类型))，则只需要注册成员即可。
	//   // const char * asd = "7student";
	//   // cout << abi::__cxa_demangle(asd,0,0,0) << endl;
	//   REGISTEREDMEMBER(student, name, age);  //注册student成员
	//   // cout << "===";
	//   student stu;
	//   stu.name = "yujing";
	//   stu.age = 21;
	//   string stu_json = "";
	//   //结构体转json
	//   Fdog::FJson(stu_json, stu);  
	//   //结果 输出stu_json为： {"name":"yujing","age":21}
	//   cout << stu_json << endl;
	//   student stu2;
	//   string stu2_json = "{\"name\":\"zhangxv\",\"age\":21}"; //引号要做处理
	//   //json转结构体
	//   Fdog::FObject(stu2, stu2_json);  
	//   //结果  stu2.name = zhangxv   stu2.age = 21
	//   cout << "name = " << stu2.name << " age = " << stu2.age << endl;
	//   // cout << "类：" << endl;

	//   // REGISTEREDMEMBER(classtest, name, age);
	//   // classtest cc;
	//   // cc.name = "1111";
	//   // cc.age = 21;
	//   // stu_json = "";
	//   // Fdog::FJson(stu_json, cc);  
	//   // cout << stu_json << endl;


	//   cout << "\n2：结构体中除了基础类型，还包括自定义结构体------------------------------------\n";

	//   //2.结构体中除了基础类型，还包括自定义结构体
	//   REGISTEREDMEMBER(teacher, name, age);  //注册teacher成员
	//   REGISTEREDMEMBER(school, stu, tea);     //注册school成员
	//   school sch;
	//   sch.stu.name = "liuliu";
	//   sch.stu.age = 18;
	//   sch.tea.name = "wufang";
	//   sch.tea.age = 48;
	//   string sch_json = "";
	//   //结构体转json
	//   Fdog::FJson(sch_json, sch); 
	//   //结果 输出sch_json为：{"stu":{"name":"liuliu","age":18},"tea":{"name":"wufang","age":48}}
	// 	cout << sch_json << endl;
	//   //json转结构体
	//   school sch2;
	//   string sch2_json = "{\"stu\":{\"name\":\"liuliu\",\"age\":18},\"tea\":{\"name\":\"wufang\",\"age\":48}}";
	//   Fdog::FObject(sch2, sch2_json);
	//   //结果  
	//   cout << "name = " << sch.stu.name << " age = " << sch.stu.age << endl;
	//   cout << "name = " << sch.tea.name << " age = " << sch.tea.age << endl;

	//   cout << "\n3：结构体中使用STL容器map------------------------------------\n";

	//   //3.结构体中使用STL容器map
	//   REGISTEREDMEMBER(class_map, grade);
	//   class_map cm;
	//   cm.grade["zhangxu"] = 88;
	//   cm.grade["yujing"] = 99;
	//   string cm_json;
	//   Fdog::FJson(cm_json, cm);
	//   cout << cm_json << endl;
	//   class_map cm2;
	//   string cm2_json = "{\"grade\":{\"lisi\":33,\"angwu\":11}}";
	//   Fdog::FObject(cm2, cm2_json);
	//   for(auto j : cm2.grade){
	//       cout << "name = " << j.first << " age = " << j.second << endl;
	//   }

	//   cout << "\n4：结构体成员存在数组，且数组存储的基础类型数据------------------------------------\n";

	//   //4.结构体成员存在数组，且数组存储的基础类型数据
	//   REGISTEREDMEMBER(class_base_array, numbers)
	//   class_base_array cba;
	//   cba.numbers[0] = 0;
	//   cba.numbers[1] = 1;
	//   cba.numbers[2] = 2;
	//   cba.numbers[3] = 3;
	//   cba.numbers[4] = 4;
	//   string cba_json = "";
	//   //结构体转json
	//   Fdog::FJson(cba_json, cba);
	//   cout << cba_json << endl;
	//   //json转结构体
	//   class_base_array cba2;
	//   string cba2_json = "{\"cba2_json\":{\"numbers\":[11,12,13,14,15]}}";
	//   Fdog::FObject(cba2, cba2_json);
	//   cout << "numbers is " << cba2.numbers[0] << " " << cba2.numbers[1] << " " << cba2.numbers[2] << " " << cba2.numbers[3] << " " << cba2.numbers[4] << endl;

	//   cout << "\n5：结构体成员存在STL容器，且容器为基础类型------------------------------------\n";

	//   //5.结构体成员存在STL容器，且容器为基础类型
	//   REGISTEREDMEMBER(class_base_vector, numbers)
	//   class_base_vector cbv; //要注意刚声明的变量 vector长度为0，使用下角标赋值会出错
	//   cbv.numbers.push_back(0);
	//   cbv.numbers.push_back(1);
	//   cbv.numbers.push_back(2);
	//   cbv.numbers.push_back(3);
	//   cbv.numbers.push_back(4);
	//   string cbv_json = "";
	//   //结构体转json
	//   Fdog::FJson(cbv_json, cbv);
	//   cout << cbv_json << endl;
	//   //json转结构体
	//   class_base_vector cbv2;
	//   string cbv2_json = "{\"cbv2_json\":{\"numbers\":[22,32,12,4,55]}}";
	//   Fdog::FObject(cbv2, cbv2_json);
	//   //cout << "numbers is " << cbv2.numbers[0] << " " << cbv2.numbers[1] << " " << cbv2.numbers[2] << " " << cbv2.numbers[3] << " " << cbv2.numbers[4] << endl;

	//   cout << "\n6：结构体成员存在自定义类型的数组------------------------------------\n";

	//   //6.结构体成员存在自定义类型的数组
	//   REGISTEREDMEMBER(class_object_array, stus)
	//   class_object_array coa;
	//   coa.stus[0].name = "zhangxu";
	//   coa.stus[0].age = 21;
	//   coa.stus[1].name = "yujing";
	//   coa.stus[1].age = 22;
	//   string coa_json = "";
	//   //结构体转json
	//   Fdog::FJson(coa_json, coa);
	//   cout << coa_json << endl;
	//   //json转结构体
	//   class_object_array coa2;
	//   string coa2_json = "{\"stus\":[{\"name\":\"jinqi\",\"age\":25},{\"name\":\"ruiming\",\"age\":12}]}";
	//   Fdog::FObject(coa2, coa2_json);
	//   cout << " name = " << coa2.stus[0].name << " age = " << coa2.stus[0].age << endl;
	//   cout << " name = " << coa2.stus[1].name << " age = " << coa2.stus[1].age << endl;

	//   cout << "\n7：结构体成员存在自定义类型STL容器------------------------------------\n";

	//   //7.结构体成员存在自定义类型STL容器
	//   REGISTEREDMEMBER(class_object_vector, stus)
	//   class_object_vector cov;
	//   student stu3;
	//   stu3.name = "zhangxu";
	//   stu3.age = 21;
	//   student stu4;
	//   stu4.name = "yujing";
	//   stu4.age = 21;
	//   cov.stus.push_back(stu3);
	//   cov.stus.push_back(stu4);
	//   string cov_json = "";
	//   //结构体转json
	//   Fdog::FJson(cov_json, cov);
	//   cout << cov_json << endl;
	//   //json转结构体
	//   class_object_vector cov2; //记得初始化长度 这个后期要改成自动
	//   cov2.stus.resize(2);
	//   string cov2_json = "{\"stus\":[{\"name\":\"jinqi\",\"age\":25},{\"name\":\"ruiming\",\"age\":12}]}";
	//   Fdog::FObject(cov2, cov2_json);
	//   cout << " name = " << cov2.stus[0].name << " age = " << cov2.stus[0].age << endl;
	//   cout << " name = " << cov2.stus[1].name << " age = " << cov2.stus[1].age << endl;

	//   cout << "\n8：支持别名------------------------------------\n";

	//   //8.支持别名
	//   //Fdog::setAliasName("student", "name", "Aliasname"); //第一个参数为类型，第二参数为原名，第三个参数为别名 单字段设置
	//   Fdog::setAliasNameS("student", "name", "Aliasname", "age", "Aliasage"); //支持多个字段设置
	//   stu_json = "";
	//   Fdog::FJson(stu_json, stu);  
	//   //结果 输出stu_json为： {"Aliasname":"yujing","age":21}
	//   cout << stu_json << endl;

	//   cout << "\n9：支持字段忽略------------------------------------\n";

	//   //9.支持字段忽略
	//   //Fdog::setIgnoreField("student", "name");  //第一个参数为类型，第二参数为需要忽略的字段 单字段设置
	//   Fdog::setIgnoreFieldS("student", "name", "age"); //支持多个字段
	//   stu_json = "";
	//   Fdog::FJson(stu_json, stu);
	//   //结果 输出stu_json为： {"age":21}  //name字段的数据将被忽略
	//   cout <<  stu_json << endl;

	//   cout << "\n10：支持忽略字段大小写------------------------------------\n";

	//   //10.支持忽略字段大小写，当将json转为对象时，如json中的键值与对象中的成员名存在大小写不同，可以设定忽略大小写。
	//   //Fdog::setIgnoreLU("student", "name"); 单字段设置
	//   //Fdog::setIgnoreLU("student", "age");

	//   Fdog::setIgnoreLUS("student", "name", "age"); //支持多字段设置
	//   stu_json = "{\"Name\":\"yujing\", \"AGE\":21}";
	//   Fdog::FObject(stu, stu_json);  //将Name对应name，AGE对应age
	//   cout << "name = " << stu.name << " age = " << stu.age << endl;

	//   cout << "\n11：针对7，8，9接口增加对应的一次性接口------------------------------------\n";
	//   //11.针对7，8，9接口增加对应的一次性接口，避免有多个字段需要设置，从而多次调用接口
	//   cout << "上面已举例，不再列出" << endl;

	//   cout << "\n12：默认支持模糊匹配------------------------------------\n";
	//   //12.默认支持模糊匹配
	//   //马上支持，当不小心写错字段名时，程序将自动进行模糊匹配，最大可能完成转换。
	//   cout << "暂无" << endl;

	//   cout << "\n13：检测Json格式是否正确------------------------------------\n";
	//   //13.检测Json格式是否正确
	//   string json_v = "{\"stus\":[{\"name\":\"jinqi\",\"age\":25},{\"name\":\"ruiming\",\"age\":12}]}";
	//   cout << "字符串:" << json_v << endl;
	//   auto res = Fdog::JsonValidS(json_v);
	//   cout << "字符串是否正确：" << res.code << endl;
	//   cout << "字符串错误提示：" << res.message << endl;

	//   cout << "\n14：查找json中某个字段是否存在------------------------------------\n";
	//   //14.查找json中某个字段是否存在
	//   if(Fdog::Exist(json_v, "age")) {
	//       cout << "存在" << endl; 
	//   } else {
	//     cout << "不存在" << endl;
	//   }

	//   cout << "\n15：支持获取某个字段的值------------------------------------\n";
	//   //15.支持获取某个字段的值(返回类型支持int, double, string, bool)
	//   //建议配合Exist函数使用
	//   if(Fdog::Exist(json_v, "age")) {
	//       cout << "age的值:" << Fdog::GetStringValue(json_v, "age") << endl;
	//   } else {
	//     cout << "不存在" << endl;
	//   }
	//   //除GetStringValue之外，还提供下面四个用于返回不同类型
	//   // GetIntValue
	//   // GetDoubleValue
	//   // GetLongValue
	//   // GetBoolValue

	//   //json还存在一些问题，并不能100%检验，还需要时间来完善

	//   cout << "\n16：支持直接输出对象------------------------------------\n";
	//   //16. 支持输出对象
	//   school sch_2;
	//   sch_2.stu.name = "liuliu";
	//   sch_2.stu.age = 18;
	//   sch_2.tea.name = "wufang";
	//   sch_2.tea.age = 48;
	//   string stu_json_22;
	//   Fdog::FJson(stu_json_22, sch_2);  
	//   //结果 输出stu_json为： {"name":"yujing","age":21}
	//   cout << stu_json_22 << endl;

	//cout << "对象值：" << Fdog::FJsonToString(sch_2) << endl;

	//17.支持其他类型指针(指针类型将拥有可选字段属性，对于指针变量，在转换时，将先判断指针地址是否为空，若为空，将不进行转换，类似于忽略字段)
	//下个版本

	//17.支持xml序列化
	//下下版本～

	cout << "===============================================================" << endl;
	return 0;
}
