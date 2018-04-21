#pragma once
#include "IResp.hpp"
#include <vector>

#include "Utils.hpp"
#include "CMPP20Head.hpp"

/*************************************************
Author:zr
Date:2018-03-10
Description:CMPP2.0 submit resp¥¶¿Ì
**************************************************/

//
class CMPP20DelivertResp :
	public IResp
{
public:
	CMPP20DelivertResp() :IResp(0x80000001) {
		setHeadType(&hd_);
	}
	virtual ~CMPP20DelivertResp() {}
	
	void resp(uint8_t result) override
	{
		uint64_t msg_id_this_ll = 0;
		msg_id_this_ll = std::stoll(msg_id_this);
		msg_id_this_ll = Utils::htonll(msg_id_this_ll);
		buf_.assign(8, 0);
		memcpy_s(&buf_[0], 8, &msg_id_this_ll, 8);
		buf_.push_back(result);
	}
	uint8_t recvResp(std::vector<uint8_t> buf) override
	{
		hd_.recvHead(buf);
		uint64_t msg_id_this_ll = 0;
		memcpy_s(&msg_id_this_ll, 8, &(buf[12]), 8);
		msg_id_this_ll = Utils::ntohll(msg_id_this_ll);
		msg_id_this = std::to_string(msg_id_this_ll);
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