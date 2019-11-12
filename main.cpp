#include "MatrixTraverser.hpp"

#include <cstdlib>
#include <ctime>

int main()
{
    MatrixTraverser mt;

    // Randomly call either the iterative or memoized solution.
    srand(time(nullptr));
    rand() % 2 ? mt.iterCalcMaxScore(0, 0, 0) : mt.memoCalcMaxScore(0, 0, 0);

    mt.writeOutSolution();
    return 0;
}
