#include "autosel.h"
#include <algorithm>

int AutoSelect::detect(Nodes& selected, const Nodes& candidate, const Qids& qids, int count_req)
{
	m_candidate.assign(candidate.begin(), candidate.end());
	std::random_shuffle(m_candidate.begin(), m_candidate.end());

	m_require_qids.assign(qids.begin(), qids.end());

	init_req_count(count_req);

	m_selected.clear();
	int result = detect_comb(0);

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

int AutoSelect::detect_comb(int pos)
{
	if (pos >= m_candidate.size()) {
		return 0;
	}
	m_selected.push_back(m_candidate[pos]);
	return detect_comb(pos+1);
}
