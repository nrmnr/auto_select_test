#pragma once
#include <vector>
#include <set>

typedef int Qid;
typedef int Kid;

struct Node
{
	Node(const Qid& q = 0, const Kid& k = 0);
	bool operator<(const Node& r) const;

	Qid qid;
	Kid kid;
};

typedef std::vector<Node> Nodes;
typedef std::set<Node> NodeSet;

typedef std::vector<Qid> Qids;
