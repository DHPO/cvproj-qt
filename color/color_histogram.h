#ifndef __COLOR_HISTOGRAM__
#define __COLOR_HISTOGRAM__
#include <vector>
#include "../matrix/matrix_map.h"
#include "./color_colorspace.h"

class GrayHistogram: public Summary<uchar, 1>
{
    protected:
        std::vector<int> histo;
    public:
        GrayHistogram();
        void record(Vec<uchar, 1> data);
        std::vector<int> getHistogram();
};

std::vector<int> getHistogram(const Mat &img);
std::vector<int> getHistogram(const Mat &img, ConvertMethod channel);

std::vector<uchar> equalize(const std::vector<int> &histogram);

class Adjuster: public MatMapper<uchar, 1, uchar, 1>
{
    private:
        std::vector<uchar> adjustMap;
    public:
        Adjuster(std::vector<uchar> adjustMap);
        Vec<uchar, 1> map(Vec<uchar, 1> data);
};

template<int cn>
class Gamma : public MatMapper<uchar, cn, uchar, cn>
{
private:
    double gamma;
public:
    Gamma(double gamma):gamma(gamma) {};
    Vec<uchar, cn> map(Vec<uchar, cn> data);
};

template<int cn>
Vec<uchar, cn> Gamma_map(Vec<uchar, cn> data, double gamma);

template<int cn>
Vec<uchar, cn> Gamma<cn>::map(Vec<uchar, cn> data)
{
    return Gamma_map<cn>(data, this->gamma);
}

class HSVAdjuster: public MatMapper<float, 3, float, 3>
{
private:
    double hbias;
    double sgamma;
    double vgamma;
public:
    HSVAdjuster(double hbias, double sgamma, double vgamma):hbias(hbias), sgamma(sgamma), vgamma(vgamma) {};
    Vec<float, 3> map(Vec<float, 3> data);
};

int otsu(vector<int> histogram);

#endif
