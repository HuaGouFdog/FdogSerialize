FdogStructJson，一个用于C++的序列化和反序列化的库



## 更新

2021年11月20日 23:09:58 更新

1. 完成简单的struct和json的互转(只能转int)


2021年11月22日00:05:22

1. 修改了某些上头的代码，
2. 支持int, float, double的互转，下一次更新应该会全部支持



struct base {

​	int a;

​	int b;

}



struct apple {

​	int c;

base d;

}

注册函数 base a,b

注册函数 apple c d

FdogJsonToStrutc



第一波进去之后，查找类型，如果不在基础类型里面，说明就在自定义类型类面

递归调用函数，实现赋值，



这里应该分类

FdogJsonToBase

FdogBaseToJson

一个基础类型解析函数

FdogJsonToObject

FdogObjectToJson

一个对象解析函数

FdogJsonToArray

FdogArrayToJson

一个数组解析函数

江苏省hao