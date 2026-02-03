#ifndef MASSSPRINGSYSTEM_MASS_H
#define MASSSPRINGSYSTEM_MASS_H

#include "app/bcifs/formalmatrix.h"

namespace mss {

class Mass {
public:
    explicit Mass(const BCIFS::FormalMatrix& pos, float damping);
    void applyForce(const std::vector<float>& force);
    void update();
    const BCIFS::FormalMatrix& position() const { return m_pos; }
    BCIFS::FormalMatrix& position() { return m_pos; }

private:
    BCIFS::FormalMatrix m_pos;
    std::vector<float> m_velocity;
    std::vector<float> m_acceleration;
    float m_damping;
};

} // mss

#endif //MASSSPRINGSYSTEM_MASS_H
