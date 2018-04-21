#pragma once
#include "IResp.hpp"
#include <vector>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time.hpp>
#include "Utils.hpp"
#include "CMPP20Head.hpp"

/*************************************************
Author:zr
Date:2018-03-10
Description:CMPP2.0 submit resp����
**************************************************/

//
class CMPP20SubmitResp :
	public IResp
{
public:
	CMPP20SubmitResp() :IResp(0x80000004) {
		setHeadType(&hd_);
	}
	virtual ~CMPP20SubmitResp() {}
	//��Ҫ��set:
	//ISMG_id
	/*���
		0����ȷ
		1����Ϣ�ṹ��
		2�������ִ�
		3����Ϣ����ظ�
		4����Ϣ���ȴ�
		5���ʷѴ����
		6�����������Ϣ��
		7��ҵ������
		8���������ƴ�
		9~����������
		*/
	void resp(uint8_t result) override
	{
		uint32_t sid = (uint32_t)std::stoll(serial_numb);
		uint32_t ismg = std::stoul(ISMG_id_);

		//ʱ��
		boost::posix_time::ptime timeTemp = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration td = timeTemp.time_of_day();
		boost::gregorian::date date = timeTemp.date();
		using std::string;
		using std::to_string;

		tm t = to_tm(date);
		tm t2 = to_tm(td);

		uint64_t msg_id_i = (((uint64_t)sid) << 48);
		msg_id_i += ((uint64_t)ismg) << 26;
		msg_id_i += ((uint64_t)t2.tm_sec) << 20;
		msg_id_i += ((uint64_t)t2.tm_min) << 14;
		msg_id_i += ((uint64_t)t2.tm_hour) << 9;
		msg_id_i += ((uint64_t)t.tm_mday) << 4;
		msg_id_i += ((uint64_t)t.tm_mon) + 1;
		msg_id_this = std::to_string(msg_id_i);
		msg_id_i = Utils::htonll(msg_id_i);
		buf_.resize(8, 0);
		memcpy_s(&(buf_[0]), 8, &msg_id_i, 8);
		buf_.push_back((unsigned char)result);
	}
	uint8_t recvResp(std::vector<uint8_t> buf) override
	{
		hd_.recvHead(buf);
		uint64_t msg_id_i;
		memcpy_s(&msg_id_i, 8, &(buf[12]), 8);
		msg_id_i = Utils::ntohll(msg_id_i);
		msg_id_this = std::to_string(msg_id_i);
		resp_result_ = buf[20];
		return resp_result_;
	}

	std::string getISMG_id() override
	{
		return ISMG_id_;
	}
	void setISMG_id(std::string ISMG_id) override
	{
		ISMG_id_ = ISMG_id;
	}
protected:
	CMPP20Head hd_;
};