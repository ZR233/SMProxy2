#pragma once
#pragma once
#include "IResp.hpp"
#include <string>
#include <vector>

/*************************************************
Author:zr
Date:2018-03-10
Description: 所有resp的接口
**************************************************/



class IRespAll
{
public:
	IRespAll(){};
	typedef std::vector<uint8_t> bytes;
	virtual ~IRespAll(){};
	
	enum EResult
	{
		correct,
		struc_err,
		cmd_err,
		same_sid,
		bind_err
	};
	virtual void bindResp(EResult result, 
		std::string authenticator_source, 
		std::string shared_secret, 
		std::string serial_numb) = 0;

	virtual void unbindResp(std::string serial_numb) = 0;
	virtual void submitResp(EResult result, std::string ISMG_id , std::string serial_numb) = 0;
	virtual void deliverResp(EResult result, std::string msg_id_this, std::string serial_numb) = 0;
	virtual void reportResp(EResult result, std::string msg_id_this, std::string serial_numb) = 0;
	virtual void activeTestResp(EResult result, std::string serial_numb) = 0;



	virtual uint8_t recvResp(std::vector<uint8_t> buf) = 0;

	
	virtual IResp* getRespInstance() = 0;
};
