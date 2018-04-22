#pragma once
/*************************************************
Author:zr
Date:2018-02-9
Description:SGIP1.2 head
**************************************************/


#include "IHead.hpp"
#include "boost/date_time/posix_time/ptime.hpp"
#include "boost/date_time/gregorian/conversion.hpp"
#include "boost/date_time/posix_time/conversion.hpp"
#include "boost/asio.hpp"
#include "exception.hpp"

namespace smproxy {


	class SGIP12Head :
		public IHead
	{
	private:
		Buffer body_;
	public:
		typedef std::vector<uint8_t> bytes;
		SGIP12Head() {
			body_.pushField(total_len_.set(4, 0));
			body_.pushField(cmdID_.set(4, 0));
			body_.pushField(SN1_.set(4, 0));
			body_.pushField(SN2_.set(4, 0));
			body_.pushField(int_serial_numb_.set(4, 0));
		};
		uint32_t createSerualNum(uint32_t serial_numb) override {
			if (serial_numb == 99999)
			{
				serial_numb = 0;
			}
			++serial_numb;
			boost::posix_time::ptime time(boost::posix_time::second_clock::local_time());
			tm date_ = boost::gregorian::to_tm(boost::gregorian::date(time.date()));
			tm time_ = boost::posix_time::to_tm(boost::posix_time::time_duration(time.time_of_day()));
			SN2_ = (date_.tm_mon + 1) * 100000000 + date_.tm_mday * 1000000 + time_.tm_hour * 10000 + time_.tm_min * 100 + time_.tm_sec;
			if (SN1_.get() == 0)
			{
				SN1_ = 3053100000 + std::stoul(sp_ID_.get());
			}
			auto serial_numb_str =std::to_string( SN1_.get());

			serial_numb_str += "|";
			serial_numb_str += std::to_string(SN2_.get());
			serial_numb_str += "|";
			serial_numb_str += std::to_string(serial_numb);
			int_serial_numb_ = serial_numb;
			serial_numb_ = serial_numb_str;
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
				cmdID_ = 0x00000002;
				break;
			case unbind_resp:
				cmdID_ = 0x80000002;
				break;
			case submit:
				cmdID_ = 0x00000003;
				break;
			case submit_resp:
				cmdID_ = 0x80000003;
				break;
			case deliver:
				cmdID_ = 0x00000004;
				break;
			case deliver_resp:
				cmdID_ = 0x80000004;
				break;
			case report:
				cmdID_ = 0x00000005;
				break;
			case report_resp:
				cmdID_ = 0x80000005;
				break;
			default:
				break;
			}
			buf_.clear();
			total_len_ = total_len;
			uint32_t SN1;
			uint32_t SN2;
			uint32_t SN3;
			stringID2IntID(serial_numb, SN1, SN2, SN3);
			SN1_ = SN1;
			SN2_ = SN2;
			int_serial_numb_ = SN3;
			buf_ = body_.to_bytes();
			return buf_;
		};

		bytes::iterator recvHead(bytes& buf) override {
			if (buf.size() < 20)
			{
				BOOST_THROW_EXCEPTION(exception("消息字符串过短"));
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
			case 0x00000002:
				cmd_ID_ = unbind;
				break;
			case 0x80000002:
				cmd_ID_ = unbind_resp;
				break;
			case 0x00000003:
				cmd_ID_ = submit;
				break;
			case 0x80000003:
				cmd_ID_ = submit_resp;
				break;
			case 0x00000004:
				cmd_ID_ = deliver;
				break;
			case 0x80000004:
				cmd_ID_ = deliver_resp;
				break;
			case 0x00000005:
				cmd_ID_ = report;
				break;
			case 0x80000005:
				cmd_ID_ = report_resp;
				break;
			default:
				break;
			}
			serial_numb_ = std::to_string(SN1_.get()) + 
				"|" + std::to_string(SN2_.get()) + 
				"|" + std::to_string(int_serial_numb_.get());
			return iter;
		};


		virtual ~SGIP12Head() {};
		virtual void stringID2IntID(std::string serial_numb, uint32_t& SN1, uint32_t& SN2, uint32_t& SN3)
		{
			if ("" == serial_numb)
			{
				BOOST_THROW_EXCEPTION(exception("流水号为空"));
				return;
			}


			size_t pos = serial_numb.find("|");
			size_t size = serial_numb.size();
			if (pos == 0)
				BOOST_THROW_EXCEPTION(exception("sn1为空"));
			SN1 = std::stoul(serial_numb.substr(0, pos));
			serial_numb.erase(0, pos + 1);
			pos = serial_numb.find("|");
			if (pos == 0)
				BOOST_THROW_EXCEPTION(exception("sn2为空"));
			SN2 = std::stoul(serial_numb.substr(0, pos));
			serial_numb.erase(0, pos + 1);
			if (serial_numb.size() == 0)
				BOOST_THROW_EXCEPTION(exception("sn3为空"));
			SN3 = std::stoul(serial_numb);
		};

	};

}