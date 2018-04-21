#pragma once
/*************************************************
Author:zr
Date:2018-01-16
Description:CMPP connect处理
**************************************************/
#include "IBind.hpp"
#include "SMGP303Head.hpp"
#include "Utils.hpp"
#include "MD5.h"
#include "Field.hpp"
//CMPP connect处理
class SMGP303Login :
	public IBind
{
public:
	SMGP303Login() :IBind(0x00000001),
		ClientID_(8),
		AuthenticatorClient_(16),
		LoginMode_(1),
		TimeStamp_(4),
		ClientVersion_(1)
	{
		setHeadType(&hd_);
		authenticator_source = bytes(16, 0);
		//3.03
		version_ = 0x33;
		ClientVersion_.set(0x33);
		LoginMode_.set(2);
		timestamp_ = 0;
	}
	virtual ~SMGP303Login() {};
	// 生成connect
	// @sp_id:			用户名
	// @shard_secret:	密码
	bytes Binder(std::string sp_id, std::string shared_secret) override
	{
		//获取时间
		const auto time_stamp = Utils::getTimeStr();

		buf_.clear();
		bytes temp;
		//存SP ID
		ClientID_.set(sp_id);
		temp = ClientID_.toBytes();
		buf_.insert(buf_.end(), temp.begin(), temp.end());

		std::string md5_s = sp_id;
		for (size_t i = 0; i < 7; i++)
		{
			md5_s.push_back(0);
		}
		md5_s += shared_secret;
		md5_s += time_stamp;

		unsigned char c[100];

		for (size_t i = 0; i <md5_s.size(); i++)
		{
			c[i] = md5_s[i];
		}
		MD5 md5;
		md5_s2_ = md5.md5sum(&c, md5_s.size());
		std::vector<uint8_t> source_v;
		for (size_t i = 0; i < 16; i++)
		{
			std::string a = "0x";
			a += md5_s2_[2 * i];
			a += md5_s2_[2 * i + 1];
			const char *char_ptr = a.c_str();
			source_v.push_back((unsigned char)strtol(char_ptr, NULL, 16));
		}
		buf_.insert(buf_.end(), source_v.begin(), source_v.end());
		
		temp = LoginMode_.toBytes();
		buf_.insert(buf_.end(), temp.begin(), temp.end());
		//存timestamp

		timestamp_ = (unsigned int)std::stoul(time_stamp);
		timestamp_ = htonl(timestamp_);
		std::vector<unsigned char> temp_char(4, 0);
		memcpy(&temp_char[0], &timestamp_, 4);
		buf_.insert(buf_.end(), temp_char.begin(), temp_char.end());
		//存Version
		buf_.push_back(version_);
		return buf_;
	}

	// 解析connect
	void recvBind(bytes& buf) override {
		if (buf.size()< 42)
		{
			throw smproxy::exception("connect字符串长度错误", 1);
		}
		auto iter = buf.begin();
		iter += 12;
		//存SP ID
		ClientID_.read(iter);
		log_name_ = ClientID_.getStr();

		//存 AS  将密码存为16进制字符串
		authenticator_source.assign(iter, iter + 16);
		log_pass_.clear();
		for (size_t i = 0; i < 16; i++)
		{
			uint8_t a = authenticator_source[i];
			auto b = uint32_t(a);
			char str[8] = { 0 };
			sprintf_s(str, "%x", b);
			std::string c(str);
			if (c.size()<2)
			{
				c.insert(c.begin(), '0');
			}
			log_pass_ += c;
		}

		iter += 16;

		//存Loginmo
		LoginMode_.read(iter);
		//存timestamp
		bytes temp_time;
		temp_time.assign(iter, iter + 4);
		memcpy(&timestamp_, &(*iter), 4);
		timestamp_ = ntohl(timestamp_);
		iter += 4;
		//存Version
		ClientVersion_.read(iter);
		version_ = ClientVersion_.getInt();

	}
private:
	/*用于鉴别源地址。其值通过单向MD5 hash计算得出，表示如下：
	AuthenticatorSource = MD5（Source_Addr + 7 字节的0 + shared	secret + timestamp）
	Shared secret 由中国电信与源地址实体事先商定，
	timestamp格式为：MMDDhhmmss，即月日时分秒，10位。*/
	bytes authenticator_source;

	std::string md5_s2_;

	SMGP303Head hd_;

	smproxy::FieldStr ClientID_;
	smproxy::FieldStr AuthenticatorClient_;

	smproxy::FieldInt LoginMode_;
	smproxy::FieldInt TimeStamp_;
	smproxy::FieldInt ClientVersion_;
};


