#include <vector>
#include <time.h>
using namespace std;

class timer {
public:
	timer() : start(clock()) {}
	double elapsed() { return (clock() - start) / CLOCKS_PER_SEC; }
	void reset() { start = clock(); }
private:
	double start;
};

//Use the following code from Weiss’ book ”Data Structures and Algorithm Analysis in C++”(4th edition) http://users.cis.fiu.edu/~weiss/dsaa_c++4/code/MaxSumTest.cpp


																										  /**
																										  * Cubic maximum contiguous subsequence sum algorithm.
																										  */
int maxSubSum1(const vector<int> & a)
{
	int maxSum = 0;

	for (int i = 0; i < a.size(); ++i)
		for (int j = i; j < a.size(); ++j)
		{
			int thisSum = 0;

			for (int k = i; k <= j; ++k)
				thisSum += a[k];

			if (thisSum > maxSum)
				maxSum = thisSum;
		}

	return maxSum;
}


/**
* Quadratic maximum contiguous subsequence sum algorithm.
*/
int maxSubSum2(const vector<int> & a)
{
	int maxSum = 0;

	for (int i = 0; i < a.size(); ++i)
	{
		int thisSum = 0;
		for (int j = i; j < a.size(); ++j)
		{
			thisSum += a[j];

			if (thisSum > maxSum)
				maxSum = thisSum;
		}
	}

	return maxSum;
}


/**
* Linear-time maximum contiguous subsequence sum algorithm.
*/
int maxSubSum4(const vector<int> & a)
{
	int maxSum = 0, thisSum = 0;

	for (int j = 0; j < a.size(); ++j)
	{
		thisSum += a[j];

		if (thisSum > maxSum)
			maxSum = thisSum;
		else if (thisSum < 0)
			thisSum = 0;
	}

	return maxSum;
}
