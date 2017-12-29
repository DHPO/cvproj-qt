#ifndef __MORPHOLOGY_BASIC__
#define __MORPHOLOGY_BASIC__

#include <opencv2/opencv.hpp>
using namespace cv;

Mat dilate_b(const Mat &img, const Mat &kernel, Point anchor = Point(-1 ,-1));
Mat erode_b(const Mat &img, const Mat &kernel, Point anchor = Point(-1 ,-1));

Mat dilate(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));
Mat erode (const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));

Mat open_b(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));
Mat close_b(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));

Mat open_g(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));
Mat close_g(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));

Mat morphGrad_b(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));

Mat tophat_b(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));
Mat blackhat_b(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));

Mat morphGrad(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));

Mat tophat(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));
Mat blackhat(const Mat &img, const Mat &kernel, Point anchor = Point(-1, -1));
#endif
