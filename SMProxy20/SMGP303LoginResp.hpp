#pragma once
#include "IResp.hpp"
#include <vector>
#include "MD5.h"
#include "exception.hpp"
#include "SMGP303Head.hpp"

/*************************************************
Author:zr
Date:2018-04-12
Description:SMGP3.03 connect resp处理
**************************************************/

//
class SMGP303LoginResp :
	public IResp
{
public:
	SMGP303LoginResp() :IResp(0x80000001) {
		setHeadType(&hd_);
	}
	virtual ~SMGP303LoginResp() {}
	//需要先set:
	//authenticator_source 源地址实体发送给ISMG的对应消息CMPP_Connect中的值。
	//SharedSecret 由中国移动与源地址实体事先商定
	void resp(uint8_t result) override
	{
		uint32_t a = uint32_t(result);
		a = htonl(a);
		buf_.assign(4,0);
		memcpy_s(&(buf_[0]), 4, &a, 4);

		unsigned char source[100];
		memcpy_s(&(source[0]), 4, &a, 4);
		for (size_t i = 0; i < 16; i++)
		{
			const char *char_ptr;
			std::string temp_a = "0x";
			temp_a += authenticator_source_[2 * i];
			temp_a += authenticator_source_[2 * i + 1];
			char_ptr = temp_a.c_str();
			source[i + 4] = (unsigned char)strtol(char_ptr, NULL, 16);
		}

		const char *temp_char = shared_secret_.c_str();
		memcpy_s(&source[20], 80, temp_char, shared_secret_.size());

		MD5 md5;
		std::string temp = md5.md5sum(&source, 20 + shared_secret_.size());
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
		buf_.push_back(0x33);
	}
	uint8_t recvResp(std::vector<uint8_t> buf) override
	{
		throw smproxy::exception("recvResp还未实现");
	}
protected:
	SMGP303Head hd_;
};