#ifndef AUTOFRAC_SUBDIVISIONPOINT_H
#define AUTOFRAC_SUBDIVISIONPOINT_H

#include <glm/vec3.hpp>

#include "formalmatrix.h"

namespace BCIFS {

class SubdivisionPoint {
public:
    SubdivisionPoint(arma::mat T, FormalMatrix posBary);
    const arma::mat& T() const { return m_T; }
    const FormalMatrix& posBary() const { return m_posBary; }
    FormalMatrix& posBary() { return m_posBary; }
    glm::vec3 posR3() const;

private:
    arma::mat m_T;
    FormalMatrix m_posBary;
};

} // BCIFS

#endif //AUTOFRAC_SUBDIVISIONPOINT_H
