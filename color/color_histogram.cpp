#include "./color_histogram.h"
#include "../expect/expect.h"
#include "./color_colorspace.h"

GrayHistogram::GrayHistogram()
{
    histo.resize(256);
    std::fill(histo.begin(), histo.end(), 0);
}

void GrayHistogram::record(Vec<uchar, 1> data)
{
    histo[data[0]] ++;
}

std::vector<int> GrayHistogram::getHistogram()
{
    return histo;
}

std::vector<int> getHistogram(const Mat &img)
{
    expect(img.type() == CV_8UC1, "GetHistogram - invalid image type");

    GrayHistogram histogram;
    MatReducer<uchar, 1>().doReduce(img, histogram);

    return histogram.getHistogram();
}

std::vector<int> getHistogram(const Mat &img, ConvertMethod channel)
{
    expect(img.type() == CV_8UC3, "GetHistogram - invalid image type");
    Mat temp = RGBToGray<uchar>(channel).doMap(img);

    GrayHistogram histogram;
    MatReducer<uchar, 1>().doReduce(temp, histogram);

    return histogram.getHistogram();
}

std::vector<uchar> equalize(const std::vector<int> &histogram)
{
    expect(histogram.size() == 256, "equalize - input is not a grayscale-histogram");

    double sum = 0;
    for (auto it = histogram.begin(); it != histogram.end(); it ++)
    {
        sum += *it;
    }

    double step = sum / 256;
    std::vector<uchar> result(256);

    int cnt = 0;
    for (int i = 0; i < 256; i ++)
    {
        int temp = cnt / step;
        result[i] = temp <= 255 ? temp : 255;
        cnt += histogram[i];
    }

    return result;
}

Adjuster::Adjuster(std::vector<uchar> adjustMap)
{
    expect(adjustMap.size() == 256, "Adjuster - invalid map");
    this->adjustMap = adjustMap;
}

Vec<uchar, 1> Adjuster::map(Vec<uchar, 1> data)
{
    return Vec<uchar, 1>(adjustMap[data[0]]);
}