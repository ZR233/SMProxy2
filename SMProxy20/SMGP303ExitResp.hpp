#pragma once
/*************************************************
Author:zr
Date:2018-04-13
Description:SMGP3.03 exit处理
**************************************************/
#include "IResp.hpp"
#include "SMGP303Head.hpp"


class SMGP303ExitResp :
	public IResp
{
public:
	SMGP303ExitResp() :IResp(0x80000006) {
		setHeadType(&hd_);
		buf_.clear();
	}
	virtual ~SMGP303ExitResp() {}

	void resp(uint8_t result) override
	{}
	uint8_t recvResp(std::vector<uint8_t> buf) override { return 0; }
private:
	SMGP303Head hd_;
};