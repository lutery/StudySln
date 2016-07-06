#include <iostream>
#include <tuple>
#include <random>
#include <memory>
#include <vector>

using namespace std;

/**
* @brief 算法导论 P66
*/
#pragma region SUBARRAY

//设置最小的整数，由于如果设置最小的值，那么将会导致算法溢出，所以这里设置最小值的一半
const int halfIntMin = INT_MIN / 2;

/**
* @brief 寻找跨越中点的最大连续整数和，这里假定的是最大连续整数和一定跨越中点
*
* @param int* subarray 连续整数数组
* @param int low 下边界
* @param int mid 中点
* @param int high 下边界
*
* @return tuple<int, int, int> 返回寻找到的最大连续整数和的下边界、上边界、最大连续整数和值
*/
tuple<int, int, int> FindMaxCrossingSubArray(int* subarray, int low, int mid, int high)
{
	//左边最小整数和
	int leftSum = halfIntMin;
	int sum = 0;
	int maxLeft = mid;

	//从中点向左边计算做大的连续整数和，并记录其边界
	for (int i = mid; i >= 0; --i)
	{
		sum = sum + subarray[i];
		if (sum > leftSum)
		{
			leftSum = sum;
			maxLeft = i;
		}
	}

	int rightSum = halfIntMin;
	sum = 0;
	int maxRight = mid;

	//从中点向右边计算最大的连续整数和，并记录其边界
	for (int i = mid + 1; i < high; ++i)
	{
		sum = sum + subarray[i];
		if (sum > rightSum)
		{
			rightSum = sum;
			maxRight = i;
		}
	}

	//返回找到的左右边界并将最大的左边和与最大的右边和相加获取跨越中点的最大连续整数和
	return make_tuple(maxLeft, maxRight, leftSum + rightSum);
}

/**
* @brief 寻找最大的连续整数和
*
* @param int* subarray 传入需要计算的整数数组
* @param int low 整数数组的下边界
* @param int high 整数数组的上边界
*
* return tuple<int, int, int> 返回最大连续整数和的下边界、上边界、最大整数和
*/
tuple<int, int, int> FindMaxNumSubArray(int* subarray, int low, int high)
{
	//如果此时的上边界与下边界相等，说明此时整个数组中只有一个元素
	if (high == low)
	{
		//此时的最大整数和是自身
		return make_tuple(low, high, subarray[low]);
	}
	else
	{
		//这里的思想是，最大的连续整数和无非就只有三种情况，出现在左半部分、右半部份、跨越中点

		//计算此时的中间索引
		int mid = (low + high) / 2;
		//寻找左半部分的最大连续整数和
		auto lll = FindMaxNumSubArray(subarray, low, mid);
		//寻找右半部份的最大连续整数和
		auto rrr = FindMaxNumSubArray(subarray, mid + 1, high);
		//寻找跨越中点的最大连续整数和
		auto ccc = FindMaxCrossingSubArray(subarray, low, mid, high);

		if ((std::get<2>(lll) >= std::get<2>(rrr)) && (std::get<2>(lll) >= std::get<2>(ccc)))
		{
			return lll;
		}
		else if ((std::get<2>(rrr) >= std::get<2>(lll) && (std::get<2>(rrr) >= std::get<2>(ccc))))
		{
			return rrr;
		}
		else
		{
			return ccc;
		}
	}
}

#pragma endregion

/**
* @brief 算法导论 P75
*/
#pragma region SUBARRAY_MATRIX

/**
* @brief 符合需求的矩形类
*/
class Matrix
{
public:
	Matrix()
	{
	}

	Matrix(int rows, int cols)
	{
		mpMatrix = new int*[rows];
		for (int i = 0; i < rows; ++i)
		{
			mpMatrix[i] = new int[cols];
			//for (int j = 0; j < cols; ++j)
			//{
				memset(mpMatrix[i], 0, sizeof(int) * cols);
			//	mpMatrix[i][j] = 0;
			//}
		}

		mRows = rows;
		mCols = cols;
	}

