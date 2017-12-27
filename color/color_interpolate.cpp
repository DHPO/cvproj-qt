#include "./color_interpolate.h"
#include "../expect/expect.h"

uchar nb_1(const Mat &img, Point2f point)
{
    expect(img.type() == CV_8UC1, "nb_1 - invalid type");
    expect(point.x < img.cols && point.y < img.rows, "nb_1 - point overflow");

    int x = point.x, y = point.y;
    if (point.x - x < 0.5)
    {
        if (point.y - y < 0.5)
            return img.at<uchar>(y, x);
        else
            return img.at<uchar>(y + 1, x);
    }
    else
    {
        if (point.y - y < 0.5)
            return img.at<uchar>(y, x + 1);
        else
            return img.at<uchar>(y + 1, x + 1);
    }
}

Vec<uchar, 3> nb_3(const Mat &img, Point2f point)
{
    expect(img.type() == CV_8UC3, "nb_3 - invalid type");
    expect(point.x < img.cols && point.y < img.rows, "nb_3 - point overflow");

    int x = point.x, y = point.y;
    if (point.x - x < 0.5)
    {
        if (point.y - y < 0.5)
            return img.at<Vec<uchar, 3>>(y, x);
        else
            return img.at<Vec<uchar, 3>>(y + 1, x);
    }
    else
    {
        if (point.y - y < 0.5)
            return img.at<Vec<uchar, 3>>(y, x + 1);
        else
            return img.at<Vec<uchar, 3>>(y + 1, x + 1);
    }
}

uchar bilinear_1(const Mat &img, Point2f point)
{
    expect(img.type() == CV_8UC1, "bilinear_1 - invalid type");
    expect(point.x < img.cols && point.y < img.rows, "bilinear_1 - point overflow");

    int x = int(point.x);
    int y = int(point.y);
    float fx = point.x - x;
    float fy = point.y - y;

    float b0 = img.at<uchar>(y, x);
    float b1 = img.at<uchar>(y, x + 1);
    float b2 = img.at<uchar>(y + 1, x);
    float b3 = img.at<uchar>(y + 1, x + 1);

    return (1 - fx) * (1 - fy) * b0 + fx * (1 - fy) * b1 + (1 - fx) * fy * b2 + fx * fy * b3;
}

Vec<uchar, 3> bilinear_3(const Mat &img, Point2f point)
{
    expect(img.type() == CV_8UC3, "bilinear_3 - invalid type");
    expect(point.x < img.cols && point.y < img.rows, "bilinear_3 - point overflow");

    int x = int(point.x);
    int y = int(point.y);
    float fx = point.x - x;
    float fy = point.y - y;

    Vec<int, 3> b0 = img.at<Vec<uchar, 3>>(y, x);
    Vec<int, 3> b1 = img.at<Vec<uchar, 3>>(y, x + 1);
    Vec<int, 3> b2 = img.at<Vec<uchar, 3>>(y + 1, x);
    Vec<int, 3> b3 = img.at<Vec<uchar, 3>>(y + 1, x + 1);

    return (1 - fx) * (1 - fy) * b0 + fx * (1 - fy) * b1 + (1 - fx) * fy * b2 + fx * fy * b3;
}

Mat resize(const Mat &img, Size size, InterpolationMethod method)
{
    Mat result(size, img.type());

    int rows = img.rows;
    int cols = img.cols;
    int rRows = result.rows;
    int rCols = result.cols;

    if (img.channels() == 1)
    {
        uchar (*interpolate)(const Mat &, Point2f) = method == NEARESTNEIGHBOR ? nb_1 : bilinear_1;
        for (int r = 0; r < rRows; r++)
        {
            for (int c = 0; c < rCols; c++)
            {
                result.at<uchar>(r, c) = interpolate(img, Point2f(float(c * cols) / rCols, float(r * rows) / rRows));
            }
        }
    }
    else if (img.channels() == 3)
    {
        Vec<uchar, 3> (*interpolate)(const Mat &, Point2f) = method == NEARESTNEIGHBOR ? nb_3 : bilinear_3;
        for (int r = 0; r < rRows; r++)
        {
            for (int c = 0; c < rCols; c++)
            {
                result.at<Vec<uchar, 3>>(r, c) = interpolate(img, Point2f(float(c) * cols / rCols, float(r) * rows / rRows));
            }
        }
    }

    return result;
}