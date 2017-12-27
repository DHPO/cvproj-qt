#ifndef __COLOR_INTERPOLATE__   
#define __COLOR_INTERPOLATE__

#include <opencv2/opencv.hpp>
using namespace cv;

uchar nb_1(const Mat &img, Point2f point);
Vec<uchar, 3> nb_3(const Mat &img, Point2f point);

uchar bilinear_1(const Mat &img, Point2f point);
Vec<uchar, 3> bilinear_3(const Mat &img, Point2f point);

enum InterpolationMethod {NEARESTNEIGHBOR, BILINEAR};

Mat resize(const Mat &img, Size size, InterpolationMethod method = BILINEAR); 

#endif