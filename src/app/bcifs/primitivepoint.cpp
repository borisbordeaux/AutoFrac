#include "app/bcifs/primitivepoint.h"

BCIFS::PrimitivePoint::PrimitivePoint(arma::mat T, arma::mat posBary, StateID stateId, std::size_t matrixId, std::size_t columnId) :
    m_T(std::move(T)), m_posBary(std::move(posBary)), m_stateId(stateId), m_matrixId(matrixId), m_columnId(columnId) {}

glm::vec3 BCIFS::PrimitivePoint::posR3() const {
    glm::vec3 res;
    arma::mat posR3 = m_T * m_posBary;
    res.x = posR3.at(0, 0);
    res.y = posR3.at(1, 0);
    res.z = posR3.at(2, 0);
    return res;
}