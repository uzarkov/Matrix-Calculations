#pragma once
#include <fstream>
#include <iostream>
#include <iomanip>
#include "Errors.h"
#include "MatrixException.h"
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>



using namespace std;
template <typename T> class Matrix {

public:
	static const int defaultSize = 4;
	Matrix(int* error);
	Matrix(int rowAmount, int columnAmount, int * error);
	Matrix(const Matrix<T>& otherMatrix, int* error);
	Matrix(Matrix<T>&& otherMatrix);
	~Matrix();

	void copy(const Matrix<T>& otherMatrix, int* error);
	void moveData(Matrix<T>& otherMatrix);
	 
	void operator=(const Matrix<T>& otherMatrix);
	Matrix<T> operator-(const Matrix<T>& otherMatrix);
	Matrix<T> operator+(const Matrix<T>& otherMatrix);
	Matrix<T> operator*(const Matrix<T>& otherMatrix);
	Matrix<T> operator*(T number);
	Matrix<T> operator~();
	void operator() (int x, int y, T value);
	T operator() (int x, int y);

	Matrix<T> substraction(const Matrix<T>& otherMatrix, int * error);
	Matrix<T> add(const Matrix<T>& otherMatrix, int * error);
	Matrix<T> multiply(const Matrix<T>& otherMatrix, int * error);
	Matrix<T> multiply(T number, int * error);
	Matrix<T> transpose(int * error);
	void setElement(int x, int y, T value, int* error);
	T getElement(int x, int y, int* error);

	void identity(int * error);
	T dotProduct(const Matrix& otherMatrix, int * error); 
	Matrix<T> rowVector(int rowNumber, int * error);
	Matrix<T> columnVector(int columnNumber, int * error);

	void fill(T value, int * error = nullptr);
	void createMatrix(int rowAmount, int columnAmount, int * error);
	void deleteMatrix(int * error);
	void print();

	void readFromFile(string path, int* error);
	void appendFromFile(string path, T valToFill, int* error);
	

private:
	T ** matrix;
	int columnAmount; 
	int rowAmount; 
	void allocateMatrix(int rowAmount, int columnAmount, int* error);
	void deallocateMatrix(int * error);
};

template <typename T>
Matrix<T>::Matrix(int* error) {
	allocateMatrix(defaultSize, defaultSize, error);
}

template <typename T>
Matrix<T>::Matrix(int rowAmount, int columnAmount, int * error) {
	allocateMatrix(rowAmount, columnAmount, error);
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& otherMatrix, int* error) {
	copy(otherMatrix, error);
}

template <typename T>
Matrix<T>::Matrix(Matrix<T>&& otherMatrix) {
	this->moveData(otherMatrix);
} 

template <typename T>
Matrix<T>::~Matrix() {
	int error = NO_ERROR;
	deallocateMatrix(&error);
}

template <typename T>
void Matrix<T>::copy(const Matrix<T>& otherMatrix, int* error) {
	*error = NO_ERROR;

	this->deallocateMatrix(error);
	this->allocateMatrix(otherMatrix.rowAmount, otherMatrix.columnAmount, error);

	for (int i = 0; i < rowAmount; i++) {
		for (int j = 0; j < columnAmount; j++) {
			this->matrix[i][j] = otherMatrix.matrix[i][j];
		}
	}

}

template <typename T>
void Matrix<T>::moveData(Matrix<T>& otherMatrix) {
	this->rowAmount = otherMatrix.rowAmount;
	this->columnAmount = otherMatrix.columnAmount;
	this->matrix = otherMatrix.matrix;

	otherMatrix.rowAmount = 0;
	otherMatrix.columnAmount = 0;
	otherMatrix.matrix = nullptr;
}

template <typename T>
void Matrix<T>::allocateMatrix(int rowAmount, int columnAmount, int* error) {
	*error = NO_ERROR;
	if (rowAmount > 0 && columnAmount > 0) {
		this->rowAmount = rowAmount;
		this->columnAmount = columnAmount;

		matrix = new T * [rowAmount];

		for (int i = 0; i < rowAmount; i++) {
			matrix[i] = new T[columnAmount];
		}

		this->fill(0, error);
	}
	else {
		*error = MATRIX_NOT_ALLOCATED_ERROR;
		matrix = nullptr;
		this->rowAmount = 0;
		this->columnAmount = 0;
	}
}

