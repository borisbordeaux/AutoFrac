#ifndef AUTOFRAC_FORMALMATRIX_H
#define AUTOFRAC_FORMALMATRIX_H

#include <vector>
#include <memory>
#include <armadillo>

#include "coefpool.h"
#include "formalcoef.h"

namespace BCIFS {

class BooleanMatrix;

class FormalMatrix {
public:
    FormalMatrix(std::size_t rows, std::size_t cols, CoefPool* pool);
    explicit FormalMatrix(std::size_t rows, std::size_t cols, float value, CoefPool* pool);
    explicit FormalMatrix(std::size_t rows, std::size_t cols, bool initRandom, CoefPool* pool);
    FormalMatrix(const std::vector<std::vector<float>>& values, CoefKind type, CoefPool* pool);

    inline std::size_t rows() const { return m_rows; }

    inline std::size_t cols() const { return m_cols; }

    inline const FormalCoef& get(std::size_t row, std::size_t col) const { return m_coefficients[row * m_cols + col]; }

    inline FormalCoef& get(std::size_t row, std::size_t col) { return m_coefficients[row * m_cols + col]; }

    inline void set(std::size_t row, std::size_t col, FormalCoef coef) { this->get(row,col) = coef; }

    FormalMatrix operator*(const FormalMatrix& other) const;

    std::string toString(bool showAddress = false) const;
    void print(bool showAddress = false) const;
    void printDebug(bool showAddress = false) const;

    BooleanMatrix toBooleanMatrix() const;

    void setRandomValuesOnFreeCoefs(bool setInBarycentricSpace = true);
    void setRandomValues();
    void setSumToOne();

    arma::mat toMat() const;

    void concatenateColumns(const FormalMatrix& matrix);

    std::size_t indexOf(const FormalMatrix& columnMatrix) const;

    FormalMatrix getCol(std::size_t indexCol) const;

    FormalMatrix variableEmbeddingMatrix() const;
    FormalMatrix variableMatrix() const;

    // bool isConst(std::size_t index) const;
    // bool isZero(std::size_t index) const;
    bool isOne(std::size_t row, std::size_t col) const;
    bool isVar(std::size_t row, std::size_t col) const;
    float value(std::size_t row, std::size_t col) const;
    float* valueRef(std::size_t row, std::size_t col) const;
    void setValue(std::size_t row, std::size_t col, float value);
    // bool isInitialized(std::size_t index) const;

private:
    std::size_t m_rows;
    std::size_t m_cols;
    std::vector<FormalCoef> m_coefficients;
    CoefPool* m_pool;
};

} // BCIFS

#endif //AUTOFRAC_FORMALMATRIX_H
