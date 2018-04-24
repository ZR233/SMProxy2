#pragma once
/*************************************************
Author:zr
Date:2018-01-16
Description:CMPP connect处理
**************************************************/
#include "IBind.hpp"
#include "Utils.hpp"
#include "MD5.h"
#include "Field.hpp"

namespace smproxy {


	//CMPP connect处理
	class SMGP303Login :
		public IBind
	{
	private:
		Buffer body_;
		/*用于鉴别源地址。其值通过单向MD5 hash计算得出，表示如下：
		AuthenticatorSource = MD5（Source_Addr + 7 字节的0 + shared	secret + timestamp）
		Shared secret 由中国电信与源地址实体事先商定，
		timestamp格式为：MMDDhhmmss，即月日时分秒，10位。*/
		FieldBytes authenticator_source_;
		std::string md5_s2_;
	public:
		SMGP303Login()
		{
			password_.setSize(34);
			body_.pushField(user_name_.set(8, ""));
			body_.pushField(authenticator_source_.set(16, '\0'));
			/*
			0＝发送短消息（send mode）
			1＝接收短消息（receive mode）
			2＝收发短消息（transmit mode）
			*/
			body_.pushField(login_type_.set(1, 2));
			body_.pushField(timestamp_.set(4, 0));
			body_.pushField(version_.set(1, 0x33));

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
			user_name_= sp_id;


			std::string md5_s = sp_id;
			for (size_t i = 0; i < 7; i++)
			{
				md5_s.push_back(0);
			}
			md5_s += shared_secret;
			md5_s += time_stamp;

			unsigned char c[100];

			for (size_t i = 0; i < md5_s.size(); i++)
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
			authenticator_source_ = source_v;

			//存timestamp

			timestamp_ = (unsigned int)std::stoul(time_stamp);
			buf_ = body_.to_bytes();
			return buf_;
		}

		// 解析connect
		void recvBind(bytes& buf, bytes::iterator buf_loc) override {
			if (buf.size() < 42)
			{
				throw smproxy::exception("connect字符串长度错误", 1);
			}
			body_.reslov(buf, buf_loc);
			std::string temp_pass;
			//存 AS  将密码存为16进制字符串
			auto authenticator_source = authenticator_source_.get();
			for (size_t i = 0; i < 16; i++)
			{
				uint8_t a = authenticator_source[i];
				auto b = uint32_t(a);
				char str[8] = { 0 };
				sprintf_s(str, "%x", b);
				std::string c(str);
				if (c.size() < 2)
				{
					c.insert(c.begin(), '0');
				}
				temp_pass += c;
			}
			password_ = temp_pass;

		}
	private:


	};


}//smproxy