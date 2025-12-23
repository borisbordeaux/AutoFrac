#ifndef MASSSPRINGSYSTEM_SPRING_H
#define MASSSPRINGSYSTEM_SPRING_H

namespace mss {

class Mass;

class Spring {
public:
    Spring(Mass& m1, Mass& m2, float k, float length);

    void applyForces();

    inline Mass const& m1() const { return m_m1; }

    inline Mass const& m2() const { return m_m2; }

    inline float k() const { return m_k; }

    inline float length() const { return m_length; }

    inline void setK(float k) { m_k = k; }

    inline void setLength(float length) { m_length = length; }

private:
    Mass& m_m1;
    Mass& m_m2;
    float m_k;
    float m_length;
};

} // mss

#endif //MASSSPRINGSYSTEM_SPRING_H