	Matrix(int num):Matrix(num, num)
	{
	}

	~Matrix()
	{
		if (mpMatrix != nullptr)
		{
			for (int i = 0; i < mRows; ++i)
			{
				delete[] mpMatrix[i];
			}

			delete[] mpMatrix;

			mpMatrix = nullptr;
		}
	};

	/**
	* @brief 重载方括号运算符
	* 
	* @param const int x 传入行索引
	*
	* @return int* 返回改行索引对应的地址值
	*/
	int* const operator[](const int x)
	{
		return mpMatrix[x];
	}

	shared_ptr<Matrix> const add(shared_ptr<Matrix> pAOther, shared_ptr<Matrix> pBOther)
	{
		for (int i = 0; i < this->mRows; ++i)
		{
			for (int j = 0; j < this->mCols; ++j)
			{
				this->setValue(i, j, pAOther->getValue(i, j) + pBOther->getValue(i, j));
			}
		}

		return shared_ptr<Matrix>(this);
	}

	int const getValue(int x, int y)
	{
		return mpMatrix[x + mOffsetRow][y + mOffsetCol];
	}

	void setValue(int x, int y, int value)
	{
		mpMatrix[x + mOffsetRow][y + mOffsetCol] += value;
	}

	void setOffset(int rowOffset, int colOffset)
	{
		mOffsetRow += rowOffset;
		mOffsetCol += colOffset;
	}

	vector<shared_ptr<Matrix>> partitionHalf()
	{
		vector<shared_ptr<Matrix>> vecMatrix;
		vecMatrix.reserve(4);

		//11
		shared_ptr<Matrix> partitionMatrix(new Matrix());
		partitionMatrix->mRows = partitionMatrix->mCols = this->mRows / 2;
		partitionMatrix->mOffsetRow = this->mOffsetRow;
		partitionMatrix->mOffsetCol = this->mOffsetCol;
		partitionMatrix->mpMatrix = this->mpMatrix;
		vecMatrix.push_back(partitionMatrix);

		//12
		partitionMatrix = shared_ptr<Matrix>(new Matrix());
		partitionMatrix->mRows = partitionMatrix->mCols = this->mRows / 2;
		partitionMatrix->mOffsetRow = this->mOffsetRow;
		partitionMatrix->mOffsetCol = this->mOffsetCol + partitionMatrix->mRows;
		partitionMatrix->mpMatrix = this->mpMatrix;
		vecMatrix.push_back(partitionMatrix);

		//21
		partitionMatrix = shared_ptr<Matrix>(new Matrix());
		partitionMatrix->mRows = partitionMatrix->mCols = this->mRows / 2;
		partitionMatrix->mOffsetRow = this->mOffsetRow + partitionMatrix->mRows;
		partitionMatrix->mOffsetCol = this->mOffsetCol;
		partitionMatrix->mpMatrix = this->mpMatrix;
		vecMatrix.push_back(partitionMatrix);

		//22
		partitionMatrix = shared_ptr<Matrix>(new Matrix());
		partitionMatrix->mRows = partitionMatrix->mCols = this->mRows / 2;
		partitionMatrix->mOffsetRow = this->mOffsetRow + partitionMatrix->mRows;
		partitionMatrix->mOffsetCol = this->mOffsetCol + partitionMatrix->mRows;
		partitionMatrix->mpMatrix = this->mpMatrix;
		vecMatrix.push_back(partitionMatrix);

		return vecMatrix;
	}

	void mergeMatrix(shared_ptr<Matrix> pM11, shared_ptr<Matrix> pM12, shared_ptr<Matrix> pM21, shared_ptr<Matrix> pM22)
	{

	}

	shared_ptr<Matrix> assignMatrix(shared_ptr<Matrix> pM)
	{

	}

