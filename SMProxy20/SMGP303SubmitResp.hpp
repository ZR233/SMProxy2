#pragma once
#include "IResp.hpp"
#include <vector>
#include "Utils.hpp"
#include "SMGP303Head.hpp"

/*************************************************
Author:zr
Date:2018-04-12
Description:SMGP3.03 submit resp处理
**************************************************/

//
class SMGP303SubmitResp :
	public IResp
{
public:
	SMGP303SubmitResp() :IResp(0x80000004) {
		setHeadType(&hd_);
	}
	virtual ~SMGP303SubmitResp() {}
	//需要先set:
	//ISMG_id
	/*结果
	0：正确
	1：消息结构错
	2：命令字错
	3：消息序号重复
	4：消息长度错
	5：资费代码错
	6：超过最大信息长
	7：业务代码错
	8：流量控制错
	9~：其他错误
	*/
	void resp(uint8_t result) override
	{
		msg_id= "531061";
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
		msg_id_this = msg_id;
		std::vector<uint8_t> temp_v;
		for (size_t i = 0; i < 10; i++)
		{
			std::string a = "0x";
			a += msg_id[2 * i];
			a += msg_id[2 * i + 1];
			const char *char_ptr = a.c_str();
			temp_v.push_back((unsigned char)strtol(char_ptr, NULL, 16));
		}
		buf_.insert(buf_.end(), temp_v.begin(), temp_v.begin() + 10);

		uint32_t state = uint32_t(result);
		state = htonl(state);
		temp_v.assign(4, 0);
		memcpy_s(&(temp_v[0]), 4, &state, 4);
		buf_.insert(buf_.end(), temp_v.begin(), temp_v.end());
	}
	uint8_t recvResp(std::vector<uint8_t> buf) override
	{
		//hd_.recvHead(buf);
		//uint64_t msg_id_i;
		//memcpy_s(&msg_id_i, 8, &(buf[12]), 8);
		//msg_id_i = Utils::ntohll(msg_id_i);
		//msg_id_this = std::to_string(msg_id_i);
		//resp_result_ = buf[20];
		//return resp_result_;
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