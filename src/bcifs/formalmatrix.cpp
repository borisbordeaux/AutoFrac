#include <stdexcept>
#include <iostream>
#include <random>
#include "bcifs/formalmatrix.h"
#include "bcifs/booleanmatrix.h"

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

FormalMatrix FormalMatrix::operator*(const FormalMatrix& other) const {
    if (m_cols != other.m_rows)
        throw std::runtime_error("Dimensions not compatible for multiplication");

    FormalMatrix result(m_rows, other.m_cols);

    for (std::size_t i = 0; i < m_rows; i++) {
        for (std::size_t j = 0; j < other.m_cols; j++) {
            FormalCoefRef sum = FormalCoef::zero();
            for (std::size_t k = 0; k < m_cols; k++) {
                FormalCoefRef prod = FormalCoef::mult(this->get(i, k), other.get(k, j));
                sum = FormalCoef::add(sum, prod);
            }
            result.set(i, j, sum);
        }
    }

    return result;
}

void FormalMatrix::print(bool showAddress) const {
    for (std::size_t i = 0; i < m_rows; i++) {
        for (std::size_t j = 0; j < m_cols; j++) {
            std::cout << this->get(i, j)->toString(showAddress) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
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
    std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
    for (std::size_t col = 0; col < m_cols; col++) {
        std::vector<std::size_t> freeValueIndices;
        float totalFreeValues = -1.0f;
        do {
            freeValueIndices.clear();
            float sum = 0.0f;
            for (std::size_t i = 0; i < m_rows; i++) {
                if (this->get(i, col)->type() == CoefType::VAR && !this->get(i, col)->initialized()) {
                    freeValueIndices.push_back(i);
                } else {
                    sum += this->get(i, col)->value();
                }
            }
            if (freeValueIndices.size() == 1) {
                this->get(freeValueIndices[0], col)->setValue(1.0f - sum);
                this->get(freeValueIndices[0], col)->setInitialized();
            } else if (!freeValueIndices.empty()) {
                if (totalFreeValues < 0) {
                    totalFreeValues = static_cast<float>(freeValueIndices.size());
                }
                //fill one value by one value, since several references to the same value might appear
                float val = (2.0f + distribution(generator)) / (2.0f * totalFreeValues);
                this->get(freeValueIndices[0], col)->setValue(val);
                this->get(freeValueIndices[0], col)->setInitialized();
            }
        } while (!freeValueIndices.empty());
    }
}

} // BCIFS