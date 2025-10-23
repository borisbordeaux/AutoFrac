#include <stdexcept>
#include <sstream>
#include "bcifs/formalcoef.h"

namespace BCIFS {

FormalCoef::FormalCoef(float value) : FormalCoef(CoefType::VAR, value) {}

FormalCoef::FormalCoef(CoefType coefType, float value) : m_type(coefType), m_value(value), m_initialized(false) {}

FormalCoefRef FormalCoef::zero() {
    static FormalCoefRef zero = std::make_shared<FormalCoef>(CoefType::ZERO, 0.0f);
    return zero;
}

FormalCoefRef FormalCoef::one() {
    static FormalCoefRef one = std::make_shared<FormalCoef>(CoefType::ONE, 1.0f);
    return one;
}

FormalCoefRef FormalCoef::var(float v) {
    return std::make_shared<FormalCoef>(CoefType::VAR, v);
}

FormalCoefRef FormalCoef::mult(const FormalCoefRef& v1, const FormalCoefRef& v2) {
    FormalCoefRef rv1 = v1->findRoot();
    FormalCoefRef rv2 = v2->findRoot();
    // 0 * anything = anything * 0 = 0
    if (rv1->m_type == CoefType::ZERO || rv2->m_type == CoefType::ZERO) { return FormalCoef::zero(); }
    // 1 * anything = anything
    if (rv1->m_type == CoefType::ONE) { return rv2; }
    // anything * 1 = anything
    if (rv2->m_type == CoefType::ONE) { return rv1; }
    // other cases are not supposed to appear
    // var * var
    throw std::logic_error("Cannot apply product VAR * VAR.");
}

FormalCoefRef FormalCoef::add(FormalCoefRef const& v1, FormalCoefRef const& v2) {
    FormalCoefRef rv1 = v1->findRoot();
    FormalCoefRef rv2 = v2->findRoot();
    // 0 + anything = anything
    if (rv1->m_type == CoefType::ZERO) { return rv2; }
    // anything + 0 = anything
    if (rv2->m_type == CoefType::ZERO) { return rv1; }
    // other cases are not supposed to appear
    // var + var, var + 1, 1 + var, 1 + 1
    throw std::logic_error("Cannot apply sum VAR + VAR, VAR + 1, 1 + VAR nor 1 + 1.");
}

FormalCoefRef FormalCoef::findRoot() {
    FormalCoefRef self = shared_from_this();
    FormalCoefRef parent = m_parent.lock();
    if (!parent) return self;
    if (parent.get() == this) return parent;
    FormalCoefRef root = parent->findRoot();
    m_parent = root; // compression
    return root;
}

void FormalCoef::unify(const FormalCoefRef& c1, const FormalCoefRef& c2) {
    FormalCoefRef ra = c1->findRoot();
    FormalCoefRef rb = c2->findRoot();
    if (ra->type() != CoefType::VAR && rb->type() != CoefType::VAR) {
        if (ra->value() != rb->value()) {
            throw std::runtime_error("Contradiction : different constant values");
        }
    } else if (ra->type() != CoefType::VAR && rb->type() == CoefType::VAR) {
        rb->m_parent = ra;
    } else if (ra->type() == CoefType::VAR && rb->type() != CoefType::VAR) {
        ra->m_parent = rb;
    } else {
        // two VAR : identify them
        // we can replace v2 by v1 (they point to the same value)
        rb->m_parent = ra;
    }
}

std::string FormalCoef::toString(bool showAddress) const {
    FormalCoefRef root = const_cast<FormalCoef*>(this)->findRoot();
    std::stringstream ss;
    ss << "[" << root->m_value;
    if (showAddress) {
        ss << " - " << root.get();
    }
    ss << "]";
    return ss.str();
}

//std::string FormalCoef::toString() const {
//    std::stringstream ss;
//    ss << "[" << m_root->toString() << " - " << m_root.get() << "]";
//    return ss.str();
//}

} // BCIFS