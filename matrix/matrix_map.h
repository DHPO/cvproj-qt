#ifndef __MATRIX_MAP__
#define __MATRIX_MAP__

#include <opencv2/opencv.hpp>
#include <vector>
#include "./color/color_interpolate.h"
using namespace cv;

template <typename T>
Mat &matMap(Mat &matrix, std::vector<T> map);

template <typename T>
Mat &matMap(Mat &matrix, T (*map)(T));

/**
 * MatMapper - map element to another through matrix
 * override map method to apply your own one
 */
template <typename T1, int cn1, typename T2, int cn2>
class MatMapper
{
    /* To override */
    virtual Vec<T2, cn2> map(Vec<T1, cn1>) = 0;
  public:
    /* To invoke */
    Mat doMap(const Mat &matrix);
};

template <typename T, int channels = 1>
class MatOperator
{
    /* To override */
    virtual Vec<T, channels> op(Vec<T, channels> d1, Vec<T, channels> d2) = 0;
  public:
    /* To invoke */
    Mat doOp(const Mat &matrix1, const Mat &matrix2);
};

template<typename T, int channels = 1>
class Summary{
    public:
        virtual void record(Vec<T, channels> data) = 0;
};

template <typename T, int channels = 1>
class MatReducer
{
public:
    //virtual void reduce(Summary& summary, Vec<T, channels> d) = 0;
    void doReduce(const Mat &matrix, Summary<T, channels> &summary);
};

template <typename T, int channels = 1>
class MatTransformmer
{
        virtual Point2f pointMap(Point2i point) = 0;
        virtual Point2f pointMapReverse(Point2i point) = 0;
        virtual Vec<T, channels> interpolate(const Mat &img, Point2f point);
    public:
        Mat doTrans(const Mat &matrix);
};

/***************************************/

#include "../expect/expect.h"

template <typename T>
Mat &matMap(Mat &matrix, std::vector<T> map)
{
    /* expect datatype of matrix is 1 byte */
    expect((matrix.type() % 8 <= 1), "MatMap - matrix datatype violate");
    expect(sizeof(T) == 1, "MatMap - datatype violate");
    expect(map.size() == 256, "MatMap - map size is not 256");

    /* go through matrix */
    int channels = matrix.channels();
    int rows = matrix.rows;
    int cols = matrix.cols * channels;

    for (int i = 0; i < rows; i++)
    {
        T *pa = matrix.ptr<T>(i);
        for (int j = 0; j < cols; j++)
        {
            pa[j] = map[pa[j]];
        }
    }

    return matrix;
}

template <typename T>
Mat &matMap(Mat &matrix, T (*map)(T))
{
    /* check whether datatype of matrix matches map */
    int datasize = 1 << ((matrix.type() % 8) >> 1);
    expect(sizeof(T) == datasize, "MatMap - datatype violate");

    /* go through matrix */
    int channels = matrix.channels();
    int rows = matrix.rows;
    int cols = matrix.cols * channels;

    for (int i = 0; i < rows; i++)
    {
        T *pa = matrix.ptr<T>(i);
        for (int j = 0; j < cols; j++)
        {
            pa[j] = map(pa[j]);
        }
    }

    return matrix;
}

template <typename T1, int cn1, typename T2, int cn2>
Mat MatMapper<T1, cn1, T2, cn2>::doMap(const Mat &matrix)
{
    expect(matrix.channels() == cn1, "MatMapper - channels violate");
    expect(1 << ((matrix.type() % 8) >> 1) == sizeof(T1), "MatMapper - type violate");

    int rows = matrix.rows;
    int cols = matrix.cols;

    Mat result(rows, cols, DataType<T2>::type + (cn2 - 1) * 8);
    for (int i = 0; i < rows; i++)
    {
        const T1 *pa = matrix.ptr<T1>(i);
        T2 *pr = result.ptr<T2>(i);
        for (int j = 0; j < cols; j++)
        {
            Vec<T2, cn2> temp = map(Vec<T1, cn1>(pa + j * cn1));
            for (int m = 0; m < cn2; m++)
                pr[j * cn2 + m] = temp[m];
        }
    }

    return result;
}

template <typename T, int channels>
Mat MatOperator<T, channels>::doOp(const Mat &matrix1, const Mat &matrix2)
{
    //expect(stride >= 1, "MatOperator - illegal stride");
    expect(matrix1.channels() == channels, "MatOperator - channels violate");
    expect(matrix2.channels() == channels, "MatOperator - channels violate");
    expect(1 << ((matrix1.type() % 8) >> 1) == sizeof(T), "MatOperator - type violate");
    expect(matrix1.type() == matrix2.type(), "MatOperator - type violate");
    expect(matrix1.rows == matrix2.rows && matrix1.cols == matrix2.cols, "MatOperator - size not matches");

    int rows = matrix1.rows;
    int cols = matrix1.cols;

    Mat result(rows, cols, matrix1.type());

    for (int i = 0; i < rows; i++)
    {
        const T *pa = matrix1.ptr<T>(i);
        const T *pb = matrix2.ptr<T>(i);
        T *pr = result.ptr<T>(i);
        for (int j = 0; j < cols * channels; j += channels)
        {
            auto data1 = Vec<T, channels>(pa + j);
            auto data2 = Vec<T, channels>(pb + j);
            auto temp = op(data1, data2);
            for (int m = 0; m < channels; m++)
                pr[j + m] = temp[m];
        }
    }

    return result;
}

template<typename T, int channels>
void MatReducer<T, channels>::doReduce(const Mat &matrix, Summary<T, channels> &summary)
{
    expect(matrix.channels() == channels, "MatReducer - channels violate");
    expect(1 << ((matrix.type() % 8) >> 1) == sizeof(T), "MatReducer - type violate");

    int rows = matrix.rows;
    int cols = matrix.cols;

    for (int i = 0; i < rows; i++)
    {
        const T *pa = matrix.ptr<T>(i);
        for (int j = 0; j < cols; j++)
        {
            summary.record(Vec<T, channels>(pa + j * channels));
        }
    }
}

template <typename T, int channels>
Vec<T, channels> MatTransformmer<T, channels>::interpolate(const Mat &img, Point2f point)
{
    return bilinear<channels>(img, point);
}

template <typename T, int channels>
Mat MatTransformmer<T, channels>::doTrans(const Mat &matrix)
{
    expect(matrix.channels() == channels, "MatTransformmer - channels violate");
    expect(1 << ((matrix.type() % 8) >> 1) == sizeof(T), "MatTransformmer - type violate");

    /* calculate result size */
    Point2f point00 = pointMap(Point2i(0, 0));
    Point2f pointx0 = pointMap(Point2i(matrix.cols, 0));
    Point2f point0y = pointMap(Point2i(0, matrix.rows));
    Point2f pointxy = pointMap(Point2i(matrix.cols, matrix.rows));

    int xmin = min(min(point00.x, pointx0.x), min(point0y.x, pointxy.x));
    int xmax = max(max(point00.x, pointx0.x), max(point0y.x, pointxy.x));
    int ymin = min(min(point00.y, pointx0.y), min(point0y.y, pointxy.y));
    int ymax = max(max(point00.y, pointx0.y), max(point0y.y, pointxy.y));

    int cols = xmax - xmin;
    int rows = ymax - ymin;
    int xbias = -xmin;
    int ybias = -ymin;

    Mat result(rows, cols, matrix.type());

    /* go through */
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Point2f point = pointMapReverse(Point2i(c - xbias, r - ybias));
            result.at<Vec<T, channels>>(r, c) = interpolate(matrix, point);
        }
    }

    return result;
}

#endif
