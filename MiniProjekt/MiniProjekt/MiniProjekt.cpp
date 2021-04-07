
#include "Matrix.h"

#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;
int main()
{
    

    int error = 1;
    
    Matrix<int> matrix1(&error);
    matrix1.readFromFile("intTest.txt", &error);
    cout << endl << "matrix1:" << endl;
    matrix1.print();

    Matrix<int> matrix2(&error);
    matrix2.readFromFile("intTest.txt", &error);
    cout << endl <<"matrix2:" << endl;
    matrix2.print();

    Matrix<int> matrix3(&error);
    matrix3 = matrix1 * 3;
    cout << endl << "matrix3:" << endl;
    matrix3.print();

    Matrix<int> matrix4(&error);
    matrix4 = matrix3 - matrix2;
    cout << endl << "matrix4:" << endl;
    matrix4.print();

    Matrix<int> matrix5(&error);
    matrix5 = matrix4 + matrix2;
    cout << endl << "matrix5:" << endl;
    matrix5.print();

    Matrix<int> matrix6(&error);
    matrix6 = ~matrix3;
    cout << endl << "matrix6:" << endl;
    matrix6.print();
    
    Matrix<int> matrix7(&error);        
    matrix7 = matrix6 * matrix2;
    cout << endl << "matrix7: " << endl;
    matrix7.print();

    int value = matrix7(0, 0);
    cout << endl << "matrix7 0,0 element: " << value << endl;

    matrix7(0, 0, 999);
    cout << endl << "matrix7 with 1st ele modified: " << endl;
    matrix7.print();

    Matrix<int> vector1(&error);
    vector1 = matrix1.columnVector(0, &error);
    cout << endl << "vector1:  " << endl;
    vector1.print();

    Matrix<int> vector2(&error);
    vector2 = matrix6.rowVector(0, &error);
    cout << endl << "vector2:  " << endl;
    vector2.print();

    int dotProduct = vector1.dotProduct(vector2, &error);
    cout << endl << "Dot product of these vectors: " << dotProduct << endl;

    matrix7.identity(&error);
    cout << endl << "matrix7 identity: " << endl;
    matrix7.print();

    matrix6.identity(&error);
    cout << endl << error << endl;
    
    Matrix<float> matrix8(&error);
    matrix8.readFromFile("doubleTest.txt", &error);
    matrix8.print();

    string testString = "aa//bbc";

    error = NO_ERROR;
    for (int i = 1; i < testString.size(); i++) {
        if (testString[i] == '/' && testString[i - 1] == '/') {
            error = -100;
        }
    }

    cout << endl << "Errror: " << error; 

    error = NO_ERROR;
    Matrix<int> matrix9(&error);
    matrix9.readFromFile("emptyTest.txt", &error);
    cout << endl << "Errror empty: " << error << endl;
    matrix9.print();

    

    //----------------------------------------------------//


    cout << endl << "---------------------------------------------------" << endl;

    Matrix<int> matrix11(2, 2, &error);
    matrix11.fill(1, &error);
    matrix11.setElement(0, 1, 2, &error);
    matrix11.setElement(1, 1, 2, &error);
   
    matrix11.appendFromFile("intTest.txt", 6, &error);
    matrix11.print();


    

}

