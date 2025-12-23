#include "app/massspringsystem/mass.h"

namespace mss {

Mass::Mass(const BCIFS::FormalMatrix& pos, float damping) : m_pos(pos), m_velocity(pos.rows()), m_acceleration(pos.rows()), m_damping(damping) {}

void Mass::applyForce(const std::vector<float>& force) {
    for (std::size_t i = 0; i < m_velocity.size(); i++) {
        m_acceleration[i] += force[i];
    }
}

void Mass::update() {
    // update velocity
    for (std::size_t i = 0; i < m_velocity.size(); i++) {
        m_velocity[i] *= m_damping; // damping system
        m_velocity[i] += m_acceleration[i]; // update velocity
        m_acceleration[i] = 0.0f; // clear forces
        // update position
        if (m_pos.get(i, 0)->type() == BCIFS::CoefType::VAR) {
            m_pos.get(i, 0)->setValue(m_pos.get(i, 0)->value() + m_velocity[i]);
        }
    }
}
} // mss