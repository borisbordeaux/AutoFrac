#ifndef MASSSPRINGSYSTEM_MASS_H
#define MASSSPRINGSYSTEM_MASS_H

#include "vector.h"

namespace mss {

class Mass {
public:
    explicit Mass(Vector const& pos, float damping);

    void applyForce(const std::vector<float>& force);
    void update();

    [[nodiscard]] inline const Vector& position() const { return m_pos; }

    [[nodiscard]] inline Vector& position() { return m_pos; }

    [[nodiscard]] inline float damping() const { return m_damping; }

private:
    Vector m_pos;
    std::vector<float> m_velocity;
    std::vector<float> m_acceleration;
    float m_damping;
};

} // mss

#endif //MASSSPRINGSYSTEM_MASS_H
