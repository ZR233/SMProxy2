#pragma once
#include "IResp.hpp"
#include <vector>
#include "MD5.h"
#include "exception.hpp"
#include "CMPP20Head.hpp"

/*************************************************
Author:zr
Date:2018-03-10
Description:CMPP2.0 connect resp����
**************************************************/

//
class CMPP20ConnectResp:
	public IResp
{
public:
	CMPP20ConnectResp():IResp(0x80000001) {
		setHeadType(&hd_);
	}
	virtual ~CMPP20ConnectResp() {}
	//��Ҫ��set:
	//authenticator_source Դ��ַʵ�巢�͸�ISMG�Ķ�Ӧ��ϢCMPP_Connect�е�ֵ��
	//SharedSecret ���й��ƶ���Դ��ַʵ�������̶�
	void resp(uint8_t result) override
	{
		char a = (uint8_t)result;
		buf_.clear();
		buf_.push_back(a);

		unsigned char source[100];
		source[0] = result;
		for (size_t i = 0; i < 16; i++)
		{
			const char *char_ptr;
			std::string a = "0x";
			a += authenticator_source_[2 * i];
			a += authenticator_source_[2 * i + 1];
			char_ptr = a.c_str();
			source[i + 1] = (unsigned char)strtol(char_ptr, NULL, 16);
		}

		const char *temp_char = shared_secret_.c_str();
		memcpy_s(&source[17], 80, temp_char, shared_secret_.size());

		MD5 md5;
		std::string temp = md5.md5sum(&source, 17 + shared_secret_.size());
		std::vector<uint8_t> source_v;
		for (size_t i = 0; i < 16; i++)
		{
			std::string a = "0x";
			a += temp[2 * i];
			a += temp[2 * i + 1];
			const char *char_ptr = a.c_str();
			source_v.push_back((unsigned char)strtol(char_ptr, NULL, 16));
		}
		buf_.insert(buf_.end(), source_v.begin(), source_v.begin() + 16);
		buf_.push_back(32);
	}
	uint8_t recvResp(std::vector<uint8_t> buf) override
	{
		throw smproxy::exception("recvResp��δʵ��");
	}
protected:
	CMPP20Head hd_;
};