#ifndef __MATRIX_MATH__
#define __MATRIX_MATH__

#include <opencv2/opencv.hpp>
using namespace cv;

/**
 * matEqual - whether two matrices equal
 * @param matrix1
 * @param matrix2
 */
bool matEqual(const Mat& matrix1, const Mat& matrix2);

/**
 * add - add two matrices
 * assert two matrices have the same size
 * @param matrix1
 * @param matrix2
 */
Mat add(Mat& matrix1, Mat& matrix2);

/**
 * sub - substract matrix2 from matrix1
 * assert two matrices have the same size
 * @param matrix1
 * @param matrix2
 */
Mat sub(Mat& matrix1, Mat& matrix2);

/**
 * negative - scale matrix by -1
 * @param matrix
 */
Mat negative(Mat& matrix);

/**
 * mulitply - scale matrix by a constant
 * @param coefficient
 * @param matrix
 */
Mat multiply(const double coefficient, Mat& matrix);

/**
 * matMul - muliply two matrices
 * assert #columns of left equals #rows of right
 * @param left
 * @param right
 */
Mat matMul(Mat& left, Mat& right);

#endif