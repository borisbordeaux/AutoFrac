#ifndef AUTOFRAC_PRIMITIVEPOINT_H
#define AUTOFRAC_PRIMITIVEPOINT_H

#include <glm/vec3.hpp>
#include <armadillo>

#include "state.h"

namespace BCIFS {

class PrimitivePoint {
public:
    PrimitivePoint(arma::mat T, arma::mat posBary, StateID stateId, std::size_t matrixId, std::size_t columnId);
    const arma::mat& T() const { return m_T; }
    const arma::mat& posBary() const { return m_posBary; }
    arma::mat& posBary() { return m_posBary; }
    glm::vec3 posR3() const;
    StateID stateId() const { return m_stateId; }
    std::size_t matrixId() const { return m_matrixId; }
    std::size_t columnId() const { return m_columnId; }

private:
    arma::mat m_T;
    arma::mat m_posBary;
    StateID m_stateId;
    std::size_t m_matrixId;
    std::size_t m_columnId;
};

} // BCIFS

#endif //AUTOFRAC_PRIMITIVEPOINT_H
