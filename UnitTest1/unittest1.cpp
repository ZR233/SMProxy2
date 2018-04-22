#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SMProxy20/SMProxy2.h"
#include <boost/date_time/gregorian/conversion.hpp>
#include <boost/date_time/posix_time/conversion.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "../SMProxy20/Utils.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: 在此输入测试代码
			int a = 1;
			int b = 2;
			int c = Sum(a , b);
			Assert::AreEqual(c, 3);

		}
		TEST_METHOD(TestMethod2)
		{
			// TODO: 在此输入测试代码
			char a[20];
			Hello(20,a);
			Assert::AreEqual(a, "hello world");

		}
		TEST_METHOD(TestMethod3)
		{
			// TODO: 在此输入测试代码
			auto timeTemp = boost::posix_time::second_clock::local_time();
			auto str1 =  Utils::getTimeStr(3);
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

			
			Assert::AreEqual(str1, str);

		}

	};
}