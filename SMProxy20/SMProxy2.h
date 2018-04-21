﻿#pragma once
#ifndef SMPROXY2DLL_H_
#define SMPROXY2DLL_H_

#ifdef SMPROXY2DLL
#define SMPROXY2DLL extern "C" _declspec(dllimport) 
#else
#define SMPROXY2DLL extern "C" _declspec(dllexport) 
#endif

#include "Defines.h"


SMPROXY2DLL int Sum(int a, int b);

SMPROXY2DLL char* Hello(int size,char result[500]);
//SMPROXY2DLL int deliver(char* err_str);
SMPROXY2DLL unsigned int submit(char buf[3000], char err_str[500], Protocol protocol,
	//发送标手机号数量
	unsigned int dest_term_ID_count,
	//发送手机号
	char user_num[][21], 
	//短信长度 不能超过140字节
	unsigned int msg_length,
	//短信内容
	char* msg
	);
#endif  //#ifndef SMPROXY2DLL_H_