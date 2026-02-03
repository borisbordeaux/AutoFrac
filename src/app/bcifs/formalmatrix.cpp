#include "app/bcifs/formalmatrix.h"

#include "app/bcifs/booleanmatrix.h"
#include "core/log.h"

#include <random>
#include <stdexcept>

namespace BCIFS {

FormalMatrix::FormalMatrix(std::size_t rows, std::size_t cols, CoefPool* pool) :
    m_rows(rows), m_cols(cols), m_coefficients(rows * cols, FormalCoef::zero()), m_pool(pool) {
}

FormalMatrix::FormalMatrix(std::size_t rows, std::size_t cols, float value, CoefPool* pool) :
    m_rows(rows), m_cols(cols), m_coefficients(rows * cols, FormalCoef::zero()), m_pool(pool) {
    if (m_cols == 1 && m_rows == 1) {
        this->set(0, 0, FormalCoef::one());
    } else {
        for (std::size_t row = 0; row < m_rows; row++) {
            for (std::size_t col = 0; col < m_cols; col++) {
                this->set(row, col, FormalCoef(pool->makeVar(value)));
            }
        }
    }
}

FormalMatrix::FormalMatrix(std::size_t rows, std::size_t cols, bool initRandom, CoefPool* pool) :
    m_rows(rows), m_cols(cols), m_coefficients(rows * cols, FormalCoef::zero()), m_pool(pool) {
    if (initRandom) {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
        for (std::size_t col = 0; col < m_cols; col++) {
            float last = 1.0f;
            for (std::size_t i = 0; i < m_rows - 1; i++) {
                float val = (2.0f + distribution(generator)) / (2.0f * static_cast<float>(m_rows));
                this->set(i, col, FormalCoef(pool->makeVar(val)));
                last -= val;
            }
            this->set(m_rows - 1, col, FormalCoef(pool->makeVar(last)));
        }
    }
}

FormalMatrix::FormalMatrix(const std::vector<std::vector<float>>& values, CoefKind type, CoefPool* pool) :
    m_rows(values.size()), m_cols(values[0].size()), m_coefficients(m_rows * m_cols, FormalCoef::zero()), m_pool(pool) {
    for (std::size_t row = 0; row < m_rows; row++) {
        for (std::size_t col = 0; col < m_cols; col++) {
            if (type == CoefKind::CONST) {
                this->get(row, col) = FormalCoef(pool->makeConst(values[row][col]));
            } else {
                this->get(row, col) = FormalCoef(pool->makeVar(values[row][col]));
            }
        }
    }
}

FormalMatrix FormalMatrix::operator*(const FormalMatrix& other) const {
    if (m_cols != other.m_rows)
        throw std::runtime_error("Dimensions not compatible for multiplication");
    FormalMatrix result(m_rows, other.m_cols, m_pool);
    for (std::size_t i = 0; i < m_rows; i++) {
        for (std::size_t j = 0; j < other.m_cols; j++) {
            FormalCoef sum = FormalCoef::zero();
            for (std::size_t k = 0; k < m_cols; k++) {
                FormalCoef prod(m_pool->mul(this->get(i, k).index(), other.get(k, j).index()));
                sum = FormalCoef(m_pool->add(sum.index(), prod.index()));
            }
            result.set(i, j, sum);
        }
    }
    return result;
}

std::string FormalMatrix::toString(bool showAddress) const {
    std::string res;
    for (std::size_t i = 0; i < m_rows; i++) {
        for (std::size_t j = 0; j < m_cols; j++) {
            res += m_pool->toString(this->get(i, j).index(), showAddress) + " ";
        }
        res += "\n";
    }
    res += "\n";
    return res;
}

void FormalMatrix::print(bool showAddress) const {
    Core::LOG_INFO("Matrix\n" + this->toString(showAddress));
}

void FormalMatrix::printDebug(bool showAddress) const {
    Core::LOG_DEBUG("Matrix\n" + this->toString(showAddress));
}

BooleanMatrix FormalMatrix::toBooleanMatrix() const {
    BooleanMatrix res(m_rows, m_cols);
    for (std::size_t row = 0; row < m_rows; row++) {
        for (std::size_t col = 0; col < m_cols; col++) {
            res.set(row, col, m_pool->getKind(this->get(row, col).index()) == CoefKind::ONE);
        }
    }
    return res;
}

void FormalMatrix::setRandomValuesOnFreeCoefs(bool setInBarycentricSpace) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    for (std::size_t col = 0; col < m_cols; col++) {
        for (std::size_t row = 0; row < m_rows; row++) {
            if (!m_pool->isInitialized(this->get(row, col).index()) && m_pool->getKind(this->get(row, col).index()) == CoefKind::VAR) {
                float val = distribution(generator);
                m_pool->setValue(this->get(row, col).index(), val);
                m_pool->setInitialized(this->get(row, col).index());
            }
        }
    }
    if (setInBarycentricSpace) {
        this->setSumToOne();
    }
}

void FormalMatrix::setRandomValues() {
    std::vector<float> averages(m_rows);
    for (std::size_t row = 0; row < m_rows; row++) {
        averages[row] = 0.0f;
        for (std::size_t col = 0; col < m_cols; col++) {
            averages[row] += this->value(row, col);
        }
        averages[row] /= static_cast<float>(m_cols);
    }

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(-2.0f, 2.0f);

    for (std::size_t col = 0; col < m_cols; col++) {
        for (std::size_t row = 0; row < m_rows; row++) {
            if (m_pool->getKind(this->get(row, col).index()) == CoefKind::VAR) {
                float val = distribution(generator);
                m_pool->setValue(this->get(row, col).index(), val + averages[row]);
            }
        }
    }
}

