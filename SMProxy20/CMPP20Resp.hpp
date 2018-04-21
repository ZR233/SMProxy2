#pragma once
/*************************************************
Author:zr
Date:2018-03-10
Description:CMPP2.0 resp
**************************************************/

#include "CMPP20ConnectResp.hpp"
#include "CMPP20SubmitResp.hpp"
#include "IRespAll.hpp"
#include "CMPP20Head.hpp"
#include "CMPP20DeliverResp.hpp"
#include "CMPP20TerminateResp.hpp"
#include "CMPP20ActiveTestResp.hpp"

class CMPP20Resp :
	public IRespAll
{
public:
	CMPP20Resp()  {
	}
	typedef std::vector<uint8_t> bytes;
	virtual ~CMPP20Resp() {}
	uint8_t recvResp(std::vector<uint8_t> buf) override
	{
		CMPP20Head hd;
		hd.recvHead(buf);
		switch (hd.getCmdID())
		{
		case 0x80000001:
			resp_ = &cr_;
			break;
		case 0x80000002:
			resp_ = &tr_;
			break;
		case 0x80000004:
			resp_ = &sr_;
			break;
		case 0x80000005:
			resp_ = &dr_;
			break;
		case 0x80000008:
			resp_ = &ar_;
			break;
		}
		resp_->recvResp(buf);
		return resp_->getResp_result();
	}


	IResp* getRespInstance() override
	{
		return resp_;
	}

	void bindResp(EResult result, std::string authenticator_source, std::string shared_secret,
		std::string serial_numb) override
	{
		resp_ = &cr_;
		resp_->setAuthenticatorSource(authenticator_source);
		resp_->setSharedSecret(shared_secret);
		resp_->setSerialNumb(serial_numb);
		getResultCode(result);
		resp_->addHead();
	}
	void unbindResp(std::string serial_numb) override
	{
		resp_ = &tr_;
		resp_->setSerialNumb(serial_numb);
		resp_->addHead();
	}
	void submitResp(EResult result, std::string ISMG_id, std::string serial_numb) override
	{
		resp_ = &sr_;
		resp_->setISMG_id(ISMG_id);
		resp_->setSerialNumb(serial_numb);
		getResultCode(result);
		resp_->addHead();
		
	}
	void deliverResp(EResult result, std::string msg_id_this, std::string serial_numb) override
	{
		resp_ = &dr_;
		resp_->setMsgIdThis(msg_id_this);
		resp_->setSerialNumb(serial_numb);
		getResultCode(result);
		resp_->addHead();
	}

	void reportResp(EResult result, std::string msg_id_this, std::string serial_numb) override
	{
		deliverResp(result, msg_id_this, serial_numb);
	}

	void activeTestResp(EResult result, std::string serial_numb) override
	{
		resp_ = &ar_;
		resp_->setSerialNumb(serial_numb);
		getResultCode(result);
		resp_->addHead();
	}
protected:
	void getResultCode(EResult result)
	{
		switch (result)
		{
		case correct:
			resp_->resp(0);
			break;
		case bind_err:
			resp_->resp(3);
			break;
		case cmd_err:
			resp_->resp(2);
			break;
		case struc_err:
			resp_->resp(1);
			break;
		default:
			resp_->resp(9);
			break;
		}
	}


	CMPP20ConnectResp cr_;
	CMPP20SubmitResp sr_;
	CMPP20DelivertResp dr_;
	CMPP20TerminateResp tr_;
	CMPP20ActiveTestResp ar_;
	IResp *resp_ = nullptr;
};
