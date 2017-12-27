#ifndef __FILTER_DIFFERENCE__
#define __FILTER_DIFFERENCE__

#include "../matrix/matrix_conv.h"
#include "../matrix/matrix_map.h"

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

#endif