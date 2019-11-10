#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <limits>
#include <fstream>
#include <string>

class Matrix {
private:
    static const int sentinelVal = std::numeric_limits<int>::min();

    struct MemoElement {
        MemoElement() : val(sentinelVal), oldRow(-1), newRow(-1) { }
        MemoElement(int val) : val(val), oldRow(-1), newRow(-1) { }
        MemoElement(int val, int oldRow, int newRow) : val(val), oldRow(oldRow), newRow(newRow) { }

        MemoElement& operator=(MemoElement right) {
            val = right.val;
            oldRow = right.oldRow;
            newRow = right.newRow;
            
            return *this;
        }

        bool operator<(MemoElement right) {
            return val < right.val;
        }

        int val;
        int oldRow;
        int newRow;
    };

public:
    Matrix();
    ~Matrix();
    void writeOutSolution();
    int memoCalcMaxScore(const int &col, const int &oldRow, const int &newRow);
    int iterCalcMaxScore();
    void printMatrix();
    void printMemo();
    
private:
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

#endif // MATRIX_HPP
