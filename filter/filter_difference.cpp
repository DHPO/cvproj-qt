#include "./filter_difference.h"
#include "./filter/filter_smooth.h"
#include "../expect/expect.h"
#include "../color/color_histogram.h"
#include <stack>

SobelFilter::SobelFilter(float threshold)
{
    this->threshold = threshold;
    /* set kernels */
    float kernel_data1[] =
        {1, 2, 1,
         0, 0, 0,
         -1, -2, -1};
    float kernel_data2[] =
        {1, 0, -1,
         2, 0, -2,
         1, 0, -1};

    this->kernel1 = Mat(3, 3, CV_32FC1, kernel_data1).clone();
    this->kernel2 = Mat(3, 3, CV_32FC1, kernel_data2).clone();
}

Mat SobelFilter::doFilter(const Mat &img)
{
    expect(img.channels() == 1, "SobelFilter - img is not a grayscale");

    Mat conv1 = conv(img, kernel1, MIRROR);
    Mat conv2 = conv(img, kernel2, MIRROR);

    class MatDistance : public MatOperator<float, 1>
    {
      protected:
        int threshold;

      public:
        MatDistance(int threshold = 0) : threshold(threshold){};
        Vec<float, 1> op(Vec<float, 1> d1, Vec<float, 1> d2)
        {
            float distance = sqrt(d1[0] * d1[0] + d2[0] * d2[0]);
            return Vec<float, 1>(distance >= threshold ? distance : 0);
        }
    };

    Mat result = MatDistance(threshold).doOp(conv1, conv2);

    return result;
}

LaplacianFilter::LaplacianFilter(KernelType type)
{
    if (type == SIMPLE4) {
        float kernel_data[] = 
            {0,  1, 0,
             1, -4, 1,
             0,  1, 0};        
        this->kernel = Mat(3, 3, CV_32FC1, kernel_data).clone();
    }
    else if (type == SIMPLE8) {
        float kernel_data[] = 
            {1,  1, 1,
             1, -8, 1,
             1,  1, 1};
        this->kernel = Mat(3, 3, CV_32FC1, kernel_data).clone();
    }
}

LaplacianFilter::LaplacianFilter(float sigma)
{
    int size = int(sigma * 3) * 2 + 1;
    int halfsize = (size + 1) / 2;

    float *data = new float[size * size];

    float sigma2 = sigma * sigma;
    float one_divide_two_sigma_square = 0.5 / sigma2;
    float coefficient = - 1.0 / (3.14159 * sigma2 * sigma2);

    for (int i = 0; i < halfsize; i++) {
        for (int j = 0; j < halfsize; j++) {
            /* calculate */
            float x2y2 = i * i + j * j;
            float x2y2_2sigma2 = x2y2 * one_divide_two_sigma_square;
            float value = coefficient * (1 - x2y2_2sigma2) * exp(- x2y2_2sigma2);
            /* assign */
            data[(halfsize - 1 + i) * size + (halfsize - 1 + j)] = value;
            data[(halfsize - 1 - i) * size + (halfsize - 1 + j)] = value;
            data[(halfsize - 1 + i) * size + (halfsize - 1 - j)] = value;
            data[(halfsize - 1 - i) * size + (halfsize - 1 - j)] = value;
            data[(halfsize - 1 + j) * size + (halfsize - 1 + i)] = value;
            data[(halfsize - 1 + j) * size + (halfsize - 1 - i)] = value;
            data[(halfsize - 1 - j) * size + (halfsize - 1 + i)] = value;
            data[(halfsize - 1 - j) * size + (halfsize - 1 - i)] = value;
        }
    }

    this->kernel = Mat(size, size, CV_32FC1, data).clone();
    delete[] data;
}

Mat LaplacianFilter::doFilter(const Mat &img)
{
    expect(img.channels() == 1, "LaplacianFilter - img is not a grayscale");

    Mat result = conv(img, kernel, MIRROR);

    return result;
}

CannyFilter::CannyFilter(const Mat &img)
{
    img.copyTo(this->img);
}

void CannyFilter::setThreshold(int high, int low)
{
    this->thigh = high;
    this->tlow = low;
}

