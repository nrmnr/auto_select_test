#include "test.h"
#include "assert.h"
#include "autosel.h"

template<typename T>
void Test::assert_equal(const T& a, const T& b)
{
	if (a == b) return;
	m_status = false;
	std::cerr
		<< std::endl
		<< "!assert_equal failed("
		<< m_test_name << ") : "
		<< a << " != " << b
		<< std::endl;
}

void Test::test_autosel()
{
	assert_equal(1, 2);
}
