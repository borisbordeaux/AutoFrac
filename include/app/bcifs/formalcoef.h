#ifndef AUTOFRAC_FORMALCOEF_H
#define AUTOFRAC_FORMALCOEF_H

#include <memory>

namespace BCIFS {

enum class CoefType {
    ZERO, ONE, VAR, CONST
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
    static FormalCoefRef constant(float v);

    static FormalCoefRef multiply(const FormalCoefRef& v1, const FormalCoefRef& v2);
    static FormalCoefRef multiplyValues(const FormalCoefRef& v1, const FormalCoefRef& v2);

    static FormalCoefRef add(const FormalCoefRef& v1, const FormalCoefRef& v2);
    static FormalCoefRef addValues(const FormalCoefRef& v1, const FormalCoefRef& v2);

    inline float value() const { return const_cast<FormalCoef*>(this)->findRoot()->m_value; }

    inline float* valueRef() { return &this->findRoot()->m_value; }

    inline CoefType type() const { return const_cast<FormalCoef*>(this)->findRoot()->m_type; }

    void setValue(float value) { this->findRoot()->m_value = value; }
    void setType(CoefType type) { this->findRoot()->m_type = type; }

    void setInitialized() { this->findRoot()->m_initialized = true; }

    inline bool initialized() const { return const_cast<FormalCoef*>(this)->findRoot()->m_initialized; }

    FormalCoefRef findRoot();
    static void unify(const FormalCoefRef& c1, const FormalCoefRef& c2);

    std::string toString(bool showAddress = false) const;

private:
    CoefType m_type;
    float m_value;
    bool m_initialized;

    std::weak_ptr<FormalCoef> m_parent;
};

} // BCIFS

#endif //AUTOFRAC_FORMALCOEF_H
