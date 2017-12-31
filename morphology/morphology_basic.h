#ifndef __MORPHOLOGY_BASIC__
#define __MORPHOLOGY_BASIC__

#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;

Mat dilate_b(const Mat &img, const Mat &kernel, Point anchor = Point(-1 ,-1), uchar mark = 0);
Mat erode_b(const Mat &img, const Mat &kernel, Point anchor = Point(-1 ,-1), uchar mark = 255);

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

Mat hitormiss(const Mat &img, const vector<Mat>& kernels, Point anchor = Point(-1, -1), uchar mark = 255);
void defaultKernels(vector<Mat>& kernels);
Mat thin(const Mat &img);
Mat thick(const Mat &img);

Mat distanceConvert(const Mat &img);
Mat bone(const Mat &img);

Mat reconstruct(const Mat &img, const Mat &mark);
Mat watershed(const Mat &img, int threshold = 1);

#endif
