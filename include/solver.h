#ifndef SOLVER_H
#define SOLVER_H

#include "algorithms/Alg_OLL.h"
#include "mtl/Vec.h"

class Solver : public OLL {
public:
	Solver(int, int);
	vec<lbool> search();
	vec<lbool> weighted();
};

#endif