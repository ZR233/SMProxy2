#pragma once
#include "CMPP20Head.hpp"
#include "CMPP20Submit.hpp"
#include "IHead.hpp"
#include "ISubmit.hpp"
#include "Defines.h"
#include <boost\shared_ptr.hpp>
#include "IBaseParam.hpp"
namespace smproxy {
	class CSubmit:
		public IBaseParam
	{
	public:
		CSubmit(Protocol protocol);
		~CSubmit();
		const bytes& getBuf(
			int& serial_numb,
			char* src_ID,
			std::vector<std::string> &user_num,
			std::vector<uint8_t>& message_content
		);
	private:
		boost::shared_ptr<IHead> hd;
		boost::shared_ptr<ISubmit> sb;
		bytes buf_;
	};
}


