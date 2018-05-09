#pragma once

/*************************************************
Author:zr
Date:2018-03-5
Description:CMPP消息头处理
**************************************************/

#include "boost/asio.hpp"
#include "IHead.hpp"
#include "exception.hpp"
namespace smproxy {


	//CMPP消息头处理
	class CMPP20Head :
		public IHead
	{
	private:
		Buffer body_;
	public:
		typedef std::vector<uint8_t> bytes;
		CMPP20Head()
		{
			body_.pushField(total_len_.set(4, 0));
			body_.pushField(cmdID_.set(4, 0));
			body_.pushField(int_serial_numb_.set(4, 0));
		};
		virtual ~CMPP20Head() {};
		//制作消息头
		//@msgL	消息总长度(含消息头及消息体)
		//@CID	命令或响应类型
		//@SN	消息流水号,顺序累加,步长为1,循环使用（一对请求和应答消息的流水号必须相同）
		bytes header(uint32_t total_len, Command com_ID, std::string SN) override
		{
			switch (com_ID)
			{
			case login:
				cmdID_ = 0x00000001;
				break;
			case login_resp:
				cmdID_ = 0x80000001;
				break;
			case unbind:
				cmdID_ = 0x00000002;
				break;
			case unbind_resp:
				cmdID_ = 0x80000002;
				break;
			case submit:
				cmdID_ = 0x00000004;
				break;
			case submit_resp:
				cmdID_ = 0x80000004;
				break;
			case deliver:
				cmdID_ = 0x00000005;
				break;
			case deliver_resp:
				cmdID_ = 0x80000005;
				break;
			case report:
				cmdID_ = 0x00000005;
				break;
			case report_resp:
				cmdID_ = 0x80000005;
				break;
			case active:
				cmdID_ = 0x00000008;
				break;
			case active_resp:
				cmdID_ = 0x80000008;
				break;
			default:
				break;
			}
			buf_.clear();
			total_len_ = total_len;
			
			serial_numb_ = SN;
			int_serial_numb_ = uint32_t(stoul(SN));
			buf_ = body_.to_bytes();
			return buf_;
		}
		// 解析消息头
		bytes::iterator recvHead(bytes& buf)override {

			if (buf.size() < 12)
			{
				BOOST_THROW_EXCEPTION(exception("消息头字符串长度过短", 1));
			}
			auto iter = body_.reslov(buf, buf.begin());
			serial_numb_ = std::to_string(int_serial_numb_.get());
			switch (cmdID_.get())
			{
			case 0x00000001:
				cmd_ID_ = login;
				break;
			case 0x80000001:
				cmd_ID_ = login_resp;
				break;
			case 0x00000002:
				cmd_ID_ = unbind;
				break;
			case 0x80000002:
				cmd_ID_ = unbind_resp;
				break;
			case 0x00000004:
				cmd_ID_ = submit;
				break;
			case 0x80000004 :
				cmd_ID_ = submit_resp;
				break;
			case 0x00000005:
				cmd_ID_ = deliver;
				break;
			case 0x80000005:
				cmd_ID_ = deliver_resp;
				break;
			case 0x00000008:
				cmd_ID_ = active;
				break;
			case 0x80000008:
				cmd_ID_ = active_resp;
				break;
			default:
				break;
			}
			return iter;
		}

		
		uint32_t createSerualNum(uint32_t serial_numb) {
			if (serial_numb == 2147483647)
			{
				serial_numb = 0;
			}
			++serial_numb;
			int_serial_numb_ = serial_numb;
			serial_numb_ = std::to_string(serial_numb);
			return int_serial_numb_.get();
		};

	};
}//smproxy 