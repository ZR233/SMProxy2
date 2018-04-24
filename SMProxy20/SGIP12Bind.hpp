#pragma once
/*************************************************
Author:zr
Date:2018-02-22
Description:SGIP1.2 Bind
**************************************************/
#include "IBind.hpp"

namespace smproxy {


	class SGIP12Bind :
		public IBind
	{
	private:
		Buffer body_;
	public:
		SGIP12Bind(){
			body_.pushField(login_type_.set(1, 1));
			body_.pushField(user_name_.set(16, ""));
			body_.pushField(password_.set(16, ""));
		}
		virtual ~SGIP12Bind() {};
		bytes Binder(std::string log_id, std::string pass_word) override
		{
			if ((log_id.size() > 16) || (pass_word.size() > 16))
			{
				BOOST_THROW_EXCEPTION(exception("bind时用户名密码过长"));
			}
			buf_ = body_.to_bytes();

			for (int i = 0; i < 8; i++)
			{
				buf_.push_back('\0');
			}
			return buf_;
		}
		void recvBind(bytes& buf, bytes::iterator buf_loc)override
		{
			if (buf.size() < 61)
			{
				BOOST_THROW_EXCEPTION(exception("connect字符串长度错误", 5));
			}
			body_.reslov(buf, buf_loc);
		}
	};
}//smproxy