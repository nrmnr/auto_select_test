#pragma once
#include <iostream>
#include <vector>
#include "node.h"

class Test
{
	template<typename T>
	void assert_equal(const T& a, const T& b);

public:
	void run();

protected:
	void test_parser();
	void test_autosel_no_overlap();
	void test_autosel_overlap();
	void test_autosel_shortage();

	void test_sub(const char *nodes_str, const char *qids_str, int count_req, const char *expected_str, int expected_return);

	std::vector<int> parse_int_array(const char *array_str) const;
	Nodes parse_nodes(const char *nodes_str) const;
	Qids parse_qids(const char *qids_str) const;
};
