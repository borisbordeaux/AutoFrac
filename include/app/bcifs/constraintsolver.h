#ifndef AUTOFRAC_CONSTRAINTSOLVER_H
#define AUTOFRAC_CONSTRAINTSOLVER_H

namespace BCIFS {

class CoefPool;
class FormalMatrix;
class FormalCoef;

class ConstraintSolver {
public:
    static void solve(const FormalMatrix& lhs, const FormalMatrix& rhs, CoefPool& pool);
};

} // BCIFS

#endif //AUTOFRAC_CONSTRAINTSOLVER_H
