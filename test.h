#pragma once
#include <iostream>
#include <string>

#define RUN_TEST(test) { \
		m_test_name = #test; \
		m_status = true;	 \
		test();													 \
		std::cerr << (m_status ? "." : "F");			\
	}

class Test
{
	template<typename T>
	void assert_equal(const T& a, const T& b);

 public:
	void run() {
		RUN_TEST(test_autosel);
		std::cerr << std::endl;
	}


 protected:
	void test_autosel();

	std::string m_test_name;
	bool m_status;

};
