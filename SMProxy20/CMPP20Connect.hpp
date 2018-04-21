#pragma once
/*************************************************
Author:zr
Date:2018-01-16
Description:CMPP connect����
**************************************************/
#include "IBind.hpp"
#include "CMPP20Head.hpp"
#include <boost/date_time/gregorian/conversion.hpp>
#include <boost/date_time/posix_time/conversion.hpp>
#include "MD5.h"

//CMPP connect����
class CMPP20Connect :
	public IBind
{
public:
	CMPP20Connect() :IBind(1) {
		setHeadType(&hd_);

		//source_addr = bytes(6, 0);
		authenticator_source = bytes(16, 0);
		//2.0
		version_ = 32;
		timestamp_ = 0;

	}
	virtual ~CMPP20Connect(){};
	//���ʱ���ַ���
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
	// ����connect
	// @sp_id:			�û���
	// @shard_secret:	����
	bytes Binder(std::string sp_id, std::string shared_secret) override
	{
		if (sp_id.size()>6)
		{
			throw smproxy::exception("�û�������6λ");
		}
		//��ȡʱ��
		const auto time_stamp = getTimeStr();

		buf_.clear();
		bytes temp;
		//��SP ID
		temp.assign(sp_id.begin(), sp_id.end());
		temp.resize(6);
		buf_.insert(buf_.end(), temp.begin(), temp.end());

		std::string md5_s = sp_id;
		for (size_t i = 0; i < 9; i++)
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

		//��Version
		buf_.push_back(version_);

		//��timestamp

		timestamp_ = (unsigned int)std::stoul(time_stamp);
		timestamp_ = htonl(timestamp_);
		std::vector<unsigned char> temp_char(4, 0);
		memcpy(&temp_char[0], &timestamp_, 4);
		buf_.insert(buf_.end(), temp_char.begin(), temp_char.end());

		return buf_;
	}

	// ����connect
	void recvBind(bytes& buf) override {
		if (buf.size()< 39)
		{
			throw smproxy::exception("connect�ַ������ȴ���", 1);
		}
		auto iter = buf.begin();
		iter += 12;
		//��SP ID
		log_name_.assign(iter, iter + 6);
		log_name_ = log_name_.c_str();
		iter += 6;


		//�� AS  �������Ϊ16�����ַ���
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


		//��Version
		version_ = *iter;
		++iter;
		//��timestamp
		bytes temp_time;
		temp_time.assign(iter, iter + 4);
		memcpy(&timestamp_, &(*iter), 4);
		timestamp_ = ntohl(timestamp_);
	}


	//Դ��ַ���˴�ΪSP_Id����SP����ҵ���롣
	//std::string getName() { 
	//	std::string name;
	//	name.assign(source_addr.begin(), source_addr.end());
	//	name = name.c_str();
	//	return name;
	//};

	/*���ڼ���Դ��ַ����ֵͨ������MD5 hash����ó�����ʾ���£�
	AuthenticatorSource = MD5��Source_Addr + 9 �ֽڵ�0 + shared	secret + timestamp��
	Shared secret ���й��ƶ���Դ��ַʵ�������̶���
	timestamp��ʽΪ��MMDDHHMMSS��������ʱ���룬10λ��*/
	//std::string& getPass() { 
	//	return pass;
	//};





private:
	//bytes source_addr;//Դ��ַ���˴�ΪSP_Id����SP����ҵ���롣
	//std::string pass;
	/*���ڼ���Դ��ַ����ֵͨ������MD5 hash����ó�����ʾ���£�
	AuthenticatorSource = MD5��Source_Addr + 9 �ֽڵ�0 + shared	secret + timestamp��
	Shared secret ���й��ƶ���Դ��ַʵ�������̶���
	timestamp��ʽΪ��MMDDHHMMSS��������ʱ���룬10λ��*/
	bytes authenticator_source;

	std::string md5_s2_;

	CMPP20Head hd_;
};


