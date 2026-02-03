#ifndef AUTOFRAC_BOOLEANMATRIX_H
#define AUTOFRAC_BOOLEANMATRIX_H

#include <string>
#include <vector>

namespace BCIFS {
class CoefPool;

class FormalMatrix;

class BooleanMatrix {
public:
    BooleanMatrix();
    BooleanMatrix(std::size_t rows, std::size_t cols);
    std::size_t rows() const { return m_rows; }
    std::size_t cols() const { return m_cols; }
    bool get(std::size_t row, std::size_t col) const { return m_coefficients[row][col]; }
    void set(std::size_t row, std::size_t col, bool coef) { m_coefficients[row][col] = coef; }
    void setIdentity();
    /**
     * get the line number of the first true value in the given column
     *
     * @param col the column in which to find the value (must be < cols())
     * @return the line number of the first true value in the given column
     */
    std::size_t lineOfTrueInColumn(std::size_t col);
    BooleanMatrix transitived() const;
    BooleanMatrix removedMultipleRows() const;
    BooleanMatrix removedMultipleCols() const;
    BooleanMatrix operator*(const BooleanMatrix& other) const;
    void print() const;
    std::string toString() const;
    FormalMatrix toFormalMatrix(CoefPool* pool) const;
    void squareAndFillByTrue();
    BooleanMatrix transposed() const;

private:
    bool areSameRows(std::size_t row1, std::size_t row2) const;
    bool areSameCols(std::size_t col1, std::size_t col2) const;
    bool rowContainsTrue(std::size_t row) const;
    bool colContainsTrue(std::size_t col) const;

private:
    std::size_t m_rows;
    std::size_t m_cols;
    std::vector<std::vector<bool>> m_coefficients;
};

} // BCIFS

#endif //AUTOFRAC_BOOLEANMATRIX_H
