#pragma once
#include "IResp.hpp"
#include <vector>
#include <sstream>
#include "Utils.hpp"
#include "SMGP303Head.hpp"

/*************************************************
Author:zr
Date:2018-04-12
Description:SMGP 3.03  DeliverResp处理
**************************************************/

//
class SMGP303DeliverResp:
	public IResp
{
public:
	SMGP303DeliverResp() :IResp(0x80000003) {
		setHeadType(&hd_);
	}
	virtual ~SMGP303DeliverResp() {}

	void resp(uint8_t result) override
	{
		buf_.clear();
		if (msg_id_this.size() != 10)
			BOOST_THROW_EXCEPTION(smproxy::exception("msg_id 长度不为10", 99));
		for (size_t i = 0; i < 10; i++)
		{
			std::string a = "0x";
			a += msg_id_this[2 * i];
			a += msg_id_this[2 * i + 1];
			const char *char_ptr = a.c_str();
			buf_.push_back((unsigned char)strtol(char_ptr, NULL, 16));
		}
		uint32_t result4 = htonl(uint32_t(result));
		std::vector<uint8_t> result_bytes(4, 0);
		memcpy_s(&(result_bytes[0]), 4, &result4, 4);
		buf_.insert(buf_.end(),result_bytes.begin(), result_bytes.end());
	}
	uint8_t recvResp(std::vector<uint8_t> buf) override
	{
		hd_.recvHead(buf);
		msg_id_this.clear();
		for (size_t i = 0; i < 10; i++)
		{
			auto temp_msg_id = Utils::hex2str(buf[i + 12]);
			msg_id_this.insert(msg_id_this.end(), temp_msg_id.begin(), temp_msg_id.end());
		}
		msg_id = msg_id_this;
		memcpy_s(&resp_result_, 4, &(buf[22]),4);
		resp_result_ = ntohl(resp_result_);
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
	SMGP303Head hd_;
};