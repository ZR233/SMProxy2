#pragma once
/*************************************************
Author:zr
Date:2018-02-22
Description:Bind Connect命令接口
**************************************************/

#include "IBaseParam.hpp"
#include "IHead.hpp"
#include "exception.hpp"
namespace smproxy {


	class IBind :
		public IBaseParam
	{
	public:
		typedef std::vector<uint8_t> bytes;
		IBind():
			password_(16),
			timestamp_(4, 0)
			//登陆密码
		{
			
		};
		virtual bytes Binder(std::string log_id, std::string pass_word) = 0;
		virtual void recvBind(bytes &buf, bytes::iterator buf_loc) = 0;
		// 取用户名
		std::string getName() {
			return sp_ID_.get();
		};
		// 取密码
		std::string getPass()
		{
			return password_.get();
		};
		std::string getUser()
		{
			return user_name_.get();
		};
		// 取登录类型。
		unsigned char getType() {
			return unsigned char(login_type_.get());
		};
		//设置登录类型。
		//SGIP:
		//1:SP->SMG; 
		//2:SMG->SP; 
		//3:SMG->SMG
		//SMGP:
		//0＝发送短消息（send mode）；
		//1＝接收短消息（receive mode）；
		//2＝收发短消息（transmit mode）；
		void setType(int type) {
			login_type_ = type;
		};
		//双方协商的版本号(高位4bit表示主版本号,低位4bit表示次版本号)
		char getVersion() { return unsigned char(version_.get()); };
		//时间戳的明文,由客户端产生,格式为MMDDHHMMSS，即月日时分秒，10位数字的整型，右对齐
		uint64_t getTimestamp() {
			return timestamp_.get();
		};
		virtual ~IBind() {};
	protected:
		smproxy::FieldInt login_type_;//登陆类型 1:SP->SMG; 2:SMG->SP; 3:SMG->SMG
		smproxy::FieldStr user_name_;
		smproxy::FieldStr password_;//登陆密码
		smproxy::FieldInt version_;//双方协商的版本号(高位4bit表示主版本号,低位4bit表示次版本号)
		tm timestamp_tm;
		smproxy::FieldInt timestamp_;//时间戳的明文,由客户端产生,格式为MMDDHHMMSS，即月日时分秒，10位数字的整型，右对齐
	};

}

