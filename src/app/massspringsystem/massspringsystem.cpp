#include <iomanip>
#include <algorithm>
#include "app/massspringsystem/massspringsystem.h"

namespace mss {
MassSpringSystem::MassSpringSystem(std::size_t dim) : m_dim(dim) {}

void MassSpringSystem::addMass(const BCIFS::FormalMatrix& pos, float damping) {
    m_masses.emplace_back(pos, damping);
}

void MassSpringSystem::addSpring(std::size_t indexMass1, std::size_t indexMass2, float k, float length) {
    if (indexMass1 < m_masses.size() && indexMass2 < m_masses.size() && indexMass1 != indexMass2) {
        bool presentInOtherDirection = std::find(m_springIndices.begin(), m_springIndices.end(), std::make_pair(indexMass1, indexMass2)) != m_springIndices.end();
        bool presentInOneDirection = std::find(m_springIndices.begin(), m_springIndices.end(), std::make_pair(indexMass2, indexMass1)) != m_springIndices.end();
        if (!presentInOneDirection && !presentInOtherDirection) {
            m_springs.emplace_back(m_masses[indexMass1], m_masses[indexMass2], k, length);
            m_springIndices.emplace_back(indexMass1, indexMass2);
        }
    }
}

void MassSpringSystem::createAngularSprings(float k) {
    std::unordered_map<std::size_t, std::vector<std::size_t>> neighborsMap;
    for (const std::pair<std::size_t, std::size_t>& springIndices : m_springIndices) {
        neighborsMap[springIndices.first].push_back(springIndices.second);
        neighborsMap[springIndices.second].push_back(springIndices.first);
    }
    for (auto& [center, neighbors] : neighborsMap) {
        const size_t n = neighbors.size();
        if (n < 2) continue;
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                Mass& m1 = m_masses[neighbors[i]];
                Mass& m2 = m_masses[center];
                Mass& m3 = m_masses[neighbors[j]];
                m_angularSprings.emplace_back(m1, m2, m3, k);
                m_angularSpringIndices.emplace_back(neighbors[i], center, neighbors[j]);
            }
        }
    }
}

void MassSpringSystem::update() {
    for (Spring& s: m_springs)
        s.applyForces();

    for (AngularSpring& s: m_angularSprings) {
        s.applyForces();
    }

    for (Mass& m: m_masses)
        m.update();
}

void MassSpringSystem::clearSprings() {
    m_springs.clear();
    m_springIndices.clear();
    m_angularSprings.clear();
    m_angularSpringIndices.clear();
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
    std::string res = "d " + std::to_string(m_dim);
    for (mss::Mass const& m: m_masses) {
        res += "\nm";
        for (std::size_t i = 0; i < m_dim; i++) {
            res += " " + MassSpringSystem::toString(m.position().get(i,0)->value());
        }
    }
    for (std::size_t i = 0; i < m_springs.size(); i++) {
        res += "\ns ";
        res += std::to_string(m_springIndices[i].first);
        res += " " + std::to_string(m_springIndices[i].second);
        res += " " + MassSpringSystem::toString(m_springs[i].k());
        res += " " + MassSpringSystem::toString(m_springs[i].length());
    }
    for (std::size_t i = 0; i < m_angularSprings.size(); i++) {
        res += "\nas ";
        res += " " + std::to_string(std::get<0>(m_angularSpringIndices[i]));
        res += " " + std::to_string(std::get<1>(m_angularSpringIndices[i]));
        res += " " + std::to_string(std::get<2>(m_angularSpringIndices[i]));
        res += " " + MassSpringSystem::toString(m_angularSprings[i].k());
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