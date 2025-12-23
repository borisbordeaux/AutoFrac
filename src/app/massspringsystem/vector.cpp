#include "app/massspringsystem/vector.h"

namespace mss {

Vector::Vector(std::vector<BCIFS::FormalCoefRef> values) : m_coordinates(std::move(values)) {}

void Vector::add(const std::vector<float>& values) {
    for (std::size_t i = 0; i < m_coordinates.size(); i++) {
        if (m_coordinates[i]->type() == BCIFS::CoefType::VAR) {
            m_coordinates[i]->setValue(m_coordinates[i]->value() + values[i]);
        }
    }
}

} // mss