#pragma once
#include "node.h"

class AutoSelect
{
 public:
	AutoSelect(){}
	virtual ~AutoSelect(){}

	int detect(Nodes& selected, const Nodes& candidate, const Qids& qids, int count_req);

 private:
	Qids m_require_qids;
};
