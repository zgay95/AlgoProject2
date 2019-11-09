#include "Matrix.hpp"

#include <iostream>
#include <algorithm>

/**
 * Default Constructor: Reads in the number rows, columns, 
 *                      initializes the matrix and memo data members.
 */
Matrix::Matrix()
{
    const std::string fileName = "input.txt";
    std::ifstream input(fileName);

    // Check if the input file was successfully opened.
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
    
    input.close();
}

/**
 * Destructor: Deletes the data members located in the heap.
 */
Matrix::~Matrix()
{
    deleteMatrix();
    deleteMemo();
}

/**
 * initMatrix: Initialize the matrix data member to the values read in from input.
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
 * initMemo: Initialize the memo data member to the initial sentinel value "negative infinity".
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
 * deleteMatrix: Deletes matrix from the heap.
 */
void Matrix::deleteMatrix()
{
    if (matrix != nullptr) {
        delete[] matrix;
    }
}

/**
 * deleteMemo: Deletes memo from the heap.
 */
void Matrix::deleteMemo()
{
    if (memo != nullptr) {
        delete[] memo;
    }
}

/**
 * memoCalcMaxScore: Calculates the maximum possible score possible for the input matrix.
 *                   This function is based on the reccurrence described in our project report
 *                   submitted with this code. Refer to the report for more detail.
 *                   
 */
int Matrix::memoCalcMaxScore(const int &j, const int &oldRow, const int &newRow)
{
    // Recursive check, make sure not out of bounds of matrix.
    if (j < cols) {
        // If the current reccurrence has already been calculated, return it.
        if (memo[index(j, oldRow, newRow)].val > sentinelVal) {
            return memo[index(j, oldRow, newRow)].val;
        }

        // Set up a temporary array to store all paths from the current column.
        MemoElement *tempArr = new MemoElement[rows];
        for (int i = 0; i < rows; i++) {
            // Recursively trace every path for every element in the current column.
            tempArr[i] = MemoElement(
                matrix[index(i, j)] - calcPenalty(i, oldRow, newRow) + memoCalcMaxScore(j + 1, newRow, i),
                newRow,
                i);
        }

        // Find the path which resulted in the max score, store it in memo.
        memo[index(j, oldRow, newRow)] = *std::max_element(tempArr, tempArr + rows);

        delete[] tempArr;
        
        return memo[index(j, oldRow, newRow)].val;
    }

    // If we are outside the range of columns, then simply return 0, to not effect prior recursive calls.
    return 0;
}

/**
 * calcPenalty: Calculate the penalty to move to the current row based
 *              on the oldRow and the newRow.
 * 
 *              The penalty is 2 * number or rows outside the range [oldRow, newRow].
 */
const int Matrix::calcPenalty(const int &currRow, const int &oldRow, const int &newRow)
{
    const int minRow = std::min(oldRow, newRow);
    const int maxRow = std::max(oldRow, newRow);

    return 2 * ((currRow < minRow) ? (minRow - currRow) : 
                (currRow > maxRow) ? (currRow - maxRow) : 0);
}

/**
 * writeOutSolution: Output both the maximum score, and the path taken to reach it.
 */
void Matrix::writeOutSolution()
{
    const std::string fileName = "output.txt";
    std::ofstream output(fileName);

    output << memoCalcMaxScore(0, 0, 0) << "\n";
    pathToMaxScore(output);

    output.close();
}

/**
 * pathToMaxSum: Outputs the path taken in order to achieve the maximum score.
 */
void Matrix::pathToMaxScore(std::ofstream &output)
{
    int oldRow = 0, newRow = 0, temp = 0;
    // For each column, output the matrix row taken to reach the max score.
    for (int i = 0; i < cols; i++) {
        output << memo[index(i, oldRow, newRow)].newRow << " ";

        // Use the oldRow and newRow MemoElement members to trace the next matrix element.
        temp = oldRow;
        oldRow = memo[index(i, oldRow, newRow)].oldRow;
        newRow = memo[index(i, temp, newRow)].newRow;
    }
}

/**
 * printMatrix: Prints a list representation of the 2D array matrix data member.
 */
void Matrix::printMatrix()
{
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
 * printMemo: Prints a list representation of the 3D array memo data member.
 */
void Matrix::printMemo()
{
    for (int i = 0; i < cols; i++) {
        std::cout << "[";
        for (int j = 0; j < rows; j++) {
            std::cout << "[";
            for (int k = 0; k < rows; k++) {
                /**
                 * If the MemoElement value is equal to the sentinel value "negative infinity",
                 * outputVal is set to "-oo" to represent negative infinity, rather than some
                 * arbitrarily large negative number. Otherwise, set outputVal to the MemoElement value.
                 */
                std::string outputVal = memo[index(i, j, k)].val == sentinelVal ? 
                    "-oo" : std::to_string(memo[index(i, j, k)].val);

                // Formatting checks.
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
            
            // Another format check.
            if (j != rows - 1) {
                std::cout << ", ";
            } 
        }
        std::cout << "]\n";
    }
    std::cout << "\n";
}

/**
 * index (2D): Returns the proper address offset for a contiguous array 
 *             that represents a 2D array.
 * 
 *             Example: A[index(i, j)] is eqivalent to A[i][j] if A were a 2D array.
 * 
 *             Note that this implementation works exclusively for the matrix data member.
 * 
 *             This implementation is used in order to guarentee contiguous memory on the heap,
 *             rather than the typical array of pointer implementation for 2D arrays on the heap.
 *             This in turn improves the caching performance of the program.
 */
const size_t Matrix::index(const int &i, const int &j)
{
    return (i * cols) + j;
}

/**
 * index (3D): Similar to above; returns the proper address offset for a contiguous array
 *             that represents a 3D array.
 * 
 *             Example: A[index(i, j, k)] is eqivalent to A[i][j][k] if A were a 3D array.
 * 
 *             Note that this implementation works exclusively for the memo data member.
 */
const size_t Matrix::index(const int &i, const int &j, const int &k)
{
    return (i * rows * rows) + (j * rows) + k;
}
