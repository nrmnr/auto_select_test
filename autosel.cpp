#include "autosel.h"
#include <iostream>
#include <algorithm>
#include <limits.h>
#include "assert.h"

int AutoSelect::detect(Nodes& selected, const Nodes& candidate, const Qids& qids, int count_req)
{
	m_candidate.assign(candidate.begin(), candidate.end());
	m_require_qids.assign(qids.begin(), qids.end());

	CountPerQid req_per_qid;
	init_req_count(req_per_qid, candidate, count_req);
	m_min_overlap = calc_min_overlap(candidate, req_per_qid);

	SelectInfo info;
	init_select_info(info, req_per_qid);

	std::random_shuffle(m_candidate.begin(), m_candidate.end());
	int overlap = INT_MAX;
	bool result = detect_comb(info, overlap, 0);
	if (!result) return INT_MAX;

	selected.clear();
	for (SelectInfo::const_iterator si = info.begin(); si != info.end(); ++si) {
		Qid qid = si->first;
		Kids kids;
		si->second.get(kids);
		for (Kids::iterator ki = kids.begin(); ki != kids.end(); ++ki) {
			selected.push_back(Node(qid, *ki));
		}
	}
	return overlap;
}

void AutoSelect::init_req_count(CountPerQid& req_per_qid, const Nodes& candidate, int count_req) const
{
	req_per_qid.clear();
	for (Nodes::const_iterator ni = candidate.begin(); ni != candidate.end(); ++ni) {
		req_per_qid[ni->qid] = std::min<int>(req_per_qid[ni->qid]+1, count_req);
	}
}

void AutoSelect::init_select_info(SelectInfo& info, const CountPerQid& req_per_qid) const
{
	for (CountPerQid::const_iterator qi = req_per_qid.begin(); qi != req_per_qid.end(); ++qi) {
		Qid qid = qi->first;
		int count = qi->second;
		info[qid] = QidSelectInfo(qid, count);
	}
}

int AutoSelect::calc_min_overlap(const Nodes& candidate, const CountPerQid& req_per_qid) const
{
	int total_count_req = 0;
	for (CountPerQid::const_iterator ci = req_per_qid.begin(); ci != req_per_qid.end(); ++ci) {
		total_count_req += ci->second;
	}
	std::set<Kid> kidset;
	for (Nodes::const_iterator ni = candidate.begin(); ni != candidate.end(); ++ni) {
		kidset.insert(ni->kid);
	}
	int count_kid = kidset.size();
	return (count_kid >= total_count_req)? 0 : (total_count_req - count_kid);
}

bool AutoSelect::detect_comb(SelectInfo& info, int& overlap, int pos)
{
	overlap = count_overlap(info);
	if (detected(info)) {
		return (overlap == m_min_overlap);
	}
	if (pos >= m_candidate.size()) {
		return false;
	}

	Node node = m_candidate[pos];

	SelectInfo info_s(info);
	SelectInfo info_u(info);
	int ol_s = INT_MAX;
	int ol_u = INT_MAX;
	if (!info[node.qid].done()) {
		info_s[node.qid].push(node.kid);
		if (detect_comb(info_s, ol_s,  pos+1)) {
			info = info_s;
			overlap = ol_s;
			return true;
		}
	}
	{
		if (detect_comb(info_u, ol_u, pos+1)) {
			info = info_u;
			overlap = ol_u;
			return true;
		}
	}
	if (ol_s <= ol_u) {
		info = info_s;
		overlap = ol_s;
		return false;
	} else {
		info = info_u;
		overlap = ol_u;
		return false;
	}
}

bool AutoSelect::detected(const SelectInfo& info) const
{
	for (SelectInfo::const_iterator si = info.begin(); si != info.end(); ++si) {
		if (!si->second.done()) return false;
	}
	return true;
}

int AutoSelect::count_overlap(const SelectInfo& info) const
{
	std::map<Kid,int> counter;
	for (SelectInfo::const_iterator si = info.begin(); si != info.end(); ++si) {
		Kids kids;
		si->second.get(kids);
		for (Kids::iterator ki = kids.begin(); ki != kids.end(); ++ki) {
			counter[*ki]++;
		}
	}
	int count;
	for (std::map<Kid,int>::iterator ci = counter.begin(); ci != counter.end(); ++ci) {
		if (ci->second > 1) ++count;
	}
	return count;
}
