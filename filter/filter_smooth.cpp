#include "./filter_smooth.h"
#include "./filter_basic.h"
#include "../color/color_colorspace.h"

GaussianFilter::GaussianFilter(float sigma)
{
    int size = int(sigma * 3) * 2 + 1;
    int halfsize = (size + 1) / 2;

    float *data = new float[size * size];

    float sigma2 = sigma * sigma;
    float one_divide_two_sigma_square = 0.5 / sigma2;
    float coefficient = -1.0 / (2 * 3.14159 * sigma2);

    for (int i = 0; i < halfsize; i++)
    {
        for (int j = 0; j < halfsize; j++)
        {
            /* calculate */
            float x2y2 = i * i + j * j;
            float x2y2_2sigma2 = x2y2 * one_divide_two_sigma_square;
            float value = coefficient * exp(-x2y2_2sigma2);
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

    Mat temp = Mat(size, size, CV_32FC1, data);
    this->kernel = kernelUniform(temp).clone();
    delete[] data;
}

Mat GaussianFilter::doFilter(const Mat &matrix)
{
    Mat kernel = this->kernel;
    if (matrix.channels() == 3)
        kernel = GrayToRGB<float>().doMap(kernel);

    return conv(matrix, kernel, MIRROR);
}

MeanFilter::MeanFilter(int size)
{
    this->kernel = Mat(size, size, CV_32FC1, Scalar(1.0 / (size * size))).clone();
}

Mat MeanFilter::doFilter(const Mat &matrix)
{
    Mat kernel = this->kernel;
    if (matrix.channels() == 3)
        kernel = GrayToRGB<float>().doMap(kernel);

    return conv(matrix, kernel, MIRROR);
}

MediumFilter::MediumFilter(int size)
{
    this->size = size;
}

Mat MediumFilter::medium_1(const Mat &img)
{
    expect(img.type() == CV_8UC1, "medium_1 - invalid data type");
    int rows = img.rows;
    int cols = img.cols;
    int rRows = rows - size + 1;
    int rCols = cols - size + 1;

    Mat result(rRows, rCols, img.type());
    for (int r = 0; r < rRows; r++) {
        for (int c = 0; c < rCols; c ++) {
            vector<float> temp;
            for (int rr = r; rr < r + size; rr ++) {
                for (int cc = c; cc < c + size; cc ++) {
                    temp.push_back(img.at<uchar>(rr, cc));
                }
            }
            sort(temp.begin(), temp.end());
            result.at<uchar>(r, c) = temp[size * size / 2 + 1];
        }
    }

    return result;
}

Mat MediumFilter::medium_3(const Mat &img)
{
    expect(img.type() == CV_8UC3, "medium_1 - invalid data type");
    int rows = img.rows;
    int cols = img.cols;
    int rRows = rows - size + 1;
    int rCols = cols - size + 1;

    Mat result(rRows, rCols * 3, img.type());
    for (int r = 0; r < rRows; r++) {
        for (int c = 0; c < rCols; c ++) {
            vector<float> tempB, tempG, tempR;
            for (int rr = r; rr < r + size; rr ++) {
                for (int cc = c; cc < c + size; cc ++) {
                    Vec<uchar, 3> color = img.at<Vec<uchar, 3>>(rr, cc);
                    tempB.push_back(color[0]);
                    tempG.push_back(color[1]);
                    tempR.push_back(color[2]);
                }
            }
            sort(tempB.begin(), tempB.end());
            sort(tempG.begin(), tempG.end());
            sort(tempR.begin(), tempR.end());
            result.at<Vec<uchar, 3>>(r, c) = Vec<uchar, 3>(tempB[size * size / 2 + 1], tempG[size * size / 2 + 1], tempR[size * size / 2 + 1]);
        }
    }

    return result;
}

Mat MediumFilter::doFilter(const Mat &img)
{
    if (img.channels() == 1)
        return medium_1(img);
    else
        return medium_3(img);
}