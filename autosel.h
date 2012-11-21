#pragma once
#include "node.h"
#include <map>

typedef std::map<Qid,int> CountPerQid;
class QidSelectInfo
{
 public:
	QidSelectInfo(const Qid& qid = -1, int req = -1)
		: qid(qid), req(req)
	{}
	bool operator<(const QidSelectInfo& r) const {
		return qid < r.qid;
	}
	bool done() const {
		return (kidset.size() == req);
	}
	void push(const Kid& kid) {
		kidset.insert(kid);
	}
	void pop(const Kid& kid) {
		kidset.erase(kid);
	}
	void get(Kids& kids) const {
		kids.assign(kidset.begin(), kidset.end());
	}
 private:
	Qid qid;
	int req;
	std::set<Kid> kidset;
};
typedef std::map<Qid, QidSelectInfo> SelectInfo;


class AutoSelect
{
 public:

	AutoSelect(){}
	virtual ~AutoSelect(){}

	int detect(Nodes& selected, const Nodes& candidate, const Qids& qids, int count_req);

 protected:
	int detect_comb(SelectInfo& info, int pos);
	void init_req_count(int count_req);
	void init_select_info(SelectInfo& info);
	int calc_min_overlap(const Nodes& candidate, const CountPerQid& req_per_qid) const;
	bool detected(const SelectInfo& info) const;
	int count_overlap(const SelectInfo& info) const;

 private:
	Nodes m_selected;
	Nodes m_candidate;
	Qids m_require_qids;

	CountPerQid m_req_per_qid;
	int m_min_overlap;
};
