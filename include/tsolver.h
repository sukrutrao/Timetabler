#ifndef TSOLVER_H
#define TSOLVER_H

#include <vector>
#include "algorithms/Alg_OLL.h"
#include "MaxSAT.h"
#include "mtl/Vec.h"

using namespace Minisat;
using namespace openwbo;

/**
 * @brief      Class for solver.
 * 
 * This class inherits from the OLL algorithm of Open WBO.
 * This reimplements the search() and the weighted()
 * functions as tSearch() and tWeighted(). Most of the code is
 * identical. The differences are that tSearch() does not print
 * the output to stdout and exit, instead, it returns the model.
 * tWeighted() also does not print to stdout, when the solver
 * terminates, it simply returns.
 */
class TSolver : public OLL {
public:
	TSolver(int, int);
	std::vector<lbool> tSearch();
	void tWeighted();
};

#endif