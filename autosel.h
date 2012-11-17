#pragma once
#include "node.h"

class AutoSelect
{
 public:
	AutoSelect(){}
	virtual ~AutoSelect(){}

	int detect(const Nodes& nodes, const Qids& qids, int count_req);

 private:
	Qids m_require_qids;
};
