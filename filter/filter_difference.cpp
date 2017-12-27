#include "./filter_difference.h"
#include "../expect/expect.h"

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
