#include <iostream>
#include "../FStruct.h"
#include <typeinfo>
#include <iostream>
#include <list>
#include <functional>
using namespace std;

//g++ -std=c++11 ../FStruct.cpp example.cpp -o main -w

int main()
{
    //1.结构体只包含基础类型(int,char,char*,string,以及由基础类型构成的数组)，则只需要注册成员即可。
    REGISTEREDMEMBER(student, name, age);  //注册成员
    student stu;
    stu.name = "yujing";
    stu.age = 21;
    string stu_json = ""; 
    //结构体转json
    Fdog::FJson(stu_json, stu);  //结果 stu_json = 
    cout << stu_json << endl;
    student stu2;
    string stu2_json = "{\"name\":\"zhangxv\",\"age\":21}";
    //json转结构体
    Fdog::FObject(stu2, stu2_json);  //结果  stu2
    cout << stu2.name << "-" << stu2.age << endl;

    REGISTEREDMEMBER(teacher, name, age);  //注册teacher成员
    REGISTEREDMEMBER(school, stu, tea);  //注册school成员
    school sch;
    sch.stu.name = "liuliu";
    sch.stu.age = 18;
    sch.tea.name = "wufang";
    sch.tea.age = 48;
    string sch_json = "";
    //结构体转json
    Fdog::FJson(sch_json, sch);
    cout << sch_json << endl;

    //text tx;
    //string a[2] = {"nihao","buhao"};
    //string a[2] ={"10", "20"};
    // tx.a[0]="11.3";
    // tx.a[1]="22.4";
    // tx.b[0]="e32";
    // tx.b[1]="das";
    // string json1;
    // Fdog::FJson(json1, tx);
    //cout << "addr1 = " << &(tx.a[0]) << "--" << tx.a[0] << endl;
    //cout << "addr2 = " << &(tx.a[1]) << "--" << tx.a[1] << endl;
    // cout << json1 << endl;

    // text tx2;
    // Fdog::FObject(tx2, json1);
    // cout << "addr1 = " << &(tx.a[0]) << "--" << tx.a[0] << endl;
    // cout << "addr2 = " << &(tx.a[1]) << "--" << tx.a[1] << endl;
    // int c[2]={1,2};
    // string json2;
    // Fdog::FJson(json2, c, "c");
    //cout << json2 << endl;


    // string json2 = "{\"a\":[312,321]}";
    // Fdog::FObject(tx, json2);
    // cout << json2 << endl;

    // zhang z;
    // z.age = 10;
    // z.a.push_back("312");
    // z.a.push_back("432");
    // z.b[1]=10;
    // z.b[2]=20;
    // z.c.push_back("不好不好");
    // z.c.push_back("你好不好");
    // z.d.push_back("张旭");
    // z.d.push_back("张旭");
    // z.e.insert("张旭2");
    // z.e.insert("张旭1");
    // z.f[0]=11;
    // z.f[1]=12;
    // string zjson;
    // Fdog::FJson(zjson, z);
    // cout << zjson << endl;

    // zhang x;
    // string xjson = "{\"age\":99,\"a\":[\"张旭\",\"是的萨\"],\"b\":{\"4\":44,\"5\":55},\"c\":[\"你好不好\",\"你好不好\"],\"d\":[\"你好不好1\",\"你好不好2\"],\"e\":[\"你好不好2\",\"你好不好1\"],\"f\":[23,24]}";
    // cout << xjson << endl;
    // Fdog::FObject(x, xjson);
    // cout << "z.age ：" << x.age << endl;
    // cout << "z.a ：" << x.a[0] << endl;
    // cout << "z.a ：" << x.a[1] << endl;
    // cout << "z.b ：" << x.b[0] << endl;
    // cout << "z.b ：" << x.b[1] << endl;
    // for( auto s1 : x.c){
    //     cout << "z.c ：" << s1 << endl;
    // }
    // cout << "z.d ：" << x.d[0] << endl;
    // cout << "z.d ：" << x.d[1] << endl;

    // for( auto s1 : x.e){
    //     cout << "z.e ：" << s1 << endl;
    // }
    // cout << "z.age ：" << x.f[0] << endl;
    // cout << "z.age ：" << x.f[1] << endl;
    // //**********************************************************************
    // cout << "基础类型序列化==================================================" << endl;
    // int base_a = 10;
    // string base_ajson;

    // //基础类型序列化为json
    // Fdog::FJson(base_ajson, base_a, "base_a");
    // cout << base_ajson << endl;
    // base_ajson = "{\"base_a\":20}";
    // //将json类型转为基础类型
    // Fdog::FObject(base_a, base_ajson, "base_a");
    // cout << base_a << endl;
    // //**********************************************************************

    // //**********************************************************************
    // cout << "基础类型数组序列化==============================================" << endl;
    // float base_array[5] = {1.2, 3.4, 5.6, 4.3, 5.6};
    // string base_arrayjson;
    // Fdog::FJson(base_arrayjson, base_array, "base_array");
    // cout << base_arrayjson << endl;
    // base_arrayjson = "{\"base_array\":[2.2, 3.6, 7.6, 2.3, 6.8]}";
    // Fdog::FObject(base_array, base_arrayjson);
    // cout << base_array[0] << "," << base_array[1] << "," << base_array[2] << "," << base_array[3] << "," << base_array[4] << endl;
    // //**********************************************************************

    //**********************************************************************
    // cout << "结构体类型序列化================================================" << endl;
    // struct student stu; //该结构体定义在text.h
    // stu.name = "zhangxv";
    // stu.age = 22;
    // REGISTEREDMEMBER(student, name, age); //注册成员
    // string stu_json;
    // Fdog::FJson(stu_json, stu);
    // cout << stu_json << endl;
    // stu_json = "{\"Name\":\"yujing\", \"AGE\":21}";
    // FdogSerialize::Instance()->setIgnoreLU("student", "name");
    // FdogSerialize::Instance()->setIgnoreLU("student", "age");
    // Fdog::FObject(stu, stu_json);
    // cout << stu.name << "," << stu.age << endl;
    //**********************************************************************

    // //**********************************************************************
    // cout << "包含结构体成员的结构体序列化=====================================" << endl;
    // school sch;
    // sch.id = 1;
    // sch.stu.name = "zhangxv";
    // sch.stu.age = 22;
    // //REGISTEREDMEMBER(student, name, age)注册成员 上面已经注册
    // REGISTEREDMEMBER(school, id, stu); //注册成员
    // //在behavior.h中的两个宏定义中添加student，因为student结构体是作为成员出现的
    // string sch_json;
    // Fdog::FJson(sch_json, sch);
    // cout << sch_json << endl;
    // sch_json = "{\"id\":3, \"stu\":{\"name\":\"yujing\", \"age\":22}}";
    // Fdog::FObject(sch, sch_json);
    // cout << sch.id << "," << sch.stu.name << "," << sch.stu.age << endl;
    // //**********************************************************************

    // //**********************************************************************
    // cout << "vector<基础类型>序列化==========================================" << endl;
    // vector<int> av;
    // av.push_back(20);
    // av.push_back(10);
    // av.push_back(5);
    // string av_json;
    // Fdog::FJson(av_json, av, "av");
    // cout << av_json << endl;
    // av_json = "{\"av\":[30, 20, 10]}";
    // Fdog::FObject(av, av_json);
    // cout << av[0] << "," << av[1] << "," << av[2] << endl;
    // //**********************************************************************

    // //**********************************************************************
    // cout << "vector<结构体>序列化,list,set类型用法一致=======================" << endl;
    // vector<student> stuv;
    // student stu_1 = {"zhangxv", 21};
    // student stu_2 = {"yujing", 21};
    // student stu_3 = {"lisi", 21};
    // stuv.push_back(stu_1);
    // stuv.push_back(stu_2);
    // stuv.push_back(stu_3);
    // string stuv_json;
    // Fdog::FJsonA(stuv_json, stuv, "stuv");
    // cout << stuv_json << endl;
    // av_json = "{\"stuv\":[{\"name\":\"zhangsan\", \"age\":32},{\"name\":\"anjing\", \"age\":12},{\"name\":\"lifei\", \"age\":20}]}";
    // Fdog::FObjectA(stuv, stuv_json);
    // cout << stuv[0].name << "," << stuv[0].age << "," << stuv[1].name << "," << stuv[1].age << "," << stuv[2].name << "," << stuv[2].age << endl;
    // //**********************************************************************

    // //**********************************************************************
    // cout << "map<基础类型,结构体>序列化======================================" << endl;
    // map<int, student> stum;
    // stum[0] = {"zhangxv", 21};
    // stum[1] = {"yujing", 21};
    // stum[2] = {"lisi", 21};
    // string stum_json;
    // Fdog::FJsonM(stum_json, stum);
    // cout << stum_json << endl;
    // stum_json = "{\"0\":{\"name\":\"huhansan\",\"age\":21},\"1\":{\"name\":\"lifei\",\"age\":11},\"2\":{\"name\":\"huruiming\",\"age\":20}}";
    // Fdog::FObjectM(stum, stum_json);
    // int len = stum.size();
    // for(int i = 0; i < len; i++){
    //     cout << stum[i].name << "," << stum[i].age << endl;
    // }
    // //**********************************************************************    
    // cout << "===============================================================" << endl;
    return 0;
}

