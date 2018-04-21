// ConsoleApplication1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

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
    return 0;
}

