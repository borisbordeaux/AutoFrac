#ifndef MASSSPRINGSYSTEM_ANGULARSPRING_H
#define MASSSPRINGSYSTEM_ANGULARSPRING_H

namespace BCIFS {

class Mass;

class AngularSpring {
public:
    AngularSpring(Mass& m1, Mass& m2, Mass& m3, float k);
    void applyForces() const;
    const Mass& m1() const { return m_m1; }
    const Mass& m2() const { return m_m2; }
    const Mass& m3() const { return m_m3; }
    float k() const { return m_k; }
    void setK(float k) { m_k = k; }

private:
    Mass& m_m1;
    Mass& m_m2;
    Mass& m_m3;
    float m_k;
};

} // BCIFS

#endif //MASSSPRINGSYSTEM_ANGULARSPRING_H
