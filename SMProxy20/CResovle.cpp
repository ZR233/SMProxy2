#include "stdafx.h"
#include "CResovle.h"
#include <boost\make_shared.hpp>


#include "CMPP20Head.hpp"
#include "CMPP20Submit.hpp"
#include "CMPP20Connect.hpp"

#include "SGIP12Head.hpp"
//#include "SGIP12Deliver.hpp"
//#include "SGIP12Report.hpp"
//#include "SGIP12Submit.hpp"
//#include "SGIP12Bind.hpp"
//
#include "SMGP303Head.hpp"
//#include "SMGP303Login.hpp"
//#include "SMGP303Submit.hpp"
//#include "SMGP303Deliver.hpp"


#include "SMProxy2.h"
namespace smproxy {
	CResolve::CResolve(Protocol protocol, std::string sp_id)
	{
		switch (protocol)
		{
		case SGIP12:
			hd.reset(new SGIP12Head);
			//sb = boost::make_shared<SGIP12Submit>(SGIP12Submit());
			//dlv = boost::make_shared<SGIP12Deliver>(SGIP12Deliver());
			//rp = boost::make_shared<SGIP12Report>(SGIP12Report());
			break;
		case CMPP20:
			hd.reset(new CMPP20Head);
			bd.reset(new CMPP20Connect);
			//sb = boost::make_shared<CMPP20Submit>(CMPP20Submit());
			//dlv = boost::make_shared<CMPP20Deliver>(CMPP20Deliver());
			//rp = boost::make_shared<CMPP20Deliver>(CMPP20Deliver());
			break;
		case SMGP303:
			hd.reset(new SMGP303Head);
			//sb = boost::make_shared<SMGP303Submit>(SMGP303Submit());
			//dlv = boost::make_shared<SMGP303Deliver>(SMGP303Deliver());
			//rp = boost::make_shared<SMGP303Deliver>(SMGP303Deliver());
			break;
		default:
			break;
		}
		sp_ID_ = sp_id;
	}

	bytes CResolve::submit(
		int& serial_numb,
		char* src_ID,
		std::vector<std::string> &user_num,
		std::vector<uint8_t>& message_content
	)
	{
		//正常短信
		if (message_content.size() <= 140)
		{
			sb->submit(
				src_ID,
				user_num,
				8,
				message_content
			);
			buf_ = sb->getBuf();
			serial_numb = hd->createSerualNum(serial_numb);
			auto head_buf = hd->header(
				buf_.size(),
				Command::submit,
				hd->getSerialNumb()
			);
			buf_.insert(buf_.begin(), head_buf.begin(), head_buf.end());
		}
		else//长短信
		{

		}

	}

	bytes CResolve::connect(
		int& serial_numb,
		const char* password,
		int login_type
	)
	{
		bd->setType(login_type);
		bd->Binder(sp_ID_, password);
		buf_ = bd->getBuf();
		serial_numb = hd->createSerualNum(serial_numb);
		auto head_buf = hd->header(
			buf_.size(),
			login,
			hd->getSerialNumb()
		);
		hd->setSpId(sp_ID_);
		head_buf = hd->header(
			buf_.size()+ head_buf.size(),
			login,
			hd->getSerialNumb()
		);
		buf_.insert(buf_.begin(), head_buf.begin(), head_buf.end());
		return buf_;
	}

	void CResolve::resolveBuf(bytes& buf)
	{
		hd->recvHead(buf);
	}
}