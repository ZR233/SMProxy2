// ConsoleApplication1.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "../SMProxy20/exception.hpp"
#include <iostream>
#include<boost/log/trivial.hpp>



int main()
{
	BOOST_LOG_TRIVIAL(info) << "An informational severity message";
	try
	{
		//throw std::exception("错误");
		BOOST_THROW_EXCEPTION(smproxy::exception("错误", 1, 1));
	}
	catch (smproxy::exception& e)
	{
		e.add("上级；");
		e.setError_code(2);
		std::cout << boost::diagnostic_information(e) << std::endl;
	}
	return 0;
}

