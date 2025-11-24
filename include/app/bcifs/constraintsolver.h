#ifndef AUTOFRAC_CONSTRAINTSOLVER_H
#define AUTOFRAC_CONSTRAINTSOLVER_H

#include <memory>

namespace BCIFS {

class FormalMatrix;

class FormalCoef;

class ConstraintSolver {
public:
    static void solve(FormalMatrix& lhs, FormalMatrix& rhs);
};

} // BCIFS

#endif //AUTOFRAC_CONSTRAINTSOLVER_H
