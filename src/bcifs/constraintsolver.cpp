#include <stdexcept>
#include "bcifs/constraintsolver.h"
#include "bcifs/formalmatrix.h"
#include "bcifs/formalcoef.h"

namespace BCIFS {

void ConstraintSolver::solve(FormalMatrix& lhs, FormalMatrix& rhs) {
    if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) {
        throw std::runtime_error("Matrices have not the same size");
    }

    for (std::size_t i = 0; i < lhs.rows(); i++) {
        for (std::size_t j = 0; j < lhs.cols(); j++) {
            FormalCoefRef& varL = lhs.get(i, j);
            FormalCoefRef& varR = rhs.get(i, j);

            FormalCoef::unify(varL, varR);
        }
    }
}

}
