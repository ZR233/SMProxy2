#pragma once
/*************************************************
Author:zr
Date:2018-01-16
Description:CMPP ÐÄÌø¼ì²â´¦Àí
**************************************************/
#include "IResp.hpp"
#include "CMPP20Head.hpp"


class CMPP20ActiveTestResp :
	public IResp
{
public:
	CMPP20ActiveTestResp() :IResp(0x80000008) {
		setHeadType(&hd_);
		buf_.clear();
	}
	virtual ~CMPP20ActiveTestResp() {}

	void resp(uint8_t result) override
	{
		buf_.push_back(0);
	}
	uint8_t recvResp(std::vector<uint8_t> buf) override { return 0; }
private:
	CMPP20Head hd_;
};


