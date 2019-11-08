#include "Matrix.hpp"

#include <iostream>
#include <string>
#include <algorithm>

/**
 * 
 */
Matrix::Matrix()
{
    const std::string fileName = "input.txt";
    std::ifstream input(fileName);

    if (input) {
        input >> rows;
        input >> cols;

        initMatrix(input);
        initMemo();
    }
    else {
        throw std::runtime_error("Error opening " + fileName + 
        ". Ensure file is in the proper directory and run again.\n");
    }
}

/**
 * 
 */
Matrix::~Matrix()
{
    deleteMatrix();
    deleteMemo();
}

/**
 * 
 */
void Matrix::initMatrix(std::ifstream &input)
{
    matrix = new int[rows * cols];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            input >> matrix[index(i, j)];
        }
    }
}

/**
 * 
 */
void Matrix::initMemo()
{
    memo = new MemoElement[cols * rows * rows];

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            for (int k = 0; k < rows; k++) {
                memo[index(i, j, k)] = sentinelVal;
            }
        }
    }
}

/**
 * 
 */
void Matrix::deleteMatrix()
{
    if (matrix != nullptr) {
        delete[] matrix;
    }
}

/**
 * 
 */
void Matrix::deleteMemo()
{
    if (memo != nullptr) {
        delete[] memo;
    }
}

/**
 * 
 */
int Matrix::memoCalcMaxSum(const int &j, const int &oldRow, const int &newRow)
{
    if (j < cols) {
        if (memo[index(j, oldRow, newRow)].val > sentinelVal) {
            return memo[index(j, oldRow, newRow)].val;
        }

        MemoElement *tempArr = new MemoElement[rows];
        for (int i = 0; i < rows; i++) {
            tempArr[i] = MemoElement(
                matrix[index(i, j)] - calcPenalty(i, oldRow, newRow) + memoCalcMaxSum(j + 1, newRow, i),
                i,
                newRow);
        }

        memo[index(j, oldRow, newRow)] = *std::max_element(tempArr, tempArr + rows);

        delete[] tempArr;

        return memo[index(j, oldRow, newRow)].val;
    }
    return 0;
}

/**
 * 
 */
void Matrix::writeOutSolution()
{
    const std::string fileName = "output.txt";
    std::ofstream output(fileName);

    output << memoCalcMaxSum(0, 0, 0) << "\n";

}

/**
 * 
 */
void Matrix::printMatrix()
{
    std::cout << "Max Sum: " << memoCalcMaxSum(0, 0, 0) << "\n";

    for (int i = 0; i < rows; i++) {
        std::cout << "[";
        for (int j = 0; j < cols; j++) {
            if (j != cols - 1) {
                std::cout << matrix[index(i, j)] << ", ";
            } 
            else {
                std::cout << matrix[index(i, j)];
            }
        }
        std::cout << "]\n";
    }
    std::cout << "\n";
}

/**
 * 
 */
void Matrix::printMemo()
{
    for (int i = 0; i < cols; i++) {
        std::cout << "[";
        for (int j = 0; j < rows; j++) {
            std::cout << "[";
            for (int k = 0; k < rows; k++) {
                std::string outputVal = memo[index(i, j, k)].val == sentinelVal ? 
                    "-oo" : std::to_string(memo[index(i, j, k)].val);

                if (k != rows - 1) {
                    std::cout << "[" << outputVal << ", " 
                    << memo[index(i, j, k)].oldRow << ", " 
                    << memo[index(i, j, k)].newRow << "], ";
                } 
                else {
                    std::cout << "[" << outputVal << ", " 
                    << memo[index(i, j, k)].oldRow << ", " 
                    << memo[index(i, j, k)].newRow << "]";
                }
            }
            std::cout << "]";
            
            if (j != rows - 1) {
                std::cout << ", ";
            } 
        }
        std::cout << "]\n";
    }
    std::cout << "\n";
}

/**
 * 
 */
const int Matrix::calcPenalty(const int &currRow, const int &oldRow, const int &newRow)
{
    const int minRow = std::min(oldRow, newRow);
    const int maxRow = std::max(oldRow, newRow);

    return 2 * ((currRow < minRow) ? (minRow - currRow) : 
                (currRow > maxRow) ? (currRow - maxRow) : 0);
}

/**
 * 
 */
const size_t Matrix::index(const int &i, const int &j)
{
    return (i * cols) + j;
}

/**
 * 
 */
const size_t Matrix::index(const int &i, const int &j, const int &k)
{
    return (i * rows * rows) + (j * rows) + k;
}
