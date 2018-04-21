#pragma once
/*************************************************
Author:zr
Date:2018-02-9
Description:SGIP1.2 head
**************************************************/


#include "IHead.hpp"
#include "boost/date_time/posix_time/ptime.hpp"
#include "boost/date_time/gregorian/conversion.hpp"
#include "boost/date_time/posix_time/conversion.hpp"
#include "boost/asio.hpp"
#include "exception.hpp"


class SGIP12Head :
	public IHead
{
public:
	typedef std::vector<uint8_t> bytes;
	SGIP12Head(){};
	virtual std::string createSerualNum(uint32_t& serial_numb) override {
		if (serial_numb >99999)
		{
			serial_numb = 0;
		}
		++serial_numb;
		boost::posix_time::ptime time(boost::posix_time::second_clock::local_time());
		tm date_ = boost::gregorian::to_tm(boost::gregorian::date(time.date()));
		tm time_ = boost::posix_time::to_tm(boost::posix_time::time_duration(time.time_of_day()));
		SN2 = (date_.tm_mon + 1) * 100000000 + date_.tm_mday * 1000000 + time_.tm_hour * 10000 + time_.tm_min * 100 + time_.tm_sec;

		this->serial_numb = SPID;

		this->serial_numb += "|";
		this->serial_numb += std::to_string(SN2);
		this->serial_numb += "|";
		this->serial_numb += std::to_string(serial_numb);
		int_serial_numb = serial_numb;
		return this->serial_numb;
	};

	bytes& header(uint32_t total_len, uint32_t cmdID, std::string serial_numb) override
	{
		this->serial_numb = serial_numb;

		stringID2IntID(serial_numb, SN1, SN2, int_serial_numb);


		bytes temp(4, 0);
		//写入总长度
		buf.clear();
		this->total_len = htonl(total_len);
		memcpy_s(&(temp[0]), 4, &(this->total_len), 4);
		buf.assign(temp.begin(), temp.end());
		//写入 指令ID
		temp.assign(4, 0);
		this->cmdID = htonl(cmdID);
		memcpy_s(&(temp[0]), 4, &(this->cmdID), 4);
		buf.insert(buf.end(), temp.begin(), temp.end());
		//SN1
		temp.assign(4, 0);
		this->SN1 = htonl(SN1);
		memcpy_s(&(temp[0]), 4, &(this->SN1), 4);
		buf.insert(buf.end(), temp.begin(), temp.end());
		//SN2
		temp.assign(4, 0);
		this->SN2 = htonl(SN2);
		memcpy_s(&(temp[0]), 4, &(this->SN2), 4);
		buf.insert(buf.end(), temp.begin(), temp.end());
		//SN3
		temp.assign(4, 0);
		this->int_serial_numb = htonl(int_serial_numb);
		memcpy_s(&(temp[0]), 4, &(this->int_serial_numb), 4);
		buf.insert(buf.end(), temp.begin(), temp.end());
		return buf;
	};

	void recvHead(const bytes buf) override {
		if (buf.size() < 20)
		{
			throw smproxy::exception("消息字符串过短");
		}
		char temp[4];
		int pt = 0;
		//拷贝消息长度
		for (int i = 0; i < 4; i++)
		{
			temp[i] = buf[i];
		}
		memcpy(&total_len, temp, 4);
		total_len = ntohl(total_len);
		if (total_len > 2000 || total_len < 20)
		{
			throw smproxy::exception("消息总长度错误");
		}
		pt += 4;
		//拷贝指令ID
		for (int i = 0; i < 4; i++)
		{
			temp[i] = buf[i + pt];
		}
		memcpy(&cmdID, temp, 4);
		cmdID = ntohl(cmdID);
		pt += 4;
		//拷贝SN1
		for (int i = 0; i < 4; i++)
		{
			temp[i] = buf[i + pt];
		}
		memcpy(&SN1, temp, 4);
		SN1 = ntohl(SN1);
		pt += 4;
		//拷贝SN2
		for (int i = 0; i < 4; i++)
		{
			temp[i] = buf[i + pt];
		}
		memcpy(&SN2, temp, 4);
		SN2 = ntohl(SN2);
		pt += 4;
		//拷贝SN3
		for (auto i = 0; i < 4; i++)
		{
			temp[i] = buf[i + pt];
		}
		memcpy(&int_serial_numb, temp, 4);
		int_serial_numb = ntohl(int_serial_numb);
		SPID = std::to_string(SN1);
		serial_numb = SPID + "|" + std::to_string(SN2) + "|" + std::to_string(int_serial_numb);
	};


	virtual ~SGIP12Head(){};
	virtual void stringID2IntID(std::string serial_numb, uint32_t& SN1, uint32_t& SN2, uint32_t& SN3)
	{
		if ("" == serial_numb)
		{
			throw smproxy::exception("流水号为空");
			return;
		}


		size_t pos = serial_numb.find("|");
		size_t size = serial_numb.size();
		if (pos == 0)
			throw smproxy::exception("sn1为空");
		SN1 = std::stoul(serial_numb.substr(0, pos));
		serial_numb.erase(0, pos + 1);
		pos = serial_numb.find("|");
		if (pos == 0)
			throw smproxy::exception("sn2为空");
		SN2 = std::stoul(serial_numb.substr(0, pos));
		serial_numb.erase(0, pos + 1);
		if (serial_numb.size() == 0)
			throw smproxy::exception("sn3为空");
		SN3 = std::stoul(serial_numb);
	};
protected:
	uint32_t SN1;
};

