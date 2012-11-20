#include "autosel.h"
#include <algorithm>
#include "assert.h"

int AutoSelect::detect(Nodes& selected, const Nodes& candidate, const Qids& qids, int count_req)
{
	m_min_overlap = calc_min_overlap(candidate, qids.size() * count_req);

	m_candidate.assign(candidate.begin(), candidate.end());
	std::random_shuffle(m_candidate.begin(), m_candidate.end());

	m_require_qids.assign(qids.begin(), qids.end());

	init_req_count(count_req);

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
}

int AutoSelect::calc_min_overlap(const Nodes& candidate, int total_count_req) const
{
	std::set<Kid> kidset;
	for (Nodes::const_iterator ni = candidate.begin(); ni != candidate.end(); ++ni) {
		kidset.insert(ni->kid);
	}
	int count_kid = kidset.size();
	return (count_kid <= total_count_req)? 0 : (count_kid - total_count_req);
}

int AutoSelect::detect_comb(SelectInfo& info, int pos)
{
	if (pos >= m_candidate.size()) {
		return INT_MAX;
	}
	m_selected.push_back(m_candidate[pos]);
	return detect_comb(info, pos+1);
}
