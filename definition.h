/*
����Ŀǩ����Apache-2.0 License��������μ�LICENSE
���� Apache ��ɣ��汾 2.0������ɡ���������
����������ɣ�����������ʹ�ô��ļ���

Copyright 2021-2022 ����Fdog(����)
*/

#ifndef BEHAVIOR_H
#define BEHAVIOR_H
#include "define.h"
#include "testType.h"
/******�ڴ���ӽṹ����������ͷ�ļ�****/
//#include "example/testType.h"
/*********************************/


//�ṹ���а����Զ���ṹ���ڴ����
//�������л�
#define Serialize_type_judgment_all\
    Serialize_type_judgment(student)\
    Serialize_type_judgment(teacher)\
    Serialize_type_judgment(classtest)\
//���ڷ����л�
#define Deserialize_type_judgment_all\
    Deserialize_type_judgment(student)\
    Deserialize_type_judgment(teacher)\
    Deserialize_type_judgment(classtest)\


//�ṹ���а����Զ������͵������ڴ������
//�������л�
#define Serialize_arraytype_judgment_all\
    Serialize_arraytype_judgment(student)\
//���ڷ����л�
#define Deserialize_arraytype_judgment_all\
    Deserialize_arraytype_judgment(student)\



//�ṹ���а����Զ�������STL�����ڴ����

//�������л�
#define Serialize_vector_type_judgment_all\
    Serialize_vector_type_judgment(student)\

#define Serialize_list_type_judgment_all\
    Serialize_list_type_judgment(student)\

#define Serialize_deque_type_judgment_all\
    Serialize_deque_type_judgment(student)\

#define Serialize_set_type_judgment_all\
    Serialize_set_type_judgment(student)\

#define Serialize_map_type_judgment_all\
    //Serialize_map_type_judgment(string,student)

//���ڷ����л�
#define Deserialize_vector_type_judgment_all\
    Deserialize_vector_type_judgment(student)\

#define Deserialize_list_type_judgment_all\
    Deserialize_list_type_judgment(student)\

#define Deserialize_deque_type_judgment_all\
    Deserialize_deque_type_judgment(student)\

#define Deserialize_set_type_judgment_all Deserialize_set_type_judgment(student)\

#define Deserialize_map_type_judgment_all //Deserialize_map_type_judgment(string,student)

#endif