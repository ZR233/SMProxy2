#pragma once
/*************************************************
Author:zr
Date:2018-02-9
Description:report命令接口
**************************************************/
#include "IMsg.hpp"
//report命令接口
class IReport :
	public virtual IMsg
{
public:

	typedef std::vector<uint8_t> bytes;
	IReport()
	{
	};
	IReport(uint32_t cmdID)
	{
		this->cmdID = cmdID;
		msg_id_this = "";
	};
	//@msg_id 该命令所涉及的Submit命令的msg_id
	//@dest_phnum 短信发送到的号码
	//@state 发送状态
	virtual void report(std::string msg_id, std::string dest_phnum, uint8_t state) = 0;
	virtual void recvReportResp(bytes buf) = 0;
	virtual ~IReport() {};
	//传入发来的SPID 即SN1

	virtual void setSubmitTime(std::string submit_time)
	{
		this->submit_time = submit_time;
	}

	virtual void setDoneTime(std::string  done_time)
	{
		this->done_time = done_time;
	}
	virtual void setSMSC_Sequence(uint32_t  SMSC_sequence)
	{
		this->SMSC_sequence = SMSC_sequence;
	}

protected:
	std::string submit_time;//提交时间
	std::string done_time;//完成时间
	uint32_t SMSC_sequence;//取自SMSC发送状态报告的消息体中的消息标识。

};
