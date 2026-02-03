#include "app/bcifs/constraintsolver.h"

#include "app/bcifs/coefpool.h"
#include "app/bcifs/formalcoef.h"
#include "app/bcifs/formalmatrix.h"

#include <stdexcept>

namespace BCIFS {

void ConstraintSolver::solve(const FormalMatrix& lhs, const FormalMatrix& rhs, CoefPool& pool) {
    if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) {
        throw std::runtime_error("Matrices have not the same size");
    }

    for (std::size_t i = 0; i < lhs.rows(); i++) {
        for (std::size_t j = 0; j < lhs.cols(); j++) {
            const FormalCoef& varL = lhs.get(i, j);
            const FormalCoef& varR = rhs.get(i, j);
            pool.unify(varL.index(), varR.index());
        }
    }
}

}
