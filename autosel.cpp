#include "autosel.h"

int AutoSelect::detect(Nodes& selected, const Nodes& candidate, const Qids& qids, int count_req)
{
	m_selected.clear();
	m_candidate.assign(candidate.begin(), candidate.end());
	m_require_qids.assign(qids.begin(), qids.end());
	int result = detect_comb(0);
	selected.assign(m_selected.begin(), m_selected.end());
	return result;
}

int AutoSelect::detect_comb(int pos)
{
	return 0;
}
