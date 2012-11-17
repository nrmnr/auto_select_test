#pragma once
#include <iostream>

class Test
{
	template<typename T>
	void assert_equal(const T& a, const T& b);

 public:
	void run();

 protected:
	void test_autosel();

};
