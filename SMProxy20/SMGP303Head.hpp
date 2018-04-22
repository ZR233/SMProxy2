#pragma once
/*************************************************
Author:zr
Date:2018-04-11
Description:SMGP 3.0.3 Head
**************************************************/


#include "IHead.hpp"
#include "boost/asio.hpp"
#include "exception.hpp"

namespace smproxy {
	class SMGP303Head :
		public IHead
	{
	private:
		Buffer body_;
	public:
		typedef std::vector<uint8_t> bytes;
		SMGP303Head() {
			body_.pushField(total_len_.set(4, 0));
			body_.pushField(cmdID_.set(4, 0));
			body_.pushField(int_serial_numb_.set(4, 0));
		};
		virtual ~SMGP303Head() {};
		uint32_t createSerualNum(uint32_t serial_numb) override {
			++serial_numb;
			serial_numb_ = std::to_string(serial_numb);
			int_serial_numb_ = serial_numb;
			return serial_numb;
		};

		bytes header(uint32_t total_len, Command cmdID, std::string serial_numb) override
		{
			switch (cmdID)
			{
			case login:
				cmdID_ = 0x00000001;
				break;
			case login_resp:
				cmdID_ = 0x80000001;
				break;
			case unbind:
				cmdID_ = 0x00000006;
				break;
			case unbind_resp:
				cmdID_ = 0x80000006;
				break;
			case submit:
				cmdID_ = 0x00000002;
				break;
			case submit_resp:
				cmdID_ = 0x80000002;
				break;
			case deliver:
				cmdID_ = 0x00000003;
				break;
			case deliver_resp:
				cmdID_ = 0x80000003;
				break;
			case report:
				cmdID_ = 0x00000003;
				break;
			case report_resp:
				cmdID_ = 0x80000003;
				break;
			case active:
				cmdID_ = 0x00000004;
				break;
			case active_resp:
				cmdID_ = 0x80000004;
				break;
			default:
				break;
			}
			buf_.clear();
			total_len_ = total_len;
			serial_numb_ = serial_numb;
			int_serial_numb_ = uint32_t(stoul(serial_numb));
			buf_ = body_.to_bytes();
			return buf_;
		};

		bytes::iterator recvHead(bytes& buf) override {
			if (buf.size() < 12)
			{
				BOOST_THROW_EXCEPTION(smproxy::exception("消息字符串过短"));
			}
			auto iter = body_.reslov(buf, buf.begin());
			switch (cmdID_.get())
			{
			case 0x00000001:
				cmd_ID_ = login;
				break;
			case 0x80000001:
				cmd_ID_ = login_resp;
				break;
			case 0x00000006:
				cmd_ID_ = unbind;
				break;
			case 0x80000006:
				cmd_ID_ = unbind_resp;
				break;
			case 0x00000002:
				cmd_ID_ = submit;
				break;
			case 0x80000002:
				cmd_ID_ = submit_resp;
				break;
			case 0x00000003:
				cmd_ID_ = deliver;
				break;
			case 0x80000003:
				cmd_ID_ = deliver_resp;
				break;
			case 0x00000004:
				cmd_ID_ = active;
				break;
			case 0x80000004:
				cmd_ID_ = active_resp;
				break;
			default:
				break;
			}
			return iter;
		};
	};
}