void FormalMatrix::setSumToOne() {
    for (std::size_t col = 0; col < m_cols; col++) {
        float sum = 0.0f;
        std::vector<std::size_t> valueReferences;

        for (std::size_t row = 0; row < m_rows; row++) {
            sum += m_pool->value(this->get(row, col).index());
            if (m_pool->getKind(this->get(row, col).index()) == CoefKind::VAR) {
                if (std::find(valueReferences.begin(), valueReferences.end(), m_pool->root(this->get(row, col).index())) == valueReferences.end()) {
                    valueReferences.push_back(m_pool->root(this->get(row, col).index()));
                }
            }
        }

        for (std::size_t ref : valueReferences) {
            m_pool->setValue(ref, m_pool->value(ref) / sum);
        }
    }
}

arma::mat FormalMatrix::toMat() const {
    arma::mat res(m_rows, m_cols);
    for (std::size_t row = 0; row < m_rows; row++) {
        for (std::size_t col = 0; col < m_cols; col++) {
            res.at(row, col) = m_pool->value(this->get(row, col).index());
        }
    }
    return res;
}

void FormalMatrix::concatenateColumns(const FormalMatrix& matrix) {
    if (matrix.m_rows != m_rows) { return; }
    std::vector<FormalCoef> newCoefs;
    newCoefs.resize(m_rows * (m_cols + matrix.m_cols), FormalCoef::zero());
    for (std::size_t row = 0; row < m_rows; row++) {
        for (std::size_t col = 0; col < m_cols; col++) {
            newCoefs[row * (m_cols + matrix.m_cols) + col] = this->get(row, col);
        }
        for (std::size_t col = m_cols; col < m_cols + matrix.m_cols; col++) {
            newCoefs[row * (m_cols + matrix.m_cols) + col] = matrix.get(row, col - m_cols);
        }
    }
    m_coefficients = std::move(newCoefs);
    m_cols += matrix.m_cols;
}

size_t FormalMatrix::indexOf(const FormalMatrix& columnMatrix) const {
    for (std::size_t col = 0; col < m_cols; col++) {
        bool sameCol = true;
        for (std::size_t row = 0; row < m_rows; row++) {
            if (m_pool->root(this->get(row, col).index()) != m_pool->root(columnMatrix.get(row, 0).index())) {
                sameCol = false;
            }
        }
        if (sameCol) {
            return col;
        }
    }
    throw std::runtime_error("Column not present in the matrix.");
}

FormalMatrix FormalMatrix::getCol(std::size_t indexCol) const {
    FormalMatrix res(m_rows, 1, m_pool);
    for (std::size_t row = 0; row < m_rows; row++) {
        res.set(row, 0, this->get(row, indexCol));
    }
    return res;
}

FormalMatrix FormalMatrix::variableEmbeddingMatrix() const {
    if (m_cols != 1) { throw std::runtime_error("The variable embedding matrix can be computed only for column matrices."); }

    std::vector<std::size_t> differentValueIndices;
    std::vector<std::size_t> valueReferences;
    std::size_t errorValue = m_rows;

    for (std::size_t row = 0; row < m_rows; row++) {
        if (m_pool->getKind(this->get(row, 0).index()) == CoefKind::VAR) {
            auto it = std::find(valueReferences.begin(), valueReferences.end(), m_pool->root(this->get(row, 0).index()));
            std::size_t index = std::distance(valueReferences.begin(), it);
            differentValueIndices.push_back(index);
            if (it == valueReferences.end()) {
                valueReferences.push_back(m_pool->root(this->get(row, 0).index()));
            }
        } else {
            differentValueIndices.push_back(errorValue);
        }
    }

    FormalMatrix res(m_rows, valueReferences.size(), m_pool);
    for (std::size_t row = 0; row < m_rows; row++) {
        std::size_t col = differentValueIndices[row];
        if (col < errorValue) {
            res.set(row, col, FormalCoef::one());
        }
    }

    return res;
}

FormalMatrix FormalMatrix::variableMatrix() const {
    if (m_cols != 1) { throw std::runtime_error("The variable embedding matrix can be computed only for column matrices."); }

    std::vector<std::size_t> differentValueIndices;
    std::vector<std::size_t> valueReferences;

    for (std::size_t row = 0; row < m_rows; row++) {
        if (m_pool->getKind(this->get(row, 0).index()) == CoefKind::VAR) {
            auto it = std::find(valueReferences.begin(), valueReferences.end(), m_pool->root(this->get(row, 0).index()));
            if (it == valueReferences.end()) {
                valueReferences.push_back(m_pool->root(this->get(row, 0).index()));
                differentValueIndices.push_back(row);
            }
        }
    }

    FormalMatrix res(differentValueIndices.size(), 1, m_pool);
    for (std::size_t row = 0; row < res.rows(); row++) {
        res.set(row, 0, this->get(differentValueIndices[row], 0));
    }

    return res;
}

bool FormalMatrix::isOne(std::size_t row, std::size_t col) const {
    return m_pool->isOne(this->get(row, col).index());
}

bool FormalMatrix::isVar(std::size_t row, std::size_t col) const {
    return m_pool->isVar(this->get(row, col).index());
}

float FormalMatrix::value(std::size_t row, std::size_t col) const {
    return m_pool->value(this->get(row, col).index());
}

float* FormalMatrix::valueRef(std::size_t row, std::size_t col) const {
    return m_pool->valueRef(this->get(row, col).index());
}

void FormalMatrix::setValue(std::size_t row, std::size_t col, float value) {
    m_pool->setValue(this->get(row, col).index(), value);
}

} // BCIFS
