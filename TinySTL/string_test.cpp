#include"String.h"
#include"Test.h"
#include<iostream>

void TinySTL::StringTest::test1()
{
	String str1("hello world.\n");
	String str2(str1);
	String str3(str1.begin(), str1.end());
	std::cout << str1 << std::endl;
	std::cout << str2 << std::endl;
	std::cout << str3 << std::endl;
}
	

