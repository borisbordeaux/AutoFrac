#ifndef AUTOFRAC_COEFPOOL_H
#define AUTOFRAC_COEFPOOL_H

#include <string>
#include <vector>

namespace BCIFS {

class FormalCoef;

enum class CoefKind : uint8_t {
    VAR, CONST, ZERO, ONE
};

class Coef {
public:
    Coef(std::size_t parent, float value, CoefKind kind) : m_parent(parent), m_value(value), m_kind(kind), m_initialized(false) {}
    std::size_t parent() const { return m_parent; }
    float value() const { return m_value; }
    float* valueRef() { return &m_value; }
    CoefKind kind() const { return m_kind; }
    bool initialized() const { return m_initialized; }
    void setParent(std::size_t parent) { m_parent = parent; }
    void setValue(float value) { m_value = value; }
    void setKind(CoefKind kind) { m_kind = kind; }
    void setInitialized() { m_initialized = true; }

private:
    std::size_t m_parent;
    float m_value;
    CoefKind m_kind;
    bool m_initialized;

};

class CoefPool {
public:
    CoefPool(); // insert 0 et 1
    std::size_t makeVar(float value);
    std::size_t makeConst(float value);
    void unify(std::size_t index1, std::size_t index2);
    float value(std::size_t index) const;
    float* valueRef(std::size_t index);
    void setValue(std::size_t index, float value);
    void setKind(std::size_t index, CoefKind kind);
    CoefKind getKind(std::size_t index) const;
    void setInitialized(std::size_t index);
    bool isConst(std::size_t index) const;
    bool isZero(std::size_t index) const;
    bool isOne(std::size_t index) const;
    bool isVar(std::size_t index) const;
    bool isInitialized(std::size_t index) const;
    std::size_t mul(std::size_t a, std::size_t b) const;
    std::size_t add(std::size_t a, std::size_t b) const;
    std::string toString(std::size_t index, bool showAddress = false) const;
    std::size_t root(std::size_t index) const;
    void reset();

private:
    std::size_t addCoef(CoefKind kind, float v);

private:
    std::vector<Coef> m_coefs;
};

} // BCIFS

#endif //AUTOFRAC_COEFPOOL_H
