#include "test.h"
#include "assert.h"
#include "autosel.h"
#include <string>
#include <sstream>

#define RUN_TEST(test) { \
		std::cerr << "-> " << #test << std::endl; \
		test(); \
	}

void Test::run()
{
	RUN_TEST(test_parser);
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

void Test::test_parser()
{
	int expected[] = { 1, 2, 3, 4, -1, 2 };

	std::vector<int> actual = parse_int_array("1,2, 3,4, -1,2");
	assert_equal(6, static_cast<int>(actual.size()));

	int *ex = expected;
	std::vector<int>::const_iterator ni = actual.begin();
	for (; ni != actual.end(); ++ni, ++ex) {
		assert_equal(*ex, *ni);
	}
}

void Test::test_autosel()
{
	test_sub("1,101, 1,102, 1,103, 1,104, 1,105, 2,201, 2,202, 2,203, 2,205, 2,101",
					 "1,2", 5, "5,5", 1);
}

void Test::test_sub(const char *nodes_str, const char *qids_str, int count_req, const char *expected_str, int expected_return)
{
	Nodes result_nodes;
	Nodes nodes = parse_nodes(nodes_str);
	Qids qids = parse_qids(qids_str);
	AutoSelect autosel;
	int result = autosel.detect(result_nodes, nodes, qids, count_req);
	assert_equal(expected_return, result);

	// std::vector<int> expected = parse_int_array(expected_str);
	// todo
}

std::vector<int> Test::parse_int_array(const char *array_str) const
{
	std::string str(array_str);
	size_t pos;
	for (; (pos = str.find(",")) != std::string::npos;) {
		str.replace(pos, 1, " ");
	}
	std::vector<int> array;
	std::istringstream ss(str);
	for (; !ss.eof();) {
		int n;
		ss >> n;
		array.push_back(n);
	}
	return array;
}

Nodes Test::parse_nodes(const char *nodes_str) const
{
	Nodes nodes;
	std::vector<int> array = parse_int_array(nodes_str);
	assert(array.size() % 2 == 0);
	for (int i = 0; i < array.size(); i += 2) {
		int qid(array[i]);
		int kid(array[i+1]);
		nodes.push_back(Node(qid, kid));
	}
	return nodes;
}

Qids Test::parse_qids(const char *qids_str) const
{
	Qids qids;
	std::vector<int> array = parse_int_array(qids_str);
	for (std::vector<int>::const_iterator ni = array.begin(); ni != array.end(); ++ni) {
		qids.push_back(Qid(*ni));
	}
	return qids;
}
