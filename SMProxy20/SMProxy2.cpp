// SMProxy20.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "SMProxy2.h"
#include <string.h>
#include <vector>
#include <string>
#include <iostream>

//Proxy* getInstance()
//{
//	Proxy *p = new Proxy();
//	return p;
//}



int Sum(int a, int b)
{
	return a + b;
}

char* Hello(int size, char result[500])
{

	strcpy_s(result, size, "hello world");
	return result;
}

unsigned int submit(char buf[3000], char err_str[500], Protocol protocol,
	//发送标手机号数量
	unsigned int dest_term_ID_count,
	//发送手机号
	char user_num[][21],
	//短信长度 不能超过140字节
	unsigned int msg_length,
	//短信内容
	char* msg
)
{
	std::vector<std::string> phone;
	for (size_t i = 0; i < dest_term_ID_count; i++)
	{
		phone.push_back(user_num[i]);
	}
}