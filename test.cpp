#include "test.h"
#include "assert.h"
#include "autosel.h"

#define RUN_TEST(test) { \
		std::cerr << "-> " << #test << std::endl; \
		test(); \
	}

void Test::run()
{
	RUN_TEST(test_autosel);
}

template<typename T>
void Test::assert_equal(const T& expected, const T& actual)
{
	if (expected == actual) return;
	std::cerr
		<< "  !assert_equal failed" << std::endl
		<< "    expected : " << expected << std::endl
		<< "    actual   : " << actual << std::endl;
}

void Test::test_autosel()
{
	AutoSelect autosel;
	Nodes nodes;
	Qids qids;
	assert_equal(2, autosel.detect(nodes, qids, 5));
}
