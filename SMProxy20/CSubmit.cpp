#include "stdafx.h"
#include "CSubmit.h"
#include <boost\make_shared.hpp>
#include "CMPP20Head.hpp"
#include "CMPP20Submit.hpp"

namespace smproxy {
	CSubmit::CSubmit(Protocol protocol)
	{
		switch (protocol)
		{
		case SGIP12:
			break;
		case CMPP20:
			hd = boost::make_shared<IHead>(CMPP20Head());
			sb = boost::make_shared<ISubmit>(CMPP20Submit());
			break;
		case SMGP303:
			break;
		default:
			break;
		}
	}
	CSubmit::~CSubmit()
	{
	}
	const bytes& CSubmit::getBuf(
		int& serial_numb,
		char* src_ID,
		std::vector<std::string> &user_num,
		std::vector<uint8_t>& message_content
	)
	{
		//正常短信
		if (message_content.size() <=140)
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
				0x00000004,
				hd->getSerialNumb()
			);
			buf_.insert(buf_.begin(), head_buf.begin(), head_buf.end());
		}
		else//长短信
		{

		}

	}
}