#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <limits>

class Matrix {
public:
    Matrix();
    ~Matrix();
    void readInMatrix();
    void writeOutSolution();
    int memoCalcMaxSum(const int j, const int oldRow, const int newRow);
    void iterCalcMaxSum();
    void printMatrix();
    void printMemo();
private:
    void initMemo();
    const int calcPenalty(const int currRow, const int oldRow, const int newRow);
    const size_t index(const int &i, const int &j);
    const size_t index(const int &i, const int &j, const int &k);

    int rows = 0;
    int cols = 0;
    int sentinelVal = std::numeric_limits<int>::min();
    int *matrix = nullptr;
    int *memo = nullptr;
};

#endif // MATRIX_HPP