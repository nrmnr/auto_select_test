#include "node.h"

Node::Node(const Qid& q, const Kid& k)
	: qid(q), kid(k)
{
}

bool Node::operator<(const Node& r) const
{
	return (qid != r.qid)? (qid < r.qid) : (kid < r.kid);
}

