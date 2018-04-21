#pragma once
/*************************************************
Author:zr
Date:2018-03-5
Description:异常类 含错误码 逐级增加信息功能
**************************************************/

#include <vector>
#include <exception>
#include "boost/exception/all.hpp"
typedef boost::error_info<struct tag_erro_no, int> err_no;
typedef boost::error_info<struct tag_erro_str, std::string> err_str;

namespace smproxy
{
	//异常类 含错误码 逐级增加信息功能
	class exception :
		virtual public std::exception,
		virtual public boost::exception
	{
	public:
		//@error_str 错误文字
		exception(std::string error_str) {
			(*this) << err_str(error_str);
			v_.push_back(error_str);
		};
		//@error_str 错误文字
		//@error_code 错误号
		exception(std::string error_str, int error_code) {
			v_.push_back(error_str);
			(*this) << err_str(error_str);
			(*this) << err_no(error_code);
		};
		virtual ~exception() {};
		//在前端加入信息
		virtual void add(std::string a) {
			v_.insert(v_.begin(), a);
			std::string str;
			if (!v_.empty())
			{
				for (const auto& i : v_)
				{
					str += i;
				}
			}
		};
		//获取错误信息vector
		virtual std::vector<std::string>& get() {
			return v_;
		};
		//获取全部错误信息
		virtual std::string what() {
			std::string str;
			if (!v_.empty())
			{
				for (const auto& i : v_)
				{
					str += i;
				}
			}
			return str;
		};
		virtual int getError_code() {
			return *boost::get_error_info<err_no>(*this);
		}
	protected:
		std::vector<std::string> v_;
	};
}
