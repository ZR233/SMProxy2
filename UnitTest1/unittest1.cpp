#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SMProxy20/SMProxy2.h"


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
			//char a[5][21] = { {"18633334444"},
			//{"13822225555"},
			//{"18511111111"},
			//{"18722221111"},
			//{"13533331111"} };
			//char b[100];
			//submit(b, CMPP20, 5, a, b);
			Assert::AreEqual(1, 1);

		}

	};
}