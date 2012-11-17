#pragma once
#include "node.h"
#include <map>

typedef std::map<Qid,int> CountPerQid;

class AutoSelect
{
 public:
	AutoSelect(){}
	virtual ~AutoSelect(){}

	int detect(Nodes& selected, const Nodes& candidate, const Qids& qids, int count_req);

 protected:
	int detect_comb(int pos);
	void init_req_count(int count_req);

 private:
	Nodes m_selected;
	Nodes m_candidate;
	Qids m_require_qids;

	CountPerQid m_req_per_qid;
};
