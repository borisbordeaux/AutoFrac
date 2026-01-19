#ifndef MASSSPRINGSYSTEM_ANGULARSPRING_H
#define MASSSPRINGSYSTEM_ANGULARSPRING_H

namespace mss {

class Mass;

class AngularSpring {
public:
    AngularSpring(Mass& m1, Mass& m2, Mass& m3, float k);

    void applyForces();

    inline Mass const& m1() const { return m_m1; }

    inline Mass const& m2() const { return m_m2; }

    inline Mass const& m3() const { return m_m3; }

    inline float k() const { return m_k; }

    inline void setK(float k) { m_k = k; }

private:
    Mass& m_m1;
    Mass& m_m2;
    Mass& m_m3;
    float m_k;
};

} // mss

#endif //MASSSPRINGSYSTEM_ANGULARSPRING_H
