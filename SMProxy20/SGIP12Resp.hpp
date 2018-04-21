#pragma once
#include "IResp.hpp"
#include "SGIP12Head.hpp"
#include "IRespAll.hpp"

class SGIP12Resp :
	public virtual IResp,
	public virtual IRespAll
{
public:
	SGIP12Resp() :IResp(0x80000001) {
		setHeadType(&hd_);
	}
	typedef std::vector<uint8_t> bytes;
	virtual ~SGIP12Resp() {};
	void resp(uint8_t result) override
	{
		buf_.push_back(result);
		for (int i = 0; i < 8; i++)
		{
			buf_.push_back(0);
		}
	}
	uint8_t recvResp(std::vector<uint8_t> buf) override
	{
		resp_result_ = buf[20];
		return resp_result_;
	}

	//消息头+消息体
	std::vector<uint8_t> getBuf() override
	{
		return buf_;
	}

	IResp* getRespInstance() override
	{
		return this;
	}

	void bindResp(EResult result, std::string authenticator_source, std::string shared_secret,
		std::string serial_numb) override
	{
		setSerialNumb(serial_numb);
		setCmdID(0x80000001);
		getResultCode(result);
		addHead();
	}
	void unbindResp(std::string serial_numb) override
	{
		setSerialNumb(serial_numb);
		setCmdID(0x80000002);
		buf_.clear();
		addHead();
	}
	void submitResp(EResult result, std::string ISMG_id, std::string serial_numb) override
	{
		setSerialNumb(serial_numb);
		setCmdID(0x80000003);
		getResultCode(result);
		addHead();
	}
	void deliverResp(EResult result, std::string msg_id_this, std::string serial_numb) override
	{
		setSerialNumb(serial_numb);
		setCmdID(0x80000004);
		getResultCode(result);
		addHead();
	}
	void reportResp(EResult result, std::string msg_id_this, std::string serial_numb) override
	{
		setSerialNumb(serial_numb);
		setCmdID(0x80000005);
		getResultCode(result);
		addHead();
	}
	void activeTestResp(EResult result, std::string serial_numb) override{}
protected:
	void getResultCode(EResult result)
	{
		switch (result)
		{
		case correct:
			resp(0);
			break;
		case bind_err:
			resp(1);
			break;
		case cmd_err:
			resp(2);
			break;
		case struc_err:
			resp(5);
			break;
		default:
			resp(99);
			break;
		}
	}
	SGIP12Head hd_;
};
