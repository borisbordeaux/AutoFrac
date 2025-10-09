#ifndef AUTOFRAC_FORMALCOEF_H
#define AUTOFRAC_FORMALCOEF_H

#include <memory>

namespace BCIFS {

enum class CoefType {
    ZERO, ONE, VAR
};

class FormalCoef;

using FormalCoefRef = std::shared_ptr<FormalCoef>;

class FormalCoef : public std::enable_shared_from_this<FormalCoef> {
public:
    explicit FormalCoef(float value);
    FormalCoef(CoefType coefType, float value);

    static FormalCoefRef zero();
    static FormalCoefRef one();
    static FormalCoefRef var(float v);

    static FormalCoefRef mult(const FormalCoefRef& v1, const FormalCoefRef& v2);

    static FormalCoefRef add(const FormalCoefRef& v1, const FormalCoefRef& v2);

    inline float value() const { return const_cast<FormalCoef*>(this)->findRoot()->m_value; }

    inline CoefType type() const { return const_cast<FormalCoef*>(this)->findRoot()->m_type; }

    void setValue(float value) { const_cast<FormalCoef*>(this)->findRoot()->m_value = value; }

    FormalCoefRef findRoot();
    static void unify(const FormalCoefRef& c1, const FormalCoefRef& c2);

    std::string toString(bool showAddress = false) const;

private:
    CoefType m_type;
    float m_value;

    std::weak_ptr<FormalCoef> m_parent;
};

} // BCIFS

#endif //AUTOFRAC_FORMALCOEF_H
