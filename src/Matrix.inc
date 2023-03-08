#include <cassert>
#include <stdexcept>
#include <numeric>
#include <cmath>
#include <utility>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

/**
 *@brief Matrix class implementation
 *@author Mike Ackerschott
 * This class definition is base upon the example file from Joost van Kraaijenveld and has been fully implemented by me (Mike Ackerschott) and the help of the internet.
 */

/**
 *
 */
template<class T, std::size_t M, std::size_t N>
Matrix<T, M, N>::Matrix(T value)
{
	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			matrix.at(row).at(column) = value;
		}
	}
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
Matrix<T, M, N>::Matrix(const std::initializer_list<T> &aList)
{
	// Check the arguments
	assert(aList.size() == M * N);

	auto row_iter = aList.begin();
	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column, ++row_iter)
		{
			matrix.at(row).at(column) = *row_iter;
		}
	}
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
Matrix<T, M, N>::Matrix(
		const std::initializer_list<std::initializer_list<T> > &aList)
{
	// Check the arguments, the static assert assures that there is at least 1 M and 1 N!
	assert(aList.size() == M && (*aList.begin()).size() == N);

	auto row_iter = aList.begin();
	for (std::size_t row = 0; row < aList.size(); ++row, ++row_iter)
	{
		auto column_iter = (*row_iter).begin();
		for (std::size_t column = 0; column < (*row_iter).size();
				++column, ++column_iter)
		{
			matrix.at(row).at(column) = *column_iter;
		}
	}
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
Matrix<T, M, N>::Matrix(const Matrix<T, M, N> &aMatrix) :
		matrix(aMatrix.matrix)
{
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
std::array<T, N>& Matrix<T, M, N>::at(std::size_t aRowIndex)
{
	return matrix.at(aRowIndex);
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
const std::array<T, N>& Matrix<T, M, N>::at(std::size_t aRowIndex) const
{
	return matrix.at(aRowIndex);
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
T& Matrix<T, M, N>::at(std::size_t aRowIndex, std::size_t aColumnIndex)
{
	return matrix.at(aRowIndex).at(aColumnIndex);
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
const T& Matrix<T, M, N>::at(std::size_t aRowIndex,
		std::size_t aColumnIndex) const
{
	return matrix.at(aRowIndex).at(aColumnIndex);
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
std::array<T, N>& Matrix<T, M, N>::operator[](std::size_t aRowIndex)
{
	return matrix[aRowIndex];
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
const std::array<T, N>& Matrix<T, M, N>::operator[](std::size_t aRowIndex) const
{
	return matrix[aRowIndex];
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
Matrix<T, M, N>& Matrix<T, M, N>::operator=(const Matrix<T, M, N> &rhs)
{
	if (this != &rhs)
	{
		matrix = rhs.matrix;
	}
	return *this;
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
bool Matrix<T, M, N>::operator==(const Matrix<T, M, N> &rhs) const
{
	return matrix == rhs.matrix;
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
template<class T2>
Matrix<T, M, N>& Matrix<T, M, N>::operator*=(const T2 &scalar)
{
	static_assert( std::is_arithmetic<T2>::value, "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			matrix.at(row).at(column) *= scalar;
		}
	}
	return *this;
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
template<class T2>
Matrix<T, M, N> Matrix<T, M, N>::operator*(const T2 &scalar) const
{
	static_assert( std::is_arithmetic<T2>::value, "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

	Matrix < T, M, N > result(*this);
	return result *= scalar;
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
template<class T2>
Matrix<T, M, N>& Matrix<T, M, N>::operator/=(const T2 &aScalar)
{
	static_assert( std::is_arithmetic<T2>::value, "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			matrix.at(row).at(column) /= aScalar;
		}
	}
	return *this;
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
template<class T2>
Matrix<T, M, N> Matrix<T, M, N>::operator/(const T2 &aScalar) const
{
	static_assert( std::is_arithmetic<T2>::value, "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

	Matrix < T, M, N > result(*this);
	return result /= aScalar;
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
Matrix<T, M, N>& Matrix<T, M, N>::operator+=(const Matrix<T, M, N> &rhs)
{
	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			matrix[row][column] += rhs.at(row, column);
		}
	}
	return *this;
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
Matrix<T, M, N> Matrix<T, M, N>::operator+(const Matrix<T, M, N> &rhs) const
{
	Matrix < T, M, N > result(*this);
	return result += rhs;
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
Matrix<T, M, N>& Matrix<T, M, N>::operator-=(const Matrix<T, M, N> &rhs)
{
	for (std::size_t row = 0; row < M; ++row)
	{
		for (std::size_t column = 0; column < N; ++column)
		{
			matrix[row][column] -= rhs.at(row, column);
		}
	}
	return *this;
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
Matrix<T, M, N> Matrix<T, M, N>::operator-(const Matrix<T, M, N> &rhs) const
{
	Matrix < T, M, N > result(*this);
	return result -= rhs;
}
/**
 * (M, N) * (N, P) -> (M, P)
 */
template<typename T, std::size_t M, std::size_t N>
template<std::size_t columns>
Matrix<T, M, columns> Matrix<T, M, N>::operator*(
		const Matrix<T, N, columns> &rhs) const
{
	Matrix < T, M, columns > temp;

	for (int k = 0; k < this->getRows(); ++k)
	{
		for (int j = 0; j < rhs.getColumns(); ++j)
		{
			int value = 0;
			for (int i = 0; i < this->getColumns(); ++i)
			{
				value += this->at(k).at(i) * rhs.at(i).at(j);
			}
			temp.at(k).at(j) = value;
		}
	}
	return temp;
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
Matrix<T, N, M> Matrix<T, M, N>::transpose() const
{
// TODO Implement this function
	Matrix < T, N, M > result;
	for (int i = 0; i < result.getRows(); ++i)
	{
		for (int j = 0; j < result.getColumns(); ++j)
		{
			result.at(i).at(j) = this->at(j).at(i);
		}
	}
	return result;
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
Matrix<T, M, N> Matrix<T, M, N>::identity() {
	Matrix < T, M, N > result(*this);

	for (std::size_t row = 0; row < M; ++row) {
		for (std::size_t column = 0; column < N; ++column) {
			if (row == column) {
				result[row][column] = 1;
			} else {
				result[row][column] = 0;
			}
		}
	}
	return result;
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
Matrix<T, M, N> Matrix<T, M, N>::gauss() const
{
// TODO Implement this function

	Matrix < T, M, N > result = *this;

	for (int i = 0; i < result.getRows(); ++i)
	{

		//check if row has zero and swap it with different row untill it it isnt zero
		int counter = 0;
		while (result.at(i).at(i) == 0 && counter < result.getRows())
		{
//			std::cout << "swapping: " << std::endl;
			++counter;
			result.swap(i, counter);
//			std::cout << result.to_string() << std::endl;
		}

		//whole column is 0; continue
		if (result.at(i).at(i) == 0)
		{
			continue;
		}

		//get first row to 1
		int valueToDivideWith = result.at(i).at(i);
		for (int j = 0; j < result.getColumns(); ++j)
		{
//			std::cout << "Set first value to 1: " << std::endl;
			result.at(i).at(j) = (result.at(i).at(j) / valueToDivideWith);
//			std::cout << result.to_string() << std::endl;
		}

		for (int j = i + 1; j < result.getRows(); ++j)
		{
			int valueToExtractWith = result.at(j).at(i);
			for (int k = 0; k < result.getColumns(); ++k)
			{
//				std::cout << "set value to zero" << std::endl;
				result.at(j).at(k) = result.at(j).at(k)
						- (result.at(i).at(k) * valueToExtractWith);
//				std::cout << result.to_string() << std::endl;
			}
		}
	}

	float valueToExtractWith = result.at(result.getRows() - 1).at(
			result.getColumns() - 3);

	for (int p = 0; p < result.getColumns(); ++p)
	{
		result.at(result.getRows() - 1).at(p) =
				result.at(result.getRows() - 1).at(p)
						- result.at(result.getRows() - 2).at(p)
								* valueToExtractWith;
	}

	float valueToDivideWith = result.at(result.getRows() - 1).at(
			result.getColumns() - 2);

	for (int p = 0; p < result.getColumns(); ++p)
	{
		result.at(result.getRows() - 1).at(p) =
				result.at(result.getRows() - 1).at(p) / valueToDivideWith;
	}

//	printf("%lf\n", valueToExtractWith);

//	std::cout << "done: " << std::endl;
//	std::cout << result.to_string() << std::endl;
	return result;

}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
Matrix<T, M, N> Matrix<T, M, N>::gaussJordan() const {
	Matrix < T, M, N > result(*this);

	for(int i = 0; i < result.getRows(); ++i){
		//check if row has zero and swap it with different row untill it it isnt zero
		if(result.at(i).at(i) == 0){
			for(int j = i+1; j < result.getRows(); ++j){
				if(result.at(j).at(i) != 0 && result.at(i).at(j) != 0){
					result.swap(i, j);
					break;
				}
			}
		}
	}
	//std::cout<<result<<std::endl;


	for(int i = 0; i < result.getRows(); ++i){
		double valueToDivideWith = result.at(i).at(i);
		//std::cout<<result<<std::endl;
		//std::cout<<valueToDivideWith<<std::endl;
		for(int j = 0; j < result.getColumns(); ++j){
			result.at(i).at(j) = (result.at(i).at(j) / valueToDivideWith);
		}
		//std::cout<<result<<std::endl;
		for(int j = 0; j < result.getRows(); ++j){
			if(j != i){
				double valueToExtractWith = result.at(j).at(i);
				for(int k = 0; k < result.getColumns(); ++k){
					result.at(j).at(k) = result.at(j).at(k) - (result.at(i).at(k) * valueToExtractWith);
				}
			}
		}
	}



	//std::cout<<result<<std::endl;
	return result;
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
Matrix<T, M, 1> Matrix<T, M, N>::solve() const {
	Matrix < T, M, N > result(*this);
	result = result.gaussJordan();

	Matrix < T, M, 1 > result2;
	for(int i = 0; i < result.getRows(); ++i){
		result2.at(i).at(0) = result.at(i).at(result.getColumns()-1);
	}

	return result2;
}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
Matrix<double, M, N> Matrix<T, M, N>::inverse() const
{
	Matrix<double, M, N> result(*this);
	Matrix<double, M, N> identity = result.identity();

	Matrix<double, M, M*2> calculationMatrix;

	for(int i = 0; i < result.getRows(); ++i){
		for(int j = 0; j < result.getColumns(); ++j){
			calculationMatrix.at(i).at(j) = result.at(i).at(j);
		}
	}

	for(int i = 0; i < identity.getRows(); ++i){
		for(int j = 0; j < identity.getColumns(); ++j){
			calculationMatrix.at(i).at(j+result.getColumns()) = identity.at(i).at(j);
		}
	}

	calculationMatrix = calculationMatrix.gaussJordan();

	for(int i = 0; i < result.getRows(); ++i){
		for(int j = 0; j < result.getColumns(); ++j){
			result.at(i).at(j) = calculationMatrix.at(i).at(j+result.getColumns());
		}
	}

	std::cout<<result<<std::endl;


	return result;

}
/**
 *
 */
template<class T, std::size_t M, std::size_t N>
std::string Matrix<T, M, N>::to_string() const
{
	std::string result = "Matrix<" + std::to_string(N) + "," + std::to_string(M)
			+ ">\n{\n";
	for (std::size_t i = 0; i < M; ++i)
	{
		for (std::size_t j = 0; j < N; ++j)
		{
			result += std::to_string(matrix[i][j]) + ",";
		}
		result += "\n";
	}
	result += "}";
	return result;
}
/**
 *
 */
template<typename T, const std::size_t N>
bool equals(const Matrix<T, 1, N> &lhs, const Matrix<T, 1, N> &rhs,
		const T aPrecision /*= std::numeric_limits<T>::epsilon()*/,
		const unsigned long aFactor /*= 1*/) {
	auto compare = aPrecision * aFactor;
	for (std::size_t i = 0; i < N; ++i) {
		if (std::abs(lhs[0][i] - rhs[0][i]) > compare) {
			return false;
		}
	}
	return true;
}
/**
 *
 */
template<typename T, const std::size_t M>
bool equals(const Matrix<T, M, 1> &lhs, const Matrix<T, M, 1> &rhs,
		const T aPrecision/* = std::numeric_limits<T>::epsilon()*/,
		const unsigned long aFactor/* = 1*/) {
	auto compare = aPrecision * aFactor;
	for (std::size_t i = 0; i < M; ++i) {
		if (std::abs(lhs[i][0] - rhs[i][0]) > compare) {
			return false;
		}
	}
	return true;
}
/**
 *
 */
template<typename T, const std::size_t M, const std::size_t N>
bool equals(const Matrix<T, M, N> &lhs, const Matrix<T, M, N> &rhs,
		const T aPrecision /*= std::numeric_limits<T>::epsilon()*/,
		const unsigned long aFactor /*= 1*/) {
	auto compare = aPrecision * aFactor;
	for (std::size_t i = 0; i < M; ++i) {
		for (std::size_t j = 0; j < N; ++j) {
			if (std::abs(lhs[i][j] - rhs[i][j]) > compare) {
				return false;
			}
		}
	}
	return true;
}

template<typename T, const std::size_t M, const std::size_t N>
inline Matrix<T, M, N> Matrix<T, M, N>::swap(const uint64_t row1,
		const uint64_t row2)
{
	auto row1Copy = this->at(row1);
	auto row2Copy = this->at(row2);

	this->at(row2) = row1Copy;
	this->at(row1) = row2Copy;
}
