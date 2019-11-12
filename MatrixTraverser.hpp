#ifndef MATRIX_TRAVERSER_HPP
#define MATRIX_TRAVERSER_HPP

#include <limits>
#include <fstream>
#include <string>

/**
 * MatrixTraverser: Class built to calculate the maximum score attainable for an input matrix
 *                  based on the specifications defined in the project documentation.
 *                  We decided to implement both the memoized and iterative solution to this
 *                  problem, and randomly call one in our driver. 
 */
class MatrixTraverser 
{
public:
    MatrixTraverser();
    ~MatrixTraverser();
    void writeOutSolution();
    int memoCalcMaxScore(const int &col, const int &oldRow, const int &newRow);
    int iterCalcMaxScore(const int &startCol, const int &startOldRow, const int & startNewRow);
    void printMatrix();
    void printMemo();
    
private:
    static const int sentinelVal = std::numeric_limits<int>::min();

    /**
     * MemoElement: Class which stores the maximum score attainable given the oldRow and newRow to index
     *              the element in the memo data structure. The currRow and nextRow data members are required
     *              to trace the path for the maximum score for our particular implementation.
     */
    struct MemoElement
    {
        MemoElement() : score(sentinelVal), currRow(-1), nextRow(-1) { }
        MemoElement(int score) : score(score), currRow(-1), nextRow(-1) { }
        MemoElement(int score, int currRow, int nextRow) : score(score), currRow(currRow), nextRow(nextRow) { }

        MemoElement& operator=(MemoElement right) {
            score = right.score;
            currRow = right.currRow;
            nextRow = right.nextRow;
            
            return *this;
        }

        bool operator<(MemoElement right) {
            return score < right.score;
        }

        int score;
        int currRow;
        int nextRow;
    };

    void initMatrix(std::ifstream &input);
    void initMemo();
    void deleteMatrix();
    void deleteMemo();
    const int calcPenalty(const int &currRow, const int &oldRow, const int &newRow);
    void pathToMaxScore(std::ofstream &output);
    const size_t index(const int &i, const int &j);
    const size_t index(const int &i, const int &j, const int &k);

    int rows = 0;
    int cols = 0;
    int *matrix = nullptr;
    MemoElement *memo = nullptr;
};

#endif // MATRIX_TRAVERSER_HPP