#include <iostream>
#include "../FStruct.h"
#include <typeinfo>
#include <iostream>
#include <list>
using namespace std;

//g++ -std=c++11 ../FStruct.cpp example.cpp -o main -w

int main()
{
    //Fdog::FJson();
    //Fdog::FObject();
    //Fdog::Valid(); //json是否有效
    //Fdog::Exists(); //字段是否存在
    //Fdog::GetValue(); //获取某个值段的值
    //当指针类型的值为nuller时，则认为当前字段是可选字段，并且没有传值，默认不序列化

    // vector<int> a;
    // a.push_back(10);
    // a.push_back(20);
    // string json_1;
    // Fdog::FJson(json_1, a, "a");
    // cout << json_1 << endl;

    // int b = 30;
    // string json_2;
    // Fdog::FJson(json_2, b, "b");
    // cout << json_2 << endl;

    // map<int,int> c;
    // c[1]=11;
    // c[2]=22;
    // string json_3;
    // Fdog::FJson(json_3, c, "c");
    // cout << json_3 << endl;

    zhang z;
    z.age = 10;
    z.a.push_back(0);
    z.a.push_back(1);
    z.b[1]=10;
    z.b[2]=20;
    z.c.push_back(0);
    z.c.push_back(1);
    z.d.push_back(0);
    z.d.push_back(1);
    z.e.insert(0);
    z.e.insert(1);
    REGISTEREDMEMBER(zhang, age, a, b, c, d, e); //注册成员
    string zjson;
    Fdog::FJson(zjson, z);
    cout << zjson << endl;

    zhang x;
    string xjson = "{\"age\":99,\"a\":[22,33],\"b\":{\"4\":44,\"5\":55},\"c\":[66,77],\"d\":[88,99],\"e\":[100,110]}";
    cout << xjson << endl;
    Fdog::FObject(x, xjson);
    cout << "z.age ：" << x.age << endl;
    cout << "z.a ：" << x.a[0] << endl;
    cout << "z.a ：" << x.a[1] << endl;
    cout << "z.b ：" << x.b[0] << endl;
    cout << "z.b ：" << x.b[1] << endl;
    for( auto s1 : x.c){
        cout << "z.c ：" << s1 << endl;
    }
    cout << "z.d ：" << x.d[0] << endl;
    cout << "z.d ：" << x.d[1] << endl;

    for( auto s1 : x.e){
        cout << "z.e ：" << s1 << endl;
    }
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
    // Fdog::FJsonA(base_arrayjson, base_array, "base_array");
    // cout << base_arrayjson << endl;
    // base_arrayjson = "{\"base_array\":[2.2, 3.6, 7.6, 2.3, 6.8]}";
    // Fdog::FObjectA(base_array, base_arrayjson);
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
    // Fdog::FJsonA(av_json, av, "av");
    // cout << av_json << endl;
    // av_json = "{\"av\":[30, 20, 10]}";
    // Fdog::FObjectA(av, av_json);
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

