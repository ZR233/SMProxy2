#pragma once
/*************************************************
Author:zr
Date:2018-04-11
Description:SMGP 3.0.3 Head
**************************************************/


#include "IHead.hpp"
#include "boost/asio.hpp"
#include "exception.hpp"


class SMGP303Head :
	public IHead
{
public:
	typedef std::vector<uint8_t> bytes;
	SMGP303Head() {};
	virtual ~SMGP303Head() {};
	virtual std::string createSerualNum(uint32_t& serial_numb) override {
		if (serial_numb > 999998)
			serial_numb = 0;
		else
			++serial_numb;
		this->serial_numb = std::to_string(serial_numb);
		int_serial_numb = serial_numb;
		return this->serial_numb;
	};

	bytes& header(uint32_t total_len, uint32_t cmdID, std::string serial_numb) override
	{
		this->serial_numb = serial_numb;
		int_serial_numb = uint32_t(stoul(serial_numb));
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
		//写入 serial_numb
		temp.assign(4, 0);
		int_serial_numb = htonl(int_serial_numb);
		memcpy_s(&(temp[0]), 4, &(int_serial_numb), 4);
		buf.insert(buf.end(), temp.begin(), temp.end());
		return buf;
	};

	void recvHead(const bytes buf) override {
		if (buf.size() < 12)
		{
			BOOST_THROW_EXCEPTION( smproxy::exception("消息字符串过短"));
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
		if (total_len > 2000 || total_len < 12)
		{
			BOOST_THROW_EXCEPTION(smproxy::exception("消息总长度错误"));
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
		//拷贝 int_serial_numb
		for (int i = 0; i < 4; i++)
		{
			temp[i] = buf[i + pt];
		}
		memcpy(&int_serial_numb, temp, 4);
		int_serial_numb = ntohl(int_serial_numb);
		pt += 4;
		serial_numb = std::to_string(int_serial_numb);
	};
};

