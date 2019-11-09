Analysis Of Algorithms Project 2 Report
William Horn, Zachary Gay, Allen Anjal

	How you can break down a problem instance of finding a maximum score for a matrix A with n rows and m columns, where your previous two selected rows are r_old and r_new into smaller instances of the same problem?  Your answer should include how you calculate the maximum score for the original problem based on the solution(s) to the subproblem(s).

You can break down a problem instance recursively by starting in the first column, and then recursively checking the maximum value you can get on every path to every element. In other words, for every element in the first column, take that element, subtract it’s penalty to reach that element based on r_old and r_new (which will both be 0 to start), and then recursively check the next column based on r_new, and the chosen row, adding the maximum possible sum from that recursion. Continue until the recursive calls are out of bounds of the matrix, which the recursion should simply return 0 to not effect prior sums. Then, as the calls return, take the maximum value calculated for each element back to the initial call, and that will result in the maximum sum. This recurrence can be described mathematically below:

M(j,r_old,r_new )={█(max{█(A[1][j]-p(1,r_old,r_new )+M(j+1,r_new,1)@A[2][j]-p(2,r_old,r_new )+M(j+1,r_new,2)@⋮@A[n][j]-p(n,r_old,r_new )+M(j+1,r_new,n) )┤      if j≤m@ @                                                         0                                             otherwise)┤

	What are the base cases of this recurrence?

The base cases of this recurrence are when the recursive calls have exceeded the column bound of the matrix for which they should return zero as these calls will not contribute to the maximum score. You could also consider the last column the base case if you define the recurrence differently.

	What data structure would you use to recognize repeated problems?   You should describe both the abstract data structure, as well as its implementation.

We would use a 3D array (say, memo) to recognize repeated problems, because the recurrence is dependent on three variables. I would first initialize the array to -∞ (the most minimum integer value in practice), and if an element of the 3D array is not equal to -∞, then the result has been calculated and return that element. When a M(i,j,k) recursive call is computed, store the result into memo[i][j][k].

 
	Give pseudocode for a memoized dynamic programming algorithm to find the maximum score for a matrix A with n rows and m columns, with previously selected rows of r_old and r_new.

The below pseudo-code assumes that A,n, and m are all

Input: A - An [n x m] matrix we wish to find the maximum traversal score.
Input: n - The number of rows of A.
Input: m - The number of columns of A.
Output: maxScore - The maximum score you can attain traversing A.
1		Algorithm: MemoMatrixTraversalDriver
2		memoMatrix = Array(m,n,n);
3		Initialize all elements of memoMatrix to -∞;
4		return MemoMatrixTraversal(0,0,0);

Input: j – Currently transversed column.
Input: r_old - the row visited two columns prior.
Input: r_new - the row visited in the prior columns.
Output: maxScore - the maximum score you can attain traversing the matrix.
1		Algorithm: MemoMatrixTraversal
2		if j<m then
3			if memoMatrix[j][r_old ][r_new ]> -∞ then
4				return memoMatrix[j][r_old][r_new];
5			end
6			tempArray = Array(n);
7			for i=0 to n-1 do
8				tempArray[i] = A[i][j]-Penalty(i,r_old,r_new)+MemoMatrixTraversal(j + 1,r_new,i);		
9			end
10		memoMatrix[j][r_old][r_new] = max(tempArray);
11		return memoMatrix[j][r_old][r_new];
12	end
13	return 0;
 

Input: i - row being travelled to.
Input: r_old- the row visited two columns prior.
Input: r_new- the row visited in the prior columns.
Output: penalty - the penalty for moving to row i.
1		Algorithm: Penalty
2		r_min = min(r_old,r_new);
3		r_max = max(r_old,r_new);
4		if i<r_min  then
5			return 2*(r_min-i);
6		else if r_min  ≤i≤r_max  then
7			return 0;
8		else
9			return 2*(i-r_max);
10	end

	What is the worst-case time complexity of your memoized algorithm?

Because memoized dynamic programming algorithms time complexity is dependent on the memorization data structure as: (# cells * cost per cell) + initialization cost, there are n^2 m cells, it takes Θ(1) to fill a cell (because cells are dependent on the base cases, which take constant time to fill), and Θ(n^2 m) time to fill the 3D array with -∞ values, thus an overall time of Θ(〖2n〗^2 m)=Θ(n^2 m).

	Give pseudocode for an iterative algorithm to find the maximum score for a matrix A with n rows and m columns, with previously selected rows of r_old and r_new. This algorithm does not need to have a reduced space complexity relative to the memoized solution.



	Describe at least one advantage and disadvantage of the iterative algorithm, and indicate which implementation you believe is better overall.

With a good implementation, the it

Give pseudocode for an algorithm that identifies which entries (rows) to select in each column in order to achieve the maximum score. Your algorithm may be iterative or recursive.