template <typename T>
void Matrix<T>::deallocateMatrix(int * error) {
	if (this->matrix != nullptr) {
		for (int i = 0; i < rowAmount; i++) {
			delete[] matrix[i];
		}
		delete[] matrix;
		*error = NO_ERROR;
	}
	*error = MATRIX_NOT_ALLOCATED_ERROR;
	matrix = nullptr;
	this->rowAmount = 0;
	this->columnAmount = 0;
}
																						
template <typename T>
void Matrix<T>::operator=(const Matrix<T>& otherMatrix) {
	int error = NO_ERROR;
	copy(otherMatrix, &error);
	if (error != NO_ERROR) {
		throw MatrixException(error);
	}
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& otherMatrix) {

	int error = NO_ERROR;

	Matrix<T> resultMatrix = this->substraction(otherMatrix, &error);

	if (error != NO_ERROR) {
		throw MatrixException(error);
	}
	else {
		return move(resultMatrix);
	}	 
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& otherMatrix) {

	int error = NO_ERROR;

	Matrix<T> resultMatrix = this->add(otherMatrix, &error);

	if (error != NO_ERROR) {
		throw MatrixException(error);
	}
	else {
		return move(resultMatrix);
	}
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& otherMatrix) {
	
	int error = NO_ERROR;

	Matrix<T> resultMatrix = this->multiply(otherMatrix, &error);

	if (error != NO_ERROR) {
		throw MatrixException(error);
 	}
	else {
		return move(resultMatrix);
	}
}

template <typename T>
Matrix<T> Matrix<T>::operator*(T number) {

	int error = NO_ERROR;

	Matrix<T> resultMatrix = this->multiply(number, &error);
	
	if (error != NO_ERROR) {
		throw MatrixException(error);
	}
	else {
		return move(resultMatrix);
	}
}

template <typename T>
Matrix<T> Matrix<T>::operator~() {
	
	int error = NO_ERROR;

	Matrix<T> resultMatrix = this->transpose(&error);

	if (error != NO_ERROR) {
		throw MatrixException(error);
	}
	else {
		return move(resultMatrix);
	}	
}

template <typename T>
T Matrix<T>::operator()(int x, int y) {
	
	int error = NO_ERROR;

	T resultValue = this->getElement(x, y, &error);

	if (error != NO_ERROR) {
		return NAN;
	}
	else {
		return resultValue;
	}
}

template <typename T>
void Matrix<T>::operator()(int x, int y, T value) {
	
	int error = NO_ERROR;

	this->setElement(x, y, value, &error);
}

template<typename T>
Matrix<T> Matrix<T>::substraction(const Matrix<T>& otherMatrix, int * error) {

	*error = NO_ERROR;

	Matrix<T> resultMatrix(0, 0, error);

	if (this->matrix == nullptr || otherMatrix.matrix == nullptr) {
		*error = MATRIX_NOT_ALLOCATED_ERROR;
		return resultMatrix;
	}
	else if (this->rowAmount != otherMatrix.rowAmount || this->columnAmount != otherMatrix.columnAmount) {
		*error = SIZES_ARE_NOT_THE_SAME_ERROR;
		return resultMatrix;
	}
	else {
		resultMatrix.createMatrix(rowAmount, columnAmount, error);

		for (int i = 0; i < rowAmount; i++) {
			for (int j = 0; j < columnAmount; j++) {
				resultMatrix.matrix[i][j] = this->matrix[i][j] - otherMatrix.matrix[i][j];
			}
		}

		return move(resultMatrix);
	}
}

