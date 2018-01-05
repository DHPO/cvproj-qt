#include "./color_histogram.h"
#include "../expect/expect.h"
#include "./color_colorspace.h"
#include <cmath>

int min(float d1, int d2)
{
    return d1 > d2 ? d2 : d1;
}

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


template<>
Vec<uchar, 1> Gamma_map(Vec<uchar, 1> data, double gamma)
{
    return Vec<uchar, 1>(std::pow(data[0] / 255.0, gamma) * 255);
}

template<>
Vec<uchar, 3> Gamma_map(Vec<uchar, 3> data, double gamma)
{
    return Vec<uchar, 3>(
                std::pow(data[0] / 255.0, gamma) * 255,
                std::pow(data[1] / 255.0, gamma) * 255,
                std::pow(data[2] / 255.0, gamma) * 255
                );
}

Vec<float, 3> HSVAdjuster::map(Vec<float, 3> data)
{
    return Vec<float, 3> (
                int(data[0] + hbias) % 360,
                std::pow(data[1], sgamma),
                std::pow(data[2] / 255.0, vgamma) * 255
                );
}

int otsu(vector<int> histogram)
{
    int total = 0;
    int frontCnt = 0;
    int backCnt = 0;
    int frontVal = 0;
    int backVal = 0;

    for (int i = 0; i < 256; i++) {
        frontVal += i * histogram[i];
        total += histogram[i];
    }

    int maxT = 0; int maxInter = 0;
    for (int t = 1; t < 256; t++) {
        frontVal -= t * histogram[t];
        backVal += t * histogram[t];
        frontCnt -= histogram[t];
        backCnt += histogram[t];
        if (frontCnt == 0 || backCnt == 0)
            continue;
        double inter = frontCnt * frontCnt * (frontVal / frontCnt - backVal / backCnt) * (frontVal / frontCnt - backVal / backCnt);
        if (inter > maxInter) {
            maxT = t;
            maxInter = inter;
        }
    }

    return maxT;
}

template<>
Vec<uchar, 3> linearMap(Vec<uchar, 3> data, const vector<Point2i> &controls)
{
    float d = (data[0] + data[1] + data[2]) / 3.0;
    for (int i = 0; i < controls.size(); i++) {
        if (d < controls[i].x) {
            float result = (d - controls[i-1].x) * (controls[i].y - controls[i-1].y) / (controls[i].x - controls[i-1].x)  + controls[i-1].y;
            result = result / d;
            return Vec<uchar, 3>(min(data[0] * result, 255), min(data[1] * result, 255), min(data[2] * result, 255));
        }
    }
    return Vec<uchar, 3>(255, 255, 255);
}

template<>
Vec<uchar, 1> linearMap(Vec<uchar, 1> data, const vector<Point2i> &controls)
{
    float d = data[0];
    for (int i = 0; i < controls.size(); i++) {
        if (d < controls[i].x) {
            int result = (d - controls[i-1].x) * (controls[i].y - controls[i-1].y) / (controls[i].x - controls[i-1].x) + controls[i-1].y;
            return Vec<uchar, 1>(result > 255 ? 255 : result);
        }
    }
    return Vec<uchar, 1>(255);
}
