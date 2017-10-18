#ifndef TSOLVER_H
#define TSOLVER_H

#include <vector>
#include "algorithms/Alg_OLL.h"
#include "MaxSAT.h"
#include "mtl/Vec.h"

using namespace Minisat;
using namespace openwbo;

class TSolver : public OLL {
public:
	TSolver(int, int);
	std::vector<lbool> tSearch();
	void tWeighted();

};

#endif