template<typename T>
Matrix<T> Matrix<T>::add(const Matrix<T>& otherMatrix, int* error) {

	Matrix<T> resultMatrix(0, 0, error);

	if (this->matrix == nullptr || otherMatrix.matrix == nullptr) {
		*error = MATRIX_NOT_ALLOCATED_ERROR;
		return resultMatrix;
	}
	else if (this->rowAmount != otherMatrix.rowAmount || this->columnAmount != otherMatrix.columnAmount) {
		*error = SIZES_ARE_NOT_THE_SAME_ERROR;
		return resultMatrix;
	}
	else {
		resultMatrix.createMatrix(rowAmount, columnAmount, error);

		for (int i = 0; i < rowAmount; i++) {
			for (int j = 0; j < columnAmount; j++) {
				resultMatrix.matrix[i][j] = this->matrix[i][j] + otherMatrix.matrix[i][j];
			}
		}

		return move(resultMatrix);
	}
}

template <typename T>
Matrix<T> Matrix<T>::multiply(const Matrix<T>& otherMatrix, int* error) {

	Matrix<T> resultMatrix(0, 0, error);

	if (this->matrix == nullptr || otherMatrix.matrix == nullptr) {
		*error = MATRIX_NOT_ALLOCATED_ERROR;
		return resultMatrix;
	}
	else if (this->columnAmount != otherMatrix.rowAmount) {
		*error = WRONG_MATRICES_SIZES_FOR_MULTIPLYING_ERROR;
		return resultMatrix;
	}
	else {
		resultMatrix.createMatrix(this->rowAmount, otherMatrix.columnAmount, error);

		for (int i = 0; i < rowAmount; i++) {
			for (int j = 0; j < otherMatrix.columnAmount; j++) {
				resultMatrix.matrix[i][j] = 0;
				for (int k = 0; k < columnAmount; k++)
				{
					resultMatrix.matrix[i][j] += this->matrix[i][k] * otherMatrix.matrix[k][j];
				}
			}
		}

		return move(resultMatrix);
	}

}

template <typename T>
Matrix<T> Matrix<T>::multiply(T number, int* error) {
	
	Matrix<T> resultMatrix(this->rowAmount, this->columnAmount, error);

	if (this->matrix == nullptr) {
		*error = MATRIX_NOT_ALLOCATED_ERROR;
		return resultMatrix;
	}
	else {
		for (int i = 0; i < rowAmount; i++) {
			for (int j = 0; j < columnAmount; j++) {
				resultMatrix.matrix[i][j] = this->matrix[i][j] * number;
			}
		}

		return move(resultMatrix);
	}
}

template <typename T>
Matrix<T> Matrix<T>::transpose(int* error) {

	Matrix<T> resultMatrix(this->columnAmount, this->rowAmount, error);

	if (this->matrix == nullptr) {
		*error = MATRIX_NOT_ALLOCATED_ERROR;
		return resultMatrix;
	}
	else {

		for (int i = 0; i < rowAmount; i++) {
			for (int j = 0; j < columnAmount; j++) {
				resultMatrix.matrix[j][i] = this->matrix[i][j];
			}
		}

		return move(resultMatrix);
	}
}

template <typename T>
T Matrix<T>::getElement(int x, int y, int * error) {
	
	if (this->matrix == nullptr) {
		*error = MATRIX_NOT_ALLOCATED_ERROR;
		return NAN;
	}
	else if (!(x < this->rowAmount && x >= 0 && y < this->columnAmount && y >= 0)) {
		*error = ELEMENT_OUT_OF_MATRIX_ERROR;
		return NAN;
	}
	else {
		return this->matrix[x][y];
	}	
}

template <typename T>
void Matrix<T>::setElement(int x, int y, T value, int * error) {

	if (this->matrix == nullptr) {
		*error = MATRIX_NOT_ALLOCATED_ERROR;
	}
	else if(!(x < rowAmount && x >= 0 && y < columnAmount && y >= 0)) {
		*error = ELEMENT_OUT_OF_MATRIX_ERROR;
	}
	else {
		this->matrix[x][y] = value;
	}
}

template <typename T> 
void Matrix<T>::identity(int * error) {

	*error = NO_ERROR;
	if (this->matrix == nullptr) {
		*error = MATRIX_NOT_ALLOCATED_ERROR;
	}
	else if (this->rowAmount != this->columnAmount) {
		*error = NOT_A_SQUARE_MATRIX_ERROR;
	}
	else {
		this->fill(0, error);

		for (int i = 0; i < rowAmount; i++) {
			for (int j = 0; j < columnAmount; j++) {
				if (i == j) {
					matrix[i][j] = 1;
				}
			}
		}
	}
}

