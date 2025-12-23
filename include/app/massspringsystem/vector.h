#ifndef MASSSPRINGSYSTEM_VECTOR_H
#define MASSSPRINGSYSTEM_VECTOR_H

#include <vector>
#include "app/bcifs/formalcoef.h"

namespace mss {

class Vector {
public:
    explicit Vector(std::vector<BCIFS::FormalCoefRef> values);

    void add(const std::vector<float>& values);

    [[nodiscard]] inline float at(std::size_t i) const { return m_coordinates[i]->value(); }

    [[nodiscard]] inline std::size_t dim() const { return m_coordinates.size(); }

private:
    std::vector<BCIFS::FormalCoefRef> m_coordinates;
};

} // mss

#endif //MASSSPRINGSYSTEM_VECTOR_H
