#include <stdexcept>
#include <iostream>
#include "bcifs/formalmatrix.h"

namespace BCIFS {

FormalMatrix::FormalMatrix(std::size_t rows, std::size_t cols) :
        m_rows(rows), m_cols(cols), m_coefficients(rows, std::vector<FormalCoefRef>(cols, FormalCoef::zero())) {}

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

} // BCIFS