template <typename T>
T Matrix<T>::dotProduct(const Matrix<T>& otherMatrix, int* error) {
	*error = NO_ERROR;

	if (this->matrix == nullptr || otherMatrix.matrix == nullptr) {
		*error = MATRIX_NOT_ALLOCATED_ERROR;
		return NAN;
	}
	else if (!(this->columnAmount == otherMatrix.columnAmount && this->rowAmount == 1 && otherMatrix.rowAmount == 1)) {
		*error = NOT_A_VECTOR_ERROR;
		return NAN;
	}
	else {
		T sum = 0;

		for (int i = 0; i < columnAmount; i++) {
				sum = sum + this->matrix[0][i] * otherMatrix.matrix[0][i];
		}

		return sum;
	}
}

template <typename T>
Matrix<T> Matrix<T>::rowVector(int rowNumber, int * error) {
	*error = NO_ERROR;

	Matrix<T> resultMatrix(0, 0, error);

	if (this->matrix == nullptr) {
		*error = MATRIX_NOT_ALLOCATED_ERROR;
		return resultMatrix;
	}
	else if(!(rowNumber < this->rowAmount && rowNumber >= 0)){
		*error = ROW_OUT_OF_MATRIX_ERROR;
		return resultMatrix;
	}
	else {
		resultMatrix.createMatrix(1, this->columnAmount, error);

		for (int i = 0; i < columnAmount; i++) {
			resultMatrix.matrix[0][i] = this->matrix[rowNumber][i];
		}

		return move(resultMatrix);
	}
}

template <typename T>
Matrix<T> Matrix<T>::columnVector(int columnNumber, int * error) {
	*error = NO_ERROR;

	Matrix<T> resultMatrix(0, 0, error);

	if (this->matrix == nullptr) {
		*error = MATRIX_NOT_ALLOCATED_ERROR;
		return resultMatrix;
	}
	else if (!(columnNumber < this->columnAmount && columnNumber >= 0)) {
		*error = COLUMN_OUT_OF_MATRIX_ERROR;
		return resultMatrix;
	}
	else {
		resultMatrix.createMatrix(1, this->rowAmount, error);
			
		for (int i = 0; i < rowAmount; i++) {
			resultMatrix.matrix[0][i] = this->matrix[i][columnNumber];
		}

		return move(resultMatrix);	
	}
}

template <typename T>
void Matrix<T>::fill(T value, int * error) {
	*error = NO_ERROR;

	if (this->matrix == nullptr) {
		*error = MATRIX_NOT_ALLOCATED_ERROR;
	}
	else {
		for (int i = 0; i < rowAmount; i++) {
			for (int j = 0; j < columnAmount; j++) {
				matrix[i][j] = value;
			}
		}
	}
}

template <typename T>
void Matrix<T>::createMatrix(int rowAmount, int columnAmount, int * error) {
	*error = NO_ERROR;

	if (rowAmount <= 0 || columnAmount <= 0) {
		*error = WRONG_MATRIX_SIZE_INPUT_ERROR;
	}
	else {
		this->deallocateMatrix(error);
		this->allocateMatrix(rowAmount, columnAmount, error);
	}
}

template <typename T>
void Matrix<T>::deleteMatrix(int * error) {
	*error = NO_ERROR;
	deallocateMatrix(error);
}

template <typename T>
void Matrix<T>::print() {
	for (int i = 0; i < rowAmount; i++) {
		for (int j = 0; j < columnAmount; j++) {
			cout << setprecision(8);
			cout << (matrix[i][j]) << " ";
		}
		cout << endl;
	}
}

