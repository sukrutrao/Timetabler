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
	bool tSearch();
	bool checkAllTrue(std::vector<Lit>);
	vec<lbool> tWeighted();

};

#endif