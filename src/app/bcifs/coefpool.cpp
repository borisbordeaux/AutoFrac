#include "app/bcifs/coefpool.h"

#include "app/bcifs/utils.h"

namespace BCIFS {

CoefPool::CoefPool() {
    this->reset();
}

std::size_t CoefPool::makeVar(float value) {
    return this->addCoef(CoefKind::VAR, value);
}

std::size_t CoefPool::makeConst(float value) {
    return this->addCoef(CoefKind::CONST, value);
}

void CoefPool::unify(std::size_t index1, std::size_t index2) {
    std::size_t ra = this->root(index1);
    std::size_t rb = this->root(index2);
    if (m_coefs[ra].kind() != CoefKind::VAR && m_coefs[rb].kind() != CoefKind::VAR) {
        if (m_coefs[ra].value() != m_coefs[rb].value()) {
            throw std::runtime_error("Contradiction : different constant values");
        }
    } else if (m_coefs[ra].kind() != CoefKind::VAR && m_coefs[rb].kind() == CoefKind::VAR) {
        m_coefs[rb].setParent(ra);
    } else if (m_coefs[ra].kind() == CoefKind::VAR && m_coefs[rb].kind() != CoefKind::VAR) {
        m_coefs[ra].setParent(rb);
    } else {
        // two VAR : identify them
        // we can replace v2 by v1 (they point to the same value)
        m_coefs[rb].setParent(ra);
    }
}

float CoefPool::value(std::size_t index) const {
    return m_coefs[this->root(index)].value();
}

float* CoefPool::valueRef(std::size_t index) {
    return m_coefs[this->root(index)].valueRef();
}

void CoefPool::setValue(std::size_t index, float value) {
    if (this->root(index) == 0 || this->root(index) == 1) {
        throw std::logic_error("Cannot modify ZERO or ONE.");
    }
    m_coefs[this->root(index)].setValue(value);
}

void CoefPool::setKind(std::size_t index, CoefKind kind) {
    if (this->root(index) == 0 || this->root(index) == 1) {
        throw std::logic_error("Cannot modify ZERO or ONE.");
    }
    m_coefs[this->root(index)].setKind(kind);
}

CoefKind CoefPool::getKind(std::size_t index) const {
    return m_coefs[this->root(index)].kind();
}

void CoefPool::setInitialized(std::size_t index) {
    if (this->root(index) == 0 || this->root(index) == 1) {
        throw std::logic_error("Cannot modify ZERO or ONE.");
    }
    m_coefs[this->root(index)].setInitialized();
}

bool CoefPool::isConst(std::size_t index) const {
    return m_coefs[this->root(index)].kind() == CoefKind::CONST;
}

bool CoefPool::isZero(std::size_t index) const {
    return m_coefs[this->root(index)].kind() == CoefKind::ZERO;
}

bool CoefPool::isOne(std::size_t index) const {
    return m_coefs[this->root(index)].kind() == CoefKind::ONE;
}

bool CoefPool::isVar(std::size_t index) const {
    return m_coefs[this->root(index)].kind() == CoefKind::VAR;
}

bool CoefPool::isInitialized(std::size_t index) const {
    return m_coefs[this->root(index)].initialized();
}

std::size_t CoefPool::mul(std::size_t a, std::size_t b) const {
    std::size_t ra = this->root(a);
    std::size_t rb = this->root(b);
    // 0 * anything = anything * 0 = 0
    if (m_coefs[ra].kind() == CoefKind::ZERO || m_coefs[rb].kind() == CoefKind::ZERO) { return 0; }
    // 1 * anything = anything
    if (m_coefs[ra].kind() == CoefKind::ONE) { return rb; }
    // anything * 1 = anything
    if (m_coefs[rb].kind() == CoefKind::ONE) { return ra; }
    // other cases are not supposed to appear
    // var * var, var * const, const * var, const * const
    throw std::logic_error("Cannot apply product VAR * VAR.");
}

std::size_t CoefPool::add(std::size_t a, std::size_t b) const {
    std::size_t ra = this->root(a);
    std::size_t rb = this->root(b);
    // 0 + anything = anything
    if (m_coefs[ra].kind() == CoefKind::ZERO) { return rb; }
    // anything + 0 = anything
    if (m_coefs[rb].kind() == CoefKind::ZERO) { return ra; }
    // other cases are not supposed to appear
    // var + var, var + 1, 1 + var, 1 + 1, var + const, const + var, const + const
    throw std::logic_error("Cannot apply sum VAR + VAR, VAR + 1, 1 + VAR nor 1 + 1.");
}

std::string CoefPool::toString(std::size_t index, bool showAddress) const {
    std::size_t root = this->root(index);
    std::stringstream ss;
    ss << "[" << utils::toString(m_coefs[root].value());
    if (showAddress) {
        ss << " - " << root;
    }
    ss << "]";
    return ss.str();
}

std::size_t CoefPool::root(std::size_t index) const {
    if (m_coefs[index].parent() != index) {
        return root(m_coefs[index].parent());
    }
    return index;
}

void CoefPool::reset() {
    m_coefs.clear();
    // index 0 = ZERO
    this->addCoef(CoefKind::ZERO, 0.0f);
    // index 1 = ONE
    this->addCoef(CoefKind::ONE, 1.0f);
}

std::size_t CoefPool::addCoef(CoefKind kind, float v) {
    std::size_t index = m_coefs.size();
    m_coefs.emplace_back(index, v, kind);
    return index;
}

} // BCIFS
