#pragma once
/*************************************************
Author:zr
Date:2018-03-5
Description:SGIP1.2 Deliver
**************************************************/


#include "IDeliver.hpp"
#include "SGIP12Head.hpp"

class SGIP12Deliver :
	public virtual IDeliver
{
public:
	SGIP12Deliver() :
		IDeliver(4) {
		setHeadType(&hd);
	};
	void deliver(std::string dest_phnum, std::string SP_num, uint8_t msg_coding, std::string msg_text) {
		try
		{
			//接收短消息的手机号，手机号码前加“86”国别标志
			std::string temp_dest_phnum = "86";
			temp_dest_phnum += dest_phnum;
			temp_dest_phnum.resize(21);
			buf_.assign(temp_dest_phnum.begin(), temp_dest_phnum.end());
			//SP的接入号码
			SP_num.resize(21);
			buf_.insert(buf_.end(), SP_num.begin(), SP_num.end());

			buf_.push_back(TP_pid);
			buf_.push_back(TP_udhi);
			buf_.push_back(msg_coding);
			//存入短信长度和内容
			switch (msg_coding)
			{
			case 8:
			{
				std::vector<uint8_t> msg_bytes;
				msg_bytes = stringToUCS2_bytes(msg_text);
				msg_len = htonl(msg_bytes.size());
				std::vector<uint8_t> msg_len_bytes(4, 0);
				memcpy_s(&(msg_len_bytes[0]), 4, &msg_len, 4);
				buf_.insert(buf_.end(), msg_len_bytes.begin(), msg_len_bytes.end());
				buf_.insert(buf_.end(), msg_bytes.begin(), msg_bytes.end());
			}
			break;
			case 15:
			{
				msg_len = htonl(msg_text.size());
				std::vector<uint8_t> msg_len_bytes(4, 0);
				memcpy_s(&(msg_len_bytes[0]), 4, &msg_len, 4);
				buf_.insert(buf_.end(), msg_len_bytes.begin(), msg_len_bytes.end());
				buf_.insert(buf_.end(), msg_text.begin(), msg_text.end());
			}
			break;
			default:
			{
				smproxy::exception e("短消息编码格式号错误",5);
				throw e;
			}
			break;
			}
			for (size_t i = 0; i < 8; i++)
			{
				buf_.push_back(0);
			}
		}
		catch (smproxy::exception& e)
		{
			e.add("[生成deliver]");
			throw e;
		}
	};
	void addHead() override {
		hd_->header(buf_.size(), cmdID, serial_numb);
		size_t head_len = hd_->getBuf().size();
		hd_->header(head_len + buf_.size(), cmdID, serial_numb);
		buf_.insert(buf_.begin(), hd_->getBuf().begin(), hd_->getBuf().end());
		msg_id = hd_->getSerialNumb();
		msg_id_this = hd_->getSerialNumb();
	};
	void recvDeliverResp(const bytes buf) override
	{
		hd.recvHead(buf);
		msg_id_this = hd.getSerialNumb();
		msg_id = msg_id_this;
		resp_result_ = uint32_t(buf[20]);
	}
	void recvDeliver() override {}
	virtual ~SGIP12Deliver(){};
protected:
	uint32_t msg_len;
	SGIP12Head hd;
};
