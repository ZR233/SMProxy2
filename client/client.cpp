// client.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
// blocking_tcp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include "../SMProxy20/CResovle.h"
using boost::asio::ip::tcp;

enum { max_length = 1024 };

int main()
{
	try
	{

		boost::asio::io_context io_context;

		tcp::socket s(io_context);
		tcp::resolver resolver(io_context);
		boost::asio::connect(s, resolver.resolve("127.0.0.1", "7890"));
		smproxy::CResolve resolve(CMPP20, "626082");
		int sn = 0;
		
		auto buf = resolve.connect(sn, "1qaz@WSX", 1);
		boost::asio::write(s, boost::asio::buffer(buf));

		buf.resize(4);
		uint32_t len;
		size_t reply_length = boost::asio::read(s,
			boost::asio::buffer(buf));

		memcpy_s(&len, 4, &(buf[0]), 4);
		len = ntohl(len);
		std::cout << "Reply len is: ";
		std::cout << std::to_string(len);
		std::cout << "\n";
		buf.resize(len);
		boost::asio::read(s,
			boost::asio::buffer(buf));
		resolve.resolveBuf(buf);
		std::cout << "Reply len is: ";
		std::cout << std::to_string(len);
		std::cout << "\n";
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

