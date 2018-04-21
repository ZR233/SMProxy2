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
		bytes& header(uint32_t total_len, uint32_t  com_ID, std::string SN) override
		{
			total_len_ = total_len;
			cmdID_ = com_ID;
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
			return body_.reslov(buf, buf.begin());
		}

		
		uint32_t createSerualNum(uint32_t serial_numb) {
			if (serial_numb = 2147483647)
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