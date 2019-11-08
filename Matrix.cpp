#include "Matrix.hpp"

#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>

Matrix::Matrix()
{
    readInMatrix();
}

Matrix::~Matrix()
{
    if (matrix != nullptr) {
        delete[] matrix;
    }

    if (memo != nullptr) {
        delete[] memo;
    }
}

void Matrix::readInMatrix()
{
    const std::string fileName = "input.txt";
    std::ifstream input(fileName);

    if (input) {
        input >> rows;
        input >> cols;

        matrix = new int[rows * cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                input >> matrix[index(i, j)];
            }
        }

        initMemo();

        std::cout << "Successfully read in matrix!\n";
    } 
    else {
        throw std::runtime_error("Error opening " + fileName + ". Ensure file is in the proper directory and run again.\n");
    }
}

int Matrix::memoCalcMaxSum(const int j, const int oldRow, const int newRow)
{
    if (j < cols) {
        if (memo[index(j, oldRow, newRow)] > sentinelVal) {
            return memo[index(j, oldRow, newRow)];
        }

        int *tempArr = new int[rows];
        for (int i = 0; i < rows; i++) {
            tempArr[i] = matrix[index(i, j)] - calcPenalty(i, oldRow, newRow) + memoCalcMaxSum(j + 1, newRow, i);
        }
        memo[index(j, oldRow, newRow)] = *(std::max_element(tempArr, tempArr + rows));
        delete[] tempArr;
        return memo[index(j, oldRow, newRow)];
    }
    return 0;
}

void Matrix::printMatrix()
{
    std::cout << memoCalcMaxSum(0, 0, 0) << "\n";
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

void Matrix::printMemo()
{
    for (int i = 0; i < cols; i++) {
        std::cout << "[";
        for (int j = 0; j < rows; j++) {
            std::cout << "[";
            for (int k = 0; k < rows; k++) {
                if (k != rows - 1) {
                    std::cout << memo[index(i, j, k)] << ", ";
                } 
                else {
                    std::cout << memo[index(i, j, k)];
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

void Matrix::initMemo()
{
    memo = new int[cols * rows * rows];
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            for (int k = 0; k < rows; k++) {
                memo[index(i, j, k)] = sentinelVal;
            }
        }
    }
}

const int Matrix::calcPenalty(const int currRow, const int oldRow, const int newRow)
{
    int minRow = std::min(oldRow, newRow);
    int maxRow = std::max(oldRow, newRow);

    return 2 * ((currRow < minRow) ? (minRow - currRow) : 
                (currRow > maxRow) ? (currRow - maxRow) : 0);
}

const size_t Matrix::index(const int &i, const int &j)
{
    return (i * cols) + j;
}

const size_t Matrix::index(const int &i, const int &j, const int &k)
{
    return (i * rows * rows) + (j * rows) + k;
}