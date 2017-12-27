#ifndef __MATRIX_CONVOLUTION__
#define __MATRIX_CONVOLUTION__

#include <opencv2/opencv.hpp>
using namespace cv;

typedef enum {NONE, SAME, MIRROR} PaddingMethod;

Mat conv(const Mat &matrix, const Mat &kernel, PaddingMethod padding = NONE, int stride = 1);

Mat padding(const Mat &matrix, int paddingSize, PaddingMethod method);
#endif