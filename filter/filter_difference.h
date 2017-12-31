#ifndef __FILTER_DIFFERENCE__
#define __FILTER_DIFFERENCE__

#include "../matrix/matrix_conv.h"
#include "../matrix/matrix_map.h"
#include <vector>
#include <stack>
using namespace std;

class SobelFilter
{
    private:
        Mat kernel1;
        Mat kernel2;
        float threshold;
    public:
        SobelFilter(float threshold = 0);
        Mat doFilter(const Mat &img);
};

class LaplacianFilter
{
    private:
        Mat kernel;
    public:
        enum KernelType{SIMPLE4, SIMPLE8};
        LaplacianFilter(KernelType type);
        LaplacianFilter(float sigma);
        Mat doFilter(const Mat &img);
};

class CannyFilter
{
private:
    Mat img;
    Mat kernel1;
    Mat kernel2;
    Mat amplitude;
    Mat direction;
    int thigh;
    int tlow;
    vector<int> histogram;
    void filt(double sigma);
    void compression();
    Mat threshold();
    void test(int r, int c, Mat &result, stack<Point2i>& stk);
public:
    CannyFilter(const Mat& img);
    void setThreshold(int high, int low);
    Mat step1(double sigma);
    Mat step2();
};

#endif
