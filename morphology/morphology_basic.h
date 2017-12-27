#ifndef __MORPHOLOGY_BASIC__
#define __MORPHOLOGY_BASIC__

#include <opencv2/opencv.hpp>
using namespace cv;

Mat dilate(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));
Mat erode (const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));

Mat open(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));
Mat close(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));

Mat morphGrad(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));

Mat tophat(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));
Mat blackhat(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));
#endif