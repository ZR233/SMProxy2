#pragma once
/*************************************************
Author:zr
Date:2018-02-22
Description: UnBind ÃüÁî½Ó¿Ú
**************************************************/

#include "IMsg.hpp"

class IUnBind :
	public virtual IMsg
{
public:
	typedef std::vector<uint8_t> bytes;
	IUnBind(uint32_t cmdID) 
	{
		this->cmdID = cmdID;
	};
	virtual bytes unBind() = 0;
	virtual void recvUnBind(bytes &buf) = 0;

	virtual ~IUnBind() {};

};