#ifndef MASSSPRINGSYSTEM_SYSTEM_H
#define MASSSPRINGSYSTEM_SYSTEM_H

#include <vector>
#include <string>
#include <sstream>

#include "mass.h"
#include "spring.h"

namespace mss {

class Vector;

class MassSpringSystem {
public:
    explicit MassSpringSystem(std::size_t dim = 0);

    void addMass(Vector const& pos, float damping);
    void addSpring(std::size_t indexMass1, std::size_t indexMass2, float k, float length);
    void update();
    void clearSprings();
    void clearMasses();
    void clear(std::size_t newDim = 0);

    inline std::vector<Mass> const& masses() const { return m_masses; }

    inline std::vector<Mass>& masses() { return m_masses; }

    inline std::vector<Spring> const& springs() const { return m_springs; }

    inline std::vector<Spring>& springs() { return m_springs; }

    inline std::size_t dimension() const { return m_dim; }

    std::string toString() const;

private:
    static inline std::string toString(float value);

private:
    std::vector<Mass> m_masses;
    std::vector<Spring> m_springs;
    std::vector<std::pair<std::size_t, std::size_t>> m_springIndices;
    std::size_t m_dim;
};

} // mss

#endif //MASSSPRINGSYSTEM_SYSTEM_H
