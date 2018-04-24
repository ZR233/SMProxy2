// ConsoleApplication1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <string>
class A
{
public:
	A& foo()
	{
		return *this;
	}
	void say()
	{
		printf("A");
	}
};

class B :
	public A
{
public:
	B& foo()
	{
		return *this;
	}
	void say()
	{
		printf("B");
	}
};
int main()
{
	A a;
	B b;
	b.foo().say();

	std::vector<int> str;
	str.push_back(1);
	str.push_back(1);
	auto i = str.begin();
	if (i != str.end())
	{
		std::cout << 1;
		++i;
	}
	if (i != str.end())
	{
		std::cout << 2;
		++i;
	}
	if (i != str.end())
	{
		std::cout << 3;
		++i;
	}
	if (i != str.end())
	{
		std::cout << 4;
		++i;
	}
    return 0;
}

