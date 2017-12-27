#ifndef __COLOR_HISTOGRAM__
#define __COLOR_HISTOGRAM__
#include <vector>
#include "../matrix/matrix_map.h"

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
std::vector<int> getHistogram(const Mat &img, int channel);

std::vector<uchar> equalize(const std::vector<int> &histogram);

class Adjuster: public MatMapper<uchar, 1, uchar, 1>
{
    private:
        std::vector<uchar> adjustMap;
    public:
        Adjuster(std::vector<uchar> adjustMap);
        Vec<uchar, 1> map(Vec<uchar, 1> data);
};

#endif