#include "app/bcifs/angularspring.h"

#include <armadillo>

#include "app/bcifs/mass.h"

namespace BCIFS {

AngularSpring::AngularSpring(Mass& m1, Mass& m2, Mass& m3, float k) :
    m_m1(m1), m_m2(m2), m_m3(m3), m_k(k) {}

void AngularSpring::applyForces() const {
    arma::mat v1 = m_m1.position().toMat() - m_m2.position().toMat();
    arma::mat v2 = m_m3.position().toMat() - m_m2.position().toMat();

    float l1 = arma::norm(v1);
    float l2 = arma::norm(v2);
    float epsilon = 0.0001f;
    if (std::abs(l1) < epsilon || std::abs(l2) < epsilon) return;

    arma::mat n1 = v1 / l1;
    arma::mat n2 = v2 / l2;

    float cosTheta = std::clamp(arma::dot(n1, n2), -1.0, 1.0);
    float theta = std::acos(cosTheta);
    float dTheta = theta - M_PIf;

    arma::mat axis = arma::cross(n1, n2);
    float axisLen = arma::norm(axis);
    if (std::abs(axisLen) < epsilon) return;

    axis /= axisLen;

    arma::mat f1 = -m_k * dTheta * (cross(n1, axis) / l1);
    arma::mat f3 = -m_k * dTheta * (cross(axis, n2) / l2);
    arma::mat f2 = -(f1 + f3);

    m_m1.applyForce(arma::conv_to<std::vector<float>>::from(f1));
    m_m2.applyForce(arma::conv_to<std::vector<float>>::from(f2));
    m_m3.applyForce(arma::conv_to<std::vector<float>>::from(f3));
}

} // BCIFS
