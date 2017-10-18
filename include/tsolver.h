#ifndef TSOLVER_H
#define TSOLVER_H

#include <vector>
//#include "algorithms/Alg_OLL.h"
#include "mtl/Vec.h"
class OLL;

using namespace Minisat;
using namespace openwbo;

class TSolver : public OLL {
public:
	TSolver(int, int);
	bool search();
	bool checkAllTrue(std::vector<Lit>);
	vec<lbool> weighted();

};

#endif