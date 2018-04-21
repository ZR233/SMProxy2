#pragma once
#include "IMsg.hpp"
#include <string>

/*************************************************
Author:zr
Date:2018-03-5
Description:resp½Ó¿Ú
**************************************************/

class IResp :
	public virtual IMsg
{
public:
	IResp(uint32_t cmdid)
	{
		this->cmdID = cmdid;
	};
	virtual void setCmdID(uint32_t cmdid)
	{
		this->cmdID = cmdid;
	};
	virtual ~IResp() {};
	virtual void resp(uint8_t result) = 0;
	virtual uint8_t recvResp(std::vector<uint8_t> buf) = 0;

	virtual void setAuthenticatorSource(std::string authenticator_source)
	{
		authenticator_source_ = authenticator_source;
	}
	virtual std::string getAuthenticatorSource()
	{
		return authenticator_source_;
	}
	virtual void setSharedSecret(std::string shared_secret)
	{
		shared_secret_ = shared_secret;
	}
	virtual std::string getSharedSecret()
	{
		return shared_secret_;
	}
protected:
	std::string authenticator_source_;
	std::string shared_secret_;
};