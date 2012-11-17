#include "autosel.h"

int AutoSelect::detect(Nodes& selected, const Nodes& candidate, const Qids& qids, int count_req)
{
	m_require_qids.assign(qids.begin(), qids.end());
	return 0;
}
