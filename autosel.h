#pragma once
#include "node.h"

class AutoSelect
{
 public:
	AutoSelect(){}
	virtual ~AutoSelect(){}

	int detect(Nodes& selected, const Nodes& candidate, const Qids& qids, int count_req);

 protected:
	int detect_comb(int pos);

 private:
	Nodes m_selected;
	Nodes m_candidate;
	Qids m_require_qids;
};
