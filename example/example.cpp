#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
#include "FStruct.h"
#include <typeinfo>
#include <iostream>
#include <list>
#include <functional>
#include <time.h>//clock()��Ҫ��ͷ�ļ�
#include <unordered_map>
#include <locale.h>
using namespace std;

//g++ -std=c++11 ../FStruct.cpp example.cpp -o main -w


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

//wstring strToStdWString3(const string& s)
//{
//	std::string curLocale = setlocale(LC_ALL, NULL);
//	setlocale(LC_ALL, "chs");
//	const char* _Source = s.c_str();
//	size_t _Dsize = s.size() + 1;
//	wchar_t* _Dest = new wchar_t[_Dsize];
//	wmemset(_Dest, 0, _Dsize);
//	size_t convertedChars = 0;
//	mbstowcs_s(&convertedChars, _Dest, _Dsize, _Source, _TRUNCATE);
//	std::wstring ws = _Dest;
//	delete[] _Dest;
//	setlocale(LC_ALL, curLocale.c_str());
//	return ws;
//}

int main()
{
	cout << "===============================================================" << endl;
	//setlocale(LC_ALL, "chs");
	wstring aaa = L"���";
	wcout.imbue(locale("chs"));
	wcout << aaa << endl;

	cout << FdogSerializer::Instance()->wstring2string(aaa) << endl;
	//wcout << aaa << endl;

	//std::string strLocale = setlocale(LC_ALL, "");
	//const wchar_t* wchSrc = aaa.c_str();
	//size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
	//char *chDest = new char[nDestSize];
	//memset(chDest, 0, nDestSize);
	//wcstombs(chDest, wchSrc, nDestSize);
	//std::string strResult = chDest;
	//delete[]chDest;
	//setlocale(LC_ALL, strLocale.c_str());
	//cout << strResult << endl;

	//wcout << L"aaa = " << aaa << endl;

	std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;

	cout << "aaa2 = " << strCnv.to_bytes(aaa) << endl;
	//std::locale::global(std::locale(""));
	//std::wcout << L"����" << std::endl;
	cout << "===============================================================" << endl;
	cout << "\n1���ṹ�����unordered_map------------------------------------\n";
	//std::wcout << L"�û���ѡ��������Ϊ" << std::locale("").name().c_str() << std::endl;
	//std::wcout << L"�û���ѡ��������Ϊ" << std::locale("").name().c_str() << std::endl;
	// ���û�ƫ�õı��ػ����滻 C++ ���ػ����� C ���ػ���
	//std::locale::global(std::locale(""));
	// �����Ŀ��ַ����ʹ���µ�ȫ�ֱ��ػ���
	//setlocale(LC_ALL, "");
	//std::wcout << L"����" << std::endl;

	// REGISTEREDMEMBER(test_map, grade1);
	// test_map tm;
	// tm.grade1[11] = 88;
	// tm.grade1[22] = 99;
	// string tm_json;
	// Fdog::FJson(tm_json, tm);
	// cout << "tm =" << tm_json << endl;

	// map<string,int> a;
	// unordered_map<string,int> b;
	// unordered_map<string,string> c;
	//std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > >
	//std::unordered_map<int, int, std::hash<int>, std::equal_to<int>, std::allocator<std::pair<int const, int> > >
	//cout << "a = " << abi::__cxa_demangle(typeid(a).name(),0,0,0) << endl;
	//cout << "b = " << abi::__cxa_demangle(typeid(b).name(),0,0,0) << endl;
	//cout << "c = " << abi::__cxa_demangle(typeid(c).name(),0,0,0) << endl;
	cout << "===============================================================" << endl;
	cout << "\n1���ṹ��ֻ������������ uint8_t��uint16_t��uint32_t��uint64_t��int��float��double��wstring------------------------------------\n";
	REGISTEREDMEMBER(TestType, age1, age2, age3, age4, age5, age6, age7, age8);
	TestType tt;
	tt.age1 = 1;
	tt.age2 = 2;
	tt.age3 = 3;
	tt.age4 = 123456;
	tt.age5 = 123456;
	tt.age6 = 123.456f;
	tt.age7 = 1234.5678;
	tt.age8 = L"���";
	string tt_json = "";
	Fdog::FJson(tt_json, tt);
	//��� ���stu_jsonΪ�� {"name":"yujing","age":21}
	cout << tt_json << endl;

	string tt2_json = "{\"age1\":1,\"age2\":2,\"age3\":3,\"age4\":123456,\"age5\":123456,\"age6\":123.456,\"age7\":1234.57,\"age8\":\"����\"}";
	TestType tt2;
	Fdog::FObject(tt2, tt2_json);
	cout << "age1" << tt2.age1 << "  age2" << tt2.age2 << "  age3" << tt2.age3
		<< "  age4" << tt2.age4 << "  age5" << tt2.age5 << "  age6" << tt2.age6 << "  age7" << tt2.age7 << endl;
	wcout << "age8 = " << tt2.age8 << endl;

	cout << "===============================================================" << endl;
	cout << "\n1���ṹ��ֻ������������------------------------------------\n";
	//1.�ṹ��ֻ������������(int,char,char*,string,�Լ��ɻ������͹��ɵ�����,������STL����(map�ݲ�֧��ȫ����))����ֻ��Ҫע���Ա���ɡ�
	// const char * asd = "7student";
	// cout << abi::__cxa_demangle(asd,0,0,0) << endl;
	REGISTEREDMEMBER(student, name, age);  //ע��student��Ա
										   // cout << "===";
	student stu;
	stu.name = "yujing";
	stu.age = 21;
	string stu_json = "";
	//�ṹ��תjson
	Fdog::FJson(stu_json, stu);
	//��� ���stu_jsonΪ�� {"name":"yujing","age":21}
	cout << stu_json << endl;
	student stu2;
	string stu2_json = "{\"name\":\"zhangxv\",\"age\":21}"; //����Ҫ������
															//jsonת�ṹ��
	Fdog::FObject(stu2, stu2_json);
	//���  stu2.name = zhangxv   stu2.age = 21
	cout << "name = " << stu2.name << " age = " << stu2.age << endl;
	// cout << "�ࣺ" << endl;

	// REGISTEREDMEMBER(classtest, name, age);
	// classtest cc;
	// cc.name = "1111";
	// cc.age = 21;
	// stu_json = "";
	// Fdog::FJson(stu_json, cc);  
	// cout << stu_json << endl;

	cout << "\n2���ṹ���г��˻������ͣ��������Զ���ṹ��------------------------------------\n";

	//2.�ṹ���г��˻������ͣ��������Զ���ṹ��
	REGISTEREDMEMBER(teacher, name, age);  //ע��teacher��Ա
	REGISTEREDMEMBER(school, stu, tea);     //ע��school��Ա
	school sch;
	sch.stu.name = "liuliu";
	sch.stu.age = 18;
	sch.tea.name = "wufang";
	sch.tea.age = 48;
	string sch_json = "";
	//�ṹ��תjson
	Fdog::FJson(sch_json, sch);
	//��� ���sch_jsonΪ��{"stu":{"name":"liuliu","age":18},"tea":{"name":"wufang","age":48}}
	cout << sch_json << endl;
	//jsonת�ṹ��
	school sch2;
	string sch2_json = "{\"stu\":{\"name\":\"liuliu\",\"age\":18},\"tea\":{\"name\":\"wufang\",\"age\":48}}";
	Fdog::FObject(sch2, sch2_json);
	//���  
	cout << "name = " << sch.stu.name << " age = " << sch.stu.age << endl;
	cout << "name = " << sch.tea.name << " age = " << sch.tea.age << endl;

	cout << "\n3���ṹ����ʹ��STL����map------------------------------------\n";

	//3.�ṹ����ʹ��STL����map
	REGISTEREDMEMBER(class_map, grade);
	class_map cm;
	cm.grade["zhangxu"] = 88;
	cm.grade["yujing"] = 99;
	string cm_json;
	Fdog::FJson(cm_json, cm);
	cout << cm_json << endl;
	class_map cm2;
	string cm2_json = "{\"grade\":{\"lisi\":33,\"angwu\":11}}";
	Fdog::FObject(cm2, cm2_json);
	for (auto j : cm2.grade) {
		cout << "name = " << j.first << " age = " << j.second << endl;
	}

	cout << "\n4���ṹ���Ա�������飬������洢�Ļ�����������------------------------------------\n";

	//4.�ṹ���Ա�������飬������洢�Ļ�����������
	REGISTEREDMEMBER(class_base_array, numbers)
		class_base_array cba;
	cba.numbers[0] = 0;
	cba.numbers[1] = 1;
	cba.numbers[2] = 2;
	cba.numbers[3] = 3;
	cba.numbers[4] = 4;
	string cba_json = "";
	//�ṹ��תjson
	Fdog::FJson(cba_json, cba);
	cout << cba_json << endl;
	//jsonת�ṹ��
	class_base_array cba2;
	string cba2_json = "{\"cba2_json\":{\"numbers\":[11,12,13,14,15]}}";
	Fdog::FObject(cba2, cba2_json);
	cout << "numbers is " << cba2.numbers[0] << " " << cba2.numbers[1] << " " << cba2.numbers[2] << " " << cba2.numbers[3] << " " << cba2.numbers[4] << endl;

	cout << "\n5���ṹ���Ա����STL������������Ϊ��������------------------------------------\n";

	//5.�ṹ���Ա����STL������������Ϊ��������
	REGISTEREDMEMBER(class_base_vector, numbers)
		class_base_vector cbv; //Ҫע��������ı��� vector����Ϊ0��ʹ���½Ǳ긳ֵ�����
	cbv.numbers.push_back(0);
	cbv.numbers.push_back(1);
	cbv.numbers.push_back(2);
	cbv.numbers.push_back(3);
	cbv.numbers.push_back(4);
	string cbv_json = "";
	//�ṹ��תjson
	Fdog::FJson(cbv_json, cbv);
	cout << cbv_json << endl;
	//jsonת�ṹ��
	class_base_vector cbv2;
	string cbv2_json = "{\"cbv2_json\":{\"numbers\":[22,32,12,4,55]}}";
	Fdog::FObject(cbv2, cbv2_json);
	//cout << "numbers is " << cbv2.numbers[0] << " " << cbv2.numbers[1] << " " << cbv2.numbers[2] << " " << cbv2.numbers[3] << " " << cbv2.numbers[4] << endl;

	cout << "\n6���ṹ���Ա�����Զ������͵�����------------------------------------\n";

	//6.�ṹ���Ա�����Զ������͵�����
	REGISTEREDMEMBER(class_object_array, stus)
		class_object_array coa;
	coa.stus[0].name = "zhangxu";
	coa.stus[0].age = 21;
	coa.stus[1].name = "yujing";
	coa.stus[1].age = 22;
	string coa_json = "";
	//�ṹ��תjson
	Fdog::FJson(coa_json, coa);
	cout << coa_json << endl;
	//jsonת�ṹ��
	class_object_array coa2;
	string coa2_json = "{\"stus\":[{\"name\":\"jinqi\",\"age\":25},{\"name\":\"ruiming\",\"age\":12}]}";
	Fdog::FObject(coa2, coa2_json);
	cout << " name = " << coa2.stus[0].name << " age = " << coa2.stus[0].age << endl;
	cout << " name = " << coa2.stus[1].name << " age = " << coa2.stus[1].age << endl;

	cout << "\n7���ṹ���Ա�����Զ�������STL����------------------------------------\n";

	//7.�ṹ���Ա�����Զ�������STL����
	REGISTEREDMEMBER(class_object_vector, stus)
		class_object_vector cov;
	student stu3;
	stu3.name = "zhangxu";
	stu3.age = 21;
	student stu4;
	stu4.name = "yujing";
	stu4.age = 21;
	cov.stus.push_back(stu3);
	cov.stus.push_back(stu4);
	string cov_json = "";
	//�ṹ��תjson
	Fdog::FJson(cov_json, cov);
	cout << cov_json << endl;
	//jsonת�ṹ��
	class_object_vector cov2; //�ǵó�ʼ������ �������Ҫ�ĳ��Զ�
	cov2.stus.resize(2);
	string cov2_json = "{\"stus\":[{\"name\":\"jinqi\",\"age\":25},{\"name\":\"ruiming\",\"age\":12}]}";
	Fdog::FObject(cov2, cov2_json);
	cout << " name = " << cov2.stus[0].name << " age = " << cov2.stus[0].age << endl;
	cout << " name = " << cov2.stus[1].name << " age = " << cov2.stus[1].age << endl;

	cout << "\n8��֧�ֱ���------------------------------------\n";

	//8.֧�ֱ���
	//Fdog::setAliasName("student", "name", "Aliasname"); //��һ������Ϊ���ͣ��ڶ�����Ϊԭ��������������Ϊ���� ���ֶ�����
	Fdog::setAliasNameS("student", "name", "Aliasname", "age", "Aliasage"); //֧�ֶ���ֶ�����
	stu_json = "";
	Fdog::FJson(stu_json, stu);
	//��� ���stu_jsonΪ�� {"Aliasname":"yujing","age":21}
	cout << stu_json << endl;

	cout << "\n9��֧���ֶκ���------------------------------------\n";

	//9.֧���ֶκ���
	//Fdog::setIgnoreField("student", "name");  //��һ������Ϊ���ͣ��ڶ�����Ϊ��Ҫ���Ե��ֶ� ���ֶ�����
	Fdog::setIgnoreFieldS("student", "name", "age"); //֧�ֶ���ֶ�
	stu_json = "";
	Fdog::FJson(stu_json, stu);
	//��� ���stu_jsonΪ�� {"age":21}  //name�ֶε����ݽ�������
	cout << stu_json << endl;

	cout << "\n10��֧�ֺ����ֶδ�Сд------------------------------------\n";

	//10.֧�ֺ����ֶδ�Сд������jsonתΪ����ʱ����json�еļ�ֵ������еĳ�Ա�����ڴ�Сд��ͬ�������趨���Դ�Сд��
	//Fdog::setIgnoreLU("student", "name"); ���ֶ�����
	//Fdog::setIgnoreLU("student", "age");

	Fdog::setIgnoreLUS("student", "name", "age"); //֧�ֶ��ֶ�����
	stu_json = "{\"Name\":\"yujing\", \"AGE\":21}";
	Fdog::FObject(stu, stu_json);  //��Name��Ӧname��AGE��Ӧage
	cout << "name = " << stu.name << " age = " << stu.age << endl;

	cout << "\n11�����7��8��9�ӿ����Ӷ�Ӧ��һ���Խӿ�------------------------------------\n";
	//11.���7��8��9�ӿ����Ӷ�Ӧ��һ���Խӿڣ������ж���ֶ���Ҫ���ã��Ӷ���ε��ýӿ�
	cout << "�����Ѿ����������г�" << endl;

	cout << "\n12��Ĭ��֧��ģ��ƥ��------------------------------------\n";
	//12.Ĭ��֧��ģ��ƥ��
	//����֧�֣�����С��д���ֶ���ʱ�������Զ�����ģ��ƥ�䣬���������ת����
	cout << "����" << endl;

	cout << "\n13�����Json��ʽ�Ƿ���ȷ------------------------------------\n";
	//13.���Json��ʽ�Ƿ���ȷ
	string json_v = "{\"stus\":[{\"name\":\"jinqi\",\"age\":25},{\"name\":\"ruiming\",\"age\":12}]}";
	cout << "�ַ���:" << json_v << endl;
	auto res = Fdog::JsonValidS(json_v);
	cout << "�ַ����Ƿ���ȷ��" << res.code << endl;
	cout << "�ַ���������ʾ��" << res.message << endl;

	cout << "\n14������json��ĳ���ֶ��Ƿ����------------------------------------\n";
	//14.����json��ĳ���ֶ��Ƿ����
	if (Fdog::Exist(json_v, "age")) {
		cout << "����" << endl;
	}
	else {
		cout << "������" << endl;
	}

	cout << "\n15��֧�ֻ�ȡĳ���ֶε�ֵ------------------------------------\n";
	//15.֧�ֻ�ȡĳ���ֶε�ֵ(��������֧��int, double, string, bool)
	//�������Exist����ʹ��
	if (Fdog::Exist(json_v, "age")) {
		cout << "age��ֵ:" << Fdog::GetStringValue(json_v, "age") << endl;
	}
	else {
		cout << "������" << endl;
	}
	//��GetStringValue֮�⣬���ṩ�����ĸ����ڷ��ز�ͬ����
	// GetIntValue
	// GetDoubleValue
	// GetLongValue
	// GetBoolValue

	//json������һЩ���⣬������100%���飬����Ҫʱ��������

	cout << "\n16��֧��ֱ���������------------------------------------\n";
	//16. ֧���������
	school sch_2;
	sch_2.stu.name = "liuliu";
	sch_2.stu.age = 18;
	sch_2.tea.name = "wufang";
	sch_2.tea.age = 48;
	string stu_json_22;
	Fdog::FJson(stu_json_22, sch_2);
	//��� ���stu_jsonΪ�� {"name":"yujing","age":21}
	cout << stu_json_22 << endl;

	//cout << "����ֵ��" << Fdog::FJsonToString(sch_2) << endl;

	//17.֧����������ָ��(ָ�����ͽ�ӵ�п�ѡ�ֶ����ԣ�����ָ���������ת��ʱ�������ж�ָ���ַ�Ƿ�Ϊ�գ���Ϊ�գ���������ת���������ں����ֶ�)
	//�¸��汾

	//17.֧��xml���л�
	//���°汾��

	cout << "===============================================================" << endl;
	return 0;
}

