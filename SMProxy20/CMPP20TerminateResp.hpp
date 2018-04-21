#pragma once
/*************************************************
Author:zr
Date:2018-01-16
Description:CMPP TerminateResp¥¶¿Ì
**************************************************/
#include "IResp.hpp"
#include "CMPP20Head.hpp"


class CMPP20TerminateResp :
	public IResp
{
public:
	CMPP20TerminateResp() :IResp(0x80000002) {
		setHeadType(&hd_);
		buf_.clear();
	}
	virtual ~CMPP20TerminateResp() {}

	void resp(uint8_t result) override{}
	uint8_t recvResp(std::vector<uint8_t> buf) override { return 0; }
private:
	CMPP20Head hd_;
};


