#include "autosel.h"
#include <iostream>
#include <algorithm>
#include <limits.h>
#include "assert.h"

int AutoSelect::detect(Nodes& selected, const Nodes& candidate, const Qids& qids, int count_req)
{
	m_candidate.assign(candidate.begin(), candidate.end());
	std::random_shuffle(m_candidate.begin(), m_candidate.end());

	m_require_qids.assign(qids.begin(), qids.end());
	init_req_count(count_req);

	m_min_overlap = calc_min_overlap(candidate, m_req_per_qid);
	std::cerr << m_min_overlap << std::endl;

	SelectInfo info;
	init_select_info(info);

	int result = detect_comb(info, 0);

	selected.clear();
	for (SelectInfo::const_iterator si = info.begin(); si != info.end(); ++si) {
		Qid qid = si->first;
		Kids kids;
		si->second.get(kids);
		for (Kids::iterator ki = kids.begin(); ki != kids.end(); ++ki) {
			selected.push_back(Node(qid, *ki));
		}
	}

	selected.assign(m_selected.begin(), m_selected.end());
	return result;
}

void AutoSelect::init_req_count(int count_req)
{
	m_req_per_qid.clear();
	for (Nodes::const_iterator ni = m_candidate.begin(); ni != m_candidate.end(); ++ni) {
		m_req_per_qid[ni->qid] = std::min<int>(m_req_per_qid[ni->qid]+1, count_req);
	}
}

void AutoSelect::init_select_info(SelectInfo& info)
{
	assert(m_req_per_qid.size() > 0);
	for (CountPerQid::iterator qi = m_req_per_qid.begin(); qi != m_req_per_qid.end(); ++qi) {
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

int AutoSelect::detect_comb(SelectInfo& info, int pos)
{
	if (detected(info)) {
		return count_overlap(info);
	}
	if (pos >= m_candidate.size()) {
		return INT_MAX;
	}

	Node node = m_candidate[pos];

	SelectInfo info_s(info);
	SelectInfo info_u(info);
	int ol_s = INT_MAX, ol_u = INT_MAX;
	if (!info[node.qid].done()) {
		info_s[node.qid].push(node.kid);
		ol_s = detect_comb(info_s, pos+1);
		if (ol_s == m_min_overlap) {
			info = info_s;
			return ol_s;
		}
	}
	{
		ol_u = detect_comb(info_u, pos+1);
		if (ol_u == m_min_overlap) {
			info = info_u;
			return ol_u;
		}
	}
	if (ol_s <= ol_u) {
		info = info_s;
		return ol_s;
	} else {
		info = info_u;
		return ol_u;
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
