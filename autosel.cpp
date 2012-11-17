#include "autosel.h"

int AutoSelect::detect(const Nodes& nodes, const Qids& qids, int count_req)
{
	m_require_qids.assign(qids.begin(), qids.end());
	return 0;
}
