#ifndef AUTOFRAC_FORMALMATRIX_H
#define AUTOFRAC_FORMALMATRIX_H

#include <vector>
#include <memory>
#include <glm/detail/type_mat4x4.hpp>
#include <armadillo>
#include "formalcoef.h"

namespace BCIFS {

class BooleanMatrix;

class FormalMatrix {
public:
    FormalMatrix();
    FormalMatrix(std::size_t rows, std::size_t cols);
    explicit FormalMatrix(std::size_t rows, std::size_t cols, float value);
    explicit FormalMatrix(std::size_t rows, std::size_t cols, bool initRandom);

    inline std::size_t rows() const { return m_rows; }

    inline std::size_t cols() const { return m_cols; }

    inline const FormalCoefRef& get(std::size_t row, std::size_t col) const { return m_coefficients[row][col]; }

    inline FormalCoefRef& get(std::size_t row, std::size_t col) { return m_coefficients[row][col]; }

    inline void set(std::size_t row, std::size_t col, FormalCoefRef coef) { m_coefficients[row][col] = std::move(coef); }

    FormalMatrix operator*(const FormalMatrix& other) const;

    void print(bool showAddress = false) const;

    BooleanMatrix toBooleanMatrix() const;

    void setRandomValuesOnFreeCoefs();
    void setSumToOne();

    arma::mat toMat() const;

    void concatenateColumns(const FormalMatrix& matrix);

    std::size_t indexOf(const FormalMatrix& columnMatrix);

private:
    std::size_t m_rows;
    std::size_t m_cols;
    std::vector<std::vector<FormalCoefRef>> m_coefficients;
};

} // BCIFS

#endif //AUTOFRAC_FORMALMATRIX_H
