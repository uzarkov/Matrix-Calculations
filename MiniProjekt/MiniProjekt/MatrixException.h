#pragma once
#include <iostream>
#include <string>
using namespace std;;

class MatrixException : public exception {

public:
	MatrixException(int errorExc) {
		this->error = errorExc;
		this->errorMessage = ("MatrixExcepetion: Error code: "  +  to_string(error));
	}

	virtual const char* what() const throw() {
		
		return this->errorMessage.c_str();
	}
private:
	int error;
	string errorMessage;
};