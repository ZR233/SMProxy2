#pragma once
/*************************************************
Author:zr
Date:2018-02-9
Description: SGIP1.2 ����report
**************************************************/


#include "IReport.hpp"
#include "SGIP12Head.hpp"


// SGIP1.2 ����report
class SGIP12Report :public virtual IReport
{
public:
	// SGIP1.2 ����report
	SGIP12Report() :
		IReport(5)
	{
		setHeadType(&hd);
	};
	//@msg_id ���������漰��Submit�����msg_id
	//@dest_phnum ���ŷ��͵��ĺ���
	//@state ����״̬	0�����ͳɹ�	1���ȴ�����	2������ʧ��
	//��Ҫ�ȴ��뷢����SPID
	void report(std::string msg_id, std::string dest_phnum, uint8_t state) {
		this->msg_id = msg_id;
		hd.stringID2IntID(msg_id, SN1_msg, SN2_msg, SN3_msg);
		SN1_msg = htonl(SN1_msg);
		SN2_msg = htonl(SN2_msg);
		SN3_msg = htonl(SN3_msg);
		buf_.assign(12, 0);
		memcpy_s(&(buf_[0]), 4, &SN1_msg, 4);
		memcpy_s(&(buf_[4]), 4, &SN2_msg, 4);
		memcpy_s(&(buf_[8]), 4, &SN3_msg, 4);
		//Report��������
		//0������ǰһ��Submit�����״̬����
		//1������ǰһ��ǰתDeliver�����״̬����
		buf_.push_back(0);
		//���ն���Ϣ���ֻ��ţ��ֻ�����ǰ�ӡ�86�������־
		std::string temp_dest_phnum = "86";
		temp_dest_phnum += dest_phnum;
		temp_dest_phnum.resize(21);
		buf_.insert(buf_.end(), temp_dest_phnum.begin(), temp_dest_phnum.end());
		buf_.push_back(state);
		//errorCode
		buf_.push_back(state);
		//reserve
		for (size_t i = 0; i < 8; i++)
		{
			buf_.push_back(0);
		}
	};
	void addHead() override {
		hd_->header(buf_.size(), cmdID, serial_numb);
		size_t head_len = hd_->getBuf().size();
		hd_->header(head_len + buf_.size(), cmdID, serial_numb);
		buf_.insert(buf_.begin(), hd_->getBuf().begin(), hd_->getBuf().end());
		msg_id_this = hd_->getSerialNumb();
	};
	void recvReportResp(bytes buf) override
	{
		hd.recvHead(buf);
		msg_id_this = hd.getSerialNumb();
		msg_id = msg_id_this;
		resp_result_ = uint32_t(buf[20]);
	}
	~SGIP12Report() = default;
private:
	uint32_t SN1_msg;
	uint32_t SN2_msg;
	uint32_t SN3_msg;
	SGIP12Head hd;
};