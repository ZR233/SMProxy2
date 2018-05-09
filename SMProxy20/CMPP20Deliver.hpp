#pragma once
/*************************************************
Author:zr
Date:2018-03-5
Description:CMPP Deliver处理
**************************************************/
#include "IReport.hpp"

namespace smproxy {
	class CMPP20Deliver :
		public IReport
	{
	private:
		FieldInt msg_id_this_int_;
		FieldInt msg_id_int_;

		Buffer body_;
		Buffer report_buffer_;
	public:
		CMPP20Deliver() :
			IReport(),
			msg_id_this_int_(8,0),
			msg_id_int_(8,0)
		{
			ISMG_ID_.set(6,"0");
			msg_id_.set(30, "0");
			msg_id_this_.set(30, "0");
			//初始化参数
			body_.pushField(msg_id_this_int_);
			body_.pushField(dest_ID_.set(21,"0"));
			body_.pushField(service_id_.set(10, "0"));
			body_.pushField(tp_pid_.set(1, 0));
			body_.pushField(tp_udhi_.set(1, 0));
			body_.pushField(msg_fmt_.set(1, 0));
			body_.pushField(src_term_ID_.set(21, "0"));
			body_.pushField(report_flag_.set(1, 0));
			body_.pushField(message_length_.set(1, 0));

			report_buffer_.pushField(msg_id_int_);
			report_buffer_.pushField(stat_.set(7, "None"));
			report_buffer_.pushField(submit_time_.set(10, "0"));
			report_buffer_.pushField(done_time_.set(10, "0"));
			report_buffer_.pushField(dest_ID_.set(21, "0"));
			report_buffer_.pushField(SMSC_sequence_.set(4, 0));

		}
		typedef std::vector<uint8_t> bytes;
		virtual ~CMPP20Deliver() {};
		// 发送上行短信
		//需要
		//	setSerialNumb (string : 原样返回  uint32_t: +1)
		//	report
		//	headdeliver
		bytes deliverBytes(std::string dest_id,
			std::string service_id,
			std::string src_terminal_id,
			std::string msg_id,
			std::string stat,
			std::string submit_time,
			std::string done_time, 
			uint8_t msg_coding, 
			bytes msg_text) override
		{
			//try
			//{
			//	extnum_ = SP_num;
			//	buf_.clear();
			//	uint64_t msg_id_this_i;
			//	msg_id_this = msg_id;
			//	if (msg_id_this == "")
			//	{
			//		msg_id_this_i = CreateMsgId(ISMG_id_);
			//		msg_id_this = std::to_string(msg_id_this_i);
			//	}
			//	else
			//	{
			//		msg_id_this_i = std::stoull(msg_id_this);
			//	}
			//	msg_id_this_i = Utils::htonll(msg_id_this_i);
			//	buf_.resize(8, 0);
			//	memcpy_s(&(buf_[0]), 8, &msg_id_this_i, 8);

			//	SP_num.resize(21);
			//	buf_.insert(buf_.end(), SP_num.begin(), SP_num.end());
			//	service_id.resize(10);
			//	buf_.insert(buf_.end(), service_id.begin(), service_id.end());
			//	buf_.push_back((uint8_t)TP_pid);
			//	buf_.push_back((uint8_t)TP_udhi);
			//	buf_.push_back((uint8_t)msg_coding);
			//	if (dest_phnum[0] != 8 && dest_phnum[1] != 6)
			//	{
			//		dest_phnum = "86" + dest_phnum;
			//	}
			//	dest_phnum.resize(21);
			//	buf_.insert(buf_.end(), dest_phnum.begin(), dest_phnum.end());
			//	//非状态报告
			//	buf_.push_back(0);
			//	//存入短信长度和内容
			//	switch (msg_coding)
			//	{
			//	case 8:
			//	{
			//		std::vector<uint8_t> msg_bytes;
			//		msg_bytes = stringToUCS2_bytes(msg_text);
			//		msg_len = (uint8_t)(msg_bytes.size());
			//		buf_.push_back(msg_len);
			//		buf_.insert(buf_.end(), msg_bytes.begin(), msg_bytes.end());
			//	}
			//	break;
			//	case 15:
			//	{
			//		msg_len = (uint8_t)(msg_text.size());
			//		buf_.push_back(msg_len);
			//		buf_.insert(buf_.end(), msg_text.begin(), msg_text.end());
			//	}
			//	break;
			//	default:
			//	{
			//		smproxy::exception e("短消息编码格式号错误", 5);
			//		throw e;
			//	}
			//	break;
			//	}


			//	for (size_t i = 0; i < 8; i++)
			//	{
			//		buf_.push_back(0);
			//	}
			//}
			//catch (const std::exception& e)
			//{
			//	smproxy::exception a(e.what());
			//	a.add("[Deliver生成]");
			//	throw a;
			//}
			bytes buf;
			return buf;
		}
		//void recvDeliverResp(bytes buf) override
		//{
		//	uint64_t msg_id_num = 0;
		//	memcpy_s(&msg_id_num, 8, &(buf[12]), 8);
		//	msg_id_num = Utils::ntohll(msg_id_num);
		//	msg_id_this = std::to_string(msg_id_num);
		//	msg_id = std::to_string(msg_id_num);
		//	resp_result_ = buf[20];
		//}
		//void recvReportResp(bytes buf) override
		//{
		//	recvDeliverResp(buf);
		//}
		void recvDeliver(bytes &buf, bytes::iterator buf_loc) override {
			if (buf.size()< 77)
				BOOST_THROW_EXCEPTION(exception("Deliver消息长度错误", 4));
			auto iter = body_.reslov(buf, buf_loc);
			msg_id_this_ = std::to_string(msg_id_this_int_.get());
			//为状态报告
			if (report_flag_.get())
			{
				if (buf.size()< 137 + 8)
					BOOST_THROW_EXCEPTION(exception("Deliver消息长度错误", 4));
				report_buffer_.reslov(buf, iter);
				msg_id_ = std::to_string(msg_id_int_.get());
				ISMG_ID_ = std::to_string(Utils::getISMGFromCMPP20MsgID(msg_id_int_.get()));
			}
			else//为上行短信
			{
				if (buf.size()< 77 + message_length_.get() + 8)
					BOOST_THROW_EXCEPTION(exception("Deliver消息长度错误", 4));
				message_content_.assign(iter, iter + message_length_.get());
			}
		};
		//生成Msg_id,本机字节序
		//uint64_t CreateMsgId(std::string IMSG)
		//{
		//	uint64_t ismg = std::stoull(IMSG);
		//	uint64_t msg_id_this_ = 0;
		//	//时间
		//	boost::posix_time::ptime timeTemp = boost::posix_time::second_clock::local_time();
		//	boost::posix_time::time_duration td = timeTemp.time_of_day();
		//	boost::gregorian::date date = timeTemp.date();
		//	using std::string;
		//	using std::to_string;

		//	tm t = to_tm(date);
		//	tm t2 = to_tm(td);

		//	uint8_t mm = (uint8_t)(t.tm_mon + 1);
		//	uint64_t msg_id_i = mm;
		//	msg_id_this_ = (((uint64_t)std::stoull(IDeliver::serial_numb)) << 48);
		//	msg_id_this_ += (((uint64_t)ismg) << 26);
		//	msg_id_this_ += (((uint64_t)t2.tm_sec) << 20);
		//	msg_id_this_ += (((uint64_t)t2.tm_min) << 14);
		//	msg_id_this_ += (((uint64_t)t2.tm_hour) << 9);
		//	msg_id_this_ += (((uint64_t)t.tm_mday) << 4);
		//	msg_id_this_ += ((uint64_t)t.tm_mon) + 1;
		//	return msg_id_this_;
		//}
		//发送短信报告
		//需要
		//	setSerialNumb (string : 原样返回  uint32_t: +1)
		//	setSPID
		//	setSubmitTime
		//	setDoneTime
		//	setSMSC_Sequence
		//	report
		//	headdeliver
		bytes reportBytes(
			std::string dest_id,
			std::string service_id,
			std::string src_terminal_id,
			std::string msg_id,
			std::string stat,
			std::string submit_time,
			std::string done_time
		) override {
			//try
			//{
			//	if (dest_phnum[0] != 8 && dest_phnum[1] != 6)
			//	{
			//		dest_phnum = "86" + dest_phnum;
			//	}
			//	this->msg_id = msg_id;
			//	buf_.clear();
			//	uint64_t msg_id_this_i;
			//	if (msg_id_this == "")
			//	{
			//		msg_id_this_i = CreateMsgId(ISMG_id_);
			//		msg_id_this = std::to_string(msg_id_this_i);
			//	}
			//	else
			//	{
			//		msg_id_this_i = std::stoull(msg_id_this);
			//	}

			//	msg_id_this_i = Utils::htonll(msg_id_this_i);
			//	buf_.resize(8, 0);
			//	memcpy_s(&(buf_[0]), 8, &msg_id_this_i, 8);
			//	auto extnum = extnum_;
			//	extnum.resize(21);
			//	buf_.insert(buf_.end(), extnum.begin(), extnum.end());
			//	service_id.resize(10);
			//	buf_.insert(buf_.end(), service_id.begin(), service_id.end());
			//	buf_.push_back((uint8_t)TP_pid);
			//	buf_.push_back((uint8_t)TP_udhi);
			//	//Msg_Fmt
			//	buf_.push_back(0);
			//	dest_phnum.resize(21);
			//	buf_.insert(buf_.end(), dest_phnum.begin(), dest_phnum.end());
			//	//状态报告
			//	buf_.push_back(1);

			//	//消息长度
			//	buf_.push_back(60);
			//	bytes temp;
			//	uint64_t msg_id_i = std::stoull(msg_id);
			//	msg_id_i = Utils::htonll(msg_id_i);
			//	temp.resize(8, 0);
			//	memcpy_s(&(temp[0]), 8, &msg_id_i, 8);
			//	buf_.insert(buf_.end(), temp.begin(), temp.end());
			//	std::string state_str;
			//	switch (state)
			//	{
			//	case 0://成功
			//		state_str = "DELIVRD";
			//		break;
			//	case 1://已推送
			//		state_str = "ACCEPTD";
			//		break;
			//	case 2://推送失败
			//		state_str = "UNDELIV";
			//		break;
			//	case 3://推送失败
			//		state_str = "UNDELIV";
			//		break;
			//	default:
			//		break;
			//	}
			//	buf_.insert(buf_.end(), state_str.begin(), state_str.end());
			//	submit_time.resize(10);
			//	buf_.insert(buf_.end(), submit_time.begin(), submit_time.end());

			//	done_time.resize(10);
			//	buf_.insert(buf_.end(), done_time.begin(), done_time.end());

			//	dest_phnum.resize(21);
			//	buf_.insert(buf_.end(), dest_phnum.begin(), dest_phnum.end());

			//	SMSC_sequence = htonl(SMSC_sequence);
			//	temp.resize(4);
			//	memcpy_s(&(temp[0]), 4, &SMSC_sequence, 4);
			//	buf_.insert(buf_.end(), temp.begin(), temp.end());


			//	for (size_t i = 0; i < 8; i++)
			//	{
			//		buf_.push_back(0);
			//	}
			//}
			//catch (const std::exception& e)
			//{
			//	smproxy::exception a(e.what());
			//	a.add("[Deliver生成]");
			//	throw a;
			//}
			bytes buf;
			return buf;
		}

		void recvReport(bytes &buf, bytes::iterator buf_loc) {};
	protected:
		uint8_t msg_len;
	};

}//namespace smproxy