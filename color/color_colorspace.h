#ifndef __COLOR_GRAY_RGB__
#define __COLOR_GRAY_RGB__

#include <opencv2/opencv.hpp>
#include "../matrix/matrix_map.h"
using namespace std;

template <typename T>
class GrayToRGB : public MatMapper<T, 1, T, 3>
{
    Vec<T, 3> map(Vec<T, 1> data)
    {
        return Vec<T, 3>(data[0], data[0], data[0]);
    }
};

enum ConvertMethod {AVG, RED, GREED, BLUE};

template <typename T>
class RGBToGray : public MatMapper<T, 3, T, 1>
{
  protected:
    ConvertMethod method;

  public:
    RGBToGray(ConvertMethod method) : method(method){};
    Vec<T, 1> map(Vec<T, 3> data)
    {
        switch (this->method)
        {
        case AVG:
            return Vec<T, 1>((data[0] + data[1] + data[2]) / 3);
        case RED:
            return Vec<T, 1>(data[2]);
        case GREED:
            return Vec<T, 1>(data[1]);
        case BLUE:
            return Vec<T, 1>(data[0]);
        }
    }
};

class GrayToHeat: public MatMapper<uchar, 1, uchar, 3>
{
    public:
        Vec<uchar, 3> map(Vec<uchar, 1> data);
};

Vec<float, 3> RGBToHSV(Vec<uchar, 3> rgb);
Vec<uchar, 3> HSVToRGB(Vec<float, 3> hsv);

class RGBToHSVConverter: public MatMapper<uchar, 3, float, 3>
{
    Vec<float, 3> map(Vec<uchar, 3> rgb);
};

class HSVToRGBConverter: public MatMapper<float, 3, uchar, 3>
{
    Vec<uchar, 3> map(Vec<float, 3> hsv);
};

class GrayToBinary: public MatMapper<uchar, 1, uchar, 1>
{
    protected:
        int threshold;
    public:
        GrayToBinary(int threshold): threshold(threshold){};
        Vec<uchar, 1> map(Vec<uchar, 1> data);
};

#endif