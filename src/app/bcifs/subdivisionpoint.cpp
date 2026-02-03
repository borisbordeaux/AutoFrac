#include "app/bcifs/subdivisionpoint.h"

namespace BCIFS {

SubdivisionPoint::SubdivisionPoint(arma::mat T, FormalMatrix posBary) :
    m_T(std::move(T)), m_posBary(std::move(posBary)) {}

glm::vec3 SubdivisionPoint::posR3() const {
    glm::vec3 res;
    arma::mat posR3 = m_T * m_posBary.toMat();
    res.x = posR3.at(0, 0);
    res.y = posR3.at(1, 0);
    res.z = posR3.at(2, 0);
    return res;
}

} // BCIFS
