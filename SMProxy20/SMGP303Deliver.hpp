#pragma once
/*************************************************
Author:zr
Date:2018-04-12
Description:SMGP3.03 Deliver处理
**************************************************/
#include "IReport.hpp"
#include "IDeliver.hpp"
#include "SMGP303Head.hpp"
#include <boost/date_time/gregorian/conversion.hpp>
#include <boost/date_time/posix_time/conversion.hpp>
#include "boost/asio.hpp"
#include "Utils.hpp"
#include "exception.hpp"


class SMGP303Deliver :
	public virtual IReport,
	public virtual IDeliver
{
public:
	SMGP303Deliver() :
		IDeliver(0x00000003),
		IReport(0x00000003)
	{
		setHeadType(&hd);
		msg_id = "";
	}
	typedef std::vector<uint8_t> bytes;
	virtual ~SMGP303Deliver() {};
	// 发送上行短信
	//需要
	//	setSerialNumb (string : 原样返回  uint32_t: +1)
	//	report
	//	headdeliver
	void deliver(std::string dest_phnum, std::string extnum, uint8_t msg_coding, std::string msg_text) override
	{
		try
		{
			extnum_ = extnum;
			buf_.clear();
			uint64_t msg_id_this_i;
			msg_id_this = msg_id;

			if (msg_id_this == "")
			{
				buf_ = CreateMsgId();
				msg_id_this = msg_id;
			}
			else
			{
				if (msg_id.size() != 20)
					BOOST_THROW_EXCEPTION(smproxy::exception("msg_id 长度不为20", 99));
				for (size_t i = 0; i < 10; i++)
				{
					std::string a = "0x";
					a += msg_id[2 * i];
					a += msg_id[2 * i + 1];
					const char *char_ptr = a.c_str();
					buf_.push_back((unsigned char)strtol(char_ptr, NULL, 16));
				}
			}
			//IsReport
			buf_.push_back(0);
			//MsgFormat
			buf_.push_back((uint8_t)msg_coding);
			//RecvTime  YYYYMMDDHHMMSS-------------------------------------
			std::string str = Utils::getTimeStr(1);
			str.resize(14);
			buf_.insert(buf_.end(), str.begin(), str.end());
			//RecvTime  YYYYMMDDHHMMSS-------------------------------------
			//SrcTermID
			if (dest_phnum[0] != 8 && dest_phnum[1] != 6)
			{
				dest_phnum = "86" + dest_phnum;
			}
			dest_phnum.resize(21);
			buf_.insert(buf_.end(), dest_phnum.begin(), dest_phnum.end());
			//DestTermID
			extnum.resize(21);
			buf_.insert(buf_.end(), extnum.begin(), extnum.end());
			//service_id.resize(10);
			//buf_.insert(buf_.end(), service_id.begin(), service_id.end());
			//buf_.push_back((uint8_t)TP_pid);
			//buf_.push_back((uint8_t)TP_udhi);
			//buf_.push_back((uint8_t)msg_coding);

			//存入短信长度和内容
			switch (msg_coding)
			{
			case 8:
			{
				std::vector<uint8_t> msg_bytes;
				msg_bytes = stringToUCS2_bytes(msg_text);
				msg_len = (uint8_t)(msg_bytes.size());
				buf_.push_back(msg_len);
				buf_.insert(buf_.end(), msg_bytes.begin(), msg_bytes.end());
			}
			break;
			case 15:
			{
				msg_len = (uint8_t)(msg_text.size());
				buf_.push_back(msg_len);
				buf_.insert(buf_.end(), msg_text.begin(), msg_text.end());
			}
			break;
			default:
			{
				smproxy::exception e("短消息编码格式号错误", 5);
				throw e;
			}
			break;
			}


			for (size_t i = 0; i < 8; i++)
			{
				buf_.push_back(0);
			}
		}
		catch (const std::exception& e)
		{
			smproxy::exception a(e.what());
			a.add("[Deliver生成]");
			throw a;
		}
		return;
	}
	void recvDeliverResp(bytes buf) override
	{
		hd_->recvHead(buf);
		msg_id_this.clear();
		for (size_t i = 0; i < 10; i++)
		{
			auto temp_msg_id = Utils::hex2str(buf[i + 12]);
			msg_id_this.insert(msg_id_this.end(), temp_msg_id.begin(), temp_msg_id.end());
		}
		msg_id = msg_id_this;
		memcpy_s(&resp_result_, 4, &(buf[22]), 4);
		resp_result_ = ntohl(resp_result_);
	}
	void recvReportResp(bytes buf) override
	{
		recvDeliverResp(buf);
	}
	void recvDeliver() override {};
	//生成Msg_id size = 20
	std::vector<uint8_t> CreateMsgId()
	{
		msg_id = "531061";
		//时间
		msg_id += Utils::getTimeStr();
		//去除ss
		msg_id.pop_back();
		msg_id.pop_back();
		std::string sn = serial_numb;
		while (sn.size() < 6)
		{
			sn = "0" + sn;
		}
		msg_id += sn;
		std::vector<uint8_t> temp_v;
		for (size_t i = 0; i < 10; i++)
		{
			std::string a = "0x";
			a += msg_id[2 * i];
			a += msg_id[2 * i + 1];
			const char *char_ptr = a.c_str();
			temp_v.push_back((unsigned char)strtol(char_ptr, NULL, 16));
		}
		return temp_v;
	}
	//发送短信报告
	//需要
	//	setSerialNumb (string : 原样返回  uint32_t: +1)
	//	setSPID
	//	setSubmitTime
	//	setDoneTime
	//	setSMSC_Sequence
	//	report
	//	headdeliver
	void report(std::string msg_id, std::string dest_phnum, uint8_t state) override {
		if (dest_phnum[0] != 8 && dest_phnum[1] != 6)
		{
			dest_phnum = "86" + dest_phnum;
		}
		buf_.clear();
		if (msg_id_this == "")
		{
			buf_ = CreateMsgId();
			msg_id_this = this->msg_id;
		}
		else
		{
			if (msg_id_this.size() != 20)
				BOOST_THROW_EXCEPTION(smproxy::exception("msg_id 长度不为20", 99));
			for (size_t i = 0; i < 10; i++)
			{
				std::string a = "0x";
				a += msg_id_this[2 * i];
				a += msg_id_this[2 * i + 1];
				const char *char_ptr = a.c_str();
				buf_.push_back((unsigned char)strtol(char_ptr, NULL, 16));
			}
			
		}
		this->msg_id = msg_id;
		//IsReport
		buf_.push_back(1);
		//MsgFormat
		buf_.push_back(0);
		//RecvTime  YYYYMMDDHHMMSS-------------------------------------
		std::string str = Utils::getTimeStr(1);
		str.resize(14);
		buf_.insert(buf_.end(), str.begin(), str.end());
		//RecvTime  YYYYMMDDHHMMSS-------------------------------------
		//SrcTermID
		if (dest_phnum[0] != 8 && dest_phnum[1] != 6)
		{
			dest_phnum = "86" + dest_phnum;
		}
		dest_phnum.resize(21);
		buf_.insert(buf_.end(), dest_phnum.begin(), dest_phnum.end());
		//DestTermID
		auto extnum = extnum_;
		extnum.resize(21);
		buf_.insert(buf_.end(), extnum.begin(), extnum.end());

		//消息长度
		buf_.push_back(66);
		if (msg_id.size() != 20)
			BOOST_THROW_EXCEPTION(smproxy::exception("msg_id 长度不为20", 99));
		for (size_t i = 0; i < 10; i++)
		{
			std::string a = "0x";
			a += msg_id[2 * i];
			a += msg_id[2 * i + 1];
			const char *char_ptr = a.c_str();
			buf_.push_back((unsigned char)strtol(char_ptr, NULL, 16));
		}
		buf_.push_back('0');
		buf_.push_back('0');
		buf_.push_back('1');
		buf_.push_back('0');
		buf_.push_back('0');
		buf_.push_back('1');

		submit_time.resize(10);
		buf_.insert(buf_.end(), submit_time.begin(), submit_time.end());

		done_time.resize(10);
		buf_.insert(buf_.end(), done_time.begin(), done_time.end());

		std::string state_str;
		switch (state)
		{
		case 0://成功
			state_str = "DELIVRD000";
			break;
		case 1://已推送
			state_str = "ACCEPTD000";
			break;
		case 2://推送失败
			state_str = "UNDELIV999";
			break;
		case 3://推送失败
			state_str = "UNDELIV999";
			break;
		default:
			break;
		}
		buf_.insert(buf_.end(), state_str.begin(), state_str.end());


		dest_phnum = "000";
		buf_.insert(buf_.end(), dest_phnum.begin(), dest_phnum.end());
		dest_phnum.assign(17, 0);
		buf_.insert(buf_.end(), dest_phnum.begin(), dest_phnum.end());


		for (size_t i = 0; i < 8; i++)
		{
			buf_.push_back(0);
		}
		return;
	}
protected:
	uint8_t msg_len;
	SMGP303Head hd;
};