template <typename T>
void Matrix<T>::readFromFile(string path, int * error) {
	*error = NO_ERROR;

	ifstream inFile;
	inFile.open(path);

	if (!inFile) {
		*error = FILE_NOT_READ_ERROR;
	}
	else {
		if (!(inFile.peek() == ifstream::traits_type::eof())) {
			string fileLine;
			vector<vector<string>> fileMatrix; 
			int maxColumnsAmount = 0;
			int counter = 0;

			while (!inFile.eof()) {

				int columnAmountCounter = 0;
				vector<string> singleColumn;
				fileMatrix.push_back(singleColumn);
				getline(inFile, fileLine);
				replace(fileLine.begin(), fileLine.end(), '$', ' ');
				stringstream stream(fileLine);
				string ele;
				

				while (stream >> ele) { 
					//stream >> ele;
					replace(ele.begin(), ele.end(), ',', '.');
					replace(ele.begin(), ele.end(), '^', '.');
					columnAmountCounter++;
					fileMatrix.at(counter).push_back(ele);
				}

				if (columnAmountCounter > maxColumnsAmount) {
					maxColumnsAmount = columnAmountCounter;
				}
				counter++;
			}
			inFile.close();

			int rows = fileMatrix.size();
			this->createMatrix(rows, maxColumnsAmount, error);

			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < fileMatrix.at(i).size(); j++) {
					try {
						matrix[i][j] = (T)stod(fileMatrix.at(i).at(j));
					}
					catch (invalid_argument& e) {
						*error = INVALID_ARGUMENT_FOUND_INSIDE_FILE_ERROR;
						matrix[i][j] = 0;
					}
				}
			}
		}
		else {
			*error = CAN_NOT_LOAD_MATRIX_FROM_EMPTY_FILE_ERROR;
		}
	}
}

template <typename T>
void Matrix<T>::appendFromFile(string path, T valToFill, int* error) {

	Matrix<T> matrixFromFile(error);

	matrixFromFile.readFromFile(path, error);

	if (!(*error == CAN_NOT_LOAD_MATRIX_FROM_EMPTY_FILE_ERROR)) {
		int newColumns = 0;

		if (this->columnAmount > matrixFromFile.columnAmount) {
			newColumns = this->columnAmount;
		}
		else {
			newColumns = matrixFromFile.columnAmount;
		}

		int newRow = this->rowAmount + matrixFromFile.rowAmount;

		Matrix<T> resMtx(newRow, newColumns, error);

		resMtx.fill(valToFill, error);

		for (int i = 0; i < rowAmount; i++) {
			for (int j = 0; j < columnAmount; j++) {
				resMtx.matrix[i][j] = this->matrix[i][j];
			}
		}

		int fileColumn = matrixFromFile.columnAmount;

		for (int i = rowAmount; i < newRow; i++) {

			for (int j = 0; j < fileColumn; j++) {
				resMtx.matrix[i][j] = matrixFromFile.matrix[i - rowAmount][j];

			}
		}

		this->copy(resMtx, error);
	}
}











/*
template <typename T>
void Matrix<T>::appendFromFile(string path, T valToFill, int* error) {

	Matrix<T> matrixFromFile(error);

	matrixFromFile.readFromFile(path, error);

	if (!(*error == CAN_NOT_LOAD_MATRIX_FROM_EMPTY_FILE_ERROR)) {

		if (this->matrix == nullptr) {
			this->copy(matrixFromFile, error);
		}
		else {
			int newColumns = 0;

			if (this->columnAmount > matrixFromFile.columnAmount) {
				newColumns = this->columnAmount;
			}
			else {
				newColumns = matrixFromFile.columnAmount;
			}

			int newRow = this->rowAmount + matrixFromFile.rowAmount;

			Matrix<T> resMtx(newRow, newColumns, error);

			resMtx.fill(valToFill, error);

			for (int i = 0; i < rowAmount; i++) {
				for (int j = 0; j < columnAmount; j++) {
					resMtx.matrix[i][j] = this->matrix[i][j];
				}
			}

			int fileColumn = matrixFromFile.columnAmount;

			for (int i = rowAmount; i < newRow; i++) {

				for (int j = 0; j < fileColumn; j++) {
					resMtx.matrix[i][j] = matrixFromFile.matrix[i - rowAmount][j];

				}
			}

			this->copy(resMtx, error);
		}
	}
}
*/