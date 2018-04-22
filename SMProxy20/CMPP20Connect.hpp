#pragma once
/*************************************************
Author:zr
Date:2018-01-16
Description:CMPP connect处理
**************************************************/
#include "IBind.hpp"
#include <boost/date_time/gregorian/conversion.hpp>
#include <boost/date_time/posix_time/conversion.hpp>
#include "MD5.h"
#include "Utils.hpp"


namespace smproxy {
	//CMPP connect处理
	class CMPP20Connect :
		public IBind
	{
	private:
		Buffer body_;
	public:
		CMPP20Connect(){
			//source_addr = bytes(6, 0);
			//2.0
			password_.setSize(34);
			body_.pushField(sp_ID_.set(6, "0"));
			body_.pushField(authenticator_source_.set(16, '\0'));
			body_.pushField(version_.set(1, 0x20));
			body_.pushField(timestamp_.set(4, 0));
		}
		virtual ~CMPP20Connect() {};
		//获得时间字符串
		std::string getTimeStr()
		{
			auto timeTemp = boost::posix_time::second_clock::local_time();
			auto td = timeTemp.time_of_day();
			auto date = timeTemp.date();
			using std::string;
			using std::to_string;

			tm t = to_tm(date);
			tm t2 = to_tm(td);

			//string YY = std::to_string(t.tm_year + 1900);
			//YY.erase(0, 2);
			string MM = to_string(t.tm_mon + 1);
			if (MM.size() == 1)
			{
				MM.insert(MM.begin(), '0');
			}
			string DD = to_string(t.tm_mday);
			if (DD.size() == 1)
			{
				DD.insert(DD.begin(), '0');
			}
			string HH = to_string(t2.tm_hour);
			if (HH.size() == 1)
			{
				HH.insert(HH.begin(), '0');
			}
			string MI = to_string(t2.tm_min);
			if (MI.size() == 1)
			{
				MI.insert(MI.begin(), '0');
			}
			auto SS = to_string(t2.tm_sec);
			if (SS.size() == 1)
			{
				SS.insert(SS.begin(), '0');
			}
			string str = MM + DD + HH + MI + SS;
			return str;
		}
		// 生成connect
		// @sp_id:			用户名
		// @shard_secret:	密码
		bytes Binder(std::string sp_id, std::string shared_secret) override
		{
			if (sp_id.size() > 6)
			{
				BOOST_THROW_EXCEPTION(exception("用户名超过6位",1));
			}
			//获取时间
			const auto time_stamp = Utils::getTimeStr(3);

			bytes temp;
			//存SP ID
			sp_ID_ = sp_id;

			std::string md5_s = sp_id;
			for (size_t i = 0; i < 9; i++)
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
			if (buf.size() < 39)
			{
				BOOST_THROW_EXCEPTION(exception("connect字符串长度错误", 1));
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


		//源地址，此处为SP_Id，即SP的企业代码。
		//std::string getName() { 
		//	std::string name;
		//	name.assign(source_addr.begin(), source_addr.end());
		//	name = name.c_str();
		//	return name;
		//};

		/*用于鉴别源地址。其值通过单向MD5 hash计算得出，表示如下：
		AuthenticatorSource = MD5（Source_Addr + 9 字节的0 + shared	secret + timestamp）
		Shared secret 由中国移动与源地址实体事先商定，
		timestamp格式为：MMDDHHMMSS，即月日时分秒，10位。*/
		//std::string& getPass() { 
		//	return pass;
		//};





	private:
		//bytes source_addr;//源地址，此处为SP_Id，即SP的企业代码。
		//std::string pass;
		/*用于鉴别源地址。其值通过单向MD5 hash计算得出，表示如下：
		AuthenticatorSource = MD5（Source_Addr + 9 字节的0 + shared	secret + timestamp）
		Shared secret 由中国移动与源地址实体事先商定，
		timestamp格式为：MMDDHHMMSS，即月日时分秒，10位。*/
		FieldBytes authenticator_source_;

		std::string md5_s2_;

	};
}

