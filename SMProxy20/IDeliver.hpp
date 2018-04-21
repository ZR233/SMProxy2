#pragma once
/*************************************************
Author:zr
Date:2018-02-9
Description:Deliver命令接口
**************************************************/

#include "IMsg.hpp"
#include <boost/locale/encoding.hpp>
#include <boost/asio.hpp>


//Deliver命令接口
class IDeliver :
	public virtual IMsg
{
public:
	typedef std::vector<uint8_t> bytes;
	IDeliver(uint32_t cmdID) :
		TP_pid(0),
		TP_udhi(0)
	{
		hd_->setCmdID(cmdID);
		msg_id_this_ = "";
		buf_.clear();
	};
	virtual ~IDeliver() = default;

	virtual void deliver(std::string dest_phnum, std::string SP_num, uint8_t msg_coding, std::string msg_text) {};
	virtual void recvDeliver()
	{
		throw smproxy::exception("recvDeliver未实现");
	};
	virtual void recvDeliverResp(bytes buf)
	{
		throw smproxy::exception("recvDeliverResp未实现");
	};
	virtual void setTP_pid(uint8_t TP_pid) { this->TP_pid = TP_pid; };
	virtual void setTP_udhi(uint8_t TP_udhi) { this->TP_udhi = TP_udhi; };
	virtual std::vector<uint8_t> stringToUCS2_bytes(std::string src)
	{
		std::wstring wmsg_text;
		wmsg_text = boost::locale::conv::to_utf<wchar_t>(src, "GBK");
		std::vector<uint8_t> text_buf(wmsg_text.size() * 2);
		for (size_t i = 0; i < wmsg_text.size(); i++)
		{
			uint16_t temp_short = wmsg_text[i];
			temp_short = htons(temp_short);
			memcpy_s(&(text_buf[i * 2]), 2, &temp_short, 2);
		}
		return text_buf;
	};

protected:
	smproxy::FieldInt TP_pid;
	smproxy::FieldInt TP_udhi;
	smproxy::FieldStr service_id;
};