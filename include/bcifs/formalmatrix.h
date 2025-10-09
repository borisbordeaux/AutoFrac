#ifndef AUTOFRAC_FORMALMATRIX_H
#define AUTOFRAC_FORMALMATRIX_H

#include <vector>
#include <memory>
#include "formalcoef.h"

namespace BCIFS {

class FormalMatrix {
public:
    FormalMatrix(std::size_t rows, std::size_t cols);

    inline std::size_t rows() const { return m_rows; }

    inline std::size_t cols() const { return m_cols; }

    inline const FormalCoefRef& get(std::size_t row, std::size_t col) const { return m_coefficients[row][col]; }

    inline FormalCoefRef& get(std::size_t row, std::size_t col) { return m_coefficients[row][col]; }

    inline void set(std::size_t row, std::size_t col, FormalCoefRef coef) { m_coefficients[row][col] = std::move(coef); }

    FormalMatrix operator*(const FormalMatrix& other) const;

    void print(bool showAddress = false) const;

private:
    std::size_t m_rows;
    std::size_t m_cols;
    std::vector<std::vector<FormalCoefRef>> m_coefficients;
};

} // BCIFS

#endif //AUTOFRAC_FORMALMATRIX_H
