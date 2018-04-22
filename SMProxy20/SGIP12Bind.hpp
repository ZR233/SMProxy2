#pragma once
/*************************************************
Author:zr
Date:2018-02-22
Description:SGIP1.2 Bind
**************************************************/


#include "IBind.hpp"
#include "SGIP12Head.hpp"

class SGIP12Bind :
	public IBind
{
public:
	SGIP12Bind() :IBind(1) {
		setHeadType(&hd_);
	}
	virtual ~SGIP12Bind(){};
	bytes Binder(std::string log_id, std::string pass_word) override
	{
		if ((log_id.size() >16) || (pass_word.size() >16))
		{
			throw std::runtime_error("bind时用户名密码过长");
		}
		buf_.clear();
		buf_.push_back(log_type_);
		log_id.resize(16);
		buf_.insert(buf_.end(), log_id.begin(), log_id.end());
		pass_word.resize(16);
		buf_.insert(buf_.end(), pass_word.begin(), pass_word.end());

		for (int i = 0; i < 8; i++)
		{
			buf_.push_back('\0');
		}
		return buf_;
	}
	void recvBind(bytes &buf)
	{
		int pt = 20;
		//拷贝登陆类型
		log_type_ = buf[pt];
		pt += 1;
		//拷贝用户名
		log_name_.assign(16, 0);
		for (int i = 0; i < log_name_.size(); i++)
		{
			log_name_[i] = buf[i + pt];
		}
		pt += 16;
		log_name_ = log_name_.c_str();
		//拷贝密码
		log_pass_.assign(16, 0);
		for (int i = 0; i < log_pass_.size(); i++)
		{
			log_pass_[i] = buf[i + pt];
		}
		pt += 16;
		log_pass_ = log_pass_.c_str();
	}
protected:
	SGIP12Head hd_;
};
