#include <stdexcept>
#include <iostream>
#include <random>
#include "app/bcifs/formalmatrix.h"
#include "app/bcifs/booleanmatrix.h"
#include "core/log.h"

namespace BCIFS {

FormalMatrix::FormalMatrix() : FormalMatrix(0, 0) {}

FormalMatrix::FormalMatrix(std::size_t rows, std::size_t cols) :
        m_rows(rows), m_cols(cols), m_coefficients(rows, std::vector<FormalCoefRef>(cols, FormalCoef::zero())) {}

FormalMatrix::FormalMatrix(std::size_t rows, std::size_t cols, float value) :
        m_rows(rows), m_cols(cols), m_coefficients(rows, std::vector<FormalCoefRef>(cols)) {
    if (m_cols == 1 && m_rows == 1) {
        this->set(0, 0, FormalCoef::one());
    } else {
        for (std::size_t row = 0; row < m_rows; row++) {
            for (std::size_t col = 0; col < m_cols; col++) {
                this->set(row, col, FormalCoef::var(value));
            }
        }
    }
}

FormalMatrix::FormalMatrix(std::size_t rows, std::size_t cols, bool initRandom) :
        m_rows(rows), m_cols(cols), m_coefficients(rows, std::vector<FormalCoefRef>(cols)) {
    if (initRandom) {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
        for (std::size_t col = 0; col < m_cols; col++) {
            float last = 1.0f;
            for (std::size_t i = 0; i < m_rows - 1; i++) {
                float val = (2.0f + distribution(generator)) / (2.0f * static_cast<float>(m_rows));
                this->set(i, col, FormalCoef::var(val));
                last -= val;
            }
            this->set(m_rows - 1, col, FormalCoef::var(last));
        }
    }
}

FormalMatrix::FormalMatrix(const std::vector<std::vector<float>>& values, CoefType type) {
    m_rows = values.size();
    m_cols = values[0].size();
    for (std::size_t row = 0; row < m_rows; row++) {
        m_coefficients.emplace_back();
        for (std::size_t col = 0; col < m_cols; col++) {
            if (type == CoefType::CONST) {
                m_coefficients[row].push_back(FormalCoef::constant(values[row][col]));
            } else {
                m_coefficients[row].push_back(FormalCoef::var(values[row][col]));
            }
        }
    }
}

FormalMatrix FormalMatrix::operator*(const FormalMatrix& other) const {
    if (m_cols != other.m_rows)
        throw std::runtime_error("Dimensions not compatible for multiplication");
    FormalMatrix result(m_rows, other.m_cols);
    for (std::size_t i = 0; i < m_rows; i++) {
        for (std::size_t j = 0; j < other.m_cols; j++) {
            FormalCoefRef sum = FormalCoef::zero();
            for (std::size_t k = 0; k < m_cols; k++) {
                FormalCoefRef prod = FormalCoef::multiply(this->get(i, k), other.get(k, j));
                sum = FormalCoef::add(sum, prod);
            }
            result.set(i, j, sum);
        }
    }
    return result;
}

FormalMatrix FormalMatrix::multiplyValues(const FormalMatrix& other) const {
    if (m_cols != other.m_rows)
        throw std::runtime_error("Dimensions not compatible for multiplication");
    FormalMatrix result(m_rows, other.m_cols);
    for (std::size_t i = 0; i < m_rows; i++) {
        for (std::size_t j = 0; j < other.m_cols; j++) {
            FormalCoefRef sum = FormalCoef::zero();
            for (std::size_t k = 0; k < m_cols; k++) {
                FormalCoefRef prod = FormalCoef::multiplyValues(this->get(i, k), other.get(k, j));
                sum = FormalCoef::addValues(sum, prod);
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
            res += this->get(i, j)->toString(showAddress) + " ";
        }
        res += "\n";
    }
    res += "\n";
    return res;
}

void FormalMatrix::print(bool showAddress) const {
    Core::LOG_INFO("Matrix\n"+this->toString(showAddress));
}

BooleanMatrix FormalMatrix::toBooleanMatrix() const {
    BooleanMatrix res(m_rows, m_cols);
    for (std::size_t row = 0; row < m_rows; row++) {
        for (std::size_t col = 0; col < m_cols; col++) {
            res.set(row, col, m_coefficients[row][col]->type() == CoefType::ONE);
        }
    }
    return res;
}

void FormalMatrix::setRandomValuesOnFreeCoefs() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    for (std::size_t col = 0; col < m_cols; col++) {
        for (std::size_t row = 0; row < m_rows; row++) {
            if (!this->get(row, col)->initialized() && this->get(row, col)->type() == CoefType::VAR) {
                float val = distribution(generator);
                this->get(row, col)->setValue(val);
                this->get(row, col)->setInitialized();
            }
        }
    }
    this->setSumToOne();
}

void FormalMatrix::setSumToOne() {
    for (std::size_t col = 0; col < m_cols; col++) {
        float sum = 0.0f;
        std::vector<std::size_t> differentValueIndices;
        std::vector<float*> valueReferences;

        for (std::size_t row = 0; row < m_rows; row++) {
            sum += this->get(row, col)->value();
            if (this->get(row, col)->type() == CoefType::VAR) {
                if (std::find(valueReferences.begin(), valueReferences.end(), this->get(row, col)->valueRef()) == valueReferences.end()) {
                    valueReferences.push_back(this->get(row, col)->valueRef());
                }
            }
        }

        for (float* f: valueReferences) {
            *f = *f / sum;
        }
    }
}

arma::mat FormalMatrix::toMat() const {
    arma::mat res(m_rows, m_cols);
    for (std::size_t row = 0; row < m_rows; row++) {
        for (std::size_t col = 0; col < m_cols; col++) {
            res.at(row, col) = m_coefficients[row][col]->value();
        }
    }
    return res;
}

void FormalMatrix::concatenateColumns(const FormalMatrix& matrix) {
    if (matrix.m_rows != m_rows) { return; }
    for (std::size_t row = 0; row < m_rows; row++) {
        m_coefficients[row].resize(m_cols + matrix.m_cols, FormalCoef::zero());
        for (std::size_t col = m_cols; col < m_coefficients[row].size(); col++) {
            m_coefficients[row][col] = matrix.get(row, col - m_cols);
        }
    }
    m_cols += matrix.m_cols;
}

size_t FormalMatrix::indexOf(const FormalMatrix& columnMatrix) {
    for (std::size_t col = 0; col < m_cols; col++) {
        bool sameCol = true;
        for (std::size_t row = 0; row < m_rows; row++) {
            if (m_coefficients[row][col]->findRoot() != columnMatrix.get(row, 0)->findRoot()) {
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
    FormalMatrix res(m_rows, 1);
    for (std::size_t row = 0; row < m_rows; row++) {
        res.set(row, 0, this->get(row, indexCol));
    }
    return res;
}

} // BCIFS