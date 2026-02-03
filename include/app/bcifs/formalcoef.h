#ifndef AUTOFRAC_FORMALCOEF_H
#define AUTOFRAC_FORMALCOEF_H

#include <cstddef>

namespace BCIFS {

class FormalCoef {
public:
    explicit FormalCoef(std::size_t index) : m_id(index) {}
    std::size_t index() const { return m_id; };
    static FormalCoef zero() { return FormalCoef(0); }
    static FormalCoef one() { return FormalCoef(1); };

private:
    std::size_t m_id;
};

} // BCIFS

#endif //AUTOFRAC_FORMALCOEF_H
