#pragma once
/*************************************************
Author:zr
Date:2018-02-22
Description:Bind Connect命令接口
**************************************************/

#include "IMsg.hpp"
#include "IHead.hpp"
class IBind :
	public virtual IMsg
{
public:
	typedef std::vector<uint8_t> bytes;
	IBind(uint32_t cmdID) :
		log_type_(1,4),//登陆类型 1:SP->SMG; 2:SMG->SP; 3:SMG->SMG
		log_name_("",16),//登录名
		log_pass_("",16),
		timestamp_(0,4)
		//登陆密码
	{
		hd_->setCmdID(cmdID);
	};
	virtual bytes Binder(std::string log_id, std::string pass_word) = 0;
	virtual void recvBind(bytes &buf) = 0;
	// 取用户名
	virtual std::string getName() {
		return log_name_.get();
	};
	// 取密码
	virtual std::string getPass()
	{
		return log_pass_.get();
	};
	// 取登录类型。
	virtual unsigned char getType() {
		return log_type_.get();
	};
	// 设置登录类型。SGIP
	//1:SP->SMG; 2:SMG->SP; 3:SMG->SMG
	virtual void setType(uint8_t type) {
		log_type_ = type;
	};
	//双方协商的版本号(高位4bit表示主版本号,低位4bit表示次版本号)
	virtual char getVersion() { return version_; };
	//时间戳的明文,由客户端产生,格式为MMDDHHMMSS，即月日时分秒，10位数字的整型，右对齐
	virtual unsigned int getTimestamp() {
		return timestamp_.get();
	};
	virtual ~IBind(){};
protected:
	smproxy::FieldInt log_type_;//登陆类型 1:SP->SMG; 2:SMG->SP; 3:SMG->SMG
	smproxy::FieldStr log_name_;//登录名
	smproxy::FieldStr log_pass_;//登陆密码
	char version_;//双方协商的版本号(高位4bit表示主版本号,低位4bit表示次版本号)
	tm timestamp_tm;
	smproxy::FieldInt timestamp_;//时间戳的明文,由客户端产生,格式为MMDDHHMMSS，即月日时分秒，10位数字的整型，右对齐
};

