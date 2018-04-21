#pragma once
/*************************************************
Author:zr
Date:2018-02-9
Description:report����ӿ�
**************************************************/
#include "IMsg.hpp"
//report����ӿ�
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
	//@msg_id ���������漰��Submit�����msg_id
	//@dest_phnum ���ŷ��͵��ĺ���
	//@state ����״̬
	virtual void report(std::string msg_id, std::string dest_phnum, uint8_t state) = 0;
	virtual void recvReportResp(bytes buf) = 0;
	virtual ~IReport() {};
	//���뷢����SPID ��SN1

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
	std::string submit_time;//�ύʱ��
	std::string done_time;//���ʱ��
	uint32_t SMSC_sequence;//ȡ��SMSC����״̬�������Ϣ���е���Ϣ��ʶ��

};
