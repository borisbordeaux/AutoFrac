#include <stdexcept>
#include <iostream>
#include "app/bcifs/booleanmatrix.h"
#include "app/bcifs/formalmatrix.h"
#include "core/log.h"

namespace BCIFS {

BooleanMatrix::BooleanMatrix() : BooleanMatrix(0, 0) {}

BooleanMatrix::BooleanMatrix(std::size_t rows, std::size_t cols) :
        m_rows(rows), m_cols(cols), m_coefficients(rows, std::vector<bool>(cols, false)) {}

void BooleanMatrix::setIdentity() {
    if (m_rows == m_cols) {
        for (std::size_t i = 0; i < m_cols; i++) {
            m_coefficients[i][i] = true;
        }
    }
}

std::size_t BooleanMatrix::lineOfTrueInColumn(std::size_t col) {
    for (std::size_t row = 0; row < rows(); row++) {
        if (m_coefficients[row][col]) {
            return row;
        }
    }
    throw std::runtime_error("No true value in this column");
}

BooleanMatrix BooleanMatrix::transitived() const {
    BooleanMatrix res = *this;
    for (std::size_t i = 0; i < rows(); i++) {
        res = res * res;
    }
    return res;
}

BooleanMatrix BooleanMatrix::removedMultipleRows() const {
    std::vector<std::size_t> indicesToAdd { 0 };
    for (std::size_t currentRow = 1; currentRow < m_rows; currentRow++) {
        bool different = true;
        for (std::size_t rowBefore = 0; rowBefore < currentRow; rowBefore++) {
            if (this->areSameRows(currentRow, rowBefore)) {
                different = false;
            }
        }
        if (different) {
            indicesToAdd.push_back(currentRow);
        }
    }
    BooleanMatrix res(indicesToAdd.size(), m_cols);
    for (std::size_t rowIndex = 0; rowIndex < indicesToAdd.size(); rowIndex++) {
        for (std::size_t col = 0; col < m_cols; col++) {
            res.set(rowIndex, col, this->get(indicesToAdd[rowIndex], col));
        }
    }
    return res;
}

BooleanMatrix BooleanMatrix::removedMultipleCols() const {
    std::vector<std::size_t> indicesToAdd { 0 };
    for (std::size_t currentCol = 1; currentCol < m_cols; currentCol++) {
        bool different = true;
        for (std::size_t colBefore = 0; colBefore < currentCol; colBefore++) {
            if (this->areSameCols(currentCol, colBefore)) {
                different = false;
            }
        }
        if (different) {
            indicesToAdd.push_back(currentCol);
        }
    }
    BooleanMatrix res(m_rows, indicesToAdd.size());
    for (std::size_t row = 0; row < m_rows; row++) {
        for (std::size_t colIndex = 0; colIndex < indicesToAdd.size(); colIndex++) {
            res.set(row, colIndex, this->get(row, indicesToAdd[colIndex]));
        }
    }
    return res;
}

bool BooleanMatrix::areSameRows(std::size_t row1, std::size_t row2) const {
    bool res = true;
    for (std::size_t col = 0; col < m_cols; col++) {
        if (this->get(row1, col) != this->get(row2, col)) {
            res = false;
        }
    }
    return res;
}

bool BooleanMatrix::areSameCols(std::size_t col1, std::size_t col2) const {
    bool res = true;
    for (std::size_t row = 0; row < m_rows; row++) {
        if (this->get(row, col1) != this->get(row, col2)) {
            res = false;
        }
    }
    return res;
}

BooleanMatrix BooleanMatrix::operator*(const BooleanMatrix& other) const {
    if (m_cols != other.m_rows)
        throw std::runtime_error("Dimensions of boolean matrices not compatible for multiplication");

    BooleanMatrix result(m_rows, other.m_cols);

    for (std::size_t i = 0; i < m_rows; i++) {
        for (std::size_t j = 0; j < other.m_cols; j++) {
            bool sum = false;
            for (std::size_t k = 0; k < m_cols; k++) {
                bool prod = this->get(i, k) && other.get(k, j);
                sum = sum || prod;
            }
            result.set(i, j, sum);
        }
    }

    return result;
}

void BooleanMatrix::print() const {
    Core::LOG_DEBUG(this->toString());
}

std::string BooleanMatrix::toString() const {
    std::string res;
    for (std::size_t i = 0; i < m_rows; i++) {
        for (std::size_t j = 0; j < m_cols; j++) {
            res += this->get(i, j) ? "1 " : "0 ";
        }
        res += "\n";
    }
    res += "\n";
    return res;
}

FormalMatrix BooleanMatrix::toFormalMatrix() const {
    FormalMatrix res(m_rows, m_cols);
    for (std::size_t row = 0; row < m_rows; row++) {
        for (std::size_t col = 0; col < m_cols; col++) {
            res.set(row, col, this->get(row, col) ? FormalCoef::one() : FormalCoef::zero());
        }
    }
    return res;
}

void BooleanMatrix::squareAndFillByTrue() {
    // square matrix
    if (m_rows > m_cols) {
        m_cols = m_rows;
        // resize rows vectors if needed
        for (std::size_t row = 0; row < m_rows; row++) {
            m_coefficients[row].resize(m_cols, false);
        }
    }
    // fill by true where it is needed
    for (std::size_t row = 0; row < m_rows; row++) {
        if (!this->rowContainsTrue(row)) {
            for (std::size_t col = 0; col < m_cols; col++) {
                if (!this->colContainsTrue(col)) {
                    this->set(row, col, true);
                    break;
                }
            }
        }
    }
}

bool BooleanMatrix::rowContainsTrue(std::size_t row) const {
    for (std::size_t col = 0; col < m_cols; col++) {
        if (this->get(row, col)) {
            return true;
        }
    }
    return false;
}

bool BooleanMatrix::colContainsTrue(std::size_t col) const {
    for (std::size_t row = 0; row < m_rows; row++) {
        if (this->get(row, col)) {
            return true;
        }
    }
    return false;
}

BooleanMatrix BooleanMatrix::transposed() const {
    BooleanMatrix result(m_cols, m_rows);

    for (std::size_t row = 0; row < m_rows; row++) {
        for (std::size_t col = 0; col < m_cols; col++) {
            result.set(col, row, this->get(row, col));
        }
    }

    return result;
}

}
// BCIFS