void CannyFilter::filt(double sigma)
{
    GaussianFilter(sigma).doFilter(this->img).convertTo(this->img, CV_8UC1);

    /* set kernels */
    float kernel_data1[] =
        {1, 2, 1,
         0, 0, 0,
         -1, -2, -1};
    float kernel_data2[] =
        {-1, 0, 1,
         -2, 0, 2,
         -1, 0, 1};

    this->kernel1 = Mat(3, 3, CV_32FC1, kernel_data1).clone();
    this->kernel2 = Mat(3, 3, CV_32FC1, kernel_data2).clone();

    Mat conv1 = conv(img, kernel1, MIRROR);
    Mat conv2 = conv(img, kernel2, MIRROR);

    class MatDistance : public MatOperator<float, 1>
    {
    public:
        Vec<float, 1> op(Vec<float, 1> d1, Vec<float, 1> d2)
        {
            float distance = sqrt(d1[0] * d1[0] + d2[0] * d2[0]) / 4;
            return Vec<float, 1>(distance > 10 ? distance : 0);
        }
    };

    class MatDirection: public MatOperator<float, 1>
    {
        Vec<float, 1> op(Vec<float, 1> d1, Vec<float, 1> d2) {
            return Vec<float, 1>(d1[0] / d2[0]);
        }
    };

    this->amplitude = MatDistance().doOp(conv1, conv2);
    this->direction = MatDirection().doOp(conv1, conv2);
}

void CannyFilter::compression()
{
    Mat result(amplitude.rows, amplitude.cols, amplitude.type());

    for (int r = 0; r < amplitude.rows; r++) {
        for (int c = 0; c < amplitude.cols; c++) {
            float d = direction.at<float>(r, c);
            float v = amplitude.at<float>(r, c);
            if (d < -2 || d > 2) {
                if (amplitude.at<float>(r - 1, c) < v
                        && amplitude.at<float>(r + 1, c) <= v)
                    result.at<float>(r, c) = v;
                else
                    result.at<float>(r, c) = 0;
            }
            else if (d < -0.5) {
                if (amplitude.at<float>(r - 1, c - 1) < v
                        && amplitude.at<float>(r + 1, c + 1) <= v)
                    result.at<float>(r, c) = v;
                else
                    result.at<float>(r, c) = 0;
            }
            else if (d > 0.5) {
                if (amplitude.at<float>(r - 1, c + 1) < v
                        && amplitude.at<float>(r + 1, c - 1) <= v)
                    result.at<float>(r, c) = v;
                else
                    result.at<float>(r, c) = 0;
            }
            else {
                if (amplitude.at<float>(r, c - 1) < v
                        && amplitude.at<float>(r, c + 1) <= v)
                    result.at<float>(r, c) = v;
                else
                    result.at<float>(r, c) = 0;
            }
        }
    }

    amplitude = result.clone();
}

Mat CannyFilter::threshold()
{
    std::stack<Point2i> stk;
    Mat result(img.rows, img.cols, CV_8UC1, Scalar(0));

    for (int r = 0; r < img.rows; r++) {
        for (int c = 0; c < img.cols; c++) {
            if (amplitude.at<float>(r, c) >= thigh && result.at<uchar>(r, c) != 255) {
                result.at<uchar>(r, c) = 255;
                stk.push(Point2i(r, c));
                while (!stk.empty()) {
                    Point2i point = stk.top();
                    stk.pop();
                    int r = point.x, c = point.y;
                    float d = direction.at<float>(r, c);
                    if (d > 1) {
                        test(r, c + 1, result, stk);
                        test(r + 1, c + 1, result, stk);
                        test(r, c - 1, result, stk);
                        test(r - 1, c - 1, result, stk);
                    }
                    else if (d > 0) {
                        test(r + 1, c, result, stk);
                        test(r + 1, c + 1, result, stk);
                        test(r - 1, c, result, stk);
                        test(r - 1, c - 1, result, stk);
                    }
                    else if (d > -1) {
                        test(r + 1, c, result, stk);
                        test(r + 1, c - 1, result, stk);
                        test(r - 1, c, result, stk);
                        test(r - 1, c + 1, result, stk);
                    }
                    else {
                        test(r, c + 1, result, stk);
                        test(r + 1, c - 1, result, stk);
                        test(r, c - 1, result, stk);
                        test(r - 1, c + 1, result, stk);
                    }
                }
            }
        }
    }
    return result;
}

Mat CannyFilter::step1(double sigma)
{
    this->filt(sigma);
    this->compression();
    Mat temp;
    amplitude.convertTo(temp, CV_8UC1);
    return temp;
}

Mat CannyFilter::step2()
{
    return threshold();
}

void CannyFilter::test(int r, int c, Mat &result, stack<Point2i> &stk)
{
    if (r < 0 || r > result.rows || c < 0 || c > result.cols || result.at<uchar>(r, c) == 255)
        return;

    if (amplitude.at<float>(r, c) >= tlow) {
        result.at<uchar>(r, c) = 255;
        stk.push(Point2i(r, c));
    }
}
