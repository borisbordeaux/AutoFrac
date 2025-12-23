#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "app/massspringsystem/massspringsystem.h"

namespace mss {
MassSpringSystem::MassSpringSystem(std::size_t dim) : m_dim(dim) {}

void MassSpringSystem::addMass(Vector const& pos, float damping) {
    m_masses.emplace_back(pos, damping);
}

void MassSpringSystem::addSpring(std::size_t indexMass1, std::size_t indexMass2, float k, float length) {
    if (indexMass1 < m_masses.size() && indexMass2 < m_masses.size() && indexMass1 != indexMass2) {
        m_springs.emplace_back(m_masses[indexMass1], m_masses[indexMass2], k, length);
        m_springIndices.emplace_back(indexMass1, indexMass2);
    }
}

void MassSpringSystem::update() {
    for (Spring& s: m_springs)
        s.applyForces();

    for (Mass& m: m_masses)
        m.update();
}

void MassSpringSystem::clearSprings() {
    m_springs.clear();
    m_springIndices.clear();
}

void MassSpringSystem::clearMasses() {
    m_masses.clear();
}

void MassSpringSystem::clear(std::size_t newDim) {
    m_dim = newDim;
    this->clearSprings();
    this->clearMasses();
}

std::string MassSpringSystem::toString() const {
    std::string res;
    res = "d " + std::to_string(m_dim);
    for (mss::Mass const& m: m_masses) {
        res += "\nm";
        for (std::size_t i = 0; i < m_dim; i++) {
            res += " " + MassSpringSystem::toString(m.position().at(i));
        }
    }
    for (std::size_t i = 0; i < m_springs.size(); i++) {
        res += "\ns ";
        res += std::to_string(m_springIndices[i].first);
        res += " " + std::to_string(m_springIndices[i].second);
        res += " " + MassSpringSystem::toString(m_springs[i].k());
        res += " " + MassSpringSystem::toString(m_springs[i].length());
    }
    return res;
}

std::string MassSpringSystem::toString(float value) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(4) << value;
    std::string res = stream.str();
    std::replace(res.begin(), res.end(), ',', '.');
    return res;
}
} // mss