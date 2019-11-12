#include "MatrixTraverser.hpp"

#include <iostream>
#include <algorithm>

/**
 * Default Constructor: Reads in the number rows, columns, 
 *                      initializes the matrix and memo data members.
 */
MatrixTraverser::MatrixTraverser()
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
MatrixTraverser::~MatrixTraverser()
{
    deleteMatrix();
    deleteMemo();
}

/**
 * initMatrix: Initialize the matrix data member to the values read in from input.
 */
void MatrixTraverser::initMatrix(std::ifstream &input)
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
void MatrixTraverser::initMemo()
{
    memo = new MemoElement[(cols + 1) * rows * rows];

    for (int i = 0; i < cols + 1; i++) {
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
void MatrixTraverser::deleteMatrix()
{
    if (matrix != nullptr) {
        delete[] matrix;
    }
}

/**
 * deleteMemo: Deletes memo from the heap.
 */
void MatrixTraverser::deleteMemo()
{
    if (memo != nullptr) {
        delete[] memo;
    }
}

/**
 * memoCalcMaxScore: Calculates the maximum possible score for the input matrix.
 *                   This function is based on the reccurrence described in our project report
 *                   submitted with this code. Refer to the report for more detail.        
 */
int MatrixTraverser::memoCalcMaxScore(const int &col, const int &oldRow, const int &newRow)
{
    // Recursive check, make sure not out of bounds of matrix.
    if (col < cols) {
        // If the current reccurrence has already been calculated, return it.
        if (memo[index(col, oldRow, newRow)].score > sentinelVal) {
            return memo[index(col, oldRow, newRow)].score;
        }

        // Set up a temporary array to store all paths from the current column.
        MemoElement *tempArr = new MemoElement[rows];
        for (int row = 0; row < rows; row++) {
            // Recursively trace every path for every element in the current column.
            tempArr[row] = MemoElement(
                matrix[index(row, col)] - calcPenalty(row, oldRow, newRow) + memoCalcMaxScore(col + 1, newRow, row),
                newRow,
                row);
        }

        // Find the path which resulted in the max score, store it in memo.
        memo[index(col, oldRow, newRow)] = *std::max_element(tempArr, tempArr + rows);

        delete[] tempArr;
        
        return memo[index(col, oldRow, newRow)].score;
    }

    // If we are outside the range of columns, then simply return 0, to not effect prior recursive calls.
    memo[index(col, oldRow, newRow)].score = 0;
    return memo[index(col, oldRow, newRow)].score;
}

/**
 * iterCalcMaxScore: Calculates the maximum possible score for the input matrix.
 *                   This function is based on iterating through the memo data structure
 *                   opposite the recursive dependencies of the recurrence described in our
 *                   project report submitted with this code. Refer to the report for more detail.
 */
int MatrixTraverser::iterCalcMaxScore(const int &startCol, const int &startOldRow, const int & startNewRow)
{
    // If the start position has already been calculated, return it.
    if (memo[index(startCol, startOldRow, startNewRow)].score > sentinelVal) {
        return memo[index(startCol, startOldRow, startNewRow)].score;
    }

    // Iterate through every column from end to beginning,
    for (int col = cols; col >= startCol; col--) {
        // Iterate for every possible oldRow,
        for (int oldRow = 0; oldRow < rows; oldRow++) {
            // Iterate for every possible newRow,
            for (int newRow = 0; newRow < rows; newRow++) {
                // If the column is the base case, set the score appropriately.
                if (col == cols) {
                    memo[index(col, oldRow, newRow)].score = 0;
                }
                else {
                    // Set up a temporary array to store the maximum score attainable for each element of the current column.
                    MemoElement *tempArr = new MemoElement[rows];
                    // Find the maximum score attainable for all the elements of the current column based on oldRow and newRow.
                    for (int row = 0; row < rows; row++) {
                        tempArr[row] = MemoElement(matrix[index(row, col)] - calcPenalty(row, oldRow, newRow) + memo[index(col + 1, newRow, row)].score,
                            newRow,
                            row);
                    }

                    // Store that maximum score in the memo data structure.
                    memo[index(col, oldRow, newRow)] = *std::max_element(tempArr, tempArr + rows);

                    delete[] tempArr;
                }
            }
        }
    }

    // Once the maximum score is calculated up the the starting position, return it.
    return memo[index(startCol, startOldRow, startNewRow)].score;
}


/**
 * calcPenalty: Calculate the penalty to move to the current row based
 *              on the oldRow and the newRow.
 * 
 *              The penalty is 2 * number or rows outside the range [oldRow, newRow].
 */
const int MatrixTraverser::calcPenalty(const int &currRow, const int &oldRow, const int &newRow)
{
    const int minRow = std::min(oldRow, newRow);
    const int maxRow = std::max(oldRow, newRow);

    return 2 * ((currRow < minRow) ? (minRow - currRow) : 
                (currRow > maxRow) ? (currRow - maxRow) : 0);
}

/**
 * writeOutSolution: Output both the maximum score, and the path taken to reach it.
 */
void MatrixTraverser::writeOutSolution()
{
    const std::string fileName = "output.txt";
    std::ofstream output(fileName);

    // Output the maximum score (must be calculated first)
    output << memo[index(0, 0, 0)].score << "\n";
    // Output the path to attain that maximum score.
    pathToMaxScore(output);

    output.close();
}

/**
 * pathToMaxSum: Outputs the path taken in order to achieve the maximum score.
 */
void MatrixTraverser::pathToMaxScore(std::ofstream &output)
{
    int oldRow = 0, newRow = 0, temp = 0;
    // For each column, output the matrix row taken to reach the max score.
    for (int i = 0; i < cols; i++) {
        output << memo[index(i, oldRow, newRow)].nextRow << " ";

        // Use the oldRow and newRow MemoElement members to trace the next matrix element taken.
        temp = oldRow;
        oldRow = memo[index(i, oldRow, newRow)].currRow;
        newRow = memo[index(i, temp, newRow)].nextRow;
    }
}

/**
 * printMatrix: Prints a list representation of the 2D array matrix data member.
 */
void MatrixTraverser::printMatrix()
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
}

/**
 * printMemo: Prints a list representation of the 3D array memo data member.
 */
void MatrixTraverser::printMemo()
{
    for (int i = 0; i < cols + 1; i++) {
        std::cout << "[";
        for (int j = 0; j < rows; j++) {
            std::cout << "[";
            for (int k = 0; k < rows; k++) {
                /**
                 * If the MemoElement score is equal to the sentinel value "negative infinity",
                 * outputVal is set to "-oo" to represent negative infinity, rather than some
                 * arbitrarily large negative number. Otherwise, set outputVal to the MemoElement score.
                 */
                std::string outputVal = memo[index(i, j, k)].score == sentinelVal ? 
                    "-oo" : std::to_string(memo[index(i, j, k)].score);

                // Formatting checks.
                if (k != rows - 1) {
                    std::cout << "[" << outputVal << ", " 
                        << memo[index(i, j, k)].currRow << ", " 
                        << memo[index(i, j, k)].nextRow << "], ";
                } 
                else {
                    std::cout << "[" << outputVal << ", " 
                        << memo[index(i, j, k)].currRow << ", " 
                        << memo[index(i, j, k)].nextRow << "]";
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
const size_t MatrixTraverser::index(const int &i, const int &j)
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
const size_t MatrixTraverser::index(const int &i, const int &j, const int &k)
{
    return (i * rows * rows) + (j * rows) + k;
}