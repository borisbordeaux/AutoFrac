#include "app/massspringsystem/mass.h"

namespace mss {

Mass::Mass(Vector const& pos, float damping) : m_pos(pos), m_velocity(pos.dim()), m_acceleration(pos.dim()), m_damping(damping) {}

void Mass::applyForce(const std::vector<float>& force) {
    for (std::size_t i = 0; i < m_velocity.size(); i++) {
        m_acceleration[i] += force[i];
    }
}

void Mass::update() {
    // update velocity
    for (std::size_t i = 0; i < m_velocity.size(); i++) {
        m_velocity[i] *= m_damping; // damping system
        m_velocity[i] += m_acceleration[i];
        m_acceleration[i] = 0.0f;
    }
    // update position
    m_pos.add(m_velocity);
}
} // mss