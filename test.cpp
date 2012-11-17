#include "test.h"
#include "assert.h"
#include "autosel.h"
#include <string>
#include <sstream>
#include <map>

#define RUN_TEST(test) { \
		std::cerr << "-> " << #test << std::endl; \
		test(); \
	}
#define ASSERT_EQUAL(expected, actual) { \
		assert_equal(expected, actual, #expected ", " #actual); \
	}

template<typename T>
void Test::assert_equal(const T& expected, const T& actual, const char *mes)
{
	if (expected == actual) return;
	std::cerr
		<< "  !!assert_equal(" << mes << ")" << std::endl
		<< "    expected : " << expected << std::endl
		<< "    actual   : " << actual << std::endl;
}

void Test::run()
{
	RUN_TEST(test_parser);
	RUN_TEST(test_autosel_no_overlap);
	RUN_TEST(test_autosel_overlap);
	RUN_TEST(test_autosel_shortage);
}

void Test::test_parser()
{
	int expected[] = { 1, 2, 3, 4, -1, 2 };

	std::vector<int> actual = parse_int_array("1,2, 3,4, -1,2");
	ASSERT_EQUAL(6, static_cast<int>(actual.size()));

	int *ex = expected;
	std::vector<int>::const_iterator ni = actual.begin();
	for (; ni != actual.end(); ++ni, ++ex) {
		ASSERT_EQUAL(*ex, *ni);
	}
}

void Test::test_autosel_no_overlap()
{
	test_sub("1,101, 1,102, 1,103, 1,104, 1,105, 2,201, 2,202, 2,203, 2,205, 2,205",
					 "1,2", 5, "5,5", 0);
}

void Test::test_autosel_overlap()
{
	test_sub("1,101, 1,102, 1,103, 1,104, 1,105, 2,201, 2,202, 2,203, 2,101, 2,102",
					 "1,2", 5, "5,5", 2);
}

void Test::test_autosel_shortage()
{
	test_sub("1,101, 1,102, 1,103, 1,104, 1,105, 2,201, 2,202, 2,203",
					 "1,2", 5, "5,3", 0);
}

void Test::test_sub(const char *nodes_str, const char *qids_str, int count_req, const char *expected_str, int expected_return)
{
	Nodes result_nodes;
	Nodes nodes = parse_nodes(nodes_str);
	Qids qids = parse_qids(qids_str);

	{
		AutoSelect autosel;
		int result = autosel.detect(result_nodes, nodes, qids, count_req);
		ASSERT_EQUAL(expected_return, result);
		ASSERT_EQUAL(expected_return, count_overlap_kid(result_nodes));
	}
	{
		std::vector<int> expected = parse_int_array(expected_str);
		int i = 0;
		for (Qids::const_iterator qi = qids.begin(); qi != qids.end(); ++qi, ++i) {
			Qid qid = *qi;
			ASSERT_EQUAL(expected[i], count_per_qid(result_nodes, qid));
		}
	}
}

int Test::count_per_qid(const Nodes& nodes, const Qid& qid) const
{
	int count = 0;
	for (Nodes::const_iterator ni = nodes.begin(); ni != nodes.end(); ++ni) {
		if (ni->qid == qid) ++count;
	}
	return count;
}

int Test::count_overlap_kid(const Nodes& nodes) const
{
	std::map<Kid,int> counter;
	for (Nodes::const_iterator ni = nodes.begin(); ni != nodes.end(); ++ni) {
		counter[ni->kid]++;
	}
	int count = 0;
	for (std::map<Kid,int>::const_iterator ki = counter.begin(); ki != counter.end(); ++ki) {
		if (ki->second > 1) ++count;
	}
	return count;
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