	void print()
	{
		for (int i = 0; i < mRows; ++i)
		{
			for (int j = 0; j < mCols; ++j)
			{
				cout << "i = " << i << " j = " << j << " : " << mpMatrix[i][j] << endl;;
			}
		}
	}

private:
	int** mpMatrix = nullptr;
	int mOffsetRow = 0;
	int mOffsetCol = 0;
	
public:
	int mRows = 0;
	int mCols = 0;
};

shared_ptr<Matrix> squareMatrixMultiplyRecuresive(shared_ptr<Matrix> pA, shared_ptr<Matrix> pB, shared_ptr<Matrix> pC)
{
	int n = pA->mRows;

	if (n == 1)
	{
		pC->setValue(0, 0, pA->getValue(0, 0) * pB->getValue(0, 0));
	}
	else
	{
		int offset = pA->mRows / 2;
		auto partitionA = pA->partitionHalf();
		auto partitionB = pB->partitionHalf();
		auto partitionC = pC->partitionHalf();

		squareMatrixMultiplyRecuresive(partitionA[0], partitionB[0], partitionC[0]), squareMatrixMultiplyRecuresive(partitionA[1], partitionB[2], partitionC[0]);
		squareMatrixMultiplyRecuresive(partitionA[0], partitionB[1], partitionC[1]), squareMatrixMultiplyRecuresive(partitionA[1], partitionB[3], partitionC[1]);
		squareMatrixMultiplyRecuresive(partitionA[2], partitionB[0], partitionC[2]), squareMatrixMultiplyRecuresive(partitionA[3], partitionB[2], partitionC[2]);
		squareMatrixMultiplyRecuresive(partitionA[2], partitionB[1], partitionC[3]), squareMatrixMultiplyRecuresive(partitionA[3], partitionB[3], partitionC[3]);
	}

	return pC;
}

#pragma endregion

int main()
{
#pragma region SUBARRAY
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> uid(-100, 100);

	const int arrayRange = 100;
	int* maxArray = new int[arrayRange];

	for (int i = 0; i < arrayRange; ++i)
	{
		maxArray[i] = uid(gen);
		//cout << maxArray[i] << " ";
	}

	auto result = FindMaxNumSubArray(maxArray, 0, 100);

	cout << "left is " << get<0>(result) << ", right is " << get<1>(result) << ", sum is " << get<2>(result) << endl;
#pragma endregion

#pragma region SUBARRAY_MATRIX

	shared_ptr<Matrix> pA(new Matrix{ 4 });
	shared_ptr<Matrix> pB(new Matrix{ 4 });
	shared_ptr<Matrix> pC(new Matrix{ 4 });

	(*pA)[0][0] = 1;
	(*pA)[0][1] = 3;
	(*pA)[0][2] = 2;
	(*pA)[0][3] = 4;
	(*pA)[1][0] = 8;
	(*pA)[1][1] = 5;
	(*pA)[1][2] = 7;
	(*pA)[1][3] = 5;
	(*pA)[2][0] = 6;
	(*pA)[2][1] = 9;
	(*pA)[2][2] = 1;
	(*pA)[2][3] = 2;
	(*pA)[3][0] = 4;
	(*pA)[3][1] = 8;
	(*pA)[3][2] = 2;
	(*pA)[3][3] = 9;

	(*pB)[0][0] = 6;
	(*pB)[0][1] = 8;
	(*pB)[0][2] = 6;
	(*pB)[0][3] = 8;
	(*pB)[1][0] = 9;
	(*pB)[1][1] = 6;
	(*pB)[1][2] = 4;
	(*pB)[1][3] = 2;
	(*pB)[2][0] = 3;
	(*pB)[2][1] = 7;
	(*pB)[2][2] = 8;
	(*pB)[2][3] = 4;
	(*pB)[3][0] = 1;
	(*pB)[3][1] = 2;
	(*pB)[3][2] = 9;
	(*pB)[3][3] = 8;

	squareMatrixMultiplyRecuresive(pA, pB, pC);

	pC->print();

#pragma endregion
	
	cin.get();
	return 0;
}