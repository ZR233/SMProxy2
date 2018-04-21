#pragma once
#include <cstdint>
#include "boost/asio.hpp"
#include <boost/date_time/gregorian/conversion.hpp>
#include <boost/date_time/posix_time/conversion.hpp>
#include <string>

class Utils
{
	Utils() {};
	~Utils() {};
public:
	static uint64_t htonll(uint64_t x)
	{
		uint64_t a = htonl(uint32_t(x));
		a = a << 32;// HHGGFFEE
		uint64_t b = htonl(uint32_t(x>>32));// DDCCBBAA
		x = a + b;
		return x;
	}
	static uint64_t ntohll(uint64_t x)
	{
		uint64_t a = ntohl(uint32_t(x));
		a = a << 32;// HHGGFFEE
		uint64_t b = ntohl(uint32_t(x >> 32));// DDCCBBAA
		x = a + b;
		return x;
	}
	//创建时间戳 字符串 MMDDhhmmss
	static std::string getTimeStr()
	{
		auto timeTemp = boost::posix_time::second_clock::local_time();
		auto td = timeTemp.time_of_day();
		auto date = timeTemp.date();
		using std::string;
		using std::to_string;

		tm t = to_tm(date);
		tm t2 = to_tm(td);

		//string YY = std::to_string(t.tm_year + 1900);
		//YY.erase(0, 2);
		string MM = to_string(t.tm_mon + 1);
		if (MM.size() == 1)
		{
			MM.insert(MM.begin(), '0');
		}
		string DD = to_string(t.tm_mday);
		if (DD.size() == 1)
		{
			DD.insert(DD.begin(), '0');
		}
		string HH = to_string(t2.tm_hour);
		if (HH.size() == 1)
		{
			HH.insert(HH.begin(), '0');
		}
		string MI = to_string(t2.tm_min);
		if (MI.size() == 1)
		{
			MI.insert(MI.begin(), '0');
		}
		auto SS = to_string(t2.tm_sec);
		if (SS.size() == 1)
		{
			SS.insert(SS.begin(), '0');
		}
		string str = MM + DD + HH + MI + SS;
		return str;
	}
	/*	创建时间戳 字符串 
		1:YYYYMMDDhhmmss
		2:YYMMDDhhmmss
		3:MMDDhhmmss
		4:MMDDhhmm
	*/
	static std::string getTimeStr(int type)
	{

		auto timeTemp = boost::posix_time::second_clock::local_time();
		auto td = timeTemp.time_of_day();
		auto date = timeTemp.date();
		using std::string;
		using std::to_string;

		tm t = to_tm(date);
		tm t2 = to_tm(td);

		string YYYY = std::to_string(t.tm_year + 1900);
		string YY = YYYY;
		YY.erase(0, 2);
		string MM = to_string(t.tm_mon + 1);
		if (MM.size() == 1)
		{
			MM.insert(MM.begin(), '0');
		}
		string DD = to_string(t.tm_mday);
		if (DD.size() == 1)
		{
			DD.insert(DD.begin(), '0');
		}
		string HH = to_string(t2.tm_hour);
		if (HH.size() == 1)
		{
			HH.insert(HH.begin(), '0');
		}
		string MI = to_string(t2.tm_min);
		if (MI.size() == 1)
		{
			MI.insert(MI.begin(), '0');
		}
		auto SS = to_string(t2.tm_sec);
		if (SS.size() == 1)
		{
			SS.insert(SS.begin(), '0');
		}
		string str;
		switch (type)
		{
		case 1:
			str = YYYY + MM + DD + HH + MI + SS;
			break;
		case 2:
			str = YY + MM + DD + HH + MI + SS;
			break;
		case 3:
			str = MM + DD + HH + MI + SS;
			break;
		case 4:
			str = MM + DD + HH + MI;
			break;
		default:
			break;
		}
		
		return str;
	}
	static std::string hex2str(uint8_t hex)
	{
		std::string str = "00";
		if (hex / 16 < 10)
			str[0] += hex / 16;
		else
		{
			str[0] = 'A';
			str[0] += hex / 16 - 10;
		}
		if (hex % 16 < 10)
			str[1] += hex % 16;
		else
		{
			str[1] = 'A';
			str[1] += hex % 16 - 10;
		}
		return str;
	}
	static uint32_t bytes2Int32(const char* bytes)
	{
		uint32_t intger;
		memcpy_s(&intger, 4, bytes, 4);
		return ntohl( intger);
	}
};
