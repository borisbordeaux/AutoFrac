#include "app/bcifs/spring.h"

#include <armadillo>

#include "app/bcifs/mass.h"

namespace BCIFS {

Spring::Spring(Mass& m1, Mass& m2, float k, float length) : m_m1(m1), m_m2(m2), m_k(k), m_length(length) {}

void Spring::applyForces() const {
    arma::Col<float> direction(m_m1.position().rows());
    for (std::size_t i = 0; i < m_m1.position().rows(); i++) {
        direction[i] = m_m2.position().value(i, 0) - m_m1.position().value(i, 0);
    }
    float x = arma::norm(direction) - m_length;
    direction = arma::normalise(direction);
    direction *= (m_k * x);
    m_m1.applyForce(arma::conv_to<std::vector<float>>::from(direction));
    direction *= -1.f;
    m_m2.applyForce(arma::conv_to<std::vector<float>>::from(direction));
}

} // BCIFS
